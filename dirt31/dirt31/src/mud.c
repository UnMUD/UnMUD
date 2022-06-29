#include "kernel.h"
#include <sys/stat.h>
#include <unistd.h>

#include "sendsys.h"
#include "pflags.h"
#include "sflags.h"
#include "mud.h"
#include "uaf.h"
#include "mobile.h"
#include "timing.h"
#include "locations.h"

static Boolean login_ok(char *name);
static void get_pname1(char *name);
static void get_pname2(char *reply);
static void get_new_pass1(char *pass);
static void get_new_pass2(char *pass);
static void get_passwd1(char *pass);
static void kick_out_yn(char *answer);
static void talker(void);

#include "global.h"

char *pwait = "Press <Return> to continue...";

void push_input_handler(void (*h)(char *str))
{
  INP_HANDLER *i;

  i = NEW(INP_HANDLER,1);
  i->next = cur_player->inp_handler;
  i->inp_handler = h;
  cur_player->inp_handler = i;
}

void pop_input_handler(void)
{
  INP_HANDLER *i = cur_player->inp_handler;
  INP_HANDLER *j;

  j = i->next;
  if (j != NULL) {
    cur_player->inp_handler = j;
    free(i);
  }
}

void replace_input_handler(void (*h)(char *str))
{
  cur_player->inp_handler->inp_handler = h;
}
 
int find_free_player_slot(void)
{
  PLAYER_REC *p;
  int i, v = max_players;
  int k;

  for (i = 0; i < v && players[i].inp_handler != NULL; i++);
  if (i >= v) i = -1;
  else {
    /* Initialize entry */
    p = players + i;
    p->fil_des     = -1;
    p->stream      = NULL;
    p->no_echo     = False;
    p->isawiz      = False;
    p->ismonitored = False;
    p->iamon       = False;
    p->in_pbfr     = False;
    p->aliased     = False;
    p->me_ivct     = 0;
    p->polymorphed = -1;
    p->i_follow    = -1;
    p->snooptarget = -1;
    p->pretend     = -1;
    p->snooped     = 0;
    p->asmortal    = 0;
    p->last_cmd    = p->logged_on = global_clock;
    strcpy(p->prev_com, "quit");
    p->quit_next   = -2;
    p->wd_it       = NULL;
    p->wd_them     = p->wd_him;
    *p->wd_him     = *p->wd_her = '\0';

    v = numchars;
    for (k = max_players; k < v; k++) {
      if (alive(k) == -1 && pscore(k) == i) {
	/* Previous occupant of this slot has killed this mobile..not me */
	setpscore(k, -1); /* forget who it was, it was someone else */
      }
    }
  }

  return i;
}

int find_pl_index(int fd)
{
  int i, v = max_players;

  for (i = 0; i < v && players[i].fil_des != fd; i++);
  if (i >= v) i = -1;
  return i;
}

void xsetup_globals(int plx)
{
  int x;

  mynum = real_mynum = x = plx;
  if (x < 0 || x >= max_players) {
    cur_player = NULL;
    cur_ublock = NULL;
    mynum = real_mynum = -1;
  } else {
    cur_player = players + plx;
    if ((x = cur_player->pretend) >= 0) mynum = x;
    cur_ublock = ublock + x;
  }
}

void setup_globals(int plx)
{
  bflush();
  xsetup_globals(plx);
}

Boolean is_host_banned(char *hostname)
{
  return infile(BAN_HOSTS,hostname);
}

Boolean is_player_banned(char *name)
{
  return infile(BAN_CHARS,name);
}

Boolean is_illegal_name(char *name)
{
  return infile(ILLEGAL_FILE,name);
}

static Boolean privileged_user(char *name)
{
  return infile(PRIVED_FILE, name);
}

static Boolean is_monitored(char *name)
{
  return infile(MONITOR_FILE, name);
}




