#include "kernel.h"
#include <time.h>
#include "levels.h"

#ifdef VARGS
#include <stdarg.h>
#endif

#include "log.h"
#include "uaf.h"
#include "bprintf.h"
#include "verbs.h"
#include "wizard.h"
#include "parse.h"
#include "sendsys.h"
#include "mobile.h"
#include "objsys.h"
#include "rooms.h"
#include "flags.h"
#include "pflags.h"
#include "lflags.h"
#include "sflags.h"
#include "cflags.h"


extern char *Oflags[];


/* The RAW command
 */
void rawcom()
{
  char x[MAX_COM_LEN], y[MAX_COM_LEN];

  if (!ptstflg(mynum, PFL_RAW)) {
    erreval();
    return;
  }
  getreinput(x);
  sprintf(y, "** SYSTEM : %s\a\a\n", x);
  broad(y);
}




/* The DELETE command. Deletes a player totally from the systems files.
 */
void deletecom()
{
    if (plev(mynum) < LVL_DEMI) {
        erreval();
        return;
      }
    if (brkword() == -1 || strlen(wordbuf) > PNAME_LEN) {
        bprintf("Delete who ?\n");
        return;
      }
    mudlog("DELETE: %s deleted %s", pname(mynum), wordbuf);

    deluaf(wordbuf);
/*    delsetins(wordbuf); */
    update_wizlist(wordbuf, LEV_MORTAL);

    bprintf("Deleted %s.\n", wordbuf);
}



/* The OPENGAME command.
 */
void opengamecom()
{
    int unlink(char *);

    if (!ptstflg(mynum, PFL_SHUTDOWN)) {
        erreval();
        return;
      }
    if (unlink(NOLOGIN) < 0) {
        bprintf("The game is already open.\n");
      }
    else {
        mudlog("OPENGAME by %s", pname(mynum));
        bprintf("MUD is now open.\n");
      }
}


/* The TOURNAMENT Command. Toggle Tournament-mode.
 */
void tournamentcom()
{
    if (plev(mynum) < LVL_ARCHWIZARD) {
        erreval();
        return;
      }
    bprintf("Tournament-mode is now %s.\n",
        (the_world->w_tournament = !the_world->w_tournament) ? "ON" : "OFF");
}


void shutdowncom(Boolean crash)
{
	FILE *nologin_file;
	char s[MAX_COM_LEN];
	int i;
	char *t = "AberMUD is closed for hacking.  Please try again later.\n";

	if (!ptstflg(mynum, PFL_SHUTDOWN)) {
		erreval();
		return;
	}

	getreinput(s);
	if (!EMPTY(s)) t = s;

	if ((nologin_file = fopen(NOLOGIN, "w")) == NULL) {
		bprintf("(Unable to write NOLOGIN file.)");
	}
	else {
		fprintf(nologin_file, "%s\n", t);
		fclose(nologin_file);
	}

	bprintf("AberMUD is now closed.\n");
	mudlog("SHUTDOWN%s by %s", crash ? " +CRASH" : "", pname(mynum));

	/* kick people off here */
	for (i = 0; i < max_players; i++) {
		if (is_in_game(i) && !players[i].isawiz) {

			p_crapup(i, t, CRAP_SAVE|CRAP_UNALIAS|CRAP_RETURN);
		}
	}

	if (crash) exit(0);
}


void syslogcom(void)
{
  char   tinn[MAX_COM_LEN];
  char      t[MAX_COM_LEN];
  char   *z;
  int    x;
  FILE  *f;

  if (!ptstflg(mynum, PFL_SYSLOG)) {
    erreval();
    return;
  }

  getreinput( tinn );

  if (EMPTY(txt1)) {
    z = ctime( &global_clock );
    z += 3;  z[8] = '\0';

    sprintf(t,"grep -i '%s' " LOG_FILE, z);
  }
  else if (tinn[0] == '*')
   strcpy(t, "cat " LOG_FILE);
  else if ((x = atoi(tinn)) > 0) {
    sprintf(t, "tail -%d " LOG_FILE, x);
  } else {
    sprintf(t, "grep -i '%s' " LOG_FILE, tinn);
  }
  ssetflg(mynum,SFL_BUSY);
/*  system(t); */
  bprintf("\001F%s\003", t);
  sclrflg(mynum,SFL_BUSY);
}



