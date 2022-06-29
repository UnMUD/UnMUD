#include <unistd.h>
#include "kernel.h"
#include "locations.h"
#include "objects.h"
#include "mobiles.h"
#include "sflags.h"
#include "pflags.h"
#include "oflags.h"
#include "lflags.h"
#include "cflags.h"
#include "quests.h"
#include "sendsys.h"
#include "levels.h"
#include "jmp.h"
#include "commands.h"
#include "rooms.h"
#include "objsys.h"
#include "mobile.h"
#include "flags.h"
#include "bprintf.h"
#include "parse.h"
#include "uaf.h"
#include "clone.h"

extern char *WizLevels[];
extern char *MLevels[];
extern char *FLevels[];


void asmortalcom(void)
{
  int a = -1;

  if (cur_player -> aliased || cur_player -> polymorphed >= 0 ) {
      bprintf( "Not while aliased!\n" );
  } else if ( plev(mynum) < LVL_WIZARD ) {
      bprintf( "You're already a mortal!\n" );
  } else if (brkword() == -1 ) {
      cur_player -> asmortal = 0;
  } else if ( (a = tlookup(wordbuf,MLevels)) >= 0
	     || ( a = tlookup(wordbuf,FLevels)) >= 0
	     || ( a = atoi(wordbuf)) > 0 ) {
      if ( a <= 0 && a >= LVL_WIZARD ) a = -2;
      else cur_player -> asmortal = a;
  } else {
      a = -2;
  }
  if ( a > 0 ) {
     bprintf( "So you want to pretend to be a %s?\n",
	     psex(mynum) ? FLevels[a] : MLevels[a] );
  } else if (a == -2 ) {
     bprintf( "Whatever you wanted to do...it didn't work..\n" );
  }
}

void lookcom(void)
{
  int a;

  if (brkword() == -1) {
    lookin(ploc(mynum),SHOW_LONG);
  } else {
    if (EQ(wordbuf,"at")) {
      strcpy(item1,item2);
      ob1 = ob2;
      examcom();
    } else if (!EQ(wordbuf, "in") && !EQ(wordbuf, "into")) {
      bprintf("I don't understand.  Are you trying to LOOK, LOOK AT, "
	      "or LOOK IN something?\n");
    } else if (EMPTY(item2)) {
      bprintf("In what?\n");
    } else if ((a = ob2) == -1) {
      bprintf("What?\n");
    } else if (a == OBJ_BLIZZARD_TUBE     ||
	       a == OBJ_TREEHOUSE_BEDDING ||
	       a == OBJ_ORCHOLD_TOPCOVER) {
      strcpy(item1, item2);
      ob1 = ob2;
      examcom();
    } else if (!otstbit(a, OFL_CONTAINER)) {
      bprintf("That isn't a container.\n");
    } else if (otstbit(a, OFL_OPENABLE) && state(a) != 0) {
      bprintf("It's closed!\n");
    } else {
      bprintf("The %s contains:\n", oname(a));
      aobjsat(a, IN_CONTAINER, 8);
     }
  }
}

void wherecom()
{
  int cha, rnd, num_obj_found = 0, num_chars_found = 0;

  if (plev(mynum) < LVL_WIZARD && pstr(mynum) < 10) {
    bprintf("You're too weak to cast any spells.\n");
    return;
  }
  if (ltstflg(ploc(mynum), LFL_NO_MAGIC) && plev(mynum) < LVL_WIZARD) {
    bprintf("Something about this location has drained your mana.\n");
    return;
  }

  if (plev(mynum) < LVL_WIZARD)
    setpstr(mynum, pstr(mynum) - 2);
  rnd = randperc();
  cha = 6 * plev(mynum);
  if (carries_obj_type(mynum, OBJ_BLIZZARD_POWERSTONE) > -1 ||
      carries_obj_type(mynum, OBJ_BLIZZARD_POWERSTONE1) > -1 ||
      carries_obj_type(mynum, OBJ_BLIZZARD_POWERSTONE2) > -1 ||
      plev(mynum) >= LVL_LEGEND)
    cha = 100;
  if (rnd > cha) {
    bprintf("Your spell fails.\n");
    return;
  }

  if (!item1[0]) {
    bprintf("What's that?\n");
    return;
  }

  for (cha = 0; cha < numobs; cha++) {

    if (cha == num_const_obs && plev(mynum) < LVL_WIZARD) break;

    if (EQ(oname(cha), item1)
        || plev(mynum) >= LVL_WIZARD && EQ(oaltname(cha), item1)) {

      num_obj_found++;
      if (plev(mynum) >= LVL_WIZARD)
        bprintf("[%3d] ", cha);
      bprintf("%16.16s - ", oname(cha));
      if (plev(mynum) < LVL_WIZARD && ospare(cha) == -1)
        bprintf("Nowhere.\n");
      else
        desloc(oloc(cha), ocarrf(cha));
    }
  }

  for (cha = 0; cha < numchars; cha++) {

	  if (cha == num_const_chars && plev(mynum) < LVL_WIZARD) break;

	  if ( EQ(xname(pname(cha)), item1) ) {

		  num_chars_found++;

		  if (plev(mynum) >= LVL_WIZARD)
		          bprintf("[%d]", GLOBAL_MAX_OBJS + cha);

		  bprintf("%16.16s - ", pname(cha));
		  desloc(ploc(cha), 0);
	  }
  }

  
  if (num_obj_found == 0 && num_chars_found == 0)
          bprintf("I don't know what that is.\n");
}

static Boolean find_stuff(int s, int o, char *t)
{
  if (odamage(s) == 0) {
    osetdamage(s,1);
    bprintf(t);
    create(o);
    setoloc(o, ploc(mynum), IN_ROOM);
    return True;
  }
  return False;
}