void new_player(void)
{
  struct stat sb;

  cur_player->iamon = False;
  bprintf("\001C                A B E R M U D \n\n");
  bflush();

  bprintf("Game Time Elapsed: ");
  eltime();

  push_input_handler(get_pname1);
  get_pname1(NULL);
}


static Boolean login_ok(char *name)
{
  Boolean priv = False;
  Boolean ok = False;

  if (mud_open(&next_event, &global_clock)) ok = True;
  if (OPERATOR(name)) ok = priv = True;
  else if (privileged_user(name)) ok = priv = True;

  cur_player->isawiz = priv;

  if (ok) {
    if (!priv && access( NOLOGIN, R_OK) == 0) {
      bprintf("\n\n\001f%s\003\n", NOLOGIN);
      bflush();
      quit_player();
      return False;
    }
    if (!priv && *cur_player->hostname == '*') {
      bprintf( "\nSorry, your host has been banned from this game.\n" );
      quit_player();
      return False;
    }
    cur_player->ismonitored = is_monitored(name);
    return True;
  } else if (next_event == TIME_NEVER) {
    bprintf("\nMUD is closed now, please try again later.\n");
  } else {
    bprintf("\nAberMUD opens in %s  (on %s)\n",
	    sec_to_str(round_to_min(next_event -
				    time((time_t)NULL)) ),
	    my_ctime( &next_event ));
    bprintf("Please come back then.\n\n");
  }
  quit_player();
  return False;
}

static void get_pname1(char *name)
{
  char *s;
/*  Boolean all_lowercase = True;*/
  Boolean a_new_player; /* Player not in UAF */
  int plx;

  if (name != NULL) {
    if (*name == 0) {
      bprintf( "Ok. bye then.\n" );
      quit_player();
      return;
    } else {
      for (s = name; *s && isalpha(*s); ++s)
          ;
/*    if (isspace(*s) ) *s = 0;*/
      if (*s) {
	bprintf( "Sorry, the name may only contain letters.\n");
      } else if (s - name > PNAME_LEN) {
	bprintf( "Pick a name with %d characters or less.\n", PNAME_LEN);
      } else {
	/*if (all_lowercase)*/  if (islower(*name)) *name = toupper(*name);
	if (is_illegal_name(name)) {
	  bprintf("Sorry, I can\'t call you \"%s\".\n", name);
	} else if (is_player_banned(name)) {
	  bprintf( "Sorry, you have been banned from this game.\n");
	} else {
	  (void)setpname(mynum, name);
	  if (a_new_player = !getuafinfo(name)) {
	    get_pname2(NULL);
	  } else if (login_ok(name)) {
	    cur_player->no_logins = 0;
	    get_passwd1(NULL); /* Ask user for passwd */
	  }
	  return;
	}
      }
    }
  } else {
    replace_input_handler(get_pname1);
  }
  strcpy(cur_player->cprompt,"What name? ");
  bprintf( "\nBy what name shall I call you? " );
  bflush();
}

static void get_pname2(char *reply)
{
  char b[100];
  char bb[140];

  if (reply == NULL) {
    sprintf( cur_player->cprompt, "Name right, %s? ", pname(mynum));
    bprintf( "\nDid I get the name right, %s? ", pname(mynum));
    replace_input_handler(get_pname2);
    return;
  } else if (*reply == 'y' || *reply == 'Y') {
    if (login_ok(pname(mynum))) {
      bprintf("Creating character...\n");
      get_new_pass1(NULL);
      return;
    }
  }
  get_pname1(NULL);
}

static void get_new_pass1(char *pass)
{
  if (pass == NULL) {
    /* IAC WILL ECHO */
    strcpy(cur_player->cprompt, "Password: ");
    bprintf( "\377\373\001\001\nPassword: " );
    cur_player->no_echo = True;
    replace_input_handler(get_new_pass1);
  } else if (*pass == 0) {
    bprintf( "Ok, bye then.\n" );
    quit_player();
  } else {
    my_crypt(cur_player->passwd,pass,sizeof(cur_player->passwd));
    get_new_pass2(NULL);
  }
}

