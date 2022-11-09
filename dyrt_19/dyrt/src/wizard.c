#include "kernel.h"
#include <time.h>
#include "levels.h"
#ifdef VARGS
#include <stdarg.h>
#endif
#include "verbs.h"
#include "locations.h"
#include "stdinc.h"
#include "objects.h"
#include "mobiles.h"

extern char *Oflags[];
static void _wiz (int level, char *text);

void saveallcom()
{
  int lv;
  int rmynum;
  
  send_msg(DEST_ALL, MODE_COLOR|MODE_QUIET, LVL_APPREN, LVL_MAX, mynum,
	   NOBODY, "[\001p%s\003 has SAVEALL'd]\n", pname(mynum));
  rmynum = mynum;
  for (lv = 0; lv < max_players; lv++)
    if (players[lv].iamon && plev(lv) < LVL_APPREN)
      {
	setup_globals(lv);
	saveme();
      }
  setup_globals(rmynum);
  mynum = rmynum;
}

void syslog_prompt (char *input)
{
  int i, j, lines=0, matched;
  char in_line[80];
  
  if (*input == 'q' || *input == 'Q') {
    if (cur_player->syslog_popened) pclose (cur_player->syslog_fp);
    else fclose (cur_player->syslog_fp);
    get_command (NULL);
    return;
  }
  if (*input == '*') {
    gamecom (input+1,False);
    return;
  }
  while (!feof(cur_player->syslog_fp) && lines < 22) {
    if (!fgets(in_line, sizeof(in_line), cur_player->syslog_fp)) {
      if (cur_player->syslog_popened) pclose (cur_player->syslog_fp);
      else fclose (cur_player->syslog_fp);
      get_command (NULL);
      return;
    }
    matched = 1;
    if (!cur_player->syslog_sameline)
      for (i = 0; matched && i < 10 && cur_player->syslog_match[i][0]; i++) {
        if (*cur_player->syslog_match[i] == '*')
          continue;
        for (j = 0; in_line[j] &&
             strncasecmp(in_line+j, cur_player->syslog_match[i],
                         strlen(cur_player->syslog_match[i]));
             j++);
        if (!in_line[j])
          matched = 0;
      }
    if (in_line[strlen(in_line)-1] == '\n')
      cur_player->syslog_sameline = 0;
    else if (matched)
      cur_player->syslog_sameline = 1;
    if (matched) {
      lines += strlen(in_line) / 80 + 1;
      bprintf("%s", in_line);
    }
  }
  if (lines >= 22)
    bprintf ("Press [RETURN] to continue or 'q' to quit...");
  else {
    if (cur_player->syslog_popened) pclose (cur_player->syslog_fp);
    else fclose (cur_player->syslog_fp);
    get_command (NULL);
  }
}

void syslogcom(void)
{
  char buff[50];
  sprintf (buff, "%s", LOG_FILE);
  filehandle (buff,1);
}



void cfrcom()
{
  char x[MAX_COM_LEN], y[MAX_COM_LEN];
  
  if (plev(mynum) < LVL_APPREN)
    {
      erreval();
      return;
    }
  getreinput(x);
  sprintf(y, "&+r** CALL FOR RESET by %s :&* %s\a\a\n", pname(mynum), x);
  broad(y);
}

void filehandle(char *filename, int mode)
{
  int i, j, k, l, lines=0, matched, sameline=0, endl=0;
  time_t t;
  char *tmp;
  char in_line[80], buf[80];
  char *pattern[11], realpattern[10][80];
  FILE *fp;
  
  if (!ptstflg(mynum, PFL_SYSLOG)) {
    erreval();
    return;
  }
  bzero(buf, sizeof(buf));
  bzero (realpattern, sizeof(realpattern));
  getreinput(buf);
  if (mode == 0) {
    i = 0;
    while ((buf[i] != ' ') && (buf[i] != '\0'))
      i++;
  }
  else
    i = 0;
  for (; i < MAX_COM_LEN && buf[i] != '\0' && buf[i] == ' '; i++);
  if (buf[i] == '\0') {
    time (&t);
    tmp = ctime (&t);
    tmp[10] = '\0';
    if (mode == 1)
      strcpy (realpattern[0], tmp);
  } else {
    if (isdigit (buf[i])) {
      for (j=0; isdigit(buf[i+j]); j++);
      if (!buf[i+j] || isspace(buf[i+j])) {
        if (!(endl = atoi (buf+i))) {
          bprintf ("You can't start your search 0 lines from the end.\n");
          return;
        }
        bprintf ("Limiting search to %d most recent records.\n", endl);
        i += j;
        lines++;
      }
    }
    pattern[0] = (char *)strtok(buf+i, "|");
    j = mode;
    while (j < 10 && (pattern[j++] = (char *)strtok(NULL, "|")));
    if (j >= 10) {
      bprintf ("The number of search patterns is limited to ten. (for future reference)\n");
      lines++;
    }
    for (; j < 10; j++)
      pattern[j] = NULL;
    for (i = 0, j = 0; i < 10; i++) {
      for (k = 0; pattern[i] && pattern[i][k] == ' '; k++);
      for (l = 0; pattern[i] && pattern[i][k]; k++,l++)
        realpattern[j][l] = pattern[i][k];
      if (l) {
        realpattern[j][l] = '\0';
        j++;
      }
    }
    for (i = 0; i < 10 && realpattern[i]; i++) {
      for (j = strlen(realpattern[i]) - 1;
           j > 0 && isspace(realpattern[i][j]); j--);
      realpattern[i][j+1] = '\0';
    }
    if (!realpattern[0][0])
      strcpy (realpattern[0], "*");
  }
  bprintf("Searching for:\n[%s]", realpattern[0]);
  for (i = 1; i < 10 && realpattern[i][0]; i++)
    bprintf (" [%s]", realpattern[i]);
  bprintf("\n");
  lines++;
  if (endl) {
    sprintf (buf, "tail -%d %s", endl, filename);
    if (!(fp = popen (buf, "r"))) {
      bprintf("Could not open file to check.  Please let someone know.\n");
      return;
    }
  } else if (!(fp = fopen(filename, "r"))) {
    bprintf("Could not open file to check.  Please let someone know.\n");
    return;
  }
  while (!feof(fp) && lines < 22) {
    if (!fgets(in_line, sizeof(in_line), fp)) {
      if (endl) pclose (fp);
      else fclose(fp);
      return;
    }
    matched = 1;
    if (!sameline)
      for (i = 0; matched && i < 10 && realpattern[i][0]; i++) {
        if (*realpattern[i] == '*')
          continue;
        for (j = 0; in_line[j] &&
	     strncasecmp(in_line+j, realpattern[i], strlen(realpattern[i]));
             j++);
        if (!in_line[j])
          matched = 0;
      }
    if (in_line[strlen(in_line)-1] == '\n')
      sameline = 0;
    else if (matched)
      sameline = 1;
    if (matched) {
      lines += strlen(in_line) / 80 + 1;
      bprintf("%s", in_line);
    }
  }
  if (lines >= 22) {
    cur_player->syslog_fp = fp;
    for (i = 0; i < 10; i++)
      strcpy (cur_player->syslog_match[i], realpattern[i]);
    cur_player->syslog_sameline = sameline;
    bprintf ("Press [RETURN] to continue or 'q' to quit...");
    if (endl) cur_player->syslog_popened = True;
    else cur_player->syslog_popened = False;
    replace_input_handler (syslog_prompt);
  } else
    if (endl) pclose (fp);
    else fclose(fp);
}

