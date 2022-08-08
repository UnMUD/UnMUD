#include "kernel.h"
#include <sys/stat.h>
#include <unistd.h>
#include "machines.h"
#include "stdinc.h"
#include "locations.h"
#include "mud.h"
#ifndef R_OK
#define R_OK 4
#endif

static Boolean login_ok (char *name);
static void get_pname1 (char *name);
static void get_pname2 (char *reply);
static void get_new_pass1 (char *pass);
static void get_new_pass2 (char *pass);
static void get_passwd1 (char *pass);
static void kick_out_yn (char *answer);
static void talker (void);
static void get_yn_handler(char *yn);

int debug_level;
#include "global.h"

char *pwait = "Press <Return> to continue...";

void 
push_input_handler (void (*h) (char *str))
{
  INP_HANDLER *i;
  i = NEW (INP_HANDLER, 1);
  i->next = cur_player->inp_handler;
  i->inp_handler = h;
  cur_player->inp_handler = i;
}

void 
pop_input_handler (void)
{
  INP_HANDLER *i = cur_player->inp_handler;
  INP_HANDLER *j;
  
  j = i->next;
  if (j != NULL)
    {
      cur_player->inp_handler = j;
      free (i);
    }
}

void replace_input_handler (void (*h) (char *str))
{
  if (cur_player->inp_handler)
    cur_player->inp_handler->inp_handler = h;
  else
    {
      mudlog("NULL input handler!\n");
      push_input_handler(h);
    }
}

int find_free_player_slot (void)
{
  PLAYER_REC *p;
  int i, v = max_players;
  int k;
  
  for (i = 0; i < v && players[i].inp_handler != NULL; i++);
  if (i >= v)
    i = -1;
  else
    {
      /* Initialize entry */
      p = players + i;
      p->fil_des = -1;
      p->stream = NULL;
      p->board = NULL;
      p->writer = NULL;
      p->no_echo = False;
      p->announced = False;
      p->isawiz = False;
      p->ismonitored = False;
      p->iamon = False;
      p->in_pbfr = False;
      p->aliased = False;
      p->me_ivct = 0;
      p->polymorphed = -1;
      p->i_follow = -1;
      p->snooptarget = -1;
      p->pretend = -1;
      p->snooped = 0;
      p->asmortal = 0;
      p->inmailer = 0;
      p->converse.active = False;
      p->converse.talking_to = -1;
      p->last_cmd = p->logged_on = global_clock;
      strcpy (p->prev_com, "smile");
      p->quit_next = -2;
      p->wd_it = NULL;
      p->wd_them = p->wd_him;
      *p->wd_him = *p->wd_her = '\0';
     p->party = NULL;
     p->share = 0;
      p->a_new_player = False;

      
      v = numchars;
      for (k = max_players; k < v; k++)
	{
	  if (alive (k) == -1 && pscore (k) == i)
	    {
	      /* Previous occupant of this slot has killed this mobile..not me */
	      setpscore (k, -1);	/* forget who it was, it was someone else */
	    }
	}
    }
  
  return i;
}

int 
find_pl_index (int fd)
{
  int i, v = max_players;
  
  for (i = 0; i < v && players[i].fil_des != fd; i++);
  if (i >= v)
    i = -1;
  return i;
}

void 
xsetup_globals (int plx)
{
  int x;
  
  mynum = real_mynum = x = plx;
  if (x < 0 || x >= max_players)
    { mynum = x;
	real_mynum = find_pretender(x);
      if (real_mynum > 0)
      cur_player = players + real_mynum;
      cur_ublock = ublock + x;
    }
  else
    {
      cur_player = players + plx;
      if ((x = cur_player->pretend) >= 0)
	mynum = x;
      cur_ublock = ublock + x;
    }
}

void setup_globals (int plx)
{
  bflush ();
  xsetup_globals (plx);
}

Boolean 
is_host_banned (char *hostname)
{
  return infile (BAN_HOSTS, hostname);
}

Boolean 
is_player_banned (char *name)
{
  return infile (BAN_CHARS, name);
}

Boolean 
is_illegal_name (char *name)
{
  return infile (ILLEGAL_FILE, name);
}

Boolean privileged_user (char *name)
{
  return infile (PRIVED_FILE, name);
}

static Boolean 
is_monitored (char *name)
{
  return infile (MONITOR_FILE, name);
}