void examcom(void)
{
  int a, foo, z;
  FILE *x;
  char ch;
  char *t;
  char text[80];

  if (!item1[0]) {
    bprintf("Examine what?\n");
    return;
  }

  if ((a = fpbn(item1)) != -1 && ploc(a) == ploc(mynum)) { /*220692*/
    if (pstr(a) < 1) {
      bprintf("You see the worm-infested corpse of %s.\n", pname(a));
      return;
    }
    if (a >= max_players && (t = pexam(a)) != NULL) {
      bprintf( "%s\n", t);
    } else {

	    if (a != mynum) {
		    sendf(a, "%s examines you closely.\n", pname(mynum));
	    }

      sprintf(text, "%s/%s", DESC_DIR, pname(a));
      if ((x = fopen(text, "r")) == NULL) {                    /*ACCESS!*/
	bprintf("A typical, run of the mill %s.\n", pname(a));
	return;
      }
      fclose(x);
      bprintf("\001f%s\003", text);
    }
    return;
  }

  if ((a = ob1) == -1) {
    bprintf("You see nothing special.\n");
    return;
  }

  send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
	   "%s examines the %s closely.\n", pname(mynum), oname(a));

  switch (onum(a)) {

#ifdef LOCMIN_ANCIENT

  case OBJ_ANCIENT_SKYMAP:
    if (ploc(mynum) == LOC_ANCIENT_ANC53) {
        bprintf("You feel like you are flying....\n"
                "     ....and land on a desolate planet,"
                "obviously far from the sun.\n\n\n");
        trapch(LOC_ANCIENT_ANC56);
    } else {
        bprintf("You ain't lost, Mac... yet!\n");
    }
    break;

  case OBJ_ANCIENT_LBOOK:
    if (oarmor(a) > 0) {
        bprintf("The Book contains old writings. You can make out\n"
                "the names of old mages like 'Elmo' and 'Infidel'.\n");
    } else {
    bprintf("Ancient wisdom fills your head, you feel much more capable...\n");
        osetarmor(a, 1);
    }
    break;
#endif

#ifdef LOCMIN_EFOREST
  case OBJ_EFOREST_TREE:
    if (odamage(a)==0) {
      osetdamage(a, 1);
      bprintf("You find a door in the tree.\n");
      setobjstate(a, 0);
      return;
    }
    break;
  case OBJ_EFOREST_DESK:
    if (find_stuff(OBJ_EFOREST_DESK,OBJ_EFOREST_BUTTON,
                   "You find a button hidden in a recess of the desk.\n"))
      return;
    break;
  case OBJ_EFOREST_THRONE_WOOD:
    if (find_stuff(OBJ_EFOREST_THRONE_WOOD,OBJ_EFOREST_CROWN,
                   "You find a crown hidden under the throne.\n"))
      return;
    break;
  case OBJ_EFOREST_COAT:
    if (find_stuff(OBJ_EFOREST_COAT,OBJ_EFOREST_COIN,
                   "You find a coin in the pocket.\n"))
      return;
    break;
  case OBJ_EFOREST_TABLE:
    if (find_stuff(OBJ_EFOREST_TABLE,OBJ_EFOREST_GAUNTLETS,
		   "You find some gauntlets admidst the rubble "
		   "of this table.\n"));
    return;
    break;
  case OBJ_EFOREST_PAINTING:
    if (find_stuff(OBJ_EFOREST_PAINTING,OBJ_EFOREST_NOTE,
                   "There was a note behind the painting.\n"))
      return;
    break;
  case OBJ_EFOREST_CHAIR:
    if (find_stuff(OBJ_EFOREST_CHAIR,OBJ_EFOREST_RING,
                   "A ring was between the cushions of that chair!\n"))
      return;
    break;
  case OBJ_EFOREST_RACK:
    if (find_stuff(OBJ_EFOREST_RACK,OBJ_EFOREST_SCARAB,
                   "You found a scarab in the rack.\n"))
      return;
    break;
  case OBJ_EFOREST_PAPERS:
    if (find_stuff(OBJ_EFOREST_PAPERS,OBJ_EFOREST_TREATY,
                   "Among the papers, you find a treaty.\n"))
      return;
    break;
  case OBJ_EFOREST_DESK_LICH:
    if (find_stuff(OBJ_EFOREST_DESK_LICH,OBJ_EFOREST_EMERALD,
                   "Inside the desk is a beautiful emerald.\n"))
      return;
    break;
  case OBJ_EFOREST_ALTAR:
    if (find_stuff(OBJ_EFOREST_ALTAR,OBJ_EFOREST_STATUE,
                   "Inside the altar is a statue of a dark elven deity.\n"))
      return;
    break;
  case OBJ_EFOREST_MATTRESS:
    if (find_stuff(OBJ_EFOREST_MATTRESS,OBJ_EFOREST_PURSE,
                   "Hidden under the mattress is a purse.\n"))
      return;
    break;
  case OBJ_EFOREST_TRASH:
    if (find_stuff(OBJ_EFOREST_TRASH,OBJ_EFOREST_TRASH_COIN,
		   "In the trash is a silver coin.\n"))
      return;
    break;
#endif

  case OBJ_BLIZZARD_TUBE:
    if (oarmor(a) == 0) {
	    int obj = clone_object(OBJ_BLIZZARD_SCROLL, -1, NULL);

	    if (obj >= 0) {
		    osetarmor(a, 1);
		    bprintf("You take a scroll from the tube.\n");
		    setoloc(obj, mynum, CARRIED_BY);
		    return;
	    }
    }
    break;

  case OBJ_BLIZZARD_SCROLL:
    if (iscarrby(OBJ_CATACOMB_CUPSERAPH, mynum)) {
      bprintf("Funny, I thought this was a teleport scroll, but "
	      "nothing happened.\n");
      return;
    }
    bprintf("As you read the scroll you are teleported!\n");
    destroy(a);
    teletrap(LOC_BLIZZARD_PENTACLE);
    return;

  case OBJ_BLIZZARD_BLACKROBE:
    if (oarmor(a) == 0) {
	    int obj = clone_object(OBJ_BLIZZARD_KEY, -1, NULL);
	    if (obj >= 0) {
		    bprintf("You take a key from one pocket.\n");
		    osetarmor(a, 1);
		    setoloc(obj, mynum, CARRIED_BY);
		    return;
	    }
    }
    break;

  case OBJ_TOWER_WAND:
    if (oarmor(a) != 0) {
      bprintf("It seems to be charged.\n");
      return;
    }
    break;

  case OBJ_BLIZZARD_BALL:
    setobjstate(a, randperc() % 3 + 1);
    switch (state(a)) {
    case 1:
      bprintf("It glows red.");
      break;
    case 2:
      bprintf("It glows blue.");
      break;
    case 3:
      bprintf("It glows green.");
      break;
    }
    bprintf("\n");
    return;

  case OBJ_TOWER_SCROLL:
    foo = carries_obj_type(mynum, OBJ_BLIZZARD_BALL);
    if (foo < 0) foo = OBJ_BLIZZARD_BALL;

    if ((z = carries_obj_type(mynum, OBJ_TOWER_RED_CANDLE)) > -1  &&
	state(foo) == 1           &&
	otstbit(z, OFL_LIT)     ||

	(z = carries_obj_type(mynum, OBJ_TOWER_BLUE_CANDLE)) > -1  &&
	state(foo) == 2           &&
	otstbit(z, OFL_LIT)    ||

	(z = carries_obj_type(mynum, OBJ_TOWER_GREEN_CANDLE)) > -1 &&
	state(foo) == 3           &&
	otstbit(z, OFL_LIT)) {

      bprintf("Everything shimmers and then solidifies into a different "
	      "view!\n");
      destroy(a);
      teletrap(LOC_TOWER_POTION);
      return;
    }
    break;

  case OBJ_VALLEY_BED:
    if (!odamage(a)) {
	    int c = clone_object(OBJ_VALLEY_LOAF, -1, NULL);
	    int b = clone_object(OBJ_VALLEY_PIE, -1, NULL);

	    if (c >= 0 && b >= 0) {
		    bprintf("Aha!  Under the bed you find a loaf and a "
			    "rabbit pie.\n");
		    setoloc(c, ploc(mynum), IN_ROOM);
		    setoloc(b, ploc(mynum), IN_ROOM);
		    osetdamage(a, 1);
		    return;
	    }
    }
    break;

#ifdef LOCMIN_ORCHOLD
  case OBJ_ORCHOLD_GARBAGE:
    if (state(a) == 0) {

	   int x = clone_mobile(max_players + MOB_ORCHOLD_MAGGOT, -1, NULL);

           if (x >= 0) {

		    bprintf("In the garbage you find a gold plate... "
			    "with a maggot on it!\n");

		    sillycom("\001s%s\003A maggot leaps out of the garbage "
			     "onto %s!\n\003");

		    setploc(x, ploc(mynum));

		    /* Make sure maggot attacks the right person. */
		    hit_player(x, mynum, -1);

		    setobjstate(a, 1);
	    } else {
		    bprintf("In the garbage you find a gold plate with some "
			    "slime on it.\n");
	    }

	    if ((x = clone_object(OBJ_ORCHOLD_ORCGOLD, -1, NULL)) > -1) {

		    setoloc(x, ploc(mynum), IN_ROOM);
		    setobjstate(a, 1);
	    }

	    return;
    }
    break;
#endif

  case OBJ_TREEHOUSE_BEDDING:
    if (a != OBJ_TREEHOUSE_BEDDING) break;

    if (!odamage(OBJ_TREEHOUSE_AMULET)) {
      create(OBJ_TREEHOUSE_AMULET);
      bprintf("You pull an amulet from the bedding.\n");
      osetdamage(OBJ_TREEHOUSE_AMULET, 1);
      return;
    }
    break;

  case OBJ_OAKTREE_BOARSKIN:
    if (a != OBJ_OAKTREE_BOARSKIN) break;

    if (!odamage(OBJ_OAKTREE_WHISTLE)) {
      create(OBJ_OAKTREE_WHISTLE);
      bprintf("Under the boarskin you find a silver whistle.\n");
      osetdamage(OBJ_OAKTREE_WHISTLE, 1);
      return;
    }
    break;
  }

  if (oexam_text(a) != NULL) {
	  bprintf("%s", oexam_text(a));
  }
  else
  if (oexamine(a) != 0 && (x = fopen(OBJECTS, "r")) != NULL) {

	  fseek(x, oexamine(a), 0);
	  while ((ch = fgetc(x)) != '^')
	      bprintf("%c", ch);
	  fclose(x);
  } else {
	  bprintf("You see nothing special.\n");
	  return;
  }
}

void incom(Boolean inv)
{
  int x, y;
  char st[MAX_COM_LEN];

  if (!ptstflg(mynum,PFL_GOTO)) {
    /* Mortals who doesn't have goto will assume that in = inventory... */
    /* Thus hiding the fact that there IS a command called in... */
    /* Must fix it so that at this should only be the case when IN is used */
    /* and not when AT is used */

    /* old: erreval(); */

    if (inv)
      inventory();
    else
      bprintf("Pardon?\n");
    return;
  }
  if ((x = getroomnum()) == 0 || !exists(x)) {
    bprintf("Unknown Player Or Room\n");
    return;
  }
  getreinput(st);
  if (EQ(st,"!")) {
    bprintf("What exactly do you want to do?\n");
    return;
  }
  y = ploc(mynum);
  setploc(mynum, x);
  gamecom(st, False);
  setploc(mynum, y);
}

/* Hit the reset-stone.
 */
void sys_reset(void)
{
  int    i;

  for (i = 0; i < max_players; i++) {
    if (is_in_game(i) && plev(i) < LVL_WIZARD && ploc(i) != ploc(mynum)) {
      bprintf("There are other players on who are not at this location,"
	      " so it won't work.\n");
      return;
    }
  }

  if (last_reset < global_clock && global_clock < last_reset + 3600) {
    bprintf("Sorry, at least an hour must pass between resets.\n");
    return;
  }
  resetcom(0);
}


/* The RESET command.
 */