/* The SET command */
/* These properties require numbers: */
#define SET_BVALUE  0
#define SET_SIZE    1
#define SET_VIS     2
#define SET_DAMAGE  3
#define SET_ARMOR   4
#define SET_STATE   5

/* These require texts: */
#define SET_TEXT_MIN 5
#define SET_DESC0  6
#define SET_DESC1  7
#define SET_DESC2  8
#define SET_DESC3  9
#define SET_NAME   10
#define SET_ANAME  11
#define SET_TEXT_MAX 11

/* Properties */
static char *Props[] =
{"BaseValue", "Size", "Visibility", "Damage",
   "Armor", "State", "Desc0", "Desc1",
   "Desc2", "Desc3", "Name", "AltName",
   TABLE_END
};

void 
setcom ()
{
  int o, p, v;			/* Object, Property, Value */
  
  Boolean is_oflag;
  
  if (!ptstflg (mynum, PFL_OBJECT))
    {
      erreval ();
      return;
    }
  
  if (brkword () == -1 || (o = fobn (wordbuf)) == -1)
    {
      bprintf ("Set what??\n");
      return;
    }
  
  if (brkword () == -1 || ((is_oflag = (p = tlookup (wordbuf, Props)) == -1)
			   && (p = tlookup (wordbuf, Oflags)) == -1))
    {
      
      bprintf ("Set what property on the %s?\n", oname (o));
      return;
    }
  
  if ((is_oflag || p <= SET_TEXT_MIN || p > SET_TEXT_MAX)
      && brkword () == -1)
    {
      
      bprintf ("Set the %s property to what ??\n",
	       is_oflag ? Oflags[p] : Props[p]);
      return;
    }
  
  if (is_oflag)
    {
      
      if ((v = tlookup (wordbuf, OO)) == -1
	  && (v = tlookup (wordbuf, TF)) == -1)
	{
	  
	  bprintf ("Value must be On or Off (or True/False).\n");
	  return;
	}
      
      if (v)
	{
	  osetbit (o, p);
	}
      else
	{
	  oclrbit (o, p);
	}
    }
  else if (p <= SET_TEXT_MIN || p > SET_TEXT_MAX)
    {
      
      int limit = -1;
      
      if (!isdigit (*wordbuf))
	{
	  bprintf ("Value must be a number >= 0.\n");
	  return;
	}
      else
	v = atoi (wordbuf);
      
      switch (p)
	{
	case SET_BVALUE:
	  if (v > O_BVALUE_MAX ||
	      (v > obaseval (o) && v > 400
	       && !ptstflg (mynum, PFL_FROB)))
	    limit = O_BVALUE_MAX;
	  else
	    osetbaseval (o, v);
	  break;
	case SET_SIZE:
	  if (v > O_SIZE_MAX)
	    limit = O_SIZE_MAX;
	  else
	    osetsize (o, v);
	  break;
	case SET_VIS:
	  if (v > O_VIS_MAX)
	    limit = O_VIS_MAX;
	  else
	    osetvis (o, v);
	  break;
	case SET_DAMAGE:
	  if (v > O_DAM_MAX)
	    limit = O_DAM_MAX;
	  else
	    osetdamage (o, v);
	  break;
	case SET_ARMOR:
	  if (v > O_ARMOR_MAX)
	    limit = O_ARMOR_MAX;
	  else
	    osetarmor (o, v);
	  break;
	case SET_STATE:
	  if (v > omaxstate (o))
	    limit = omaxstate (o);
	  else
	    setobjstate (o, v);
	  break;
	default:
	  bprintf ("Internal error\n");
	  mudlog ("Internal errror in setcom(): p = %d", p);
	  return;
	}
      
      if (limit > -1)
	{
	  bprintf ("Sorry, value must be <= %d.\n", limit);
	  return;
	}
      
    }
  else
    {
      char **q;
      
      if (opermanent (o))
	{
	  bprintf ("You can only change %s on non-permanent"
		   " (wiz-created) objects.\n", Props[p]);
	  return;
	}
      
      q = p == SET_DESC0 ? &olongt (o, 0)
	: p == SET_DESC1 ? &olongt (o, 1)
	  : p == SET_DESC2 ? &olongt (o, 2)
	    : p == SET_DESC3 ? &olongt (o, 3)
	      : p == SET_NAME ? &oname (o)
		: p == SET_ANAME ? &oaltname (o)
		  : NULL;
      
      if (q == NULL)
	{
	  bprintf ("Internal Error\n");
	  mudlog ("Internal error in setcom(), p = %d", p);
	  return;
	}
      
      if (strchr (getreinput (wordbuf), '^') != NULL)
	{
	  bprintf ("Illegal character(s) (^) in text.\n");
	  return;
	}
      
      if (p == SET_NAME || p == SET_ANAME)
	{
	  char *s = wordbuf;
	  
	  if (strlen (wordbuf) > ONAME_LEN)
	    {
	      bprintf ("Name too long. Max = %d chars.\n",
		       ONAME_LEN);
	      return;
	    }
	  
	  while (*s != '\0' && isalpha (*s))
	    s++;
	  
	  if (*s != '\0')
	    {
	      bprintf ("Name must only contain latters.\n");
	      return;
	    }
	  
	  if (EMPTY (wordbuf))
	    {
	      if (p == SET_ANAME)
		strcpy (wordbuf, "<null>");
	      else
		{
		  bprintf ("Name is missing.\n");
		  return;
		}
	    }
	}
      
      if (*q != NULL)
	free (*q);
      
      *q = COPY (wordbuf);
    }
  bprintf ("Ok.\n");
}