static void get_new_pass2(char *pass)
{
  char b[sizeof(cur_player->passwd)];

  if (pass == NULL) {
    strcpy( cur_player->cprompt, "Confirm: ");
    bprintf( "\nPlease retype the password for confirmation: " );
    replace_input_handler(get_new_pass2);
  } else if (*pass == 0) {
    bprintf( "Ok, bye then.\n" );
    quit_player();
  } else {
    my_crypt(b,pass,sizeof(b));
    if (strcmp(b,cur_player->passwd) != 0) {
      bprintf( "\nPlease give same password both times." );
      get_new_pass1(NULL);
    } else {
      /* IAC WON'T ECHO (turn on local echo again) */
      bprintf("\377\374\001\001");
      cur_player->no_echo = False;
      get_gender(NULL);
    }
  }
}
    
static void get_passwd1(char *pass)
{
  char b[sizeof(cur_player->passwd)];
  int plx;

  if (pass == NULL) {
    strcpy(cur_player->cprompt, "Password: ");
    /* IAC WILL ECHO (turn off local echo) */
    bprintf( "\377\373\001\001\nPassword: " );
    cur_player->no_echo = True;
    cur_player->no_logins = 0;
    replace_input_handler(get_passwd1);
  } else if (*pass == 0) {
    /*        IAC WON'T ECHO (turn on local echo again) */
    bprintf( "\377\374\001\001Ok, bye then.\n" );
    cur_player->no_echo = False;
    quit_player();
  } else {
    my_crypt(b,pass,sizeof(b));
    if (strcmp(cur_player->passwd,b) == 0) {
      /*        IAC WON'T ECHO (turn on local echo again) */
      bprintf( "\377\374\001\001" ); 
      cur_player->no_echo = False;
      if ( (plx = fpbns(pname(mynum))) >= 0) {
	bprintf( "There is already someone named %s in the game.\n",
		pname(mynum));
	if (plx >= max_players) {
	  bprintf( "You can't use %s as a name.\n", pname(mynum));
	  quit_player();
	} else {
	  kick_out_yn(NULL);
	}
	return;
      }

      do_motd(NULL);
    } else {
      bprintf( "Incorrect password.\n" );
      if (++cur_player->no_logins >= 3) {
	cur_player->no_echo = False;
	bprintf( "Bad password!\377\374\001\001\n" );
	mudlog("Multiple login-failures: %s from %s",
	       pname(mynum),cur_player->hostname);
	quit_player();
      } else {
	bprintf( "Password: " );
      }
    }
  }
}

static void kick_out_yn(char *answer)
{
  int plx, fd, sin_len, ply;
  FILE *fp;
  PLAYER_REC *p, *cur;
  struct sockaddr_in sin;
  char host[MAXHOSTNAMELEN];

  if ( (plx = fpbns(pname(mynum))) < 0) {
    do_motd(NULL);
    return;
  } else if (plx >= max_players) {
    bprintf( "You can't use the name of a mobile!\n" );
    quit_player();
    return;
  } else if (answer == NULL) {
    bprintf( "Want me to kick out %s? (Y/N) ", pname(mynum));
    sprintf(cur_player->cprompt,"Kick out %s? ", pname(mynum));
    replace_input_handler(kick_out_yn);
    return;
  } else {
    while (*answer == ' ' || *answer == '\t') ++answer;
    if (*answer == '\0') {
      bprintf("%s", cur_player->cprompt);
      return;
    } else if (*answer != 'y' && *answer != 'Y') {
      bprintf( "Ok, bye then.\n" );
      quit_player();
      return;
    } else {
      p = players + plx;
      cur = cur_player;
      fd = cur->fil_des;
      fp = cur->stream;
      sin = cur->sin;
      sin_len = cur->sin_len;
      strcpy(host,cur->hostname);
      cur->fil_des = p->fil_des;
      cur->stream = p->stream;
      cur->sin = p->sin;
      cur->sin_len = p->sin_len;
      strcpy(cur->hostname,p->hostname);
      p->fil_des = fd;
      p->stream = fp;
      p->sin = sin;
      p->sin_len = sin_len;
      strcpy(p->hostname,host);
      quit_player();
      ply = real_mynum;
      setup_globals(plx);
      do_motd(NULL);
      setup_globals(ply);
    }
  }
}