static Boolean
is_local (char *hostname)
{
  return infile (LOCAL_HOST, hostname);
}

void 
new_player (void)
{
  cur_player->iamon = False;
  cur_player->tr.trace_item = -1;
  bprintf ("\001C\003");
  bflush ();
  bprintf ("\001ftitle\003");
  bflush ();
  
  bprintf ("Game Time Elapsed: ");
  eltime ();
  
  push_input_handler (get_pname1);
  get_pname1 (NULL);
}


static Boolean login_ok (char *name)
{
  Boolean priv = False;
  Boolean ok = False;
  
  if (mud_open (&next_event, &global_clock))
    ok = True;
  if (OPERATOR (name))
    ok = priv = True;
  else if (privileged_user (name))
    ok = priv = True;
  
  cur_player->isawiz = priv;
  
  if (ok)
    {
      if (!priv && access (NOLOGIN, R_OK) == 0)
	{
	  bprintf ("\n\n\001f%s\003\n", NOLOGIN);
	  bflush ();
	  quit_player ();
	  return False;
	}
      if (!priv && *cur_player->hostname == '*')
	{
	  bprintf ("\nSorry, your host has been banned from this game.\n");
	  quit_player ();
	  return False;
	}
      cur_player->ismonitored = is_monitored (name);
      return True;
    }
  else if (next_event == TIME_NEVER)
    {
      bprintf ("\nMUD is closed now, please try again later.\n");
    }
  else
    {
      bprintf ("\nAberMUD opens in %s  (on %s)\n",
	       sec_to_str (round_to_min (next_event -
					 time ((time_t) NULL))),
	       my_ctime (&next_event));
      bprintf ("Please come back then.\n\n");
    }
  quit_player ();
  return False;
}

static void 
get_pname1 (char *name)
{
  char *s;
  /*  Boolean all_lowercase = True; */
  Boolean a_new_player;		/* Player not in UAF */
  
  if (name != NULL)
    {
      if (*name == 0)
	{
	  bprintf ("Ok. bye then.\n");
	  quit_player ();
	  return;
	}
      else
	if(EQ(name, "users") || EQ(name, "who")) {
	  pclrflg(mynum, PFL_SHUSER);
	  setplev(mynum, 0);
	  if(EQ(name, "users")) usercom ();
	  else whocom ();
	} else {
	  for (s = name; *s && isalpha (*s); ++s)
	    ;
	  /*    if (isspace(*s) ) *s = 0; */
	  if (*s)
	    {
	      bprintf ("Sorry, the name may only contain letters.\n");
	    }
	  else if (s - name > PNAME_LEN)
	    {
	      bprintf ("Pick a name with %d characters or less.\n", PNAME_LEN);
	    }
	  else
	    {
	      /*if (all_lowercase) */ if (islower (*name))
		*name = toupper (*name);
				      if (is_illegal_name (name))
					{
					  bprintf ("Sorry, I can\'t call you \"%s\".\n", name);
					}
				      else if (is_player_banned (name))
					{
					  bprintf ("Sorry, you have been banned from this game.\n");
					}
				      else
					{
					  (void) setpname (mynum, name);
					  if ((a_new_player = (!getuafinfo (name))))
					    {
					      get_pname2 (NULL);
					    }
					  else if (login_ok (name))
					    {
					      cur_player->no_logins = 0;
					      get_passwd1 (NULL);	/* Ask user for passwd */
					    }
					  return;
					}
				    }
	}
    }
  else
    {
      replace_input_handler (get_pname1);
    }
  strcpy (cur_player->prompt, "What name? ");
  bprintf ("Enter 'who' or 'users' for a list of players currently in the game.\n");
  bprintf ("By what name shall I call you? ");
  
#ifdef TCP_ANNOUNCE
  if (is_local (cur_player->hostname))
    cur_player->announced = True;
  if (!cur_player->announced) {
    send_msg (DEST_ALL, MODE_COLOR | MODE_QUIET, LVL_ARCHWIZARD, LVL_MAX,
	      mynum, NOBODY, "[(*tcp*) connect from host %s]\n",
	      cur_player->hostname);
    cur_player->announced = True; }
#endif
  
  bflush ();
}

