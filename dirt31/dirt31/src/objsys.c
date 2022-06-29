#include "kernel.h"
#include "objects.h"
#include "mobiles.h"
#include "mobile.h"
#include "locations.h"
#include "sflags.h"
#include "pflags.h"
#include "mflags.h"
#include "oflags.h"
#include "lflags.h"
#include "cflags.h"
#include "quests.h"
#include "sendsys.h"
#include "levels.h"
#include "parse.h"
#include "objsys.h"
#include "exec.h"
#include "oflagnames.h"
#include "rooms.h"
#include "objsys.h"


typedef struct {
  char *class_name;
  OFLAGS class_mask;
  short class_state;
} CLASS_DATA;

static int	   value_class(CLASS_DATA *cl, int plx, Boolean silent);
static CLASS_DATA *findclass(char *n);
static Boolean     classmatch(int ob,CLASS_DATA *cl);
static void	   dropall(CLASS_DATA *cl);
static void	   getall(CLASS_DATA *cl);
static void	   getallfr(CLASS_DATA *cl);


static CLASS_DATA class_data[] = {
  { "clothing",   1 << OFL_WEARABLE,  -2},
  { "weapons",    1 << OFL_WEAPON,    -1},
  { "containers", 1 << OFL_CONTAINER, -1},
  { "food",       1 << OFL_FOOD,      -1},
  { "keys",       1 << OFL_KEY,       -1},
  { "all",        0,                  -1},
  { NULL,         0,                  -1}};

int pits[] = {
  OBJ_START_PIT, OBJ_START_CHURCH_PIT,
  OBJ_CATACOMB_PIT_NORTH, OBJ_CATACOMB_PIT_EAST,
  OBJ_CATACOMB_PIT_SOUTH, OBJ_CATACOMB_PIT_WEST,
  -1};

Boolean ispit(register int o)
{
  register int i = 0;
  register int j;

  while ((j = pits[i++]) != o && j != -1);
  return (j >= 0);
}

void givecom(void)
{
  int a, c;

  if (EMPTY(item1)) {
    bprintf("Give what to who?\n");
    error();
  }
  if (pl1 != -1) {
    if ((a = pl1) == -1) {
      bprintf("Who's that?\n");
      error();
    }
    if (mynum == a) {
      bprintf("Cheap skate!\n");
      error();
    }
    if (EMPTY(item2)) {
      bprintf("Give them what?\n");
      error();
    }
    if ((c = ob2) == -1) {
      bprintf("You don't have it.\n");
      error();
    }
    dogive(c, a);
    return;
  }
  if ((a = ob1) == -1) {
    bprintf("You don't have any %s.\n", item1);
    error();
  }
  if (EMPTY(item2)) {
    bprintf("To who?\n");
    error();
  }
  if ((c = pl2) == -1) {
    bprintf("Who's that?\n");
    error();
  }
  if (mynum == c) {
    bprintf("Cheap skate!\n");
    error();
  }
  dogive(a, c);
}

void dogive(int ob, int pl)
{
  char z[60];
  int i, j, o, p;

  if (plev(mynum) < LVL_WIZARD && ploc(pl) != ploc(mynum)) {
    bprintf("They aren't here.\n");
    return;
  }
  if (!iscarrby(ob, mynum)) {
    bprintf("You don't have any %s.\n", oname(ob));
    return;
  }
  if (!cancarry(pl)) {
    bprintf("They can't carry it.\n");
    return;
  }
  if (pl >= max_players && mtstflg(pl, MFL_QFOOD) && otstbit(ob, OFL_FOOD)) {
    bprintf("%s thanks you.\n", pname(pl));
    sendf(ploc(mynum), "%s has left the game.\n", pname(pl));
    setpscore(pl, pscore(pl) + 50);
    dumpstuff(pl, ploc(pl));
    strcpy(pname(pl), "");
    eat(ob);
    return;
  }

/*  p = pl - max_players;*/

  if (pnum(pl) == MOB_CATACOMB_BEGGAR && otstbit(ob, OFL_FOOD)) {
    bprintf("The Beggar thanks you and greedily devours the %s.\n",
	    oname(ob));
    setpscore(mynum, pscore(mynum) + 50);
    eat(ob);
    bprintf("After finishing his meal, the beggar stares at you and says '");
    switch (my_random() % 4) {
    case 0: bprintf("Charity");
      o = OBJ_CATACOMB_PIT_NORTH;
      break;
    case 1: bprintf("Faith");
      o = OBJ_CATACOMB_PIT_EAST;
      break;
    case 2: bprintf("Wisdom");
      o = OBJ_CATACOMB_PIT_SOUTH;
      break;
    case 3: bprintf("Courage");
      o = OBJ_CATACOMB_PIT_WEST;
      break;
    }
    bprintf("'\n");
    for (i = 0; (j = pits[i++]) != -1;)
      setobjstate(j, 0);
    setobjstate(o, 1);
    sendf(oloc(o), "A faint glow emanates from the pit.\n");
    return;
  }
  if (pnum(pl) == MOB_OAKTREE_VIOLA && onum(ob) == OBJ_OAKTREE_FAN) {
    bprintf("Viola kisses you%s.\n", psex(mynum) == 0 ? "" : " on the cheek");
    bprintf("Viola says 'Thank you, %s.  Won't you please come in?'\n",
            psex(mynum) == 0 ? "kind sir" : "madame");
    setpscore(mynum, pscore(mynum) + 50);
  }

  if (onum(ob) == OBJ_VALLEY_ROSE && psex(mynum) != psex(pl)) {
    bprintf("You give %s the %s.\n", him_or_her(pl), oname(ob));
    setpscore(mynum, pscore(mynum) + 60);
    setpscore(pl, pscore(pl) + 50);
    sendf(pl, "%s gives you the %s.\n", see_name(pl, mynum), oname(ob));
    setoloc(ob, pl, CARRIED_BY);
    if (++odamage(ob) >= 2) {
	    sendf(ploc(pl), "The %s turns to dust.\n", oname(ob));
	    destroy(ob);
    }
    return;
  }

  setoloc(ob, pl, CARRIED_BY);
  sendf(pl, "%s gives you the %s.\n", see_name(pl, mynum), oname(ob));
  send_msg(ploc(pl), 0, LVL_MIN, LVL_MAX, pl, mynum,
	   "\001p%s\003 gives \001p%s\003 the %s.\n",
	   pname(mynum), pname(pl), oname(ob));
  return;
}

void stealcom(void)
{
  int a, c, e, f;
  char x[128], tb[128];

  if (EMPTY(item1)) {
    bprintf("Steal what?\n");
    return;
  }
  strcpy(x, item1);
  if (EMPTY(item2)) {
    bprintf("From who?\n");
    return;
  }
  if ((c = pl2) == -1) {
    bprintf("Who is that?\n");
    return;
  }
  if (mynum == c) {
    bprintf("A true kleptomaniac.\n");
    return;
  }
  if ((a = fobncb(x, c)) == -1) {
    bprintf("They don't have it.\n");
    return;
  }
  if (plev(mynum) < LVL_WIZARD && ploc(c) != ploc(mynum)) {
    bprintf("They're not here!\n");
    return;
  }
  if (ocarrf(a) == WORN_BY) {
    bprintf("They're wearing it.\n");
    return;
  }
  if (pwpn(c) == a) {
    bprintf("They have it firmly to hand ... for KILLING people with!\n");
    return;
  }
  if (pnum(c) == MOB_CATACOMB_DEFENDER || mtstflg(c, MFL_NOSTEAL)) {
    sendf(ploc(c), "%s says 'How dare you steal from me, %s!'\n",
	  pname(c), pname(mynum));
    hit_player(c, mynum, -1);
    return;
  }
  if (!do_okay( mynum, c, PFL_NOSTEAL)) {
    int i = randperc() % 3;
    switch (i) {
    case 0:
      bprintf("%s is too watchful.\n", he_or_she(c));
      return;
    case 1:
      bprintf("%s is too alert.\n", he_or_she(c));
      return;
    case 2:
      bprintf("%s is too crafty.\n", he_or_she(c));
      return;
    }
  }
  if (!cancarry(mynum)) {
    bprintf("You can't carry any more.\n");
    return;
  }
  f = randperc();
  e = (10 + plev(mynum) - plev(c)) * 5;
  if (f < e || plev(mynum) >= LVL_WIZARD) {
    bprintf("Got it!\n");
    sendf(c, "%s steals the %s from you!\n",
	  see_name(c, mynum), oname(a));
    setoloc(a, mynum, CARRIED_BY);
    if ((f & 1) && (c >= max_players))
        hit_player(c, mynum, -1);
    return;
  }
  bprintf("Your attempt fails.\n");
}