void resetcom(int flags)
{
	if ((flags & RES_TEST) != 0 && !ptstflg(mynum, PFL_RESET)) {
		erreval();
		return;
	}

	if (brkword() != -1 && plev(mynum) >= LVL_WIZARD) {
		int zone, r_locs, r_mobs, r_objs, nlocs, nmobs, nobjs,
		          d_locs, d_mobs, d_objs;

		if ((zone = get_zone_by_name(wordbuf)) == -1) {

			bprintf("No such zone: %s\n", wordbuf);
			return;
		}

		nlocs = znumloc(zone);
		nobjs = znumobs(zone);
		nmobs = znumchars(zone);

		reset_zone(zone, &global_clock, &d_locs, &d_mobs, &d_objs,
			   &r_locs, &r_mobs, &r_objs);

		if (!ztemporary(zone)) {

			bprintf("Zone %s: Reset Completed.\n", zname(zone));

			bprintf("\n%3d (from %d) "
				"Room(s) were successfully reset.\n",
				r_locs, nlocs - d_locs);

			bprintf("%3d (from %d) "
				"Object(s) were successfully reset.\n",
				r_objs, nobjs - d_objs);

			bprintf("%3d (from %d) "
				"Mobile(s) were successfully reset.\n",
				r_mobs, nmobs - d_mobs);
		} else {
			bprintf("Zone %s:\n\n"
				"%d Room(s) destroyed.\n"
				"%d Objects(s) destroyed.\n"
				"%d Mobiles(s) destroyed.\n",
				zname(zone),
				nlocs, nobjs, nmobs);
		}
	} else {
		int i;
		last_reset = global_clock;
		broad("\001A\033[31m\003Reset in progress....\n");
		mudlog("RESET by %s", pname(mynum));

		for (i = 0; i < numzon; i++) {

			reset_zone(i, &global_clock,
				   NULL, NULL, NULL, NULL, NULL, NULL);
		}

		broad("Reset Completed....\001A\033[0;37m\003\n");
	}
}



void unveilcom(char *unv_pass)
{
  int lev;

  if (!cur_player->isawiz) {
    erreval();
    return;
  }

  if (unv_pass == NULL) {
    if (brkword() == -1) {
      cur_player->work = LVL_GOD;
    } else {
      cur_player->work = atoi(wordbuf);
    }
    strcpy(cur_player->cprompt, "Magic Word: ");

    /*          IAC WILL ECHO */
    bprintf( "\n\377\373\001\001Magic Word: " );
    cur_player->no_echo = True;
    push_input_handler(unveilcom);
  } else {
    bprintf("\377\374\001\001");
    cur_player->no_echo = False;
    pop_input_handler();
    if (!EQ(unv_pass,UNVEIL_PASS)) {
      bprintf( "Eek!  Go away!\a\n" );
      mudlog("BAD UNVEIL: %s", pname(mynum));
    } else {
      lev = cur_player->work;
      if (lev >= LVL_MAX) {
	mudlog("WRONG UNVEIL: %s to %d", pname(mynum), plev(mynum));
	bprintf("The maximum level is %d.\n", LVL_MAX - 1);
      } else {
	set_xpflags(lev,&pflags(mynum),&pmask(mynum));
	setplev(mynum,lev);
	update_wizlist( pname(mynum), wlevel(lev));
	mudlog("UNVEIL: %s to %d", pname(mynum), plev(mynum));
	bprintf("Certainly, master!\n");
      }
    }
    get_command(NULL);
  }
}


void posecom(void)
{
  char x[128];
  int       n;
  int       m;

  char *POSE[] = { "gestures", "fireball", "hamster", 
                   "sizzle", "crackle", TABLE_END };

  if (plev(mynum) < LVL_MAGICIAN) {
    bprintf("You're not up to this yet.\n");
	return;
  }

  if (brkword() != -1 ) {
    if (plev(mynum) < LVL_WIZARD) {
      bprintf("Usage: POSE\n");
      return;
    }
    if(((n = atoi(wordbuf)) > 5 || n < 1)  &&
       (m = tlookup(wordbuf, POSE)) == -1 )  {
      bprintf("Usage: POSE <gestures/fireball/hamster/sizzle/crackle>\n");
      return;
    }
    if (m != -1) n = m; else --n;
  } else n = randperc() % 5;

  switch (n) {
  case 0:
    sprintf(x, "\001s%%s\003%%s raises %s arms in mighty magical "
	    "invocations.\n\003", his_or_her(mynum));
	sillycom(x);
    bprintf("You make mighty magical gestures.\n");
	break;
  case 1:
    sillycom("\001s%s\003%s throws out one arm and sends a huge bolt of "
	     "fire high into the sky.\n\003");
    bprintf("You toss a fireball high into the sky.\n");
    broad("\001cA massive ball of fire explodes high up in the sky.\n\003");
	break;
  case 2:
    sillycom("\001s%s\003%s turns casually into a hamster before resuming "
	     "normal shape.\n\003");
    bprintf("You casually turn into a hamster before resuming normal "
	    "shape.\n");
    break;
  case 3:
    sillycom("\001s%s\003%s starts sizzling with magical energy.\n\003");
    bprintf("You sizzle with magical energy.\n");
    break;
  case 4:
    sillycom("\001s%s\003%s begins to crackle with magical fire.\n\003");
    bprintf("You crackle with magical fire.\n");
    break;
  }
}

void praycom(void)
{

    sillycom("\001s%s\003%s falls down and grovels in the dirt.\n\003");
    bprintf("You fall down and grovel in the dirt.\n");
}


/* Brian Preble -- shows current set*in/set*out settings.
 * (Improved by Alf. Extended to include other players setins by Nicknack.)
 */
void reviewcom(void)
{
  char       xx[SETIN_MAX+1];
  SETIN_REC  s;
  PERSONA    p;
  int        x;
  Boolean    me = False;
  PLAYER_REC *v = NULL;
  char *in_ms, *out_ms, *min_ms, *mout_ms;
  char *vin_ms, *vout_ms, *qin_ms, *qout_ms;
  char *pro;
  char *name;

  if (brkword() == -1 || EQ(wordbuf, pname(mynum))) {
    me = True;
    v = cur_player;
    x = mynum;
  } else if (plev(mynum) < LVL_WIZARD) {
    erreval();
    return;
  } else if ((x = fpbns(wordbuf)) >= 0 && x < max_players &&
	     plev(x) >= LVL_WIZARD) {
    v = players + x;
  } else if (!getuaf(wordbuf, &p) || p.p_level < LVL_WIZARD) {
    bprintf("I know no wizard with the name '%s'.\n", wordbuf);
    return;
  } else if (!getsetins(wordbuf, &s)) {
    bprintf("%s has no non-standard travel-messages.\n", p.p_name);
    return;
  }
  if (v != NULL) {
    in_ms   = v->setin;
    out_ms  = v->setout;
    min_ms  = v->setmin;
    mout_ms = v->setmout;
    vin_ms  = v->setvin;
    vout_ms = v->setvout;
    qin_ms  = v->setqin;
    qout_ms = v->setqout;
    pro     = v->prompt;

    name = pname(x);

  } else {
    in_ms   = s.setin;
    out_ms  = s.setout;
    min_ms  = s.setmin;
    mout_ms = s.setmout;
    vin_ms  = s.setvin;
    vout_ms = s.setvout;
    qin_ms  = s.setqin;
    qout_ms = s.setqout;
    pro     = s.prompt;

    name = p.p_name;
  }

  bprintf("Current travel messages");
  if (!me) bprintf( " for %s", name);
  bprintf( ":\n------------------------\n");
  bprintf("SETIN   : %s\n", build_setin(xx, in_ms, name, NULL));
  bprintf("SETOUT  : %s\n", build_setin(xx,out_ms, name,"<dir>"));
  bprintf("SETMIN  : %s\n", build_setin(xx, min_ms, name, NULL));
  bprintf("SETMOUT : %s\n", build_setin(xx, mout_ms, name,NULL));
  bprintf("SETVIN  : %s\n", build_setin(xx, vin_ms, name, NULL));
  bprintf("SETVOUT : %s\n", build_setin(xx, vout_ms, name,NULL));
  bprintf("SETQIN  : %s\n", build_setin(xx, qin_ms, name, NULL));
  bprintf("SETQOUT : %s\n", build_setin(xx, qout_ms, name,NULL));
  if (!me) bprintf("PROMPT  : %s\n", pro);
}



static char* shout_test(int player, int sender, char *text)
{
  static char buff[MAX_COM_LEN];

  if (player == sender ||
      plev(player) >= LVL_WIZARD && ststflg(player, SFL_NOSHOUT) ||
      plev(player) < LVL_WIZARD && ststflg(player, SFL_DEAF))
      /* || ltstflg(ploc(player), LFL_SOUNDPROOF)
	    && ploc(sender) != ploc(player)  ADD IF SOUNDPROOF IS IMPL.*/
    return NULL;

  if (plev(player) >= LVL_WIZARD || plev(sender) >= LVL_WIZARD
      || ploc(player) == ploc(sender)) {
    sprintf(buff, "%s shouts '%s'\n", see_name(player, sender), text);
  }
  else sprintf(buff, "A voice shouts '%s'\n", text);

  return buff;
}