/* (C) Rassilon (Brian Preble) */
void echocom()
{
  char x[MAX_COM_LEN];

  if (!ptstflg(mynum, PFL_ECHO)) {
    bprintf("You hear echos.\n");
    return;
  }
  getreinput(x);
  if (EMPTY(x)) {
    bprintf("ECHO what?\n");
    return;
  }
  send_msg(ploc(mynum), 0, LVL_MIN, LVL_MAX, NOBODY, NOBODY, "%s\n", x);
  bprintf("Ok\n");
}


/* (C) Rassilon (Brian Preble) */
void echoallcom()
{
  char x[MAX_COM_LEN], a[MAX_COM_LEN + 10];

  if (!ptstflg(mynum, PFL_RAW)) {
    bprintf("You hear echos.\n");
    return;
  }
  getreinput(x);
  if (EMPTY(x)) {
    bprintf("Echo what?\n");
    return;
  }
  sprintf(a, "\001c%s\n\003", x);
  broad(a);
  bprintf("Ok\n");
}


/* (C) Rassilon (Brian Preble) */
void echotocom()
{
    int b;

    if (!ptstflg(mynum, PFL_ECHO)) {
        bprintf("You hear echos.\n");
        return;
      }
    if (EMPTY(item1)) {
        bprintf("Echo to who?\n");
        return;
      }
    if ((b = pl1) == -1) {
        bprintf("No one with that name is playing.\n");
        return;
      }
    if (b == mynum) {
        bprintf("What's the point?\n");
        return;
      }
    if (EMPTY(txt2)) {
        bprintf("What do you want to echo to them?\n");
        return;
      }
    if (check_busy(b)) return;

    send_msg(b, 0, LVL_MIN, LVL_MAX, NOBODY, NOBODY, "%s\n", txt2);
    bprintf("Ok\n");
}


/*
 * Wizard-stuff
 */


/* (C) Jim Finnis  (Yes he really did write one or two routines/A) */
void emotecom()
{
  char buf[MAX_COM_LEN + 10];

  if (!ptstflg(mynum, PFL_EMOTE)
      && !ltstflg(ploc(mynum), LFL_PARTY)) {
    bprintf("Your emotions are strictly limited!\n");
    return;
  }
  if (EMPTY(item1)) {
    bprintf("What do you want to emote?\n");
    return;
  }
  strcpy(buf, "\001P%s\002 ");
  getreinput(buf + 6);
  strcat(buf, "\n");
  sillycom(buf);
  bprintf("Ok\n");
}

/* (C) Rassilon (Brian Preble) */
void emotetocom()
{
  int a;

  if (!ptstflg(mynum, PFL_EMOTE)
      && !ltstflg(ploc(mynum), LFL_PARTY)) {
    bprintf("Your emotions are strictly limited!\n");
    return;
  }
  if (EMPTY(item1)) {
    bprintf("Emote to who?\n");
    return;
  }
  if ((a = pl1) == -1) {
    bprintf("No one with that name is playing.\n");
    return;
  }
  if (a == mynum) {
    bprintf("Good trick, that.\n");
    return;
  }
  if(EMPTY(txt2)) {
    bprintf("Emote what?\n");
    return;
  }
  if (check_busy(a)) return;

  sillytp(a, txt2);
  bprintf("Ok\n");
}



/* The SET command
 */
/* These properties require numbers:
 */
#define SET_BVALUE  0
#define SET_SIZE    1
#define SET_VIS     2
#define SET_DAMAGE  3
#define SET_ARMOR   4
#define SET_STATE   5

/* These require texts:
 */
#define SET_TEXT_MIN 5
#define SET_DESC0  6
#define SET_DESC1  7
#define SET_DESC2  8
#define SET_DESC3  9
#define SET_NAME   10
#define SET_ANAME  11
#define SET_TEXT_MAX 11

/* Properties
 */
static char *Props[] = { "BaseValue", "Size",   "Visibility", "Damage",
			 "Armor",     "State",  "Desc0",      "Desc1",
			 "Desc2",     "Desc3",  "Name",       "AltName",
			 TABLE_END
		       };