Boolean is_shield(int obj)
{
	return
	   onum(obj) == OBJ_TREEHOUSE_SHIELD ||
	   onum(obj) == OBJ_BLIZZARD_SHIELD1 ||
	   onum(obj) == OBJ_BLIZZARD_SHIELD2 ||
	   onum(obj) == OBJ_CATACOMB_SHIELD  ||
	   onum(obj) == OBJ_EFOREST_SHIELD   ||
	   onum(obj) == OBJ_FROBOZZ_SHIELD_ATTIC ||
	   onum(obj) == OBJ_ANCIENT_BSHIELD;
}

/* Does player pl wear a shield ?
 */
Boolean wears_shield(int pl)
{
	int i;
	for (i = 0; i < pnumobs(pl); i++) {

		if (iswornby(pobj_nr(i, pl), pl) && is_shield(pobj_nr(i, pl)))
		        return True;
	}

	return False;
}



Boolean is_armor(int obj)
{
	return onum(obj) == OBJ_CAVE_ARMOR ||
#ifdef LOCMIN_ANCIENT
	  onum(obj) == OBJ_ANCIENT_ACLOAK ||
#endif
	  onum(obj) == OBJ_ORCHOLD_CHAINMAIL ||
	  onum(obj) == OBJ_FROBOZZ_MITHRIL_JACKET;
}


/* Does player pl wear armor ?
 */
Boolean wears_armor(int pl)
{
	int i;
	for (i = 0; i < pnumobs(pl); i++) {

		if (iswornby(pobj_nr(i, pl), pl) && is_armor(pobj_nr(i, pl)))
		        return True;
	}
	return False;
}

Boolean is_mask(int obj)
{
	return
	onum(obj) == OBJ_LABYRINTH_MASK || onum(obj) == OBJ_ANCIENT_WARMASK;
}


Boolean wears_mask(int pl)
{
	int i;

	for (i = 0; i < pnumobs(pl); i++) {

		if (iswornby(pobj_nr(i, pl), pl) && is_mask(pobj_nr(i, pl)))
		        return True;
	}

	return False;
}

Boolean is_boat(int obj)
{
	return
	onum(obj) == OBJ_VILLAGE_BOAT
	  || onum(obj) == OBJ_VILLAGE_RAFT
	  || onum(obj) == OBJ_ANCIENT_CANOE;
}

Boolean carries_boat(int pl)
{
	int i;

	for (i = 0; i < pnumobs(pl); i++) {

		if (iscarrby(pobj_nr(i, pl), pl) && is_boat(pobj_nr(i, pl)))
		        return True;
	}

	return False;
}

/* Does pl carry object type or a clone of it ?
 */
int carries_obj_type(int pl, int type)
{
	int i;

	for (i = 0; i < pnumobs(pl); i++) {

		if (iscarrby(pobj_nr(i, pl), pl) &&
		    onum(pobj_nr(i, pl)) == type)
		        return pobj_nr(i, pl);
	}
	return -1;
}


/* Does pl wear object type or a clone of it ?
 */
int wears_obj_type(int pl, int type)
{
	int i;

	for (i = 0; i < pnumobs(pl); i++) {

		if (iswornby(pobj_nr(i, pl), pl) &&
		    onum(pobj_nr(i, pl)) == type)
		        return pobj_nr(i, pl);
	}
	return -1;
}





void wearcom(void)
{
  int a;
  int b;
  char buff[80];

  if ((a = ohereandget()) == -1) return;
  if (!iscarrby(a, mynum)) {
    bprintf("You don't have it.\n");
    return;
  }
  if (iswornby(a, mynum)) {
    bprintf("You're already wearing it.\n");
    return;
  }


#ifdef LOCMIN_ANCIENT
  if (onum(a) == OBJ_ANCIENT_HEALBALSAM) {
    if (pstr(mynum) < maxstrength(mynum) - 20) {
      setpstr(mynum, pstr(mynum) + 20);
      bprintf("You feel some of your wounds dissappear.\n");
    } else if (pstr(mynum) < maxstrength(mynum)) {
      setpstr(mynum, maxstrength(mynum));
      bprintf("The balsam heals all your wounds!\n");
    } else {
      bprintf("The balsam has a nice cooling effect.\n");
    }
    destroy(a);
    return;
  }


  if (onum(a) == OBJ_ANCIENT_EMBBALSAM) {
    bprintf("You start applying the embalming balsam ...\n");
    bprintf("You begin to feel sleepy, and after a while some mummies\n"
	    "turn up helping you with the embalming ...\n");
    destroy(a);
    crapup("\tThe mummies carry you away to a safe restingplace."
	   " You are dead...", SAVE_ME);
    return;
  }

#endif

  if (is_shield(a) && wears_shield(mynum)) {
    bprintf("You can't use two shields at once.\n");
    return;
  }

  if (is_armor(a) && wears_armor(mynum)) {
    bprintf("You can't wear two suits of armor at once.\n");
    return;
  }

  if (is_mask(a) && wears_mask(mynum)) {
    bprintf("You can't wear two masks at once.\n");
    return;
  }

  if (!otstbit(a, OFL_WEARABLE)) {
    bprintf("Is this a new fashion?\n");
    return;
  }
  b = WORN_BY;
  if (ocarrf(a) == WIELDED_BY)
      b = BOTH_BY;
  setcarrf(a, b);

  send_msg( ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
	   "\001p%s\003 wears the %s.\n", pname(mynum), oname(a));

  bprintf("Ok\n");
}

void removecom(void)
{
  int a;
  int b;
  char buff[80];

  if ((a = ohereandget()) == -1) return;
  if (!iswornby(a, mynum)) {
    bprintf("You're not wearing it.\n");
    return;
  }
  b = (ocarrf(a) == BOTH_BY) ? WIELDED_BY : CARRIED_BY;
  setcarrf(a, b);

  send_msg( ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
	   "%s removes the %s.\n", pname(mynum), oname(a));

  bprintf("Ok\n");
}


/* VALUE command:
 *****************************************************************/

static int value_class(CLASS_DATA *cl, int plx, Boolean silent)
{
  int obj, sum = 0;

  for (obj = 0; obj < numobs; obj++ ) {

	  if (in_inventory(obj, plx) && classmatch(obj, cl)) {
		  sum += ovalue(obj);
		  if (!silent) {
			  if (plev(plx) >= LVL_WIZARD) bprintf("[%3d]", obj);
		          bprintf("%12.12s:%5d points\n",
				  oname(obj), ovalue(obj));
		  }
	  }
  }
  return sum;
}