/* The SAVESET command.  */
void saveset ()
{
  SETIN_REC s;
  
  if (plev (mynum) < LVL_APPREN)
    {
      erreval ();
      return;
    }
  
  if (cur_player->aliased || cur_player->polymorphed != -1)
    {
      bprintf ("Not while aliased.\n");
      return;
    }
  strcpy (s.name, pname (mynum));
  strcpy (s.prompt, cur_player->prompt);
  strcpy (s.setin, cur_player->setin);
  strcpy (s.setout, cur_player->setout);
  strcpy (s.setmin, cur_player->setmin);
  strcpy (s.setmout, cur_player->setmout);
  strcpy (s.setvin, cur_player->setvin);
  strcpy (s.setvout, cur_player->setvout);
  strcpy (s.setqin, cur_player->setqin);
  strcpy (s.setqout, cur_player->setqout);
  strcpy (s.setsit, cur_player->setsit);
  strcpy (s.setstand, cur_player->setstand);
  strcpy (s.settrenter, cur_player->settrenter);
  strcpy (s.settrvict, cur_player->settrvict);
  strcpy (s.settrroom, cur_player->settrroom);
  putsetins (pname (mynum), &s);
  bprintf ("Saving &+Wprompt&* and &+Wset*in/out&* messages.\n");
}

void exorcom ()
{
  int x, q, y = real_mynum;
  
  if (!ptstflg (mynum, PFL_EXOR))
    {
      bprintf ("You couldn't exorcise your way out of a paper bag.\n");
      return;
    }
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_DEMI)) {
    bprintf ("You can't exorcise from here\n");
    return;
   }
  if (EMPTY (item1))
    {
      bprintf ("Exorcise who?\n");
      return;
    }
  if ((x = pl1) == -1)
    {
      bprintf ("They aren't playing.\n");
      return;
    }
  if (!do_okay (mynum, x, PFL_NOEXOR))
    {
      bprintf ("They don't want to be exorcised.\n");
      return;
    }
#ifdef LOG_EXORCISE
  mudlog ("%s EXORCISEd %s", pname (mynum), pname (x));
#endif
  
  if (EQ("Oracle", pname(pl1)))
    {
      bprintf("The Oracle does not want to be exo'd.\n");
      return;
    }
  bprintf("You have &+GEXORCISED&* the demon!\n"); 
  send_msg (DEST_ALL, MODE_QUIET, pvis (x), LVL_MAX, x, NOBODY,
	    "%s has been kicked off.\n", pname (x));
  
  if ((q = find_pretender (x)) >= 0)
    {
      sendf (q, "You have been kicked off!\n");
      unalias (q);
      unpolymorph (q);
    }
  
  if (x >= max_players)
    {
      
      dumpstuff (x, ploc (x));
      setpfighting (x, -1);
      
      if (!ppermanent (x))
	{
	  destruct_mobile (x, NULL);
	}
      else
	{
	  setpname (x, "");
	}
      
    }
  else
    {
      
      setup_globals (x);
      crapup ("\t&+RYou have been kicked off!&*", CRAP_SAVE | CRAP_RETURN);
      setup_globals (y);
    }
}