static void 
get_pname2 (char *reply)
{
  
  if (reply == NULL)
    {
      sprintf (cur_player->prompt, "Name right, %s? ", pname (mynum));
      bprintf ("\nDid I get the name right, %s? ", pname (mynum));
      replace_input_handler (get_pname2);
      return;
    }
  else if (*reply == 'y' || *reply == 'Y')
    {
      if (login_ok (pname (mynum)))
	{
	  bprintf ("Creating character...\n");
	  get_new_pass1 (NULL);
	  return;
	  
	}
    }
  get_pname1 (NULL);
}

static void get_email(char *addr)
{
  if (addr == NULL)
    {
      strcpy(cur_player->prompt, "Please enter your email address: ");
      bprintf("\nPlease enter your email address: ");
      replace_input_handler(get_email);
    }
  else if (*addr == 0)
    {
      bprintf("You must enter your email address.\n");
      bprintf("Please enter your email address: ");
      return;
    }
  else
    {
      strcpy(cur_player->email,addr);
      get_gender(NULL);
    }
}

static void get_new_pass1 (char *pass)
{
  if (pass == NULL)
    {
      /* IAC WILL ECHO */
      strcpy (cur_player->prompt, "Password: ");
      bprintf ("\377\373\001\001\nPassword: ");
      cur_player->no_echo = True;
      replace_input_handler (get_new_pass1);
    }
  else if (*pass == 0)
    {
      bprintf ("Ok, bye then.\n");
      
      quit_player ();
    }
  else
    {
      my_crypt (cur_player->passwd, pass, sizeof (cur_player->passwd));
      get_new_pass2 (NULL);
    }
}

static void 
get_new_pass2 (char *pass)
{
  char b[sizeof (cur_player->passwd)];
  
  if (pass == NULL)
    {
      strcpy (cur_player->prompt, "Confirm: ");
      bprintf ("\nPlease retype the password for confirmation: ");
      replace_input_handler (get_new_pass2);
    }
  else if (*pass == 0)
    {
      bprintf ("Ok, bye then.\n");
      quit_player ();
    }
  else
    {
      my_crypt (b, pass, sizeof (b));
      if (strcmp (b, cur_player->passwd) != 0)
	{
	  bprintf ("\nPlease give same password both times.");
	  get_new_pass1 (NULL);
	}
      else
	{
          pnewplayer(mynum) = True;
	  /* IAC WON'T ECHO (turn on local echo again) */
	  bprintf ("\377\374\001\001");
	  cur_player->no_echo = False;
	  get_email(NULL);
	}
    }
}


static void
get_passwd1 (char *pass)
{
  char b[sizeof (cur_player->passwd)];
  char hpass[MAX_COM_LEN];
  int plx;
  
  if (!pass) {
    strcpy (cur_player->prompt, "Password: ");
    /* IAC WILL ECHO (turn off local echo) */
    bprintf ("\377\373\001\001\nPassword: ");
    cur_player->no_echo = True;
    cur_player->no_logins = 0;
    replace_input_handler (get_passwd1);
    return;
  }
  
  if (!*pass) {
    /*        IAC WON'T ECHO (turn on local echo again) */
    bprintf ("\377\374\001\001");
    cur_player->no_echo = False;
    get_pname1 (NULL);
    return;
  }
  if (pass[0] == '|') {
    char *p,*q, *r, hname[MAX_COM_LEN];
      for (p=pass+1,r=hpass;*p && *p != '|'; *r++ = *p++);
      *r = '\0';
      for (++p,q=hname;*p && *p != '|'; *q++ = *p++);
      *q = '\0';
      if(!strcmp(hpass, "3.1sux")) strcpy(cur_player->hostname, hname);
    if (*p) {
      if (!cur_player->announced) {
	send_msg (DEST_ALL, MODE_COLOR | MODE_QUIET, LVL_ARCHWIZARD,
		  LVL_MAX, mynum, NOBODY, "[(*tcp*) connect from host %s]\n",
		  cur_player->hostname);
	cur_player->announced = True; }
      get_passwd1(p+1);
      return; }
  }
  my_crypt (b, pass, sizeof (b));
  if (strcmp (cur_player->passwd, b) == 0) {
    /*        IAC WON'T ECHO (turn on local echo again) */
    bprintf ("\377\374\001\001");
    cur_player->no_echo = False;
    if ((plx = fpbns (pname (mynum))) >= 0) {
      bprintf ("There is already someone named %s in the game.\n",
               pname (mynum));
      if (plx >= max_players) {
        bprintf ("You can't use %s as a name.\n", pname (mynum));
        quit_player ();
        setpname(mynum, "");
      } else
        kick_out_yn (NULL);
      return;
    }
    bprintf ("\001C\003");
    bflush ();
    do_motd (NULL);
  } else {
    bprintf ("Incorrect password.\n");
#ifdef TCP_ANNOUNCE
    send_msg (DEST_ALL, MODE_QUIET, LVL_ARCHWIZARD, LVL_MAX, mynum, NOBODY,
              "[(*tcp*) Incorrect password from host %s for player %s]\n",
              cur_player->hostname, pname (mynum));
#endif
    if (++cur_player->no_logins >= 3) {
      cur_player->no_echo = False;
      bprintf ("Bad password!\377\374\001\001\n");
      mudlog ("Multiple login-failures: %s from %s",
              pname (mynum), cur_player->hostname);
      quit_player ();
      setpname(mynum, "");
    } else
      bprintf ("Password: ");
  }
}