void setcom()
{
	int o, p, v;  /* Object, Property, Value */

	Boolean is_oflag;

	if ( !ptstflg(mynum, PFL_OBJECT) ) {
		erreval();
		return;
	}

	if (brkword() == -1 || (o = fobn(wordbuf)) == -1) {
		bprintf("Set what??\n");
		return;
	}

        if (brkword() == -1 || (is_oflag = (p = tlookup(wordbuf, Props)) == -1)
	    && (p = tlookup(wordbuf, Oflags)) == -1) {

		bprintf("Set what property on the %s?\n", oname(o));
		return;
	}

	if ((is_oflag || p <= SET_TEXT_MIN || p > SET_TEXT_MAX)
		&& brkword() == -1) {

		bprintf("Set the %s property to what ??\n",
			is_oflag ? Oflags[p] : Props[p]);
		return;
	}

	if (is_oflag) {

		if ((v = tlookup(wordbuf, OO)) == -1
		    && (v = tlookup(wordbuf, TF)) == -1) {

			bprintf("Value must be On or Off (or True/False).\n");
			return;
		}

		if (v) {
			osetbit(o, p);
		}
		else {
			oclrbit(o, p);
		}
	}
	else if (p <= SET_TEXT_MIN || p > SET_TEXT_MAX) {

		int limit = -1;

		if (!isdigit(*wordbuf)) {
			bprintf("Value must be a number >= 0.\n");
			return;
		}
		else v = atoi(wordbuf);

		switch( p ) {
		      case SET_BVALUE:
			if (v > O_BVALUE_MAX ||
			       v > obaseval(o) && v > 400
			    && !ptstflg(mynum, PFL_FROB)) limit = O_BVALUE_MAX;
			else
			    osetbaseval(o, v);
			break;
		      case SET_SIZE:
			if (v > O_SIZE_MAX) limit = O_SIZE_MAX;
			else osetsize(o, v);
			break;
		      case SET_VIS:
			if (v > O_VIS_MAX) limit = O_VIS_MAX;
			else osetvis(o, v);
			break;
		      case SET_DAMAGE:
			if (v > O_DAM_MAX) limit = O_DAM_MAX;
			else osetdamage(o, v);
			break;
		      case SET_ARMOR:
			if (v > O_ARMOR_MAX) limit = O_ARMOR_MAX;
			else osetarmor(o, v);
			break;
		      case SET_STATE:
			if (v > omaxstate(o)) limit = omaxstate(o);
			else setobjstate(o, v);
			break;
		      default:
			bprintf("Internal error\n");
			mudlog("Internal errror in setcom(): p = %d", p);
			return;
		}

		if (limit > -1) {
			bprintf("Sorry, value must be <= %d.\n", limit);
			return;
		}

	} else {
		char **q;

		if (opermanent(o)) {
			bprintf("You can only change %s on non-permanent"
				" (wiz-created) objects.\n", Props[p]);
			return;
		}

		q = p == SET_DESC0 ? &olongt(o, 0)
		  : p == SET_DESC1 ? &olongt(o, 1)
		  : p == SET_DESC2 ? &olongt(o, 2)
		  : p == SET_DESC3 ? &olongt(o, 3)
		  : p == SET_NAME ? &oname(o)
		  : p == SET_ANAME ? &oaltname(o)
		  : NULL;

		if (q == NULL) {
			bprintf("Internal Error\n");
			mudlog("Internal error in setcom(), p = %d", p);
			return;
		}

		if (strchr(getreinput(wordbuf), '^') != NULL ) {
			bprintf("Illegal character(s) (^) in text.\n");
			return;
		}

		if (p == SET_NAME || p == SET_ANAME) {
			char *s = wordbuf;

			if (strlen(wordbuf) > ONAME_LEN) {
				bprintf("Name too long. Max = %d chars.\n",
					ONAME_LEN);
				return;
			}

			while (*s != '\0' && isalpha(*s)) s++;

			if (*s != '\0') {
				bprintf("Name must only contain latters.\n");
				return;
			}

			if (is_classname(wordbuf)) {
				bprintf("That's the name of a object-class\n");
				return;
			}

			if (EMPTY(wordbuf)) {
				if (p == SET_ANAME) strcpy(wordbuf, "<null>");
				else {
					bprintf("Name is missing.\n");
					return;
				}
			}
		}

		if (*q != NULL) free(*q);

		*q = COPY(wordbuf);
	}
	bprintf("Ok.\n");
}