void valuecom(void)
{
  CLASS_DATA *c;
  int a;

  if (brkword() == -1) {
	  bprintf("Total value of all your possessions: %d points.\n",
		  value_class(findclass("all"), mynum, True));
  } else {
	  do {
		  if ((c = findclass(wordbuf)) != NULL) {
			  bprintf("\nTotal value:%*d points.\n",
				  plev(mynum) >= LVL_WIZARD ? 11 : 6,
				  value_class(c, mynum, False));
		  } else if ((a = fobn(wordbuf)) == -1) {
			  bprintf("%s: no such object\n", wordbuf);
		  } else {
			  if (plev(mynum) >= LVL_WIZARD) bprintf("[%3d]", a);
			  bprintf("%12.12s:%5d points\n", oname(a), ovalue(a));
		  }
	  } while (brkword() != -1);
  }
}
/*End, VALUE command
 ****************************************************************/


void putcom(void)
{
  int a;
  char ar[128];
  int c;

  if ((a = ohereandget()) == -1)
    return;
  if (EMPTY(item2)) {
    bprintf("Where?\n");
    return;
  }
  if ((c = ob2) == -1) {
    bprintf( "I can't see any %s here.\n", item2);
    return;
  }

#ifdef LOCMIN_EFOREST

  if (onum(c) == OBJ_EFOREST_HOLE) {
    if (onum(a) != OBJ_EFOREST_HOPE) {
      bprintf("Nothing happens.\n");
      return;
    }
    if (state(c) == 0) {
      bprintf("You hear a 'click' sound but nothing seems to happen.\n");
      return;
    }

    bprintf("The gem clicks into place...\n...and the door opens!\n");
    send_msg(LOC_EFOREST_HOT, 0, LVL_MIN, LVL_MAX, mynum, NOBODY,
	     "You hear a 'click' sound and the door opens!\n");
    setobjstate(OBJ_EFOREST_HOLE, 0);
    return;
  }
  if (c == OBJ_EFOREST_LAKE) {
    if (a != OBJ_EFOREST_SPONGE) {
      bprintf("Nothing happens.\n");
      return;
    }
    if (state(OBJ_EFOREST_LAKE) == 0) {
      bprintf("What lake ? It is dried up!\n");
      return;
    } else {
      setobjstate(OBJ_EFOREST_LAKE, 0);
      setobjstate(OBJ_EFOREST_SPONGE, 1);
      bprintf("The sponge seems to miraculously suck up the"
	      " water in the lake!\n");
      bprintf("It has dried the entire lake...wow!\n");
      send_msg(ploc(mynum), 0, LVL_MIN, LVL_MAX, mynum, NOBODY,
	       "A sponge seems to suck up the water in the lake!\n");
      return;
    }
  }
#endif

#ifdef LOCMIN_FROBOZZ
  if(c == OBJ_FROBOZZ_WINDOW_OUTSIDE) {
    if(a != OBJ_FROBOZZ_LEAFLET_MAILBOX) {
      bprintf("Nothing happens.\n");
      return;
    } else if(state(OBJ_FROBOZZ_VAULTDOOR_OUTSIDE) == 2) {
      setobjstate(OBJ_FROBOZZ_VAULTDOOR_OUTSIDE,1);
      bprintf("You hear a nearly inaudible click from "
	      "the southern wall.\n");
      return;
    } else {
      bprintf("Nothing happens.\n");
      return;
    }
  }
#endif

#ifdef LOCMIN_ANCIENT
  if (c == OBJ_ANCIENT_PEDESTAL)
    if (a != OBJ_ANCIENT_SUNDISC) {
      bprintf("Nothing happens.\n");
      return;
    } else {
      bprintf("The sundisc fits perfectly on top of the pedestal with a loud "
              "click!\n");
      send_msg(ploc(mynum), 0, LVL_MIN, LVL_MAX, mynum, NOBODY,
	       "You hear a loud click as %s puts the sundisc on "
	      "the pedestal.", pname(mynum));

      setoloc(OBJ_ANCIENT_SUNDISC, ploc(mynum), IN_ROOM);

      osetbit(OBJ_ANCIENT_SUNDISC, OFL_NOGET);
      setobjstate(OBJ_ANCIENT_SUNDISC, 2);
      setobjstate(OBJ_ANCIENT_PEDESTAL, 0);
      setpscore(mynum, pscore(mynum) + ovalue(OBJ_ANCIENT_SUNDISC)*3);
      return;
    }
#endif

  if (onum(c) == OBJ_TOWER_CANDLESTICK) {
    if (onum(a) != OBJ_TOWER_RED_CANDLE && onum(a) != OBJ_TOWER_BLUE_CANDLE &&
	onum(a) != OBJ_TOWER_GREEN_CANDLE) {
      bprintf("You can't do that.\n");
      return;
    }
    if (state(c) != 2) {
      bprintf("There's already a candle in it!\n");
      return;
    }
    bprintf("The candle fixes firmly into the candlestick.\n");
    setpscore(mynum, pscore(mynum) + 50);
    destroy(a);
    osetarmor(c, a);
    osetbit(c, OFL_LIGHTABLE);
    osetbit(c, OFL_EXTINGUISH);
    if (otstbit(a, OFL_LIT)) {
      osetbit(c, OFL_LIT);
      setobjstate(c, 0);
      return;
    }
    setobjstate(c, 1);
    oclrbit(c, OFL_LIT);
    return;
  }

  if (onum(c) == OBJ_TOWER_BALL) {
    if (onum(a) == OBJ_TOWER_WAND && oarmor(a) == 0) {
      bprintf("The wand seems to soak up energy.\n");
      osetarmor(a, 4);
      return;
    }
    bprintf("Nothing happens.\n");
    return;
  }

  if (c == OBJ_BLIZZARD_SLIME_PIT) {
    if (state(c) == 0) {
      setoloc(a, LOC_BLIZZARD_SLIME, IN_ROOM);
      bprintf("Ok\n");
      return;
    }
    destroy(a);
    bprintf("It dissappears with a fizzle into the slime.\n");
    if (onum(a) == OBJ_BLIZZARD_SOAP) {
      bprintf("The soap dissolves the slime away!\n");
      setobjstate(OBJ_BLIZZARD_SLIME_PIT, 0);
    }
    return;
  }
  if (c == OBJ_TOWER_CHUTE_BOT) {
    bprintf("You can't do that, the chute leads up from here!\n");
    return;
  }
  if (c == OBJ_TOWER_CHUTE_TOP) {
    if (onum(a) == OBJ_CASTLE_RUNESWORD) {
      bprintf("You can't let go of it!\n");
      return;
    }
    bprintf("It vanishes down the chute....\n");
    sendf(oloc(OBJ_TOWER_CHUTE_BOT),
	  "The %s comes out of the chute.\n", oname(a));
    setoloc(a, oloc(OBJ_TOWER_CHUTE_BOT), IN_ROOM);
    return;
  }
  if (c == OBJ_TOWER_HOLE) {
    if (onum(a) == OBJ_TOWER_SCEPTRE && state(OBJ_TOWER_DOOR_SHAZARETH) == 1) {
      setobjstate(OBJ_TOWER_DOOR_TREASURE, 0);
      strcpy(ar, "The door clicks open!\n");
      sendf( oloc(OBJ_TOWER_DOOR_TREASURE), ar);
      sendf( oloc(OBJ_TOWER_DOOR_SHAZARETH), ar);
      return;
    }
    bprintf("Nothing happens.\n");
    return;
  }
  if (c == a) {
    bprintf("What do you think this is, the goon show?\n");
    return;
  }
  if (otstbit(c, OFL_CONTAINER) == 0) {
    bprintf("You can't do that.\n");
    return;
  }
  if (state(c) != 0) {
    bprintf("It's not open.\n");
    return;
  }
  if (oflannel(a)) {
    bprintf("You can't take that!\n");
    return;
  }
  if ((ishere(a)) && (dragget()))
    return;
  if (onum(a) == OBJ_CASTLE_RUNESWORD) {
    bprintf("You can't let go of it!\n");
    return;
  }
  if (onum(a) == OBJ_START_UMBRELLA && state(a) == 1) {
    bprintf("Close it first...\n");
    return;
  }
  if (otstbit(a, OFL_LIT)) {
    bprintf("I'd try putting it out first!\n");
    return;
  }
  if (!willhold(c, a)) {
    bprintf("It won't fit.\n");
    return;
  }
  setoloc(a, c, IN_CONTAINER);
  bprintf("Ok\n");

  send_msg(ploc(mynum), 0, LVL_MIN, LVL_MAX, mynum, NOBODY,
	   "\001D%s\003\001c puts the %s in the %s.\n\003",
	  pname(mynum), oname(a), oname(c));

  if (otstbit(a, OFL_GETFLIPS))
    setobjstate(a, 0);
  if (ploc(mynum) == LOC_TOWER_TREASURE && state(OBJ_TOWER_DOOR_TREASURE) == 0
      && ishere(a)) {
    setobjstate(OBJ_TOWER_DOOR_TREASURE, 1);
    strcpy(ar, "The door clicks shut....\n");
    sendf( LOC_TOWER_TREASURE, ar);
    sendf( oloc(OBJ_TOWER_DOOR_SHAZARETH), ar);
  }
}