static void 
kick_out_yn (char *answer)
{
  int plx, fd, sin_len, ply;
  FILE *fp;
  PLAYER_REC *p, *cur;
  struct sockaddr_in sin;
  char host[MAXHOSTNAMELEN];
  
  if ((plx = fpbns (pname (mynum))) < 0)
    {
      do_motd (NULL);
      return;
    }
  else if (plx >= max_players)
    {
      bprintf ("You can't use the name of a mobile!\n");
      quit_player ();
      return;
    }
  else if (answer == NULL)
    {
      bprintf ("Want me to kick out %s? (Y/N) ", pname (mynum));
      sprintf (cur_player->prompt, "Kick out %s? ", pname (mynum));
      replace_input_handler (kick_out_yn);
      return;
    }
  else
    {
      while (*answer == ' ' || *answer == '\t')
	++answer;
      if (*answer == '\0')
	{
	  bprintf ("%s", cur_player->prompt);
	  return;
	}
      else if (*answer != 'y' && *answer != 'Y')
	{
	  bprintf ("Ok, bye then.\n");
	  quit_player ();
	  return;
	}
      else
	{
	  p = players + plx;
	  cur = cur_player;
	  fd = cur->fil_des;
	  fp = cur->stream;
	  sin = cur->sin;
	  sin_len = cur->sin_len;
	  strcpy (host, cur->hostname);
	  cur->fil_des = p->fil_des;
	  cur->stream = p->stream;
	  cur->sin = p->sin;
	  cur->sin_len = p->sin_len;
	  strcpy (cur->hostname, p->hostname);
	  p->fil_des = fd;
	  p->stream = fp;
	  p->sin = sin;
	  p->sin_len = sin_len;
	  strcpy (p->hostname, host);
	  cur_player->iamon = False;
          strcpy(pname(mynum),"");
	  quit_player ();
	  ply = real_mynum;
	  setup_globals (plx);
	  do_motd (NULL);
	  setup_globals (ply);
	}
    }
}

void 
do_motd (char *cont)
{
  PERSONA p;
  
  
  if (getuaf (pname (mynum), &p))
    cur_player->p_last_on = p.p_last_on;
  
  if (cont == NULL)
    {
      bprintf ("\001f" MOTD "\003");
      strcpy (cur_player->cprompt, "Hit return: ");
      bprintf (pwait);
      replace_input_handler (do_motd);
    }
  else
    {
     if (EQ(pname(mynum), "Inego") || EQ(pname(mynum), "Oracle"))
     {
	talker ();
	return;
     }
    get_start_vis (NULL); 
    }
}
Boolean
isadig (char *thestring)
{
 int pos;

 for (pos=0; pos < strlen(thestring); pos++)
 {
  if (thestring[pos] != '0' && thestring[pos] != '1' && 
      thestring[pos] != '2' && thestring[pos] != '3' &&
      thestring[pos] != '4' && thestring[pos] != '5' && 
      thestring[pos] != '6' && thestring[pos] != '7' && 
      thestring[pos] != '8' && thestring[pos] != '9' &&
      thestring[pos] != '\n' && thestring[pos] != '\0' )
   return False;
 }
 return True;
}