/* The SAVESET command.
 */
void saveset()
{
    SETIN_REC s;

    if (plev(mynum) < LVL_WIZARD) {
        erreval();
        return;
      }

    if (cur_player->aliased || cur_player->polymorphed != -1) {
        bprintf("Not while aliased.\n");
        return;
      }

    strcpy(s.name, pname(mynum));
    strcpy(s.prompt, cur_player->prompt);
    strcpy(s.setin, cur_player->setin);
    strcpy(s.setout, cur_player->setout);
    strcpy(s.setmin, cur_player->setmin);
    strcpy(s.setmout, cur_player->setmout);
    strcpy(s.setvin, cur_player->setvin);
    strcpy(s.setvout, cur_player->setvout);
    strcpy(s.setqin, cur_player->setqin);
    strcpy(s.setqout, cur_player->setqout);

    putsetins(pname(mynum), &s);
    bprintf("Saving prompt and set*in/out messages.\n");
  }



void exorcom()
{
  int x, q, y = real_mynum;

  if (!ptstflg(mynum, PFL_EXOR)) {
    bprintf("You couldn't exorcise your way out of a paper bag.\n");
    return;
  }
  if (EMPTY(item1)) {
    bprintf("Exorcise who?\n");
    return;
  }
  if ((x = pl1) == -1) {
    bprintf("They aren't playing.\n");
    return;
  }

  if (! do_okay( mynum, x, PFL_NOEXOR )) {
    bprintf("They don't want to be exorcised.\n");
    return;
  }

  if (x < max_players) mudlog("%s exorcised %s", pname(mynum), pname(x));

    send_msg(DEST_ALL, MODE_QUIET, pvis(x), LVL_MAX, x, NOBODY,
	     "%s has been kicked off.\n", pname(x));

  if ((q = find_pretender(x)) >= 0) {
      sendf(q,"You have been kicked off!\n");
      unalias(q);
      unpolymorph(q);
  }

  if (x >= max_players) {

	  dumpstuff(x, ploc(x)); 
	  setpfighting(x, -1);

	  if (!ppermanent(x)) {
		  destruct_mobile(x, NULL);
	  }
	  else {
		  setpname(x, "");
	  }
    
  } else {

    setup_globals(x);
    crapup("\tYou have been kicked off!", CRAP_SAVE|CRAP_RETURN);
    setup_globals(y);
  }
}


void setstart(void)
{
    PERSONA   d;
    int       loc, p;
    char      buff[80];
    char      *who;
    char      *where;

    if (plev(mynum) < LVL_WIZARD) {
        erreval();
        return;
      }

    /* Parse the command, find *WHO*'s home we want to change and *WHERE*
     * to change it to.
     */
    if (brkword() == -1) {

        who = pname(mynum);
        where = NULL;

      } else if (getuaf(strcpy(buff, wordbuf), &d)) {

        who = d.p_name;
        where = (brkword() == -1) ? NULL : wordbuf;

        if (!EQ(who, pname(mynum))) {
            if (!ptstflg(mynum, PFL_UAF)) {
                bprintf("You can only set your own start-location.\n");
                return;
	      }
            if (!do_okay_l(plev(mynum), d.p_level, False)) {
                bprintf("That is beyond your powers.\n");
                return;
	      }
	  }

      } else if (brkword() == -1) {

        who = pname(mynum);
        where = buff;

      } else {

        bprintf("No such player: %s\n", buff);
        return;
      }

    /* Got the arguments. If the operation is not 'erase home' (where=null),
     * see if the argument corresponds to a real room.
     */
    if (where == NULL) {
        loc = 0;
      }
    else if ((loc = findroomnum(where)) == 0 || !exists(loc)) {
        bprintf("No such location.\n");
        return;
      }

    if (exists(loc)) loc = loc_id(loc);

    /*  Got the room number. Finally, set the home for the player.
     */
    if ((p = fpbns(who)) > -1) {
        setphome(p, loc);
      }
    else {
        d.p_home = loc;
        putuaf(&d);
      }
    bprintf("Ok.\n");
  }