void eatcom(void)
{
  int b;
  char s[100];

  if (brkword() == -1) {
    bprintf("Eat what?\n");
    return;
  }
  if (EQ(wordbuf, "water"))
    strcpy(wordbuf, "spring");
  if ((b = ob1 == -1 ? ob2 : ob1) == -1) {
    bprintf("It isn't here.\n");
    return;
  }
  switch (onum(b)) {
  case OBJ_MOOR_CHALICE:
    bprintf("However much blood you drink from the chalice it "
            "stays just as stained!\n");
    break;
  case OBJ_OAKTREE_TART:
  case OBJ_OAKTREE_CAKES:
  case OBJ_OAKTREE_TOAST:
    bprintf("That was delicious, but not very filling.\n");
    eat(b);
    setpstr(mynum, pstr(mynum) + 6);
    calibme();
    break;
  case OBJ_OAKTREE_SOUP:
    bprintf("As you finish off the last of the caterpillar consume\n");
    bprintf("you notice a small diamond in the bottom of the cup.\n");
    eat(b);
    create(OBJ_OAKTREE_CUPDIAMOND);
    setoloc(OBJ_OAKTREE_CUPDIAMOND, mynum, CARRIED_BY);
    create(OBJ_OAKTREE_CUPCHINA);
    setoloc(OBJ_OAKTREE_CUPCHINA, mynum, CARRIED_BY);
    setpstr(mynum, pstr(mynum) + 6);
    calibme();
    break;
  case OBJ_TOWER_CAULDRON:
    bprintf("You feel funny and pass out....\n");
    bprintf("You wake up elsewhere....\n");
    teletrap(LOC_TOWER_MAGICAL);
    break;
  case OBJ_VALLEY_SPRING:
    bprintf("Very refreshing.\n");
    break;
  case OBJ_TOWER_POTION:
    setpstr(mynum, maxstrength(mynum));
    bprintf("You feel much much stronger!\n");
    setoloc(b, LOC_DEAD_EATEN, IN_ROOM);
    destroy(b);
    break;
  case OBJ_TREEHOUSE_WAYBREAD:
    if (plev(mynum) < LVL_WIZARD && cur_player->pretend < 0) {
      pl1 = (my_random() >> 3) % (numchars - 1);
      if (ststflg(pl1, SFL_OCCUPIED) || pl1 < max_players) {
        bprintf("There is a sudden feeling of failure...\n");
        break;
      }
      polymorph(pl1,25);        /* aliased for 25 moves */
    }
    setpstr(mynum, pstr(mynum) + 16);
    eat(b);
    break;
  case OBJ_ICECAVE_FOUNTAIN:
    if (plev(mynum) >= LVL_NOVICE && plev(mynum) < LVL_HERO) {
      setpscore(mynum, pscore(mynum) + 40);
      calibme();
      bprintf("You feel a wave of energy sweeping through you.\n");
    } else {
      bprintf("Faintly magical by the taste.\n");
      if (plev(mynum) >= LVL_HERO && pstr(mynum) < 10)
        setpstr(mynum, pstr(mynum) + 4);
      calibme();
    }
    break;

#ifdef LOCMIN_ANCIENT
  case OBJ_ANCIENT_FOUNTAIN_OF_YOUTH:
    if (pscore(mynum) >= 20) {
        setpscore(mynum, pscore(mynum) - 20);
        calibme();
        bprintf("You feel younger and less experienced...\n");
      }
    break;
#endif

  default:
    if (otstbit(b, OFL_FOOD)) {
      eat(b);
      bprintf("Delicious!\n");

      setpstr(mynum, pstr(mynum) + 12);
      calibme();
      sprintf(s, "\001P%s\003 greedily devours the %s.\n",
	      pname(mynum), oname(b));
      sillycom(s);
    } else {
      bprintf("I think I've lost my appetite.\n");
      return;
    }
    break;
  }
}

void inventory(void)
{
	if (plev(mynum) < LVL_WIZARD) {
		send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
			 "%s rummages through %s backpack.\n", 
			 pname(mynum), his_or_her(mynum));
	}

	bprintf("You are carrying:\n");
	aobjsat(mynum, CARRIED_BY, 0);
}


void listobject(int loc,int mode)
{
	aobjsat(loc, mode, 0);
}

void mlobjsat(int x, int m)
{
	aobjsat(x, CARRIED_BY, m);
}



/* All OBJectS AT - list all objects at the destination given.
 */
void aobjsat(int loc, int mode, int marg)
{
	int ostack[64], ostackp = 0;

	char b[80], *s;

	int col;
	Boolean wwl;    /* worn, wielded or lit ?*/
	Boolean d, show_contents, empty = True;
	int obj;
	int stp;
	int_set *inv = mode == IN_ROOM ? linv(loc) :
	               mode == IN_CONTAINER ? oinv(loc) : pinv(loc);


	for (col = 0; col < marg; col++)  bprintf(" ");

	for (obj = first_obj(inv); obj != SET_END; obj = next_obj(inv)) {

		show_contents = False;

		s = b;

		if ((mode == CARRIED_BY && iscarrby(obj, loc))
		    || (mode == IN_CONTAINER && iscontin(obj, loc))) {

			empty = False;
			show_contents = True;

			if (d = otstbit(obj, OFL_DESTROYED)) {
				*s++ = '(';
			}

			strcpy(s, oname(obj));
			s += strlen(oname(obj));

			if (d) {
				*s++ = ')';
			}

			*s++ = ' ';

			wwl = False;

			if (mode == CARRIED_BY) {
				if (iswornby(obj, loc)) {
					strcpy(s,"<worn");
					s += strlen(s);
					wwl = True;
				}

				if (pwpn(loc) == obj
				    && ocarrf(obj) == WIELDED_BY) {
					if (wwl) *s++ = ',';
					else {
						*s++ = '<';
						wwl = True;
					}
					strcpy(s,"wielded");
					s += strlen(s);
				}

				if (otstbit(obj, OFL_LIT)) {
					if (wwl) *s++ = ',';
					else {
						*s++ = '<';
						wwl = True;
					}
					strcpy(s,"lit");
					s += strlen(s);
				}
				if (wwl) *s++ = '>';
			}

			*s = 0;

			if (s - b + 1 + col > 79) {
				bprintf("\n");
				for (col = 0; col < marg; col++) bprintf(" ");
			}

			bprintf( "%s ", b);
			col += strlen(b) + 1;

			if (otstbit(obj, OFL_CONTAINER) && show_contents &&
			    (!otstbit(obj, OFL_OPENABLE) || state(obj) == 0)) {
				ostack[ostackp++] = obj;
			}
		}
	}


	if (empty)  bprintf("Nothing");

	bprintf("\n");

	for (stp = 0; stp < ostackp; stp++) {
		for (col = 0; col < marg; col++)  bprintf(" ");

		obj = ostack[stp];

		bprintf("    The %s contains:\n", oname(obj));
/*		pbfr();*/
		aobjsat(obj, IN_CONTAINER, marg + 8);
	}
}