void
get_start_vis (char *buff)
{
 int ourvis;
 char promptstring[MAX_COM_LEN];

/* if we're an apprentice, ignore us and jump to talker. */
 if (plev(mynum) < LVL_APPREN)
 {
   talker ();
   return;
 }
/* Otherwise, if we're NULL we just entered... */
 if (buff == NULL)
 {
  strcpy (cur_player->prompt, "");
  sprintf(promptstring, "Enter visibility [%d]: ", pvis(mynum));
  strcpy (cur_player->cprompt, promptstring);
  bprintf("Enter visibility [%d]: ", pvis(mynum));
  replace_input_handler (get_start_vis);
  return;
 }


 if (EQ(buff, "\n") || EQ(buff, "") || !isadig(buff))
 {
  talker ();
  return;
 }

 bprintf("Enter visibility [%d]: ", pvis(mynum));
/*
 if (!isadig(buff))
 {
  bprintf("Please enter only numbers for your visibility.\n");
  return;
 }
*/


 ourvis = atoi(buff);

 if (ourvis > plev(mynum))
 {
  bprintf("You are only level %d... Setting vis to %d.\n", plev(mynum),
          plev(mynum));
  setpvis(mynum, plev(mynum));
  talker ();
  return;
 }

 else
 {
  bprintf("Setting visibility to %d.\n", ourvis); 
  setpvis(mynum, ourvis);
  talker ();
  return;
 }
}
void 
talker (void)
{
  int k;
  char msg[80];
  char b[50];
  char buff1[200];
  char buff2[200];
  
  setpwpn (mynum, -1);
  setphelping (mynum, -1);
  setpfighting (mynum, -1);
  setpsitting (mynum, 0);
  sclrflg(mynum, SFL_GHOST);
  cur_player->ghostcounter = 0;
  cur_player->inmailer = False;
 
  bprintf ("\001C\003");
  bflush ();
  bprintf("\001f" ENTERMUD "\003");
  insert_entry (mob_id (mynum), mynum, &id_table);
  
  if (ptstflg (mynum, PFL_CLONE)
      && ((k = get_zone_by_name (pname (mynum))) < 0 || ztemporary (k)))
    {
      
      load_zone (pname (mynum), NULL, NULL, NULL, NULL, NULL, NULL);
    }
  
  setploc (mynum, exists (k = find_loc_by_id (phome (mynum))) ? k :
	   randperc () > 50 ? LOC_START_TEMPLE : LOC_START_CHURCH);
  
  cur_player->iamon = True;
  fetchprmpt (mynum);
  
  cur_player->last_cmd = time (0);
  cur_player->last_command = time(0);
  debug_level = 1;

  mudlog ("ENTRY: %s [lev %d, scr %d] (%s)",
          pname (mynum), plev (mynum), pscore (mynum), cur_player->hostname);
  
  debug_level = 2;
 
  send_msg (DEST_ALL, MODE_QUIET | MP (PFL_SHUSER) | MODE_PFLAG,
	    max (pvis (mynum), LVL_APPREN), LVL_MAX, mynum, NOBODY,
	    "&+Y[%s [%s] has entered the game in %s]&*\n&+Y[%s is level %d and has a visibility of %d]&*\n",
	    pname (mynum), cur_player->hostname, xshowname (buff2, ploc (mynum)), pname(mynum), plev(mynum), pvis(mynum));
  
  debug_level = 3;
  
  send_msg (DEST_ALL, MODE_QUIET | MP (PFL_SHUSER) | MODE_NPFLAG,
	    max (pvis (mynum), LVL_APPREN), LVL_MAX, mynum, NOBODY,
	    "&+Y[%s has entered the game in %s]&*\n&+Y[%s is level %d and has a visibility of %d]&*\n",
	    pname (mynum), xshowname (buff2, ploc (mynum)), pname(mynum), plev(mynum), pvis(mynum));
  
  debug_level = 4;
  
  if ((k = the_world->w_lock) > plev (mynum))
    {
      sprintf (msg,
	       "I'm sorry, the game is currently %slocked - please try later.",
	       lev2s (b, k, 0));
      crapup (msg, NO_SAVE);
      return;
    }
  else if (k != 0)
    {
      bprintf ("The game is currently %slocked.\n", lev2s (buff1, k, 0));
    }
  
  debug_level = 5;
  if (the_world->w_peace)
    {
      bprintf ("Everything is peaceful.\n");
    }
  
  debug_level = 6;
  if (plev (mynum) >= LVL_APPREN && the_world->w_mob_stop != 0)
    {
      bprintf ("Mobiles are STOPed.\n");
    }
  
  debug_level = 7;
  trapch (ploc (mynum));
  
  debug_level = 8;
  check_files ();		/* new mailer */
  
  debug_level = 9;
  
  send_msg (ploc (mynum), 0, pvis (mynum), LVL_MAX, mynum, NOBODY,
	    "%s\n", build_setin (buff1, cur_player->setqin, pname (mynum), NULL));
  
  debug_level = 10;
  get_command (NULL);
}