void shoutcom(void)
{
  char blob[MAX_COM_LEN];

  if (plev(mynum) < LVL_WIZARD && ststflg(mynum, SFL_NOSHOUT)) {
    bprintf("I'm sorry, you can't shout anymore.\n");
    error();
  } else {
    getreinput(blob);
    if (EMPTY(blob)) {
      bprintf("What do you want to shout?\n");
      error();
    } else 
      send_g_msg(DEST_ALL, shout_test, mynum, blob);
  }
}



void saycom(void)
{
  char foo[MAX_COM_LEN];

  getreinput(foo);
  if (EMPTY(txt1)) {
    bprintf("What do you want to say?\n");
    error();
  } else {
    send_msg(ploc(mynum), MODE_NODEAF, pvis(mynum), LVL_MAX, mynum, NOBODY,
	     "%s says '%s'\n", pname(mynum), foo);

    send_msg(ploc(mynum), MODE_NODEAF, LVL_MIN, pvis(mynum), mynum, NOBODY,
	     "Someone says '%s'\n", foo);

  }
}


void tellcom(void)
{
  int b;

  if (EMPTY(item1)) {
    bprintf("Tell who?\n");
    return;
  }

/*  bprintf( "tellcom: item1 = %s, pl1 = %d, Nicknack = %d\n",
	  item1, pl1, fpbns(item1));
*/

  if ((b = pl1) == -1) {
    bprintf("No one with that name is playing.\n");
    error();
  }
  if (b == mynum) {
    bprintf("You talk to yourself.\n");
    return;
  }
  if (EMPTY(txt2)) {
    bprintf("What do you want to tell them?\n");
    error();
  }
  if (check_busy(b)) return;

  if (b == (max_players + MOB_CATACOMB_SERAPH)) {
    if (strchr(txt2, '?')) {
      switch (my_random() % 4) {
      case 0: sprintf(txt2, "Charity");
        break;
      case 1: sprintf(txt2, "Faith");
        break;
      case 2: sprintf(txt2, "Wisdom");
        break;
      case 3: sprintf(txt2, "Courage");
        break;
      }
    }
    else
      sprintf(txt2, "A blessing be upon your house.");
  }
  sendf(b, "%s tells you '%s'\n", see_name(b, mynum), txt2);
}



void scorecom(void)
{
  static char *t[] = {
    "%s near death.\n",
    "%s near death.\n",
    "%s mortally wounded.\n",
    "%s seriously wounded.\n",
    "some wounds, but %s still fairly strong.\n",
    "minor cuts and abrasions.\n",
    "minor cuts and abrasions.\n",
    "feel a bit dazed.\n",
    "%s in better than average condition.\n",
    "%s in exceptional health.\n",
    "%s in exceptional health.\n"};


  int p;
  int x;
  int plx = mynum;

  if (plev(plx) >= LVL_WIZARD && brkword() != -1 &&
      (plx = fpbn(wordbuf)) < 0) {
    bprintf( "Hmm...%s seems to be hiding...\n", wordbuf);
  } else if (plx >= max_players) {
    if (plx == mynum) {
      bprintf( "Your " );
    } else {
      bprintf( "%s's ", pname(plx));
    }
    bprintf("strength is %d.\n", pstr(plx));
  } else {
    p = (int) (pstr(mynum) * 10 / maxstrength(mynum));
    if (p > 10) p = 10;
    if (plx == mynum) {
      bprintf( "You " );
      if (p >= 4 && p <= 6) bprintf( "have " );
      bprintf( t[p], "are" );
      bprintf("Your strength is %d (from %d), your score is %d\n",
	      pstr(plx), maxstrength(plx), pscore(plx));
      bprintf("This ranks you as %s\n", make_title(ptitle(plx), pname(plx)));
    } else {
      bprintf( "%s ", pname(plx));
      if (p >= 4 && p <= 6) bprintf( "has ");
      bprintf( t[p], "is" );
      bprintf("%s's strength is %d (from %d), %s score is %d\n",
	      pname(plx), pstr(plx), maxstrength(plx),
	      his_or_her(plx), pscore(plx));
      bprintf("This ranks %s as %s\n",
	      him_or_her(plx),
	      make_title(ptitle(plx), pname(plx)));
    }
  }
}

void sitcom(void)
{
  if (psitting(mynum)) {
    bprintf("You're already sitting.\n");
    return;
  } else if (pfighting(mynum) >= 0) {
    bprintf("You want to sit down while fighting?  Do you have a death "
	    "wish or something?\n");
    return;
  } else{
    bprintf("You assume the lotus position.\n");
    send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
	     "%s sits down.\n", pname(mynum));
    setpsitting(mynum, 1);
  }
}

void standcom(int player)
{
  if (!psitting(player)) {
    sendf(player, "You're already standing.\n");
    return;
  } else {
    sendf(player, "You clamber to your feet.\n");
    send_msg(ploc(player), 0, pvis(player), LVL_MAX, player, NOBODY,
	     "%s clambers to %s feet.\n", pname(player), his_or_her(player));
    setpsitting(player, 0);
  }
}

void pncom(void)
{
  int plx = real_mynum;
  char *p;

  if (brkword() == -1 || (plx = fpbn(wordbuf)) == -1 ||
    plx >= max_players ||  plx != mynum && plev(mynum) < LVL_WIZARD) {
    plx = real_mynum;
  }
  if (plx == real_mynum) {
    bprintf( "Current pronouns are:\n" );
  } else {
    bprintf( "Current pronouns for %s are:\n", pname(plx));
  }
  bprintf("Me    : %s\n", pname(plx));
  bprintf("It    : %s\n", (p = players[plx].wd_it) == NULL ? "<none>" : p);
  bprintf("Him   : %s\n", (p = players[plx].wd_him) == NULL ? "<none>" : p);
  bprintf("Her   : %s\n", (p = players[plx].wd_her) == NULL ? "<none>" : p);
  bprintf("Them  : %s\n", (p = players[plx].wd_them) == NULL ? "<none>" : p);
}

void bugcom(void)
{
  char x[MAX_COM_LEN];

  getreinput(x);
  if (EMPTY(txt1)) {
    bprintf("What do you want to bug me about?\n");
  } else {
    mudlog("BUG BY %s: %s", pname(mynum), x);
    bprintf("Ok.\n");
  }
}

void typocom(void)
{
  int l = ploc(mynum);
  char x[MAX_COM_LEN], y[80];

  getreinput(x);
  if(EMPTY(x)) {
    bprintf("What typo do you wish to inform me of?\n");
  } else {
    mudlog("TYPO BY %s in %s [%d|%s]: %s", pname(mynum),
	   xshowname(y,l), l, sdesc(l), x);
    bprintf("Ok.\n");
  }
}


void helpcom(void)
{
  int a, b;

  if (item1[0] != 0) {
    if (showhelp(item1) < 0)
      return;
    if ((a = vichere()) < 0)
      return;
    if ((b = phelping(mynum)) >= 0) {
      sillytp(b, "stopped helping you.");
      bprintf("Stopped helping \001p%s\003.\n", pname(b));
    }
    if (a == mynum) {
      bprintf("You are beyond help.\n");
      return;
    }
    setphelping(mynum, a);
    sillytp(a, "has offered to help you.");
    bprintf("Started helping \001p%s\003.\n", pname(phelping(mynum)));
    return;
  }
  bprintf("\001f%s\003", HELP1);
  if (plev(mynum) < LVL_WIZARD) {
    return;
  }
  bprintf( pwait );
  replace_input_handler(help2);
}

void help2(char *cont)
{
  bprintf("\001f%s\003", HELP2);
  bprintf("\n");
  if (plev(mynum) < LVL_ARCHWIZARD) {
    get_command(NULL);
  } else {
    bprintf(pwait);
    replace_input_handler(help3);
  }
}

void help3(char *cont)
{
  bprintf("\001f%s\003", HELP3);
  bprintf("\n");
  if (plev(mynum) < LVL_DEMI) {
    get_command(NULL);
  } else {
    bprintf(pwait);
    replace_input_handler(help4);
  }
}

void help4(char *cont)
{
  bprintf("\001f%s\003", HELP4);
  bprintf("\n");
  get_command(NULL);
}