void 
setstart (void)
{
  PERSONA d;
  int loc, p;
  char buff[80];
  char *who;
  char *where;
  int locid;
  
  if (plev (mynum) < LVL_APPREN)
    {
      erreval ();
      return;
    }
 if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_DEMI)) {
   bprintf ("Almost forgot about this one...almost\n");
   return;
  }
  
  /* Parse the command, find *WHO*'s home we want to change and *WHERE*
   * * to change it to.
   */
  if (brkword () == -1)
    {
      who = pname (mynum);
      where = NULL;
    }
  else if (getuaf (strcpy (buff, wordbuf), &d))
    {
      who = d.p_name;
      where = (brkword () == -1) ? NULL : wordbuf;
      
      if (!EQ (who, pname (mynum)))
	{
	  if (!ptstflg (mynum, PFL_UAF))
	    {
	      bprintf ("You can only set your own start-location.\n");
	      return;
	    }
	  if (!do_okay_l (plev (mynum), d.p_level, False))
	    {
	      bprintf ("That is beyond your powers.\n");
	      return;
	    }
	}
    }
  else if (brkword () == -1)
    {
      who = pname (mynum);
      where = buff;
    }
  else
    {
      bprintf ("No such player: %s\n", buff);
      return;
    }
  
  /* Got the arguments. If the operation is not 'erase home' (where=null),
   * * see if the argument corresponds to a real room.
   */
  if (where == NULL)
    {
      loc = 0;
    }
  else if ((loc = findroomnum (where)) == 0 || !exists (loc))
    {
      bprintf ("No such location.\n");
      return;
    }
  
#ifdef LOCMIN_RHOME
  if ((loc == LOC_RHOME_15) && (!EQ(pname(mynum), "Rex")))
    {
      bprintf("You can't set your start to this room.\n");
      return;
    }
#endif
  
  locid=loc;
  if (exists (loc))
    locid = loc_id (loc);
  
  /*  Got the room number. Finally, set the home for the player.  */
  
  if (!checklevel(mynum, loc))
    return;
  
  if ((p = fpbns (who)) > -1)
    {
      setphome (p, locid);
    }
  else
    {
      d.p_home = locid;
      putuaf (&d);
    }
  bprintf ("Ok.\n");
}


void 
showlocation (int o)
{
  int uc, gotroom;
  
  uc = 1;
  gotroom = 0;
  while (!gotroom)
    {
      switch (ocarrf (o))
	{
	case IN_ROOM:
	  o = oloc (o);
	  gotroom = 1;
	  break;
	case IN_CONTAINER:
	  bprintf (" &+R%cnside the %s&*", (uc ? 'I' : 'i'), oname (oloc (o)));
	  uc = 0;
	  o = oloc (o);
	  break;
	case CARRIED_BY:
	  bprintf (" &+R%carried by %s&*", (uc ? 'C' : 'c'), pname (oloc (o)));
	  uc = 0;
	  o = ploc (oloc (o));
	  gotroom = 1;
	  break;
	case WORN_BY:
	  bprintf (" &+R%corn by %s&*", (uc ? 'W' : 'w'), pname (oloc (o)));
	  uc = 0;
	  o = ploc (oloc (o));
	  gotroom = 1;
	  break;
	case WIELDED_BY:
	  bprintf (" &+R%cielded by %s&*", (uc ? 'W' : 'w'), pname (oloc (o)));
	  uc = 0;
	  o = ploc (oloc (o));
	  gotroom = 1;
	  break;
	case BOTH_BY:
	  bprintf (" &+R%corn and wielded by %s&*", (uc ? 'W' : 'w'), pname (oloc (o)));
	  uc = 0;
	  o = ploc (oloc (o));
	  gotroom = 1;
	  break;
	default:
	  bprintf ("\n");
	  return;
	}
    }
  bprintf (" &+R%cn &*", (uc ? 'I' : 'i'));
  bprintf ("&+R%s&*\n", showname (o));
}


/* The SHOW command.  */
void showitem ()
{
  int x;
  
  if (!ptstflg (mynum, PFL_STATS))
    {
      erreval ();
      return;
    }
  if (EMPTY (item1))
    {
      bprintf ("Show what?\n");
      return;
    }
  if ((x = fobn (item1)) == -1)
    {
      bprintf ("What's that?\n");
      return;
    }
  _showitem(x);
}

void _showitem(int x)
{
  int i;
  OFLAGS *p;
  
  bprintf ("\n&+BItem [&+R%d&+B]&+W:&+R  %s&*", x, oname (x));
  if (!EQ (oaltname (x), "<null>"))
    bprintf (" &+R(%s)&*", oaltname (x));
  
  bprintf ("\n&+BLocation:   ");
  showlocation (x);
  
  if (olinked (x) > -1)
    {
      bprintf ("&+BLinked to:&*   ");
      if (olinked (x) >= numobs)
	bprintf ("non-existant object! (%d)", olinked (x));
      else
	{
	  bprintf ("&+R%s&*", oname (olinked (x)));
	  if (olinked (olinked (x)) != x)
	    bprintf (" ERROR: Not linked back! (%d)\n",
		     olinked (olinked (x)));
	  else
	    showlocation (olinked (x));
	}
    }
  bprintf ("&+BZone/Owner:  &+R%s&*\n", zname (ozone (x)));
  
  bprintf ("\n&+BState: &+R%d&*\t&+BMax State: &+R%d&*\n",
	   state (x), omaxstate (x));
  bprintf ("&+BDamage: &+R%d&*\t&+BArmor Class: &+R%d&*\t\t&+BSize: &+R%d&*\n",
	   odamage (x), oarmor (x), osize (x));
  bprintf ("&+BBase Value: &+R%-4d&*\t\t&+BCurrent Value: &+R%d&*\n\n",
	   obaseval (x), ovalue (x));
  bprintf ("&+BProperties:&+R ");
  p = &(obits (x));
  show_bits ((int *) p, sizeof (OFLAGS) / sizeof (int), Oflags);
  
  bprintf ("\n&+BState:   Description:&*\n");
  
  for (i = 0; i < 4; i++)
    {
      bprintf ("&+R[%d]    %s&*\n", i, olongt (x, i) == NULL ? "" : olongt (x, i));
    }
}