void do_motd(char *cont)
{
  if (cont == NULL) {
    bprintf( "\n\001C\001f" MOTD "\003" );
    strcpy(cur_player->cprompt, "Hit return: ");
    bprintf(pwait);
    replace_input_handler(do_motd);
  } else {
    talker();
  }
}

void talker(void)
{
  int  k;
  char *s;
  char msg[80];
  char b[50];
  char buff1[200];
  char buff2[200];

  setpwpn(mynum, -1);
  setphelping(mynum, -1);
  setpfighting(mynum, -1);
  setpsitting(mynum, 0);

  insert_entry(mob_id(mynum), mynum, &id_table);

  if (ptstflg(mynum, PFL_CLONE)
      && ((k = get_zone_by_name(pname(mynum))) < 0 || ztemporary(k))) {

	  load_zone(pname(mynum), NULL, NULL, NULL, NULL, NULL, NULL);
  }

  setploc(mynum, exists(k = find_loc_by_id(phome(mynum))) ? k :
	  randperc() > 50 ? LOC_START_TEMPLE : LOC_START_CHURCH);

  cur_player->iamon = True;
  fetchprmpt(mynum);

  mudlog("ENTRY: %s [lev %d, scr %d] (%s)",
	 pname(mynum), plev(mynum), pscore(mynum), cur_player->hostname);

  send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR|MP(PFL_SHUSER)|MODE_PFLAG,
	   max(pvis(mynum), LVL_WIZARD),
	   LVL_MAX, mynum, NOBODY,
	   "[%s [%s] has entered the game in %s]\n",
	   pname(mynum), cur_player->hostname, xshowname(buff2, ploc(mynum)));

  send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR|MP(PFL_SHUSER)|MODE_NPFLAG,
	   max(pvis(mynum), LVL_WIZARD),
	   LVL_MAX, mynum, NOBODY,
	   "[%s has entered the game in %s]\n",
	   pname(mynum), xshowname(buff2, ploc(mynum)));

  if ((k = the_world->w_lock) > plev(mynum)) {
    sprintf(msg,
	    "I'm sorry, the game is currently %slocked - please try later.",
	    lev2s(b,k));
    crapup(msg, NO_SAVE);
    return;
  } else if (k != 0) {
    bprintf("The game is currently %slocked.\n", lev2s(buff1,k));
  }

  if (the_world->w_peace) {
    bprintf("Everything is peaceful.\n");
  }

  if (plev(mynum) >= LVL_WIZARD && the_world->w_mob_stop != 0) {
    bprintf("Mobiles are STOPed.\n");
  }

  trapch( ploc(mynum) );

  if (ststflg(mynum,SFL_MAIL)) {
    bprintf("\n**You have mail**\n");
  }

  send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
	   "%s\n", build_setin(buff1, cur_player->setqin, pname(mynum), NULL));

  get_command(NULL);
}



void get_command(char *cmd)
{
  Boolean x;

  if (cmd != NULL) {

    gamecom(cmd,True);
    /* Check if he is using same command input handler */
    x = (cur_player->inp_handler->inp_handler != get_command);
    if (x || cur_player->quit_next >= -1) return;
  } else {
    x = (cur_player->inp_handler->inp_handler != get_command);
  }

  strcpy(cur_player->cprompt, build_prompt(real_mynum));
  bprintf( "\r%s", cur_player->cprompt);
  if (x) replace_input_handler(get_command);
}



void quit_player(void)
{
  if (cur_player->quit_next == -2) {
     cur_player->quit_next = quit_list;
     quit_list = real_mynum;
  }
}