int showhelp(char *verb)
{
  char file[80];
  char outc[80];
  char line[80];
  int  scanreturn = EOF;
  int v;
  char ch;
  FILE *fp;

  sprintf(file, "../bin/pfilter %d 0x%08x:0x%08x 0x%08x:0x%08x " FULLHELP,
          plev(mynum), pflags(mynum).h, pflags(mynum).l,
          pmask(mynum).h, pmask(mynum).l);
  if ((fp = popen(file, "r")) == NULL) {
    bprintf("Someone's editing the help file.\n");
    return -1;
  }
  ssetflg(mynum,SFL_BUSY);
  pbfr();
  v = strlen(verb);
  while (True) {
    if (fgets(line, sizeof(line), fp) == NULL) {
      scanreturn = EOF;
      break;
    }
    if ((scanreturn = strncasecmp(line, verb, v)) == 0) break;
    do {
      if (fgets(line, sizeof(line), fp) == NULL) {
        scanreturn = EOF;
        break;
      }
    } while (strcmp(line,"^\n") != 0);
  }

  if (scanreturn != 0) {        /* command not found in extern list */
    pclose(fp);
    sclrflg(mynum,SFL_BUSY);
    return 0;
  }

  bprintf("\nUsage: %s\n", line);
  while (fgets(line,sizeof(line),fp) && strcmp(line,"^\n") != 0) {
    bprintf("%s", line);
  }
  bprintf("\n");

  pclose(fp);
  sclrflg(mynum,SFL_BUSY);
  return -1;
}




/* The QUESTS-Command. Mortals may find out which quests they have
 * comleted and which are left. Arch-Wizards may in addition set or
 * clear quests for a mortal, Wizards only for themselves.
 * Usage: QUESTS <player> <questname> <true/false>
 */
void questcom(void)
{
  int a, b, c, l;
  char *n;
  Boolean f, all;
  QFLAGS q, *p;
  PERSONA d;

  f = False;

  if (brkword() == -1) {
    a = mynum;
    p = &(qflags(a));
    n = pname(a);
    l = plev(a);
  } else if ((a = fpbn(wordbuf)) != -1) {
    if (a != mynum && plev(mynum) < LVL_WIZARD) {
      bprintf("You can only check your own Quest-stats.\n");
      return;
    }
    p = &(qflags(a));
    n = pname(a);
    l = plev(a);
  } else if (!getuaf(wordbuf,&d)) {
    bprintf("No such persona in system.\n");
    return;
  } else if (plev(mynum) < LVL_WIZARD) {
    bprintf("You can only check your own Quest-stats.\n");
    return;
  } else {
    f = True;
    p = &(d.p_quests);
    n = d.p_name;
    l = d.p_level;
  }

  if (brkword() == -1) {
    bprintf("\nPlayer: %s\n\n", n);

    all = False;
    bprintf("Completed Quests:\n");
    if ((*p & Q_ALL) == Q_ALL) {
      bprintf("All!\n");
      all = True;
    } else if ((*p & Q_ALL) != 0) {
      show_bits((int *)p, sizeof(QFLAGS)/sizeof(int), Quests);
    } else {
      bprintf("None!\n");
    }
    if (!all) {
      bprintf("\nStill to do:\n");
      q = (~*p & Q_ALL);
      show_bits((int *)&q, sizeof(QFLAGS)/sizeof(int), Quests);
    }
    return;
  } else if ((b = tlookup(wordbuf,Quests)) == -1) {
    bprintf("%s: No such Quest.\n", wordbuf);
    return;
  } else if (brkword() == -1 || plev(mynum) < LVL_WIZARD) {
    c = xtstbit(*p,b)?1:0;
    bprintf("Value of %s is %s\n", Quests[b], TF[c]);
    return;
  } else if (plev(mynum) < LVL_ARCHWIZARD && !EQ(n, pname(mynum)) ) {
    bprintf("You can't change other players Quest-stats.\n");
    return;
  } else if ((c = tlookup(wordbuf,TF)) == -1) {
    bprintf("Value must be True or False.\n");
    return;
  }

  mudlog("Quest: %s by %s, %s := %s", n, pname(mynum), Quests[b], TF[c]);

  if (c == 0) {
    xclrbit(*p,b);
  } else {
    xsetbit(*p,b);
  }
  if (f) {
    putuaf(&d);
  }
}

/* The INFO command.
 */
void infocom(void)
{
  char file[100];

  if (brkword() == -1) {
    strcpy(file, INFO);
  } else {
    sprintf(file, "%s.i", lowercase(wordbuf));
    if (access(file, R_OK) < 0) {
      bprintf("No info available on that topic.\n");
      return;
    }
  }
  bprintf("\001f%s\003", file);
}

/* The GLOBAL command.
 */
void globalcom(void)
{
  char buff[80];

  if (plev(mynum) < LVL_WIZARD) {
    erreval();
    return;
  } else if (plev(mynum) >= LVL_GOD && dump_pflags()) return;

  bprintf("Global Settings:\n===============\n");
  bprintf("Weather           : %s\n", wthr_type(the_world->w_weather));
  bprintf("Lock-status       : %slocked\n", lev2s(buff, the_world->w_lock));
  bprintf("Mobiles           : %s\n",
	  (the_world->w_mob_stop) ? "Stopped" : "Started");
  bprintf("War/Peace         : %s\n", (the_world->w_peace) ? "Peace" : "War");
  bprintf("Tournament-Mode   : %s\n",
	  (the_world->w_tournament) ? "On" : "Off");
  bprintf("Max. Users        : %d\n", max_players);
  bprintf("Number of Rooms   : "
	  "%4d permanent +%4d wiz-made = %4d (max %d)\n",
	  num_const_locs, numloc - num_const_locs, numloc, GLOBAL_MAX_LOCS);
  bprintf("Number of Mobiles : "
	  "%4d permanent +%4d wiz-made = %4d (max %d)\n",
	  num_const_chars - max_players, numchars - num_const_chars,
	  numchars - max_players, GLOBAL_MAX_MOBS);
  bprintf("Number of Objects : "
	  "%4d permanent +%4d wiz-made = %4d (max %d)\n",
	  num_const_obs, numobs - num_const_obs, numobs, GLOBAL_MAX_OBJS);
}

static void do_pretend(int plx)
{
  int p;

  if ((p = cur_player->pretend) >= 0) {
    /* We are someone else, unalias him */
    sclrflg(p,SFL_OCCUPIED);
    mynum = real_mynum;
    cur_player->pretend = -1;
  }
  if (plx < 0) {
    /* Back to ourselves */
    if (p < 0) return; /* we already are ourselves */

    strcpy(cur_player->setin, cur_player->o_setin);
    strcpy(cur_player->setout, cur_player->o_setout);
  } else {
    /* We will pretend to be plx from now */
    mynum = plx;
    cur_player->pretend  = plx;
    ssetflg(plx, SFL_OCCUPIED);
    if (p < 0) {
      strcpy(cur_player->o_setin, cur_player->setin);
      strcpy(cur_player->o_setout, cur_player->setout);
    }
    if (plx >= max_players) {
      strcpy(cur_player->setin, "%n has arrived.");
      strcpy(cur_player->setout, "%n has gone %d.");
    } else {
      strcpy(cur_player->setin,players[plx].setin);
      strcpy(cur_player->setout,players[plx].setout);
    }
  }
}

int find_pretender(int plx)
{
  int p;

  if (ststflg(plx,SFL_OCCUPIED)) {
    for (p = 0; p < max_players; p++) {
      if (players[p].pretend == plx && is_in_game(p)) {
	return p;
      }
    }
  }
  return -1;
}

void aliascom(void)
{
  if (pl1 == -1) {
    if (ptstflg(mynum,PFL_ALIAS) || ptstflg(mynum,PFL_ALIASP)) {
      bprintf("Who?\n");
      error();
    } else {
      erreval();
    }
    return;
  }
  if (cur_player->polymorphed >= 0) {
    bprintf("A mysterious force stops you.\n");
    error();
    return;
  }

  if (ststflg(pl1, SFL_OCCUPIED)) {
    bprintf("Already occupied!\n");
    error();
    return;
  }

  if (pl1 < max_players ) {
    if (!ptstflg(mynum, PFL_ALIASP)) {
      bprintf("You can't become another player!\n");
      error();
      return;
    }
    if (!do_okay( mynum, pl1, PFL_NOALIAS)) {
      if ( players[pl1].asmortal > 0 && plev(mynum) <= LVL_WIZARD)
        bprintf("Already occupied!\n");
      else
        bprintf("They don't want to be aliased!\n");
      error();
      return;
    }
  } else if (!ptstflg(mynum, PFL_ALIAS)) {
    bprintf("You can't become a mobile!\n");
    error();
    return;
  }

  do_pretend(pl1);
  cur_player->aliased = True;
  bprintf("Aliased to %s.\n", pname(pl1));
}

void polymorph(int plx,int turns)
{
  /* Polymorph to PLX for TURNS turns. */
  if (plx < 0 || turns < 0) {
    unpolymorph(real_mynum);
    return;
  }
  do_pretend(plx);
  cur_player->polymorphed = turns;
  bprintf("You pass out.....\n..... and wake up.\n");
}
  

void unalias(int pl)
{
  int me = real_mynum;

  setup_globals(pl);
  if (cur_player->aliased) {
    do_pretend(-1);
    cur_player->aliased = False;
    bprintf("Back to good old %s....\n", pname(mynum));
  }
  setup_globals(me);
}