/* Is o1 contained in o2 ?
 */
Boolean iscontin(int o1, int o2)
{
	if (ocarrf(o1) != IN_CONTAINER || oloc(o1) != o2)
	        return False;

	if (plev(mynum) < LVL_WIZARD && otstbit(o1, OFL_DESTROYED))
	        return False;

	return True;
}


/* The room where an object, or its container or its carrier, are at.
 */
int obj_loc(int obj)
{
	for (; ocarrf(obj) == IN_CONTAINER; obj = oloc(obj))
	  ;
	return ocarrf(obj) >= CARRIED_BY ? ploc(oloc(obj)) : oloc(obj);
}



/* The 'Find Object By Name' system.
 *
 * Name can be either 1) <object-number>
 *                 or 2) <object-name>
 *                 or 3) <object-name><number-in-sequence-with-that-name>
 */
static int fobnsys(char *name, int ctrl, int ct_inf, int_set *inv);

int fobn(char *word)
{
	int x;

	/* Look for all available objects (=in room or inventory)
	 */
	if ((x = fobna(word)) != -1) return x;

	/* we didn't find any available object...look for *any* object
	 */
	return fobnsys(word, 0, 0, NULL);
}

/* Look for available objects */
int fobna(char *word)
{
	int i;

	return (i = fobnc(word)) >= 0 ? i : fobnh(word);
}

/* Look for objects contained in ct */
int fobnin(char *word, int ct)
{
	return fobnsys(word, 5, ct, oinv(ct));
}

/* look for objects carried by me */
int fobnc(char *word)
{
	return fobncb(word, mynum);
}

/* look for objects carried by 'by' */
int fobncb(char *word, int by)
{
	return fobnsys(word, 3, by, pinv(by));
}

/* Look for objects that's here */
int fobnh(char *word)
{
	return fobnsys(word, 4, ploc(mynum), linv(ploc(mynum)));
}

/* Look for a obj. that's here and not scenery (can be taken) */
int fobn_can_take(char *word)
{
	return fobnsys(word, 6, ploc(mynum), linv(ploc(mynum)));
}


static int fobnsys(char *name, int ctrl, int ct_inf, int_set *inv)
{
	char b[ONAME_LEN + 1], *p = b;
	int i, obj, num;
	char *n;

	if (name == NULL || strlen(name) > ONAME_LEN)  return -1;

	while (*name != '\0' && isalpha(*name)) *p++ = *name++;
	*p = '\0';

	if (isdigit(*name)) {
		num = atoi(name);

		while (isdigit(*++name));
		if (*name != '\0') return -1;
	}
	else if (*name != '\0') {
		return -1;
	}
	else num = 1;

	if (num < 0 || num >= numobs) return -1;

	if (*b == '\0') {
		switch(ctrl) {
		      case 0: return num;
		      case 3: return iscarrby(num, ct_inf) ? num : -1;
		      case 4: return ishere(num) ? num : -1;
		      case 5: return iscontin(num, ct_inf) ? num : -1;
		      case 6: return ishere(num) && !oflannel(num) ? num : -1;
		      default: return -1;
		}
	}

	if (ctrl == 0) {    /* Look for first object with this name*/

		for (obj = 0; obj < numobs; obj++) {
			n = EQ(b, oname(obj))    ? oname(obj) :
			    EQ(b, oaltname(obj)) ? oaltname(obj) : NULL;

			if (n != NULL && --num == 0) {
				cur_player->wd_it = n;
				return obj;
			}
		}
		return -1;
	}

	for (i = 0; i < set_size(inv); i++) {

		obj = int_number(i, inv);

		n = EQ(b, oname(obj))    ? oname(obj) :
 		    EQ(b, oaltname(obj)) ? oaltname(obj) : NULL;

		if (n != NULL) {
			cur_player->wd_it = n;

			switch (ctrl) {
			      case 3: /* Look for objects carried by ct_inf */
				if (iscarrby(obj, ct_inf) && --num == 0)
				  return obj;
				break;
			      case 4: /* Look for objects that's here */
				if (ishere(obj) && --num == 0)
				  return obj;
				break;
			      case 5: /* look for objects contained in ct_inf*/
				if (iscontin(obj, ct_inf) && --num == 0)
				  return obj;
				break;
			      case 6: /* objects that are here and gettable */
				if (ishere(obj) && !oflannel(obj) && --num ==0)
				  return obj;
				break;
			      default: 	return -1;
			  }
		}
	}
	return -1;
}


/* Find an object's in-game index from its ID.
 * Return -1 if not found.
 */
int find_object_by_id(long int id)
{
	long int x;

	if (id >= 0 && id < num_const_obs) return id;

	return (x = lookup_entry(id, &id_table)) == NOT_IN_TABLE
	  || x < 0 || x >= numobs ? -1 : x;
}




int get1objfrom(int ob,int container)
{
  int l;
  char *s;
  char bf[81];

#ifdef LOCMIN_ANCIENT
  if (ob == OBJ_ANCIENT_SUNDISC && state(ob) == 1) {
    if (!iscarrby(OBJ_ANCIENT_ESTONE, mynum) ||
        !iscarrby(OBJ_ANCIENT_QFEATHER, mynum)) {
      bprintf("You feel that you need more magical equipment "
              "than just your hands for this job.\n");
      return 0;
    }
    else {
      setobjstate(OBJ_ANCIENT_SUNDISC, 0);
    }
  }
#endif

  if (ob == OBJ_BLIZZARD_SHIELD) {
    if (ishere(OBJ_BLIZZARD_SHIELD1)) ob = OBJ_BLIZZARD_SHIELD1;
    else if (ishere(OBJ_BLIZZARD_SHIELD2)) ob = OBJ_BLIZZARD_SHIELD2;
    else if (container == -1) {
      if (otstbit(OBJ_BLIZZARD_SHIELD1, OFL_DESTROYED))
	ob = OBJ_BLIZZARD_SHIELD1;
      else if (otstbit(OBJ_BLIZZARD_SHIELD2, OFL_DESTROYED))
	ob = OBJ_BLIZZARD_SHIELD2;
      if (ob == OBJ_BLIZZARD_SHIELD1 || ob == OBJ_BLIZZARD_SHIELD2)
	create(ob);
      else {
	bprintf("The shields are too firmly secured to the walls.\n");
	return 0;
      }
    }
  }

  if (oflannel(ob)) {

    int i = fobn_can_take(oaltname(ob));

    if (i == -1) i = fobn_can_take(oname(ob));

    if (i == -1) {
      bprintf("You can't take that!\n");
      return -1;

    } else ob = i;
  }

  if (container == -1 || !iscarrby(container, mynum)) {

    if (dragget())
      return -1;
  }

  if (!cancarry(mynum)) {
    bprintf("You can't carry any more.\n");
    return -1;
  }

  if (onum(ob) == OBJ_CASTLE_RUNESWORD && state(ob) == 1 &&
      ptothlp(mynum) == -1) {
    bprintf("It's too well embedded to shift alone.\n");
    return 0;
  }
  if (ob == OBJ_CATACOMB_CUPSERAPH &&
      (l = alive((max_players + MOB_CATACOMB_SERAPH))) != -1 &&
      ploc(l) == ploc(mynum)) {
    bprintf("\001pThe Seraph\003 says 'Well done, my %s.  "
            "Truly you are a %s of virtue.'\n",
	    psex(mynum) ? "daughter" : "son", psex(mynum) ? "woman" : "man");
  }
  setoloc(ob, mynum, CARRIED_BY);
  if (container == -1) {
    *bf = '\0';
  } else {
    sprintf(bf, " from the %s", oname(container));
  }
  send_msg(ploc(mynum), 0, LVL_MIN, LVL_MAX, mynum, NOBODY,
	   "\001p%s\003 takes the %s%s.\n", pname(mynum), oname(ob), bf);

  if (otstbit(ob, OFL_GETFLIPS))
    setobjstate(ob, 0);
  if ((ploc(mynum) == LOC_TOWER_TREASURE) &&
      (state(OBJ_TOWER_DOOR_TREASURE) == 0)) {
    setobjstate(OBJ_TOWER_DOOR_TREASURE, 1);
    sendf(LOC_TOWER_TREASURE, s = "The door clicks shut...\n");
    sendf(obj_loc(olinked(OBJ_TOWER_DOOR_TREASURE)), s); /*Other side of door*/
  }
  if (ob == OBJ_CATACOMB_CUPSERAPH &&
      (l = alive((max_players + MOB_CATACOMB_SERAPH))) != -1 &&
      ploc(l) == ploc(mynum)) {
    bprintf("The Seraph gestures and you are transported to ...\n");
    send_msg( ploc(mynum), 0, LVL_MIN, LVL_MAX, mynum, NOBODY,
	     "%s vanishes, taking \001p%s\003 with him!\n",
	     pname(l), pname(mynum));
    sendf(LOC_START_CHURCH,
	  "\001p%s\003 appears, accompanied by an angel!\n", pname(mynum));
    setploc((max_players + MOB_CATACOMB_SERAPH), LOC_START_CHURCH);
    qsetflg(mynum, Q_GRAIL);
    trapch(LOC_START_CHURCH);
    return 0;
  }
  bprintf("Ok\n");
  return 0;
}