/*
* Routine to start or end tracing a player/mobile/object.
* Original code from dyrt 1.1c (Thrace?)
* modified by Twizzly July 1995
*/
void
tracecom ()
{
  int i;

  if (!ptstflg (mynum, PFL_TRACE))
    {
      erreval ();
      return;
    }
  if (cur_player->tr.trace_item != -1)
    {
      bprintf ("Stopped tracing %s.\n",
               cur_player->tr.trace_class == 1 ?
               oname (cur_player->tr.trace_item) :
               pname (cur_player->tr.trace_item));
      cur_player->tr.trace_item = -1;
      if (brkword () == -1)
        return;
    }
  else if (EMPTY (item1))
    {
      bprintf ("Trace what?\n");
      return;
    }
  if ((i = fpbn (item1)) != -1)
    {
      if (!do_okay (mynum, i, PFL_NOTRACE))
        {
          bprintf ("They don't want to be traced.\n");
          return;
        }
      cur_player->tr.trace_class = 2;
      cur_player->tr.trace_item = i;
      cur_player->tr.trace_loc = 0;
      cur_player->tr.trace_carrf = IN_ROOM;
      bprintf ("Ok\n");
      return;
    }
  if ((i = fobn (item1)) != -1)
    {
      cur_player->tr.trace_class = 1;
      cur_player->tr.trace_item = i;
      cur_player->tr.trace_loc = 0;
      cur_player->tr.trace_carrf = IN_ROOM;
      cur_player->tr.trace_oroom = roomobjin(i);
      bprintf ("Ok\n");
      return;
    }
  bprintf ("What's that?\n");
}



/* Warnzap and puntzap - Dragorn */

void warnzapcom()
{
 int vic;
 int phase;

  if (plev(mynum) < LVL_EMERITUS)
  {
    bprintf("The spell fails.\n");
    return;
  } else if (brkword() == -1) {
    bprintf("Warn who?\n");
    return;
  } else if (pl1 == -1) {
    bprintf("No one with that name playing.\n");
    return;
  } else if (pname(mynum) == pname(pl1)) {
    bprintf("What are you trying to do? Kill yourself?\n");
    return;
  } else if (plev(mynum)<plev(pl1)) {
    bprintf("You can't warn a higher immortal!\n");
    return;
  } else if (pl1 > max_players -1) {
    bprintf("You can only warn players!\n");
    return;
  } else {
  vic = pl1;

  if (EMPTY(item2))
   phase = 1;
  else
   phase = atoi(item2);  

if (phase >= 1 && phase <= 4)
{}
else {
 bprintf("Invalid warnzap level. Valid Levels: 1 through 4.\n");
 return; }

  send_msg (ploc (vic), 0, pvis (vic), LVL_MAX, vic, NOBODY,
	    "&+rA lightning bolt strikes &+y%s&+r, driving %s to %s knees!\n",
     pname(vic), him_or_her(vic), his_or_her(vic));
  bprintf("&+rYou drive %s to %s knees with a lightning bolt!\n",pname(vic),his_or_her(vic));
  
  setpstr(vic, 1);

/* Rainbow Spells Pts */
#ifdef setpspellpts
  setpspellpts(vic, 1);
#endif

 switch (phase)
 {
  case 1:

  sendf(vic, "&+r%s drives you to your knees with a lightning bolt, your body wracked\n"
             "&+rwith pain. As you approach unconsiousness, a voice echos in your head.\n"
             "&+r'&+YThis was but a warning.&+r' As you stumble to your feet you realize how\n"
             "&+rlucky you are to have been spared.\n", pname(mynum));
  mudlog("&+rWARN&+Y: %s warned %s, &+bWarnlevel %d",pname(mynum), pname(vic), phase);
  send_msg (DEST_ALL, MODE_QUIET, LVL_APPRENTICE, LVL_MAX, NOBODY, NOBODY,
		    "[%s has been warned by %s, Warnlevel %d]\n",
		    pname (vic), pname (mynum), phase);
    break;

  case 2:
    sendf(vic, "&+r%s drives you to your knees with a lightning bolt, your body wracked\n"
               "&+rwith pain. As you approach unconsiousness, you feel knowledge slipping\n"
               "&+raway. %s's voice echos in your head. 'This was but a warning.'\n",
                pname(mynum), pname(mynum));
    setpscore(vic, (pscore(vic) - (pscore(vic) / 10)));
    calib_player(vic);
  mudlog("&+rWARN&+Y: %s warned %s, &+bWarnlevel %d",pname(mynum), pname(vic), phase);
  send_msg (DEST_ALL, MODE_QUIET, LVL_APPRENTICE, LVL_MAX, NOBODY, NOBODY,
		    "[%s has been warned by %s, Warnlevel %d]\n",
		    pname (vic), pname (mynum), phase);
    break;
  case 3:
    sendf(vic, "&+r%s drives you to your knees with a lightning bolt, your body wracked\n"
               "&+rwith pain. As you approach unconsiousness, you feel knowledge ripped\n"
               "&+raway by an icy wind. %s's voice echos in your head. 'Do &+Rnot&+r\n"
               "&+rcontinue your actions.'\n",
                pname(mynum), pname(mynum));
    setpscore(vic, (pscore(vic) - (pscore(vic) / 10)));
    calib_player(vic);
  mudlog("&+rWARN&+Y: %s warned %s, &+bWarnlevel %d",pname(mynum), pname(vic), phase);
  send_msg (DEST_ALL, MODE_QUIET, LVL_APPRENTICE, LVL_MAX, NOBODY, NOBODY,
		    "[%s has been warned by %s, Warnlevel %d]\n",
		    pname (vic), pname (mynum), phase);
    p_crapup (vic,
    "\t\tDo NOT continue your actions.", CRAP_SAVE | CRAP_RETURN);
		  break;
  case 4:
    sendf(vic, "&+r%s drives you to your knees with a lightning bolt, your body wracked\n"
               "&+rwith pain. As you approach unconsiousness, you feel knowledge ripped\n"
               "&+raway by an icy wind. %s's voice echos in your head. 'Do &+Rnot&+r\n"
               "&+rcontinue your actions. This is your &+RFINAL&+r warning.'\n",
                pname(mynum), pname(mynum));
    setpscore(vic, (pscore(vic) - (pscore(vic) / 2)));
    calib_player(vic);
  mudlog("&+rWARN&+Y: %s warned %s, &+bWarnlevel %d",pname(mynum), pname(vic), phase);
  send_msg (DEST_ALL, MODE_QUIET, LVL_APPRENTICE, LVL_MAX, NOBODY, NOBODY,
		    "[%s has been warned by %s, Warnlevel %d]\n",
		    pname (vic), pname (mynum), phase);
    p_crapup (vic,
    "\t\tThis is your FINAL warning.", CRAP_SAVE | CRAP_RETURN);
		  break;
  default:
   bprintf("Invalid Warnzap Level. Valid Levels: 1 through 4.\n");
   break;
 }
  }
}