void unpolymorph(int pl)
{
  int me = real_mynum;

  setup_globals(pl);
  if (cur_player->polymorphed >= 0) {
    do_pretend(-1);
    cur_player->polymorphed = -1;
    setpfighting(mynum,-1);
    bprintf("Suddenly you awake... were you dreaming or what?\n");
  }
  setup_globals(me);
}



Boolean disp_file(char *fname, FILE *f)
{
  char buff[BUFSIZ];

  if (f == NULL && (f = fopen(fname, "r")) == NULL) {
    return False;
  }

  while (fgets(buff, BUFSIZ, f) != NULL) {
    bprintf("%s", buff);
  }
  fclose(f);
  return True;
}




void becom(char *passwd)
{
  char x[128];
  PERSONA p;
  SETIN_REC s;
  Boolean f;

  if (passwd == NULL) {

    if (cur_player->polymorphed != -1 || cur_player->aliased) {
      bprintf("Not when aliased.");
      error();
      return;
    }
    if (cur_player->writer != NULL) {
      bprintf("Finish whatever you're writing first!\n");
      error();
      return;
    }
    if (pfighting(mynum) != -1) {
      bprintf("Not while fighting!\n");
      return;
    }
    if (brkword() == -1) {
      bprintf("Become what?  Inebriated?\n");
      return;
    }
    strcpy(cur_player->work2, wordbuf);
    strcpy(cur_player->cprompt, "Password: ");
    cur_player->no_echo = True;
    /*          IAC WILL ECHO */
    bprintf( "\n\377\373\001\001Password: " );
    push_input_handler(becom);
  } else {
    bprintf("\377\374\001\001");
    cur_player->no_echo = False;
    pop_input_handler();

    if (!getuaf(cur_player->work2, &p)) {
      bprintf("No such player.\n");
    } 
    else if (!EQ(p.p_passwd, my_crypt(x, passwd, strlen(p.p_passwd)+1))) {
      bprintf("Incorrect password!\n");
    }
    else if (fpbns(cur_player->work2) >= 0) {
      bprintf("That player is allready on the game.\n");
    }
    else {
      saveme();
      remove_entry(mob_id(mynum), &id_table);

      pers2player(&p, mynum);
      strcpy(cur_player->cprompt,cur_player->prompt);
      mudlog("%s has BECOME %s", pname(mynum), p.p_name);

      send_msg(DEST_ALL, MODE_QUIET, max(LVL_WIZARD, pvis(mynum)), LVL_MAX,
	       mynum, NOBODY, "[%s has BECOME %s]\n", pname(mynum), p.p_name);
      send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
	       "%s disintegrates, and reappears as %s.\n",
	       pname(mynum), p.p_name);

      setpname(mynum, p.p_name);
      fetchprmpt(mynum);
      insert_entry(mob_id(mynum), mynum, &id_table);

      bprintf("You are now %s.\n", pname(mynum));

      if (ststflg(mynum,SFL_MAIL)) {
	bprintf("\n**You have mail**\n\n");
      }
    }
    get_command(NULL);
  }
}



void rollcom(void)
{
  int a;

  if ((a = ohereandget()) == -1)
    return;
  switch (a) {
  case OBJ_BLIZZARD_PILLAR_WEST:
  case OBJ_BLIZZARD_PILLAR_EAST:
    gamecom("push pillar", True);
    break;
  case OBJ_BLIZZARD_BOULDER:
    gamecom("push boulder", True);
    break;
  default:
    bprintf("You can't roll that.\n");
  }
}

void emptycom(void)
{
  int a, b, c;
  char x[128];

  if ((a = ohereandget()) == -1)
    return;

  if (otstbit(a, OFL_LOCKABLE) && state(a) == 2 && !ohany(1<<OFL_KEY)) {
	  bprintf("The %s is locked, and you have no key.\n", oname(a));
	  return;
  } else
    if ((otstbit(a, OFL_OPENABLE) || otstbit(a, OFL_LOCKABLE))
	&& state(a) > 0) {
          bprintf("You open the %s.\n", oname(a));
          setobjstate(a, 0);
  }

  for (b = ofirst_obj(a); b != SET_END; b = onext_obj(a)) {
    if (iscontin(b, a)) {
      setoloc(b, mynum, CARRIED_BY);
      bprintf("You empty the %s from the %s.\n", oname(b), oname(a));
      sprintf(x, "drop %s", oname(b));
      gamecom(x, False);
    }
  }
}

void dircom(void)
{
  int a, c, oc = 0;
  char b[40], d[40];

  if (plev(mynum) < LVL_WIZARD) {
    erreval();
    return;
  }
  for (a = 0; a < numobs; a++) {
    oc++;
    c = findzone(oloc(a), b);
    sprintf(d, "%s%d", b, c);
    if (ocarrf(a) >= CARRIED_BY)
      strcpy(d, "Carried");
    else if (ocarrf(a) == IN_CONTAINER)
      strcpy(d, "In item");
    bprintf("%-13s%-13s", oname(a), d);
    if (a % 3 == 2)
      bprintf("\n");
/*    if (a % 18 == 17)
      pbfr();*/
  }
  bprintf("\nTotal of %d objects.\n", oc);
}


void treasurecom(void)
{
  int a, c, tc = 0;
  char b[40], d[40];

  if (plev(mynum) < LVL_WIZARD) {
    erreval();
    return;
  }
  for (a = 0; a < numobs; a++) {
    if (!oflannel(a) && !otstbit(a, OFL_DESTROYED) && ocarrf(a) == IN_ROOM) {
      tc++;
      c = findzone(oloc(a), b);
      sprintf(d, "%s%d", b, c);
      bprintf("%-13s%-13s", oname(a), d);
      if (tc % 3 == 0)
        bprintf("\n");
      if (tc % 18 == 17)
        pbfr();
    }
  }
  bprintf("\nApproximately %d treasures remain.\n", tc);
}



/* The LEVELS command. Display the number of points required to reach each
 * new level together with the corresponding male and female titles.
 */
void levelscom(void)
{
    int i;

    bprintf("Level   Points        Male                    Female\n");
    bprintf("=====   ======        ====                    ======\n");

    for (i = 1; i <= LVL_WIZARD; i++) {

        bprintf("%-5d   %6d        the %-17s   the %-20s\n",
                i, levels[i], MLevels[i], FLevels[i]);
      }
    bprintf("\n");
  }


void promptcom(void)
{
    char buff[MAX_COM_LEN];

    getreinput(buff);
    if (strlen(buff) > PROMPT_LEN) {
        bprintf("Max. prompt length is %d.\n", PROMPT_LEN);
        return;
    }
    strcpy(cur_player->prompt, buff);
    bprintf("Ok\n");
}


void flowercom(void)
{
    int a;

    if ((a = vichere()) < 0)
        return;
    sillytp(a, "sends you flowers.\n\n         \001A\033[1;33m\002(*)\n"
               "     \001A\033[35m\002*    \001A\033[32m\002|"
               "\001A\033[1;36m\002    *\n    \001A\033[32m\002"
               "\\|/  \\|/  \\|/\n\001A\033[0;33m\002"
               "   ---------------\001A\033[0m\002");
    bprintf("You send %s flowers.\n", him_or_her(a));
  }

void ticklecom(void)
{
    int a;

    if ((a = vichere()) < 0)
        return;
    if (a == mynum) {
        bprintf("You tickle yourself.\n");
        return;
      }
    sillytp(a, "tickles you.");
    bprintf("You tickle %s.\n", him_or_her(a));
  }

void petcom(void)
{
    int a;

    if ((a = vichere()) < 0)
        return;
    if (a == mynum) {
        bprintf("No petting yourself in public, please.\n");
        return;
      }
    sillytp(a, "pats you on the head.");
    bprintf("You pat %s on the head.\n", him_or_her(a));
}


void wishcom(void)
{
    char x[MAX_COM_LEN];

    if (EMPTY(item1)) {
        bprintf("Wish for what?\n");
        return;
    }

    getreinput(x);

    send_msg(DEST_ALL, MODE_QUIET, LVL_WIZARD, LVL_MAX, NOBODY, NOBODY,
	     "[Wish from \001p%s\003]\n[%s]\n", pname(mynum), x);

    sillycom("\001s%s\002%s begs and grovels to the powers that be.\n\003");
    bprintf("Ok\n");
}