int getcom(void)
{
  CLASS_DATA *cl;
  int ob;
  int i;
  int container = -1;
  char bf[1024];

  if (brkword() == -1) {
    bprintf("Get what?\n");
    return -1;
  }
  if (isdark()) {
    bprintf("It's dark!\n");
    return -1;
  }
  if ((cl = findclass(wordbuf)) != NULL) {
    getall(cl);
    return 0;
  }
  ob = fobnh(wordbuf);
  /* Hold */
  i = stp;
  strcpy(bf, wordbuf);
  if (brkword() != -1 && (EQ(wordbuf, "from") || EQ(wordbuf, "out"))) {
    if (brkword() == -1) {
      bprintf("From what?\n");
      return -1;
    }
    if ((container = fobna(wordbuf)) == -1) {
      bprintf("You can't take things from that!  It's not here!\n");
      return -1;
    }

    if (otstbit(container, OFL_LOCKABLE) &&
        	                state(container) == 2 && !ohany(1<<OFL_KEY)) {
      bprintf("The %s is locked, and you have no key.\n", oname(container));
      return -1;
    }

    if ((otstbit(container, OFL_OPENABLE) || otstbit(container, OFL_LOCKABLE))
	&& state(container) > 0) {
          bprintf("You open the %s.\n", oname(container));
          setobjstate(container, 0);
    }

    ob = fobnin(bf, container);
  }
  stp = i;
  if (ob == -1) {
    bprintf("It's not here.\n");
    return -1;
  }

  return get1objfrom(ob,container);
}

static void getall(CLASS_DATA *cl)
{
  int x;

  if (brkword() != -1) {
    getallfr(cl);
    return;
  }

    for (x = lfirst_obj(ploc(mynum)); x != SET_END; x=lnext_obj(ploc(mynum))) {

    if (ishere(x) && !oflannel(x) && classmatch(x, cl)) {
      strcpy(strbuf, oname(x));
      stp = 0;
      bprintf("%s: ", oname(x));
      if (getcom() == -1)
	break;
    }
  }
}

static void getallfr(CLASS_DATA *cl)
{
  int container;
  int ob;

  if (EQ(wordbuf,"from")) {
    if (brkword() == -1) {
      bprintf("From what?\n");
      return;
    }
  }
  if ((container = fobna(wordbuf)) == -1) {
    bprintf("That isn't here.\n");
    return;
  }
  /* Do items */

    for (ob = ofirst_obj(container); ob != SET_END; ob = onext_obj(container)){

    if (iscontin(ob, container) && !oflannel(ob) && classmatch(ob, cl)) {
      sprintf(strbuf, "%s from %s", oname(ob), oname(container));
      stp = 0;
      bprintf("%s: ", oname(ob));
      if (getcom() == -1)
	break;
    }
  }
}

static void dropall(CLASS_DATA *cl)
{
  int ob;

    for (ob = pfirst_obj(mynum); ob != SET_END; ob = pnext_obj(mynum)) {

    if (iscarrby(ob, mynum) && classmatch(ob, cl)) {
      strcpy(strbuf, oname(ob));
      stp = 0;
      bprintf("%s: ", oname(ob));
      if (dropobj() == -1)
	return;
    }
  }
}


/* Is the item in the same room as the player ?
 */
Boolean p_ishere(int plr,int item)
{
	if (plev(plr) < LVL_WIZARD && otstbit(item, OFL_DESTROYED))
	        return False;

	if (ocarrf(item) != IN_ROOM || oloc(item) != ploc(plr)
	    || ploc(plr) == 0)
	  return False;

  return True;
}


Boolean ishere(int item)
{
	return p_ishere(mynum,item);
}



Boolean iscarrby(int item, int user)
{
	if (plev(mynum) < LVL_WIZARD && otstbit(item, OFL_DESTROYED))
	        return False;

	if (ocarrf(item) < CARRIED_BY)
	        return False;

	if (oloc(item) != user)
	        return False;

	return True;
}


/* Is the object in a players inventory ?
 * (also handles objects in a container in a container etc...)
 */
Boolean in_inventory(int obj, int player)
{
	while (ocarrf(obj) == IN_CONTAINER)  obj = oloc(obj);

	return iscarrby(obj, player);
}