void puntzapcom()
{
  int a=pl1,r_pl;
 /* int crater;*/
  char *s;
  s=NEW(char,100);
  if (plev(mynum)<LVL_ARCHWIZARD) {
    bprintf("You can't do that now.\n");
    return;
  } else if (brkword() == -1) {
    bprintf("Puntzap who?\n");
    return;
  } else if (pl1 == -1) {
    bprintf("No one with that name playing.\n");
    return;
  } else if (pname(mynum) == pname(a)) {
    bprintf("What are you trying to do? Fry yourself?\n");
    return;
  } else if (plev(mynum)<plev(a)) {
    bprintf("Your puny lightning bolt has no effect.\n");
    sendf(pl1,"%s hits you with a puny lightning bolt that does no damage.\n",pname(mynum));
    return;
  } else {
    r_pl = convroom(my_random()%numloc);
    if (brkword () != -1){
      if ((r_pl = findroomnum (wordbuf)) == 0)
	{
	  bprintf ("Unknown room\n");
	  return;
	}
    }
    send_msg(ploc(a), 0, pvis(a), LVL_MAX, a, NOBODY,
	     "A lightning bolt slams into the ground next to %s and slams %s\ninto the sky!\n",pname(a), him_or_her(a));
    send_msg(DEST_ALL,0,LVL_MIN,LVL_MAX,a,NOBODY,
	     "You hear a loud crash of thunder and a cry.\n"
	     "%s flies through the air leaving a trail of smoke behind %s.\n",pname(a), him_or_her(a));
    send_msg(a,0,LVL_MIN,LVL_MAX,NOBODY,NOBODY,
	     "%s slams an immense bolt of lightning into the ground next to you, sending\n"
	     "you flying into the air, a trail of smoke behind you.\n"
	     "You land on the ground somewhere else, and frantically beat at the flames...\n",pname(mynum));
    sendf(r_pl,"%s lands on the ground in front of you, smoking slightly.\n",
          pname(a));
/*     sprintf(s, "The crater smokes slightly, and in the distance you can hear %s\nsoftly whimpering.\n");
    oexam_text(OBJ_LIMBO_CRATER)=NULL;
    oexam_text(OBJ_LIMBO_CRATER)=s;
    setoloc(OBJ_LIMBO_CRATER, ploc(a), IN_ROOM); */
    setploc(a,r_pl);
  }
}