void noshoutcom()
{
    int x;

    if (plev(mynum) < LVL_WIZARD) {
      erreval();
      return;
    }
    if ((x = pl1) == -1 || x == mynum) {
        if (ststflg(mynum, SFL_NOSHOUT)) {
            bprintf("You can hear shouts again.\n");
            sclrflg(mynum, SFL_NOSHOUT);
	  } else {
            bprintf("From now on you won't hear shouts.\n");
            ssetflg(mynum, SFL_NOSHOUT);
	  }
      } else if (plev(x) >= LVL_WIZARD) {
        if (ststflg(x, SFL_NOSHOUT)) {
            bprintf("%s can hear shouts again.\n", pname(x));
            sclrflg(x, SFL_NOSHOUT);
	  } else {
            bprintf("From now on %s won't hear shouts.\n", pname(x));
            ssetflg(x, SFL_NOSHOUT);
	  }
      } else if (ststflg(x, SFL_NOSHOUT)) {
        bprintf( "%s is allowed to shout again.\n", pname(x));
        sclrflg(x, SFL_NOSHOUT);
      } else {
        bprintf( "From now on %s cannot shout.\n", pname(x));
        ssetflg(x, SFL_NOSHOUT);
      }
  }




void showlocation(int o)
{
  int uc, gotroom;

  uc = 1;
  gotroom = 0;
  while (!gotroom) {
    switch (ocarrf(o)) {
    case IN_ROOM:
      o = oloc(o);
      gotroom = 1;
      break;
    case IN_CONTAINER:
      bprintf(" %cnside the %s", (uc ? 'I' : 'i'), oname(oloc(o)));
      uc = 0;
      o = oloc(o);
      break;
    case CARRIED_BY:
      bprintf(" %carried by %s", (uc ? 'C' : 'c'), pname(oloc(o)));
      uc = 0;
      o = ploc(oloc(o));
      gotroom = 1;
      break;
    case WORN_BY:
      bprintf(" %corn by %s", (uc ? 'W' : 'w'), pname(oloc(o)));
      uc = 0;
      o = ploc(oloc(o));
      gotroom = 1;
      break;
    case WIELDED_BY:
      bprintf(" %cielded by %s", (uc ? 'W' : 'w'), pname(oloc(o)));
      uc = 0;
      o = ploc(oloc(o));
      gotroom = 1;
      break;
    case BOTH_BY:
      bprintf(" %corn and wielded by %s", (uc ? 'W' : 'w'), pname(oloc(o)));
      uc = 0;
      o = ploc(oloc(o));
      gotroom = 1;
      break;
    default:
      bprintf("\n");
      return;
    }
  }
  bprintf(" %cn ", (uc ? 'I' : 'i'));
  bprintf("%s\n", showname(o));
}


/* The SHOW command.
 */
void showitem()
{
  int x, i;
  OFLAGS *p;

  if (!ptstflg(mynum, PFL_STATS)) {
    erreval();
    return;
  }
  if (EMPTY(item1)) {
    bprintf("Show what?\n");
    return;
  }
  if ((x = fobn(item1)) == -1) {
    bprintf("What's that?\n");
    return;
  }
  bprintf("\nItem [%d]:  %s", x, oname(x));
  if (!EQ(oaltname(x), "<null>"))
    bprintf(" (%s)", oaltname(x));

  bprintf("\nLocation:   ");
  showlocation(x);

  if (olinked(x) > -1) {
	  bprintf("Linked to:   ");
	  if (olinked(x) >= numobs)
	          bprintf("non-existant object! (%d)", olinked(x));
	  else {
		  bprintf("%s", oname(olinked(x)));
		  if (olinked(olinked(x)) != x)
		          bprintf(" ERROR: Not linked back! (%d)\n",
				  olinked(olinked(x)));
		  else
		          showlocation(olinked(x));
	  }
  }
  bprintf("Zone/Owner:  %s\n", zname(ozone(x)));

  bprintf("\nState: %d\tMax State: %d\n",
	  state(x), omaxstate(x));
  bprintf("Damage: %d\tArmor Class: %d\t\tSize: %d\n",
	  odamage(x), oarmor(x), osize(x));
  bprintf("Base Value: %-4d\t\tCurrent Value: %d\n\n",
	  obaseval(x), ovalue(x));
  bprintf("Properties: ");
  p = &(obits(x));
  show_bits((int *)p,sizeof(OFLAGS)/sizeof(int),Oflags);

  bprintf("\nState   Description:\n");

  for (i = 0; i < 4; i++) {
	  bprintf("[%d]  %s\n", i, olongt(x, i) == NULL ? "" : olongt(x, i));
  }
}