int dropobj(void)
{
  CLASS_DATA *cl;
  int a, i, l, j;
  char bf[80];

  if (brkword() == -1) {
    bprintf("Drop what?\n");
    return -1;
  }
  if ((cl = findclass(wordbuf)) != NULL) {
    dropall(cl);
    return 0;
  }
  if ((a = fobnc(wordbuf)) == -1) {
    bprintf("You don't have it.\n");
    return -1;
  }
  if (plev(mynum) < LVL_WIZARD && onum(a) == OBJ_CASTLE_RUNESWORD) {
    bprintf("You can't let go of it!\n");
    return 0;
  }
  if (a == OBJ_CATACOMB_CUPSERAPH
      && ploc((max_players + MOB_CATACOMB_SERAPH)) == LOC_START_CHURCH)
    setplev((max_players + MOB_CATACOMB_SERAPH), -2);
  l = ploc(mynum);
  /* MINE LADDER */
  if (l == LOC_QUARRY_TUNNEL || l == LOC_QUARRY_LADDER_UU) {
    bprintf("The %s falls down the ladder.\n", oname(a));
    l = LOC_QUARRY_LADDER_UD;
  }
  if ((l >= LOC_QUARRY_LADDER_D4 && l <= LOC_QUARRY_LADDER_D2) ||
      (l >= LOC_QUARRY_LADDER_D1 && l <= LOC_QUARRY_LADDER_UD)) {
    bprintf("The %s falls down the ladder.\n", oname(a));
    l = LOC_QUARRY_MINE;
  }
  /* ALL AT SEA */
  if (ltstflg(l, LFL_ON_WATER) && onum(a) != OBJ_VILLAGE_BOAT && 
      onum(a) != OBJ_VILLAGE_RAFT && onum(a) != OBJ_ANCIENT_CANOE) {
    bprintf("The %s sinks into the sea.\n", oname(a));
    l = LOC_SEA_7;
  }
  /* OAKTREE */
  if ((l >= LOC_OAKTREE_MAGNOLIA && l <= LOC_OAKTREE_TREE1)
      || l == LOC_OAKTREE_ILEX
      || (l >= LOC_OAKTREE_WALNUT && l <= LOC_OAKTREE_FIG)) {
    bprintf("The %s falls through the leaves to the ground far below.\n",
	    oname(a));
    l = LOC_OAKTREE_GROVE;
    sendf(l, "Something falls to the ground.\n");
  }
  for (j = 0; (i = pits[j++]) != -1;)
    if (oloc(i) == l)
      break;
  if (i >= 0 && state(i) == 0 || oloc(OBJ_SEA_HOLE) == l) {
    bprintf("The %s disappears into the bottomless pit.....\n", oname(a));
    send_msg(ploc(mynum), 0, LVL_MIN, LVL_MAX, mynum, NOBODY,
	     "\001p%s\003 drops the %s into the pit.\n",
	     pname(mynum), oname(a));
    dropinpit(a);
    return 0;
  }
  else if (i >= 0) {
    bprintf("The %s disappears into the bottomless pit....."
	    "and hits bottom.\n", oname(a));
    send_msg(ploc(mynum), 0, LVL_MIN, LVL_MAX, mynum, NOBODY,
	     "\001p%s\003 drops the %s into the pit.\n",
	     pname(mynum), oname(a));
    l = LOC_CATACOMB_CHAMBER;
    setoloc(a, l, IN_ROOM);	/* to spherical room in CATACOMB section */
    sendf(l, "Something falls to the ground.\n");
    return 0;
  }
  setoloc(a, l, IN_ROOM);
  send_msg(ploc(mynum), 0, LVL_MIN, LVL_MAX, mynum, NOBODY,
	   "\001p%s\003 drops the %s.\n", pname(mynum), oname(a));
  if (l = ploc(mynum))
    bprintf("Ok\n");
  return 0;
}

void dropinpit(int o)
{
  int i;

  setpscore(mynum, pscore(mynum) + ovalue(o));

  calib_player(mynum);

  osetbit(o, OFL_DESTROYED);
  setoloc(o, LOC_PIT_PIT, IN_ROOM);
  if (o == OBJ_WASTE_THRONE) {
    if (alive(i = max_players + MOB_WASTE_DJINNI) == -1 &&
	pscore(i) == mynum) {
      qsetflg(mynum, Q_FIERY_KING);
    }
  } else if (o == OBJ_TOWER_CROWN) {
    if (alive(i = max_players + MOB_TOWER_SHAZARETH) == -1 &&
	pscore(i) == mynum) {
      qsetflg(mynum, Q_TOWER);
    }
  }
  if (otstbit(o, OFL_CONTAINER)) {

      for (i = ofirst_obj(o); i != SET_END; i = onext_obj(o))

      if (iscontin(i, o)) {
	dropinpit(i);
      }
  }
}


/* List the objects at the current players location.
 */
void list_objects(int n, Boolean f)
{
	int i, a;

	for (i = 0; i < lnumobs(ploc(mynum)); i++) {

		a = lobj_nr(i, ploc(mynum));

		if (ishere(a) && (n == 0 || otstmask(a,n) == f)) {
			if (state(a) > 3)
			  continue;
			if (!EMPTY(olongt(a, state(a)))) {
				if (otstbit(a, OFL_DESTROYED))
				  bprintf("--");
				oplong(a);
				cur_player->wd_it = oname(a);
			} else if (plev(mynum) >= LVL_ARCHWIZARD) {
				bprintf("<marker>%s\n", oname(a));
			}
		}
	}
}


void dumpitems()
{
	dumpstuff(mynum, ploc(mynum));
}

void dumpstuff(int n, int loc)
{
	int b;

	for (b = pfirst_obj(n); b != SET_END; b = pnext_obj(n))
	  if (iscarrby(b, n)) {
		  if (loc == LOC_PIT_PIT)
		          dropinpit(b);
		  else
		          setoloc(b, loc, IN_ROOM);
	  }
}

/* Set a players weapon. Sets both the carry-flag of the weapon and the
 * 'pweapon' entry for that player in the world. A negative value removes
 * any current weapon. Return True if a new weapon got set for the player,
 * else False.
 */
Boolean set_weapon(int plr, int wpn)
{
	int owpn, i;

	/* Erase any weapon we were allready wielding:
	 */
	if ((owpn = pwpn(plr)) != -1 && oloc(owpn) == plr) {
		if (ocarrf(owpn) == BOTH_BY)
		        setcarrf(owpn, WORN_BY);
		else if (ocarrf(owpn) == WIELDED_BY)
		        setcarrf(owpn, CARRIED_BY);
	}

	if (wpn < 0 || odamage(wpn) == 0 || ocarrf(wpn) < CARRIED_BY
	    || oloc(wpn) != plr) {

		setpwpn(plr, -1);
		return False;
	}

	i = WIELDED_BY;

	if (ocarrf(wpn) == WORN_BY)  i = BOTH_BY;

	setpwpn(plr, wpn);
	setcarrf(wpn, i);

	return True;
}





void oplong(int x)
{
  char *t = olongt(x, state(x));

  if (!EMPTY(t)) {
    bprintf("%s\n", t);
  }
}


int gotanything(int x)
{
	int ct;
    
	for (ct = 0; ct < pnumobs(x); ct++) {

		if (iscarrby( pobj_nr(ct, x), x))  return 1;
	}

	return 0;
}


static CLASS_DATA *findclass(char *n)
{
  CLASS_DATA *cl;

  for (cl = class_data; cl->class_name != NULL; cl++) {
    if (EQ(cl->class_name,n)) return cl;
  }
  return NULL;
}

static Boolean classmatch(int ob, CLASS_DATA *cl)
{
  register short st;

  return (cl == NULL ||
	  ((st = cl->class_state) < 0 || st == state(ob)) &&
	  otstmask(ob,cl->class_mask));
}

Boolean is_classname(char *name)
{
	return name != NULL && findclass(name) != NULL;
}


/* Can player 'plyr' carry any more objects now ?
 */
Boolean cancarry(int plyr)
{
	int i, a;
	int num = 0;

	if (plev(plyr) >= LVL_WIZARD || plyr >= max_players)
	        return True;

	for (i = 0; i < pnumobs(plyr); i++) {

		a = pobj_nr(i, plyr);

		if (iscarrby(a, plyr) && !iswornby(a, plyr)) num++;
	}

	return num < plev(plyr) + 5;
}


Boolean iswornby( int ob, int plr)
{
	return isworn(ob) && iscarrby(ob, plr);
}

/* Is object 'ob' available ?
 */
Boolean isavl(int ob)
{
	return ishere(ob) || iscarrby(ob, mynum);
}


/* Try to reset an object, return True on success.
 */