void wavecom(void)
{
    int a;

    if (pfighting(mynum) >= 0) {
        bprintf("What are you trying to do?  Make 'em die laughing?\n");
        return;
    }
    if (EMPTY(item1)) {
        sillycom("\001s%s\002%s waves happily.\n\003");
        bprintf("You wave happily.\n");
    }

    if ((a = ob1) < 0) return;

    switch (onum(a)) {

      case OBJ_BLIZZARD_BRIDGE_WAND:
        if ((state(OBJ_BLIZZARD_BRIDGE_FIRE) == 1) 
	    && (oloc(OBJ_BLIZZARD_BRIDGE_FIRE) == ploc(mynum))) {
            setobjstate(OBJ_BLIZZARD_BRIDGE_HALL, 0);
            sendf(ploc(mynum), "The drawbridge is lowered!\n");
            return;
	  }
        break;

      case OBJ_BLIZZARD_ROD:
        if (iscarrby(OBJ_CATACOMB_CUPSERAPH, mynum)) {
            bprintf("Something prevents the rod's functioning.\n");
            return;
	  }
        if (oarmor(a) >= 3) {
            bprintf("The rod crumbles to dust!\n");
            destroy(a);
            return;
	  }
        oarmor(a)++;
        bprintf("You are teleported!\n");
        teletrap(LOC_BLIZZARD_PENTACLE);
        return;

      case OBJ_TOWER_WAND:

        if (oarmor(a) > 0) {
            osetarmor(a, oarmor(a) - 1); 
            cur_player->me_ivct = 60;
            setpvis(mynum, 10);
            bprintf("You seem to shimmer and blur.\n");
            return;
	}
      }
    bprintf("Nothing happens.\n");
}




void blowcom(void)
{
    int a;
    char s[100];

    if ((a = ohereandget()) == -1)
        return;
    if (onum(a) == OBJ_CAVE_BAGPIPES) {
        broad("\001dA hideous wailing sounds echos all around.\n\003");
        return;
      }
    if (onum(a) == OBJ_OAKTREE_WHISTLE) {
        broad("\001dA strange ringing fills your head.\n\003");
        if (alive(max_players + MOB_OAKTREE_OTTIMO) != -1) {
            bprintf("A small dachshund bounds into the room "
		    "and leaps on you playfully.\n");
            sprintf(s, "A small dachshund bounds into the room "
		    "and leaps on %s playfully.\n", pname(mynum));
            sillycom(s);
            setploc(max_players + MOB_OAKTREE_OTTIMO, ploc(mynum));
	  }
        return;
      }

    if (onum(a) == OBJ_LABYRINTH_HORN) {
        broad("\001dA mighty horn blast echoes around you.\n\003");
        if (ploc(mynum) >= LOC_SEA_TREASURE && ploc(mynum) <= LOC_SEA_1 &&
            oarmor(OBJ_SEA_EXCALIBUR) == 0) {
            setoloc(OBJ_SEA_EXCALIBUR, ploc(mynum), IN_ROOM);
            setobjstate(OBJ_SEA_EXCALIBUR, 1);
            bprintf("A hand breaks through the water holding up "
		    "the sword Excalibur!\n");
            osetarmor(OBJ_SEA_EXCALIBUR, 1);
            qsetflg(mynum, Q_EXCALIBUR);
	  }
        return;
      }
    bprintf("You can't blow that.\n");
}


void lightcom()
{
    int a;
    char s[100];

    if ((a = ohereandget()) == -1)
        return;
    if (!ohany(1 << OFL_LIT)) {
        bprintf("You have nothing to light things from.\n");
        return;
      }
#ifdef LOCMIN_EFOREST

    if (a == OBJ_EFOREST_THORNS || a == OBJ_EFOREST_THORNSEAST) {
      if (state(OBJ_EFOREST_THORNS) == 0) {
        bprintf("The thorns have already been burned away.\n");
        return;
      }
      setobjstate(OBJ_EFOREST_THORNS, 0);
      bprintf("You burn the wall of thorns away!\n");
      bprintf("Behind them you can see the entrance to a cave.\n");
      sprintf(s, "%s burns the wall of thorns away!\n", pname(mynum));

      send_msg(LOC_EFOREST_THORNY, 0, LVL_MIN, LVL_MAX, mynum, NOBODY, s);
      send_msg(LOC_EFOREST_CAVE, 0, LVL_MIN, LVL_MAX, mynum, NOBODY, s);

      return;
    }
#endif

    if (!otstbit(a, OFL_LIGHTABLE))
        bprintf("You can\'t light that!\n");
    else if (otstbit(a, OFL_LIT))
        bprintf("It\'s already lit.\n");
    else {
        setobjstate(a, 0);
        osetbit(a, OFL_LIT);
        bprintf("Ok\n");
    }
}

void extinguishcom()
{
  int a;

  if ((a = ohereandget()) == -1)
    return;
  if (!otstbit(a, OFL_LIT))
    bprintf("It\'s not lit!\n");
  else if (!otstbit(a, OFL_EXTINGUISH))
    bprintf("You can\'t extinguish that!\n");
  else {
    setobjstate(a, 1);
    oclrbit(a, OFL_LIT);
    bprintf("Ok\n");
  }
}