void puntcom() /* written by Kender (kender@esu.edu) Winter, 1994 */
{
  int save_mynum;
  int temp_me = mynum;
  int newloc=0;
  int punter = mynum;
  
  if (plev(punter) < LVL_APPREN) {
    bprintf ("You stub your toe!\n");
    return;
  }
  
  if (brkword () == -1) {
    bprintf ("Who do you want to drop kick?\n");
    return;
  }
  if (pl1 == -1) {
    bprintf ("No one with that name is playing.\n");
    return;
  }
  
   if (ltstflg(ploc(pl1), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD)) {
    bprintf ("You kick with all your might, but the "
               "gravity in here is too great\n");
    return;
   }
 
  if (brkword () != -1) {
    if ((newloc = findroomnum(wordbuf)) == 0) {
      bprintf ("Unknown player, object, or room\n");
      return; }
  }
  else newloc = convroom (my_random()%numloc);
  
  if (newloc == 0)
    newloc = convroom(my_random()%numloc);
  
  if ((plev(punter) < LVL_GOD) && ltstflg(newloc, LFL_PRIVATE))
    if (count_players (newloc, LVL_MIN, LVL_MAX,
		       COUNT_PLAYERS | COUNT_MOBILES) > 1)
      {
	bprintf ("I'm sorry.  There's a private conference in that location.\n");
	return;
      }
  if (!checklevel(punter, newloc)) return;
  
  if (plev(punter) < LVL_DEMI && ploc(pl1) != ploc(punter)) {
    bprintf ("Sorry, but it ain't here for you to drop kick!\n");
    return;
  }
  
  if (plev(pl1) > plev(punter) && !EQ("Puff",pname(punter))) {
    bprintf ("You kick %s with all your might, but stub your toe!\n",
             pname(pl1));
    send_msg (ploc(punter), 0, LVL_MIN, LVL_MAX, punter, pl1,
              "%s slams %s foot into %s!  %s does NOT look happy.\n",
              pname(punter), his_or_her(punter), pname(pl1), pname(pl1));
    sendf (pl1, "%s slams %s foot into your shin!\n",
           pname(punter), his_or_her(punter));
    return;
  }
  if (EQ("Oracle", pname(pl1)))
    {
      bprintf("The Oracle is not your little punting-toy!!!\n");
      setup_globals(pl1);
      punter = pl1;
      pl1 = temp_me;
    }
 
  if (EQ("Puff", pname(pl1)))
    {
      bprintf("Puff scowls, spins, and kicks you!\n");
      bprintf("Puff tells you 'Never annoy a dragon, for thou art crunchy and good tasting.\n\n");
      setup_globals(pl1);
      punter = pl1;
      pl1 = temp_me;
    }
  
  send_msg (DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, LVL_MAX, punter, pl1,
            "[\001p%s\003 has just punted \001p%s\003.]\n",
            pname(punter), pname(pl1));
  
  bprintf ("You grab %s by the ears and deliver a thunderous kick with your "
           "right foot!\n%s goes flying off into the air with a pitiful "
           "'&+YWahoohoohoo&*'...\nYou hear a thunderous thud in the distance.\n",
           pname(pl1), pname(pl1));
  sendf (pl1, "With an evil grin, \001p%s\003 grabs you by the ears and "
         "delivers\na thunderous kick!  You feel yourself rising up into the "
         "air.....\nYou land with a thunderous crash on the ground!"
         "\n", pname(punter));
  send_msg (ploc(punter), 0, LVL_MIN, LVL_MAX, punter, pl1,
            "With an evil grin, \001p%s\003 grabs \001p%s\003 by the ears, "
            "and\ndelivers a thunderous kick!\n", pname(punter), pname(pl1));
  
  send_msg (DEST_ALL, MODE_QUIET|MODE_NODEAF,
            LVL_MIN, LVL_MAX, punter, pl1,
            "You hear a thunderous BOOT......\nYou hear a pitiful "
            "'&+YWahoohoohoo&*' flying over your head...\nThe ground shakes "
            "with a monstrous THUD in the distance.\n");
  
  if (pl1 >= max_players)
    setploc (pl1, newloc);
  else {
    save_mynum = punter;
    setup_globals (pl1);
    trapch (newloc);
    setup_globals (save_mynum);
  }
  
  mudlog ("%s PUNTed %s to %s", pname(punter), pname(pl1), showname(ploc(pl1)));
  
  send_msg (ploc(pl1), 0, LVL_MIN, LVL_MAX, pl1, NOBODY,
            "You hear a yelp and a thud as \001p%s\003 comes flying in.\n",
            pname(pl1));
  setup_globals(temp_me);
}

#define notavail(i) ((ospare(i) == -1) || ((oloc(i) < 0) ? ltstflg(oloc(i), LFL_DEATH) : 0))
void
acctcom()
{
  int i, j;
  static int lf[] = { OFL_WEAPON, OFL_ARMOR, OFL_FOOD, OFL_DRINK, OFL_CONTAINER,
                        OFL_LIGHTABLE, OFL_KEY };
  int objcount[arraysize(lf)];
  int destcount[arraysize(lf)];
  int movecount[arraysize(lf)];
  int roomcount, mobilecount, livemobile, deadmobile, possmobile;
  int nohassmobile, objectcount, remobject, destobject, flanobject;
  int pitresetobject;
  
  /*
     if (plev(mynum) < LVL_APPREN) {
     bprintf ("Are you sure you know how to type?\n");
     return;
     }
     */
  roomcount = mobilecount = livemobile = deadmobile = possmobile =
    nohassmobile = objectcount = remobject = destobject = flanobject =
      pitresetobject = 0;
  for (j = 0; j < arraysize(lf); j++)
    movecount[j] = objcount[j] = destcount[j] = 0;
  /* Isn't C great? */
  /* yeah, but howabout trying memset, instead? */
  
  for (i = max_players; i < numchars; i++) {
    if (pstr(i) > 0) livemobile++;
    else deadmobile++;
    mobilecount++;
    if (ststflg(i, SFL_ALOOF)) nohassmobile++;
  }
  
  for (i = 0; i < numobs; i++) {
    objectcount++;
    if (notavail(i)) destobject++;
    else remobject++;
    if (otstbit(i, OFL_PITRESET)) pitresetobject++;
    if (oflannel(i)) flanobject++;
    if (zpermanent(ozone(i)))
      for (j = 0; j < arraysize(lf); j++)
        if (otstbit (i, lf[j])) {
          objcount[j]++;
          if (notavail(i)) destcount[j]++;
        }
  }
  
  bprintf ("          &+WCurrent System Accounting&*\n");
  bprintf ("&+R========================================================&*\n");
  bprintf ("\n&+BTotal Rooms     &+W:&*    %5i\n", num_const_locs);
  bprintf ("\n&+BTotal Mobiles   &+W:&*   %5i     &+BLiving Mobiles  &+W:&*   %5i\n",
           mobilecount, livemobile);
  bprintf ("&+BDead Mobiles    &+W:&*   %5i     &+BPercent Living  &+W:&*   %5i\n",
           deadmobile, (100*livemobile)/mobilecount);
  bprintf ("&+BPossessed       &+W:&*   %5i     &+BNoHassle        &+W:&*   %5i\n\n",
           possmobile, nohassmobile);
  bprintf ("&+BTotal objects   &+W:&*   %5i     &+BRemaining       &+W:&*   %5i\n",
           objectcount, remobject);
  bprintf ("&+BDestroyed       &+W:&*   %5i     &+BPercent left    &+W:&*   %5i\n",
           destobject, (100*remobject)/objectcount);
  bprintf ("&+BFlannel         &+W:&*   %5i     \n\n", flanobject);
  bprintf ("          &+WDetailed List of Objects Avalible to Mortals&*\n");
  bprintf ("&+R========================================================================&*\n\n");
  for (j = 0; j < arraysize(lf); j++)
    bprintf ("&+B%-14s&+W:&*  %5i               &+BDestroyed:&*  %5i  \n",
             Oflags[lf[j]], objcount[j], destcount[j]);
  
}