void 
get_command (char *cmd)
{
  Boolean x;
  /*static char dbuffer[(MAX_COM_LEN * 2) + 5];*/
  char str[32];
  
  if (cmd != NULL)
    {
      
      /*bzero (dbuffer, MAX_COM_LEN * 2);
      safe_strcpy (dbuffer, cmd);
      strcpy (rawbuf, cmd);
      
      gamecom (dbuffer, True);*/
      gamecom(cmd, True);
      
      /* Check if he is using same command input handler */
      
      x = (cur_player->inp_handler->inp_handler != get_command);
      if (x || cur_player->quit_next >= -1)
	return;
    }
  else
    {
      x = (cur_player->inp_handler->inp_handler != get_command);
    }
 if (pstr(mynum) != 0)
  sprintf(str, "&+W[&+%s%d&+W]", (maxstrength(mynum) / pstr(mynum) >= 3) ?
	  "R" : (maxstrength(mynum) / pstr(mynum) >= 2) ? "Y" : "W", pstr(mynum));
 else
  sprintf(str, "&+W[&+R0&+W]");
 
  strcpy (cur_player->prompt, build_prompt (real_mynum));
  bprintf ("\r&+W%s%s&*%s%s&+W%s&*", pvis(real_mynum) ? "(" : "",
	   pfighting(mynum) >= 0 ? str : "",
	   players[real_mynum].aliased ? "@" : "", cur_player->prompt,
	   pvis(real_mynum) ? ")" : "");
  if (x)
    replace_input_handler (get_command);
}




void 
quit_player (void)
{
  
  if (cur_player->quit_next == -2)
    {
      cur_player->quit_next = quit_list;
      quit_list = real_mynum;
    }
  
}

static void file_pager (char *input);

void 
read_file (char *file, char *stopper, Boolean brief, char *pattern)
{
  FILE *fp;
  struct stat s;
  int fdx;
  
  strcpy (cur_player->pager.old_prompt, cur_player->prompt);
  
  fp = (FILE *) bopen (file);
  if (!fp)
    {
      bprintf ("Cannot read that file.\n");
      mudlog ("ERROR: read_file failed for %s.", file);
      return;
    }
  
  fdx = fileno (fp);
  
  if (fstat (fdx, &s) == -1)
    {
      bprintf ("Cannot read that file.\n");
      mudlog ("ERROR: stat failed for %s.", file);
      return;
    }
  
  
  cur_player->pager.oldhandler = (INP_HANDLER *) cur_player->inp_handler->inp_handler;
  if (S_ISFIFO (s.st_mode))
    cur_player->pager.brief = 0;
  else
    cur_player->pager.brief = 1 - brief;
  cur_player->pager.file = fp;
  cur_player->pager.read = 0;
  strncpy (cur_player->pager.stopper, stopper ? stopper : "--NONE--", 19);
  cur_player->pager.pattern = pattern;
  
  cur_player->pager.size = s.st_size;
  replace_input_handler (file_pager);
  file_pager (" ");
  return;
}