void wizlock()
{
    extern char *WizLevels[];
    extern char *MLevels[];
    extern char *FLevels[];

  static int k[] = { 0, LVL_WIZARD, LVL_WIZARD + 1, LVL_ARCHWIZARD,
		     LVL_DEMI, LVL_GOD };
  char s[80];
  char b[50];
  int l, v, n, x, y;


  l = the_world->w_lock;
  if (brkword() == -1) {
    bprintf("The game is currently %slocked.\n", lev2s(b,l));
    return;
  }
  if (!ptstflg(mynum,PFL_LOCK) || (v = plev(mynum)) <= 0){
    erreval();
    return;
  }
  if (l > v || v > LVL_WIZARD && l > k[wlevel(v)]) {
    bprintf("Sorry, the game is already %slocked.\n", lev2s(b,l));
    return;
  }

  y = strlen(wordbuf);
  
  if (strncasecmp(wordbuf,"Off", y) == 0
      || strncasecmp(wordbuf,"Unlock",y) == 0) {
    n = 0;
  } else if ((x = tlookup(wordbuf,WizLevels)) > 0) {
    n = k[x];
  } else if ((x = tlookup(wordbuf,MLevels)) > 0) {
    n = x;
  } else if ((x = tlookup(wordbuf,FLevels)) > 0) {
    n = x;
  } else if (isdigit(*wordbuf)) {
    n = atoi(wordbuf);
  } else {
    bprintf("Illegal argument to wizlock command.\n");
    return;
  }

  if (n > v || v > LVL_WIZARD && n > k[wlevel(v)]) {
    bprintf("You can't %slock the game!\n", lev2s(b,n));
    return;
  }
  the_world->w_lock = n;
  sprintf(s,"** SYSTEM : The Game is now %slocked.\007\007\n", lev2s(b,n));
  broad(s);
}



void warcom()
{
  if (plev(mynum) < LVL_WIZARD) {
    erreval();
    return;
  }
  the_world->w_peace = 0;
  broad("The air of peace and friendship lifts.\n");
}

void peacecom()
{
  int m;
  if (plev(mynum) < LVL_WIZARD) {
    erreval();
    return;
  }
  the_world->w_peace = 1;
  for (m=0 ; m < max_players ; m++) {
      setpfighting(m, -1);
  }
  broad("Peace and tranquility descend upon the world.\n");
}


#if 0
void tracecom()
{
  int i;

  if (!ptstflg(mynum, PFL_TRACE)) {
    erreval();
    return;
  }
  if (trace_item != -1) {
    bprintf("Stopped tracing %s.\n",
	    trace_class == 1 ? oname(trace_item) : pname(trace_item));
    trace_item = -1;
    if (brkword() == -1)
      return;
  }
  else if (EMPTY(item1)) {
    bprintf("Trace what?\n");
    return;
  }
  if ((i = fpbn(item1)) != -1) {
    if (!do_okay(mynum, i, PFL_NOTRACE)) {
      bprintf("They don't want to be traced.\n");
      return;
    }
    trace_class = 2;
    trace_item = i;
    trace_loc = 0;
    trace_carrf = IN_ROOM;
    bprintf("Ok\n");
    return;
  }
  if ((i = fobn(item1)) != -1) {
    trace_class = 1;
    trace_item = i;
    trace_loc = 0;
    trace_carrf = IN_ROOM;
    bprintf("Ok\n");
    return;
  }
  bprintf("What's that?\n");
}
#endif