Boolean reset_object(int o)
{
	int loc;

	osetbaseval(o, ovalue_reset(o));
	osetsize(o, osize_reset(o));
	osetvis(o, ovis_reset(o));
	osetdamage(o, odamage_reset(o));
	osetarmor(o, oarmor_reset(o));
	state(o) = state_reset(o);
	obits(o) = obits_reset(o);

	if (!opermanent(o)) {
		if (ocarrf_reset(o) == IN_ROOM) {
		        if ((loc = find_loc_by_id(oloc_reset(o))) == 0) {
				destroy(o);
				return False;
			}
		}
		else
		if (ocarrf_reset(o) == IN_CONTAINER) {
			if ((loc = find_object_by_id(oloc_reset(o))) < 0) {
				destroy(o);
				return False;
			}
		}
		else
		if (ocarrf_reset(o) >= CARRIED_BY) {
			if ((loc = find_mobile_by_id(oloc_reset(o))) < 0) {
				destroy(o);
				return False;
			}
		}
	} else {
		loc = oloc_reset(o);
		if (ocarrf_reset(o) >= CARRIED_BY) loc += max_players;
	}

	setoloc(o, loc, ocarrf_reset(o));

	return True;
}



void setobjstate(int obj, int state)
{
	if (state >= 0 && state <= omaxstate(obj)
	    && (olinked(obj) == -1 || state <= omaxstate(olinked(obj)))) {

		state(obj) = state;

		if (olinked(obj) != -1) {
			state( olinked(obj) ) = state;
		}

	} else {
		mudlog( "Attempt to set object %s[%d] to state %d",
		       oname(obj), obj, state);
	}
}


void destroy(int ob)
{
	osetbit(ob, OFL_DESTROYED);
	setoloc(ob, LOC_DEAD_DESTROYED, IN_ROOM);
}

void eat(int ob)
{
	  if (!opermanent(ob) && otemporary(ob)) {
		  destruct_object(ob, NULL);
	  }
	  else {
		  osetbit(ob, OFL_DESTROYED);
		  setoloc(ob, LOC_DEAD_EATEN, IN_ROOM);
	  }
}

void create(int ob)
{
	oclrbit(ob, OFL_DESTROYED);
}


/* SET Object LOCation.
 */
void setoloc(int obj, int loc, int c)
{
	int j;

	/* First remove the object from wherever it is:
	 */
	switch( ocarrf(obj) ) {
	case IN_ROOM:
		if (exists(oloc(obj))) remove_int(obj, linv(oloc(obj)));
		break;
	case IN_CONTAINER:
		if (oloc(obj) >= 0 && oloc(obj) < numobs)
		        remove_int(obj, oinv(oloc(obj)));
		break;
	case CARRIED_BY:
	case WORN_BY:
	case WIELDED_BY:
	case BOTH_BY:
		if (oloc(obj) >= 0 && oloc(obj) < numchars)
		        remove_int(obj, pinv(oloc(obj)));
		break;
	}

	/* Then add it to the right place:
	 */
	switch(c) {
	case IN_ROOM:
		if (exists(loc)) add_int(obj, linv(loc));
		break;
	case IN_CONTAINER:
		if (loc >= 0 && loc < numobs) add_int(obj, oinv(loc));
		break;
	case CARRIED_BY:
	case WORN_BY:
	case WIELDED_BY:
	case BOTH_BY:
		if (loc >= 0 && loc < numchars) add_int(obj, pinv(loc));
		break;
	}

	oloc(obj) = loc;
	ocarrf(obj) = c;

	if (c >= WIELDED_BY) set_weapon(loc, obj);
}



/* is there an object, either carried by the player or in the same
 * room that satisfies certain criteria (determined by mask) ?
 */
Boolean p_ohany(int plr,int mask)
{
	int i, a;

	mask &= 0xffff;

	for (i = 0; i < lnumobs(ploc(plr)); i++) {

		a = lobj_nr(i, ploc(plr));

		if (p_ishere(plr, a) && (obits(a) & mask)) return True;
	}

	for (i = 0; i < pnumobs(plr); i++) {

		a = pobj_nr(i, plr);

		if (iscarrby(a, plr) && (obits(a) & mask)) return True;
	}

	return False;
}


Boolean ohany(int mask)
{
	return p_ohany(mynum,mask);
}

int ovalue(int ob)
{
	return (tscale() * obaseval(ob) / 9);
}


char *xdesloc(char *b, int loc, int cf)
{
  char k[256];
  char v[256];
  char buff[256];

  *buff = '\0';

  while (cf == IN_CONTAINER) {
	  
	  sprintf(v,"In the %s ", oname(loc));
	  strcat(buff, v);
	  cf = ocarrf(loc);
	  loc = oloc(loc);
  }

  if (cf >= CARRIED_BY) {
	  if (cf == CARRIED_BY) strcat(buff, "Carried");
	  if (cf == WORN_BY)    strcat(buff, "Worn");
	  if (cf == WIELDED_BY) strcat(buff, "Wielded");
	  if (cf == BOTH_BY)    strcat(buff, "Worn & Wielded");
	  sprintf(v," by %s ", see_name(mynum, loc));
	  strcat(buff, v);
	  loc = ploc(loc);
  }

  if (!exists(loc)) {
	  if (plev(mynum) < LVL_GOD) return strcpy(b,"Out in the void");
	  else {
		  sprintf(b,"NOT IN UNIVERSE[%d]", loc);
		  return b;
	  }
  }

  if (*buff != '\0')  strcat(buff, "in ");

  if (plev(mynum) >= LVL_WIZARD)
          sprintf(v, "| %s", xshowname(k,loc));
  else
    *v = '\0';

  strcat(buff, sdesc(loc));

  sprintf(b,"%-40.40s%s", buff, v);

  return b;
}

void desloc(int loc, int cf)
{
	char b[512];

	bprintf("%s\n", xdesloc(b,loc,cf));
}

Boolean otstmask(int ob, int v)
{
	return tstbits(obits(ob),v);
}


/* Will the container x hold object y ?
 */
Boolean willhold(int x, int y)
{
	int i, a, sum = 0;

	for (i = 0; i < onumobs(x); i++) {

		a = oobj_nr(i, x);

		if (iscontin(a, x)) sum += osize(a);
	}

	sum += osize(y);

	return sum <= osize(x);
}

int ohereandget(void)
{
  int obj;

  if (EMPTY(item1)) {
    bprintf("Tell me more?\n");
    return -1;
  }
  if ((obj = ob1) == -1) {
    bprintf("It isn't here.\n");
  }
  return obj;
}


/* plx drops the objects that he carries that are not worn or wielded,
 */
void drop_some_objects(int plx)
{
	int obj;

	for (obj = pfirst_obj(plx); obj != SET_END; obj = pnext_obj(plx)) {
		if (ocarrf(obj) == CARRIED_BY && oloc(obj) == plx) {
			setoloc(obj, ploc(plx), IN_ROOM);
		}
	}
}


char *xdesrm(char *b, int loc, int cf)
{
  char k[25];
  char v[30];

  if (plev(mynum) < LVL_WIZARD && cf == IN_ROOM && loc == LOC_LIMBO_LIMBO) {
    return strcpy(b,"Somewhere.....");
  }
  if (cf == IN_CONTAINER) {
    sprintf(b,"In the %s", oname(loc));
    return b;
  }
  if (cf >= CARRIED_BY) {
    if (!seeplayer(loc))
      return strcpy(b,"Somewhere.....");
    else {
      sprintf(b,"Carried by %s", pname(loc));
      return b;
    }
  }
  if (!exists(loc)) {
    if (plev(mynum) < LVL_ARCHWIZARD)
      return strcpy(b,"Out in the void");
    else {
      sprintf(b,"NOT IN UNIVERSE[%d]", loc);
      return b;
    }
  }
  if (plev(mynum) >= LVL_WIZARD)
    sprintf(v, "| %s", xshowname(k,loc));
  else
    *v = 0;

  sprintf(b,"%-30s%s", sdesc(loc), v);
  return b;
}


void desrm(int loc, int cf)
{
	char b[80];

	bprintf("%s\n", xdesrm(b,loc,cf));
}