void placediscs()
{
  int newloc, disc, i;
  char y[MAX_COM_LEN];

  if (plev(mynum) < LVL_SENIOR)
    {
      bprintf ("Pardon?\n");
      return;
    }
  if (oloc(OBJ_HOME_DISC1) == LOC_HOME_1)
    {
      disc = OBJ_HOME_DISC1;
      for (i = 1; i <= 10; i++)
        {
          newloc = convroom(my_random()%numloc);
          setoloc((disc + i), newloc, IN_ROOM);
        }
      bprintf("&+YDiscs&* placed.\n");
      sprintf(y, "&+YDISC HUNT!!! Ten discs to find.  Worth LOTS!&*\n");
      broad(y);
    }
  else
    {
      bprintf("There is already a disc hunt in progress.\n");
    }
}

void bancom(Boolean hosts)
{
  FILE *ban_file;
  char names[1024][256], foo[MAX_COM_LEN];
  int  i = 0, j, num;

  getreinput(foo);
  if(EMPTY(foo) || !ptstflg(mynum, PFL_BAN)) {
    read_file(hosts ? BAN_HOSTS : BAN_CHARS, NULL, NULL, NULL);
    return;
  }

  if((ban_file = fopen(hosts ? BAN_HOSTS : BAN_CHARS, "r")) == NULL) {
    bprintf("Error opening banned %s file for reading.\n",
                hosts ? "hosts" : "characters");
    return;
  }
  while(!feof(ban_file)) {
    j = 0;
    while((names[i][j++] = getc(ban_file)) != EOF)
      if(names[i][j - 1] == '\n') break;
    names[i++][j - 1] = 0;
  }
  fclose(ban_file);
  num = i;
  for(i = 0; i < num; i++)
    if(EQ(names[i], foo)) {
      if((ban_file = fopen(hosts ? BAN_HOSTS : BAN_CHARS, "w")) == NULL) {
        bprintf("Error opening banned %s file for writing.\n",
                        hosts ? "hosts" : "characters");
        return;
      }
      for(j = 0; j < num; j++)
        if(j != i && !EMPTY(names[j])) fprintf(ban_file, "%s\n", names[j]);
      bprintf("Removed `%s' from banned %s file.\n", names[i],
                hosts ? "hosts" : "characters");
      mudlog("%s un-banned %s %s", pname(mynum), hosts ? "host" :
                "character", foo);
      fclose(ban_file);
      return;
    }
  if((ban_file = fopen(hosts ? BAN_HOSTS : BAN_CHARS, "a")) == NULL) {
    bprintf("Error opening banned %s file for appending.\n",
                hosts ? "hosts" : "characters");
    return;
  }
  fprintf(ban_file, "%s\n", foo);
  bprintf("Added `%s' to banned %s file.\n", foo,
                hosts ? "hosts" : "characters");
  mudlog("%s banned %s %s", pname(mynum), hosts ? "host":
                "character", foo);
  fclose(ban_file);
}

void wizechocom(void)
{
  char x[MAX_COM_LEN];

  if(plev(mynum) < LVL_APPREN) 
  {
    erreval();
    return;
  }
  getreinput(x);
  if(EMPTY(x)) 
    {
      bprintf("What do you want to wizecho?\n");
      return;
    }
  if(!ptstflg(mynum, PFL_ECHO)) 
    {
      bprintf("You hear echoes.\n");
      return;
    }
  send_msg(DEST_ALL, 0, LVL_MIN, LVL_MAX, NOBODY, NOBODY, "&+Y%s\n", x);
  send_msg(DEST_ALL, MODE_QUIET, max(LVL_ARCHWIZARD, pvis(mynum)), LVL_MAX,
                mynum, NOBODY, "&+Y[%s has WIZECHOed]\n", pname(mynum));
}

void wemcom (void)
{
  char input[1024];

  if (EMPTY(item1))
    {
      bprintf("What would you like to emote to the wizzes?\n");
      return;
    }
  if (plev(mynum) < LVL_APPREN)
    {
      bprintf("You can't do that now.\n");
      return;
    }
  getreinput(input);
  if (ststflg(mynum, SFL_SAYBACK))
    {
      bprintf("&+CYour emote to the wizards looked like this --&*\n");
      if (pvis(mynum) > 0)
        bprintf("&+Y(%s) %s&*\n", pname(mynum), input);
      else
        bprintf("&+Y%s %s&*\n", pname(mynum), input);
    }
  else
    {
      bprintf("Ok.\n");
    }
  send_msg(DEST_ALL, MODE_COLOR|MODE_NOWIZ, LVL_APPREN, LVL_MAX, mynum, NOBODY,
           "\001p%s\003 %s\n", pname(mynum), input);
}