void zapcom(void)
{
  int vic, x;
  char zb[100];

  if (!ptstflg(mynum, PFL_ZAP)) {
    bprintf("The spell fails.\n");
    return;
  }
  if (brkword() == -1) {
    bprintf("Zap who?\n");
    return;
  }
  if ((vic = pl1) == -1) {
    bprintf("There is no one on with that name.\n");
    return;
  }

  /* Message to everyone in the same room */
  send_msg(ploc(vic), 0, pvis(vic), LVL_MAX, vic, NOBODY,
	   "\001A\033[1m\003\001cA massive lightning "
	   "bolt strikes \003\001D%s\003\001c!\n\003\001A\033[0m\003",
	   pname(vic));

  if (!do_okay( mynum, vic, PFL_NOZAP)) {

    sendf(vic, "%s casts a lightning bolt at you!\n",
	  see_player(vic,mynum) ? pname(mynum) : "Someone");

  } /* end ZAP not successful */
  else {

      sendf(vic,"\001A\033[1m\003A massive lightning bolt arcs down out of "
	    "the sky to strike you between\nthe eyes!\001A\033[0m\003\n"
	    "You have been utterly destroyed by %s.\n",
	    see_player(vic,mynum) ? pname(mynum) : "Someone");


      if (vic < max_players) {

	mudlog("%s zapped %s", pname(mynum), pname(vic));

	if (plev(vic) >= LVL_WIZARD) {
	  update_wizlist( pname(vic), LEV_MORTAL);
	}

	deluaf(pname(vic));

	send_msg(ploc(vic),0, pvis(vic), LVL_MAX, vic, NOBODY,
		 "\001s%s\003%s has just died.\n\003",
		 pname(vic), pname(vic));

	/* Send info-msg. to wizards */
	send_msg(DEST_ALL, 0, LVL_WIZARD, LVL_MAX, vic, NOBODY,
		 "[\001p%s\003 has just been zapped by \001p%s\003]\n",
		 pname(vic), pname(mynum));
	
	x = real_mynum;
	setup_globals(vic);
	crapup("\t\tBye Bye.... Slain by a Thunderbolt", CRAP_RETURN);
	setup_globals(x);
	
      } /* end if PC was zapped */
      else {

	wound_player(mynum, vic, pstr(vic)+1, VERB_ZAP);

      } /* end if NPC was zapped */


  } /* end ZAP successful */

  broad("\001dYou hear an ominous clap of thunder in the distance.\n\002");

} /* end ZAPcom */



void wizcom(void)
{
  if (plev(mynum) < LVL_WIZARD) {
    bprintf("Such advanced conversation is beyond you.\n");
    return;
  }
  else if (ststflg(mynum, SFL_NOWIZ)) {
    nowizcom();
  }

  getreinput(wordbuf);

  send_msg(DEST_ALL, MODE_COLOR|MODE_NSFLAG|MS(SFL_NOWIZ), LVL_WIZARD, LVL_MAX,
	   NOBODY,NOBODY, "\001p%s\003: %s\n", pname(mynum), wordbuf);
}

void nowizcom(void)
{
  if (plev(mynum) < LVL_WIZARD) {
      erreval();
      return;
  }

  if (!ststflg(mynum, SFL_NOWIZ)) {
      ssetflg(mynum, SFL_NOWIZ);
      send_msg(DEST_ALL, MODE_COLOR|MODE_NSFLAG|MS(SFL_NOWIZ),
	       max(LVL_WIZARD, pvis(mynum)), LVL_MAX, mynum, NOBODY,
	       "[%s has left the wiz-channel]\n", pname(mynum));
      sendf(mynum, "You are no longer listening to the wiz-channel.\n");
  }
  else {
      sclrflg(mynum, SFL_NOWIZ);
      send_msg(DEST_ALL, MODE_COLOR|MODE_NSFLAG|MS(SFL_NOWIZ),
	       max(LVL_WIZARD, pvis(mynum)), LVL_MAX, mynum, NOBODY,
	       "[%s is back on the wiz-channel]\n", pname(mynum));
      sendf(mynum, "You are once again listening to the wiz-channel.\n");
  }
}

void awizcom(void)
{
  if (plev(mynum) < LVL_ARCHWIZARD) {
    bprintf("Such advanced conversation is beyond you.\n");
    return;
  }
  getreinput(wordbuf);

  send_msg(DEST_ALL, MODE_COLOR, LVL_ARCHWIZARD, LVL_MAX, NOBODY, NOBODY,
	   "\001p%s\003# %s\n", pname(mynum), wordbuf);
}

void godcom(void)
{
  if (plev(mynum) < LVL_DEMI) {
    bprintf("Such advanced conversation is beyond you.\n");
    return;
  }
  getreinput(wordbuf);

  send_msg(DEST_ALL, MODE_COLOR, LVL_DEMI, LVL_MAX, NOBODY, NOBODY,
	   "\001p%s\003%% %s\n", pname(mynum), wordbuf);
}


void debugcom(void)
{
	return;
}