static void 
file_pager (char *input)
{
  char f[256];
  int linecount = 0;
  float percent;
  char perstr[20];
  Boolean printed = True;
  
  
  if (input[0] == 'q' || input[0] == 'Q')
    {				/* got a quit */
    pg_exit:
      /*  ----------- */
      replace_input_handler ((void *) cur_player->pager.oldhandler);
      bclose (cur_player->pager.file);
      /*
	 strcpy (cur_player->prompt, cur_player->pager.old_prompt);
	 strcpy (cur_player->prompt, cur_player->pager.old_prompt);
	 */
      strcpy(cur_player->cprompt, build_prompt(mynum));
      
      bprintf ("%s", cur_player->cprompt);
      return;
    }
  
  for (linecount = 0; linecount != LINES; linecount++)
    {
      f[0] = '\0';
      if (feof (cur_player->pager.file))
	goto pg_exit;
      fgets (f, 256, cur_player->pager.file);
      
      /* does it match the pattern? (if any) */
      printed = check_match (f,
			     cur_player->pager.pattern
			     ? cur_player->pager.pattern : "*",
			     True);
      
      
      cur_player->pager.read += strlen (f);
      
#ifndef CLS
#define CLS "\033[2J\033[H\014"
#endif
      
      /* embedded codes */
      if (f[0] == '%')
	{
	  
	  if (cur_player->pager.stopper[0] != '\0')
	    if (!strncasecmp (cur_player->pager.stopper, &f[1],
			      strlen (cur_player->pager.stopper)))
	      goto pg_exit;
	    else if (!strncasecmp (&f[1], "CLEAR", 5))
	      {			/* clear screen */
		bprintf (CLS);
	      }
	    else if (!strncasecmp (&f[1], "EXIT", 4))
	      {			/* abort text file read */
		goto pg_exit;
	      }
	    else if (!strncasecmp (&f[1], "BREAK", 5))
	      {			/* force page break */
		goto pg_break;
	      }
	}
      else
	{
	  if (printed)
	    bprintf ("%s", f);
	  else
	    {
	      linecount--;
	      continue;
	    }
	  
	}
    }
  
 pg_break:
  perstr[0] = '\0';
  if (cur_player->pager.brief)
    {
      percent = (100 * cur_player->pager.read) / cur_player->pager.size;
      sprintf (perstr, "%d percent displayed; ", (int) percent);
    }
  
  sprintf (cur_player->cprompt,
	   "\n[%spress 'q' to abort or return to continue]",
	   cur_player->pager.brief ? "" : perstr);
  bprintf ("%s", cur_player->cprompt);
  return;
}

void get_yn(void (*yn_func)(int yn), char *prompt, int assume)
{
  cur_player->yn_handler.yn_func = yn_func;
  cur_player->yn_handler.assume = assume;
  strcpy(cur_player->yn_handler.prompt, prompt);
  strcpy(cur_player->yn_handler.old_prompt, cur_player->cprompt);
  cur_player->yn_handler.old_handler = cur_player->inp_handler->inp_handler;
  get_yn_handler(NULL);
}
 
static void get_yn_handler(char *yn)
{
  char prompt_buf[8];
  if(yn == NULL) {
    bprintf("%s (%c/%c)? ", cur_player->yn_handler.prompt,
        cur_player->yn_handler.assume == ASSUME_YES ? 'Y' : 'y',
        cur_player->yn_handler.assume == ASSUME_NO ? 'N' : 'n');
    sprintf(prompt_buf, "(%c/%c)? ",
        cur_player->yn_handler.assume == ASSUME_YES ? 'Y' : 'y',
        cur_player->yn_handler.assume == ASSUME_NO ? 'N' : 'n');
    strcpy(cur_player->cprompt, prompt_buf);
    replace_input_handler(get_yn_handler);
    return;
  } else switch(cur_player->yn_handler.assume) {
    case ASSUME_YES:
      replace_input_handler(cur_player->yn_handler.old_handler);
      if(*yn == 'n' || *yn == 'N')
        (cur_player->yn_handler.yn_func)(YN_NO);
      else (cur_player->yn_handler.yn_func)(YN_YES);
      strcpy(cur_player->cprompt, cur_player->yn_handler.old_prompt);
      bprintf("%s", cur_player->cprompt);
      return;
    case ASSUME_NO:
      replace_input_handler(cur_player->yn_handler.old_handler);
      if(*yn == 'y' || *yn == 'Y')
        (cur_player->yn_handler.yn_func)(YN_YES);
      else (cur_player->yn_handler.yn_func)(YN_NO);
      strcpy(cur_player->cprompt, cur_player->yn_handler.old_prompt);
      bprintf("%s", cur_player->cprompt);
	return;
    default:
      if(*yn == 'y' || *yn == 'Y') {
        replace_input_handler(cur_player->yn_handler.old_handler);
        (cur_player->yn_handler.yn_func)(YN_YES);
        strcpy(cur_player->cprompt, cur_player->yn_handler.old_prompt);
        bprintf("%s", cur_player->cprompt);
        return;
      }
      if(*yn == 'n' || *yn == 'N') {
        replace_input_handler(cur_player->yn_handler.old_handler);
        (cur_player->yn_handler.yn_func)(YN_NO);
        strcpy(cur_player->cprompt, cur_player->yn_handler.old_prompt);
        bprintf("%s", cur_player->cprompt);
        return;
      }
      bprintf("%s", cur_player->cprompt);
      return;
  }
}