void pushcom(void)
{
  int x;
  char s[128];

  if (brkword() == -1) {
    bprintf("Push what?\n");
    return;
  }
  if ((x = fobna(wordbuf)) == -1) {
    bprintf("That is not here.\n");
    return;
  }


  /* If it's the original object OR a copy:
   */
  switch (onum(x)) {

  case OBJ_BLIZZARD_TRIPWIRE:
    bprintf("The tripwire moves and a huge stone crashes down from above!\n");
    broad("\001dYou hear a thud and a squelch in the distance.\n\003");
    crapup("             S   P    L      A         T           !", SAVE_ME);
    break;
  case OBJ_BLIZZARD_BOOKCASE:
    bprintf("A trapdoor opens at your feet and you plumment downwards!\n");
    sillycom("\001p%s\003 disappears through a trapdoor!\n");
    teletrap(LOC_BLIZZARD_FISSURE);
    return;
  }


  /* If it's the original object (only), not a copy:
   */
  switch (x) {

#ifdef LOCMIN_EFOREST

  case OBJ_EFOREST_BUTTON:
    setobjstate(OBJ_EFOREST_BUTTON, 0);
    setobjstate(OBJ_EFOREST_THRONE_CATHEDRAL,
       1 - state(OBJ_EFOREST_THRONE_CATHEDRAL));

    sendf(ploc(mynum),
	  "You hear a grinding sound from near the entrance of the caves.\n");

    sprintf(s, "You hear a grinding sound as a mysterious force moves "
	    "the throne!\n");

    sendf(LOC_EFOREST_EASTEND, s);
    sendf(LOC_EFOREST_BOTTOM, s);
    break;
#endif

#ifdef LOCMIN_FROBOZZ
  case OBJ_FROBOZZ_BUTTON_OUTSIDE :
    if(state(OBJ_FROBOZZ_VAULTDOOR_OUTSIDE) == 1) {
        setobjstate(OBJ_FROBOZZ_VAULTDOOR_OUTSIDE,0);

	sendf(LOC_FROBOZZ_OUTSIDE, "Without a sound the whole southern "
	      "wall moves some feet westwards!\n");

        sendf(LOC_FROBOZZ_VAULT, "Without a sound the whole northern wall "
	      "moves some feet westwards!\n");
        break;
      }
    if(state(OBJ_FROBOZZ_VAULTDOOR_OUTSIDE) == 0) {
        setobjstate(OBJ_FROBOZZ_VAULTDOOR_OUTSIDE,2);
        sendf(LOC_FROBOZZ_OUTSIDE, "The southern wall glides some feet "
	      "to the east closing the vault!\n");

        sendf(LOC_FROBOZZ_VAULT, "The northern wall glides some feet to "
	      "the east closing the vault!\n");
        break;
      }
    if(state(OBJ_FROBOZZ_VAULTDOOR_OUTSIDE) == 2) {
        bprintf("Nothing happens.\n");
        break;
      }

  case OBJ_FROBOZZ_RUG_LIVING :
    if (state(OBJ_FROBOZZ_RUG_LIVING) == 1) {
      setobjstate(OBJ_FROBOZZ_RUG_LIVING,0);
      setobjstate(OBJ_FROBOZZ_TRAPDOOR_LIVING,2);
      bprintf("You move the rug and uncover a trapdoor!\n");
      sprintf(s,"%s moves the heavy rug and uncovers a trapdoor!\n",
	      pname(mynum));
      sillycom(s);
    }
    else {
      bprintf("You move the rug, but find nothing.\n");
      sprintf(s,"%s moves the rug.\n",pname(mynum));
      sillycom(s);
    }
    break;
#endif
#ifdef LOCMIN_ANCIENT
  case OBJ_ANCIENT_MOONCROSS:
    if (state(OBJ_ANCIENT_MOONCROSS)>0) {
        setobjstate(OBJ_ANCIENT_MOONCROSS, 0);
        if (state(OBJ_ANCIENT_BIGCROSS)>0) {
            bprintf("You move the cross to one side, but nothing happens.\n");
	  }
        else {
            bprintf("You hear a loud hollow sound from a room nearby!\n");
            setobjstate(OBJ_ANCIENT_HOLE_ANC31, 0);
	  }
      }else {
        setobjstate(OBJ_ANCIENT_MOONCROSS, 1);
        setobjstate(OBJ_ANCIENT_HOLE_ANC31, 1);
        bprintf("You move the cross back into place.\n");
      }
    break;
  case OBJ_ANCIENT_BIGCROSS:
    if (state(OBJ_ANCIENT_BIGCROSS)>0) {
        setobjstate(OBJ_ANCIENT_BIGCROSS, 0);
        if (state(OBJ_ANCIENT_MOONCROSS)>0) {
            bprintf("You move the cross to one side, but nothing happens.\n");
	  }
        else {
            bprintf("You hear a loud hollow sound from a room nearby!\n");
            setobjstate(OBJ_ANCIENT_HOLE_ANC31, 0);
	  }
      }else {
        setobjstate(OBJ_ANCIENT_BIGCROSS, 1);
        setobjstate(OBJ_ANCIENT_HOLE_ANC31, 1);
        bprintf("You move the cross back into place.\n");
      }
    break;
#endif

  case OBJ_BLIZZARD_IRONBAR:
    if (state(OBJ_BLIZZARD_PASS_STONE) == 1) {
      setobjstate(OBJ_BLIZZARD_PASS_FISSURE, 0);
      bprintf("A secret panel opens in the east wall!\n");
      break;
    }
    bprintf("Nothing happens.\n");
    break;
  case OBJ_BLIZZARD_BOULDER:
    bprintf("With a mighty heave you manage to move the boulder a few feet\n");
    if (state(OBJ_BLIZZARD_HOLE_CRACK) == 1) {
      bprintf("uncovering a hole behind it.\n");
      setobjstate(OBJ_BLIZZARD_HOLE_CRACK, 0);
    }
    else {
      bprintf("covering a hole behind it.\n");
      setobjstate(OBJ_BLIZZARD_HOLE_CRACK, 1);
    }
    break;
  case OBJ_BLIZZARD_LEVER:
    if (ptothlp(mynum) == -1) {
      bprintf("It's too stiff.  Maybe you need help.\n");
      return;
    }
    bprintf("Ok\n");
    if (state(OBJ_BLIZZARD_SLIME_PIT) != 0) {
      sillycom("\001s%s\002%s pulls the lever.\n\003");
      sendf(oloc(OBJ_BLIZZARD_LEVER),
              "\001dYou hear a gurgling noise and then silence.\n\003");
      setobjstate(OBJ_BLIZZARD_SLIME_PIT, 0);
      sendf(oloc(OBJ_BLIZZARD_SLIME_PIT),
	    "\001cThere is a muffled click and the slime drains away.\n\003");
    }
    break;
  case OBJ_BLIZZARD_THRONE_CURTAINS:
  case OBJ_BLIZZARD_ROUGH_CURTAINS:
    setobjstate(OBJ_BLIZZARD_THRONE_CURTAINS,
		1 - state(OBJ_BLIZZARD_THRONE_CURTAINS));
    bprintf("Ok\n");
    break;
  case OBJ_CASTLE_LEVER:
    setobjstate(OBJ_CASTLE_PORT_INSIDE, 1 - state(OBJ_CASTLE_PORT_INSIDE));
    if (state(OBJ_CASTLE_PORT_INSIDE)) {
      sendf(oloc(OBJ_CASTLE_PORT_INSIDE), "\001cThe portcullis falls.\n\003");
      sendf(oloc(OBJ_CASTLE_PORT_OUTSIDE),"\001cThe portcullis falls.\n\003");
    }
    else {
      sendf(oloc(OBJ_CASTLE_PORT_INSIDE), "\001cThe portcullis rises.\n\003");
      sendf(oloc(OBJ_CASTLE_PORT_OUTSIDE), "\001cThe portcullis rises.\n\003");
    }
    break;
  case OBJ_BLIZZARD_BRIDGE_LEVER:
    setobjstate(OBJ_BLIZZARD_BRIDGE_HALL, 1 - state(OBJ_BLIZZARD_BRIDGE_HALL));
    if (state(OBJ_BLIZZARD_BRIDGE_HALL)) {
      sendf(oloc(OBJ_BLIZZARD_BRIDGE_HALL),"\001cThe drawbridge rises.\n\003");
      sendf(oloc(OBJ_BLIZZARD_BRIDGE_FIRE),"\001cThe drawbridge rises.\n\003");
    }
    else {
      sendf(oloc(OBJ_BLIZZARD_BRIDGE_HALL),
	    "\001cThe drawbridge is lowered.\n\003");
      sendf(oloc(OBJ_BLIZZARD_BRIDGE_FIRE),
	    "\001cThe drawbridge is lowered.\n\003");
    }
    break;
  case OBJ_CASTLE_TORCH:
    if (state(OBJ_CASTLE_DOOR_GOLEM) == 1) {
      setobjstate(OBJ_CASTLE_DOOR_GOLEM, 0);
      sendf(oloc(OBJ_CASTLE_DOOR_GOLEM),
	    "A secret door slides quietly open in the south wall!\n");
    }
    else
      bprintf("It moves but nothing seems to happen.\n");
    return;
  case OBJ_CHURCH_ROPE:
    if (oarmor(OBJ_CHURCH_ROPE) >= 12)
      bprintf("\001dChurch bells ring out around you.\n\003");
    else {
      broad("\001dChurch bells ring out around you.\n\003");
      if (++oarmor(OBJ_CHURCH_ROPE) == 12) {
        bprintf("A strange ghostly guitarist shimmers briefly before you.\n");
        setpscore(mynum, pscore(mynum) + 300);
        broad("\001dA faint ghostly guitar solo "
	      "floats through the air.\n\003");
      }
    }
    break;
  case OBJ_CATACOMB_DUST:
    bprintf("Great clouds of dust billow up, causing you to sneeze "
	    "horribly.\nWhen you're finished sneezing, you notice "
	    "a message carved into one wall.\n");

    broad("\001dA loud sneeze echoes through the land.\n\003");
    destroy(OBJ_CATACOMB_DUST);
    create(OBJ_CATACOMB_KOAN);
    break;
  case OBJ_ORCHOLD_BOTCOVER:
    bprintf("You can't seem to get enough leverage to move it.\n");
    return;
  case OBJ_ORCHOLD_TOPCOVER:
    if (ptothlp(mynum) == -1) {
      bprintf("You try to shift it, but it's too heavy.\n");
      break;
    }
    sillytp(ptothlp(mynum), "pushes the cover aside with your help.");
    setobjstate(x, 1 - state(x));
    oplong(x);
    return;
  case OBJ_ORCHOLD_SWITCH:
    if (state(x)) {
      bprintf("A hole slides open in the north wall!\n");
      setobjstate(x,0);
    }
    else
      bprintf("You hear a little 'click' sound.\n");
    return;
  case OBJ_BLIZZARD_STATUE_DOWN:
    if (ptothlp(mynum) == -1) {
      bprintf("You can't shift it alone, maybe you need help.\n");
      break;
    }
    sillytp(ptothlp(mynum), "pushes the statue with your help.");
    /* FALLTHROUGH */
  default:
    if (otstbit(x, OFL_PUSHABLE)) {
      setobjstate(x, 0);
      oplong(x);
      return;
    }
    if (otstbit(x, OFL_PUSHTOGGLE)) {
      setobjstate(x, 1 - state(x));
      oplong(x);
      return;
    }
    bprintf("Nothing happens.\n");
  }
}




void followcom(void)       /* Not yet implemebnted */
{
  if (cur_player->i_follow >= 0) {
    bprintf("Stopped following \001P%s\003.\n", pname(cur_player->i_follow));
    cur_player->i_follow = -1;
  }
  if (pfighting(mynum) != -1) {
    bprintf("Not in a fight!\n");
    return;
  }
  if (EMPTY(item1)) {
    bprintf("Who?\n");
    return;
  }
  if (pl1 == -1) {
    bprintf("Who?\n");
    return;
  }
  if (ploc(pl1) != ploc(mynum)) {
    bprintf("They aren't here.\n");
    return;
  }
  if (mynum == pl1) {
    bprintf("You run round in circles!\n");
    return;
  }
  sendf(pl1, "%s is following you.", see_name(pl1, mynum));
  cur_player->i_follow = pl1;
  bprintf("Ok\n");
}


void losecom(void)
{
  if (cur_player->i_follow == -1) {
    bprintf("You aren't following anyone.\n");
    return;
  }
  cur_player->i_follow = -1;
  bprintf("Ok\n");
}






/*****************************************************
 *                                                   *
 *               The BULLETIN command                *
 *                                                   *
 *****************************************************/

static void bull3(char *cont)
{
  bprintf("\001f%s\003", BULLETIN3);
/*  bprintf("\n");*/
  get_command(NULL);
}

static void bull2(char *cont)
{
  bprintf("\001f%s\003", BULLETIN2);
/*  bprintf("\n");*/
  if (plev(mynum) < LVL_ARCHWIZARD) {
    get_command(NULL);
  } else {
    bprintf( pwait );
    replace_input_handler(bull3);
  }
}

void bulletincom(void)
{
  bprintf("\001f%s\003", BULLETIN1);
  if (plev(mynum) < LVL_WIZARD) {
    return;
  }
  bprintf( pwait );
  replace_input_handler(bull2);
}
