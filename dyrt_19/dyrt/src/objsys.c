#include "kernel.h"
#include "objects.h"
#include "mobiles.h"
#include "locations.h"
#include "oflagnames.h"
#include "stdinc.h"

#define MAX_ARMOR	60	/* Take a WIIIILD guess. :P */

int	   value_class(CLASS_DATA *cl, int plx, Boolean silent);
static Boolean     classmatch(int ob,CLASS_DATA *cl);
static void	   dropall(CLASS_DATA *cl);
static void	   getall(CLASS_DATA *cl);
static void	   getallfr(CLASS_DATA *cl);
static void	   trywear(int a, Boolean print_stuff);

static CLASS_DATA class_data[] = {
  { "clothing",   1 << OFL_WEARABLE,  -2},
  { "weapons",    1 << OFL_WEAPON,    -1},
  { "containers", 1 << OFL_CONTAINER, -1},
  { "food",       1 << OFL_FOOD,      -1},
  { "drink",      1 << OFL_DRINK,     -1},
  { "keys",       1 << OFL_KEY,       -1},
  { "all",        0,                  -1},
  { NULL,         0,                  -1}};

int pits[] = {
  OBJ_START_PIT, OBJ_START_CHURCH_PIT,
  OBJ_CATACOMB_PIT_NORTH, OBJ_CATACOMB_PIT_EAST,
  OBJ_CATACOMB_PIT_SOUTH, OBJ_CATACOMB_PIT_WEST,
  -1};

#define is_wearable(O) (otstbit(O, OFL_WEARABLE) || \
			otstbit(O, OFL_WEARONHANDS) || otstbit(O, OFL_WEARONBODY) || \
			otstbit(O, OFL_WEARONHEAD) || otstbit(O, OFL_WEARONBACK) || \
			otstbit(O, OFL_WEARONLEGS) || otstbit(O, OFL_WEARONFACE) || \
			otstbit(O, OFL_WEARONFEET) || otstbit(O, OFL_WEARONNECK) || \
			otstbit(O, OFL_WEARONARMS))

static int cmp_reg_obj(int *a, int *b)
{
  return ovalue(*b) - ovalue(*a);
}

static int cmp_arm_obj(int *a, int *b)
{
  return oarmor(*b) - oarmor(*a);
}

/*
Boolean inorigloc (int i)
{
 if (!opermanent(i)) {
    if (ocarrf_reset(i) == IN_ROOM)
       return (oloc(i) == find_loc_by_id (oloc_reset(i)));
    if (ocarrf_reset(i) == IN_CONTAINER)
       return (oloc(i) == find_object_by_id (oloc_reset(i)));
    if (ocarrf_reset(i) >= CARRIED_BY)
       return (oloc(i) == find_mobile_by_id (oloc_reset(i)));
   }
  else
    if (ocarrf_reset(i) >= CARRIED_BY)
       return (oloc(i) == (oloc_reset(i) + max_players));
    else
       return (oloc(i) == (oloc_reset(i)));
}
*/

Boolean ispit(register int o)
{
  register int i = 0;
  register int j;
  
  while ((j = pits[i++]) != o && j != -1);
  return (j >= 0);
}

void giveallcom (void)
{
  int obj;
  int_set *inv;
  int a;

  if (EMPTY(item1)) 
    {
      bprintf ("Give all to who?? \n");
      return;
    }
  if ((a = pl1) == -1) 
    {
      bprintf("Who's that?\n");
      return;
    }
  inv = pinv(mynum);
  if (mynum == a) 
    {
      bprintf("Cheap skate!\n");
      return;
    }
  if ((plev(mynum) < LVL_APPREN) && (ploc(mynum) != ploc(a)))
    {
      bprintf ("They're not here.\n");
      return;
    }
  for (obj=first_obj(inv); obj!=SET_END; obj=next_obj(inv))
   {
      dogive (obj, a);
      bprintf ("%s: Ok\n", oname(obj));
   }
}

void givecom(void)
{
  int a, c;

  if (EMPTY(item1))
    {
      bprintf("Give what to whom?\n");
      return;
    }
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_DEMI)) {
    bprintf ("You can't give items to people in this room\n");
    return;
   }
  if (pl1 != -1)
    {
      if ((c = ob2) == -1)
        {
          bprintf("You don't have a %s\n", item1);
          return;
        }
      if ((a = pl1) == -1)
  	{
	  bprintf("Who's that?\n");
	  return;
	}
      if (mynum == a)
	{
	  bprintf("Cheap skate!\n");
	  return;
	}
      if (EMPTY(item2))
	{
	  bprintf("Give them what?\n");
	  return;
	}
      dogive(c, a);
      bprintf("Ok.\n");
      return;
    }
  if ((a = ob1) == -1)
    {
      bprintf("You don't have a %s\n", item1);
      return;
    }
  if (EMPTY(item2))
    {
      bprintf("To who?\n");
      return;
    }
  if ((c = pl2) == -1) 
    {
      bprintf("Who's that?\n");
      return;
    }
  if (mynum == c)
    {
      bprintf("Cheap skate!\n");
      return;
    }
  dogive(a, c);
  bprintf("Ok.\n");
}

void dogive(int ob, int pl)
{
  int i, j, o, p;

  o = 0;
  p = 0;
  if (plev(mynum) < LVL_APPREN && ploc(pl) != ploc(mynum))
    {
      bprintf("They aren't here.\n");
      return;
    }
  if (!iscarrby(ob, mynum))
    {
      bprintf("You don't have a %s.\n", oname(ob));
      return;
    }
  if (!cancarry(pl))
    {
      bprintf("They can't carry it.\n");
      return;
    }
  if (pl >= max_players && mtstflg(pl, MFL_QFOOD) && otstbit(ob, OFL_FOOD))
    {
      bprintf("%s thanks you.\n", pname(pl));
      sendf(ploc(mynum), "%s has left the game.\n", pname(pl));
      setpscore(pl, pscore(pl) + 50);
      dumpstuff(pl, ploc(pl));
      strcpy(pname(pl), "");
      eat(ob);
      return;
    }
  
#ifdef LOCMIN_CATACOMB
  if (pnum(pl) == MOB_CATACOMB_BEGGAR && otstbit(ob, OFL_FOOD))
    {
      bprintf("The Beggar thanks you and greedily devours the %s.\n", oname(ob));
      setpscore(mynum, pscore(mynum) + 50);
      eat(ob);
      bprintf("After finishing his meal, the beggar stares at you and says '");
      switch(my_random() % 4)
        {
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
#endif

#ifdef LOCMIN_OAKTREE
  if (pnum(pl) == MOB_OAKTREE_VIOLA && onum(ob) == OBJ_OAKTREE_FAN) 
    {
      bprintf("Viola kisses you%s.\n", psex(mynum) == 0 ? "" : " on the cheek");
      bprintf("Viola says 'Thank you, %s.  Won't you please come in?'\n",
            psex(mynum) == 0 ? "kind sir" : "madame");
      setpscore(mynum, pscore(mynum) + 50);
      setoloc(ob, pl, CARRIED_BY);
      return;
    }
#endif

#ifdef LOCMIN_NARNIA
  if (pnum(pl) == MOB_NARNIA_PETER && onum(ob) == OBJ_NARNIA_PETERSHIELD)
    {
      bprintf("You kneel before Peter and offer him his shield.\n");
      bprintf("Peter smiles at you sagely and thanks you for returning it safely.\n");
      bprintf("He gestures at Lucy, who hands you a healing potion.\n");
      setoloc(OBJ_NARNIA_CORDIAL, mynum, CARRIED_BY);
      questsdone[Q_NARNIA] = True;
      if (!qtstflg(mynum, Q_NARNIA))
        {
          qsetflg(mynum, Q_NARNIA);
          send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, LVL_MAX,
                   NOBODY, NOBODY, "[%s solved the Narnia Quest]\n", pname(mynum));
          bprintf("You have solved the Narnia Quest!\n");
        }
      return;
    }
#endif

#ifdef LOCMIN_JBS
  if (pnum(pl) == MOB_JBS_GOAT && onum(ob) == OBJ_JBS_CAN)
    {
      bprintf("The goat eats the tin can!\n");
      bprintf("After a few minutes of straining the goat excretes a helmet!\n");
      setobjstate(ob,0);
      destroy(ob);
      setoloc(OBJ_JBS_HELMET,ploc(mynum),IN_ROOM);
    }
#endif

#ifdef LOCMIN_START
  if ((onum(ob) == OBJ_START_UMBRELLA) && (pnum(pl) == MOB_VALLEY_CHICKEN))
    {
      setoloc(ob, pl, CARRIED_BY);
      bprintf("It looks confused.\n");
      bprintf("It probably doesn't know how to use it, but it takes the umbrella anyway.\n");
      setpscore(mynum, pscore(mynum) + 50);
      return;
    }
#endif

#ifdef LOCMIN_FOREST
  if ((onum(ob) == OBJ_FOREST_PIPES) && (pnum(pl) == MOB_BLIZZARD_SEAMAS))
    {
      setoloc(ob, pl, CARRIED_BY);
      bprintf("Seamas takes the pipes, thanks you, and begins to play a haunting melody.\n");
      broad("A haunting pipe melody echoes through the air.\n");
      return;
    }
#endif

#ifdef LOCMIN_CAVE
  if ((onum(ob) == OBJ_CAVE_BAGPIPES) && (pnum(pl) == MOB_VALLEY_PIPER))
    {
      setoloc(ob, pl, CARRIED_BY);
      bprintf("The Piper thanks you and begins to play.\n");
      setpscore(mynum, pscore(mynum) + 50);
      broad("In the distance, you hear a piper playing \"Scotland the Brave.\"\n");
      return;
    }
#endif

#ifdef LOCMIN_CASTLE
  if (plev(mynum) < LVL_APPREN)
    {
      if ((onum(ob) == OBJ_CASTLE_RUNESWORD))
	{
	  bprintf("The Runesword does not wish to be given away.\n");
	  return;
	}
    }
#endif

#ifdef LOCMIN_OAKTREE  /* Ithor. Orig zone from aberIV */
  if ((onum(ob) == OBJ_OAKTREE_WATER) && (pnum(pl) == MOB_OAKTREE_ENT))
    {
      sendf(ploc(mynum), "The Ent says 'Ahhh! I needed that.  Thanks you.  A "
	    "word of advice to you:'\n             'Beware, to touch the silver "
	    "spriti is death.'");
      setploc(pl, LOC_DEAD_DEAD);
      strcpy(pname(pl), "");
      eat(ob);
      dumpstuff(pl, ploc(pl));
      sendf(ploc(mynum), "The Ent makes like a tree and leaves east.\n");
      return;
    }
  if ((onum(ob) == OBJ_BLIZZARD_GARLAND) && (pnum(pl) == MOB_OAKTREE_DRYAD))
    {
      sendf(ploc(mynum), "The Dryad says 'Thank you!  It's very lovely.\n");
      bprintf("Ok\n");
      setobjstate(OBJ_OAKTREE_PORTAL1, 0);
      strcpy(pname(pl), "");
      setploc(pl, LOC_DEAD_DEAD);
      dumpstuff(pl, ploc(pl));
      destroy(ob);
      sendf(ploc(mynum),"The Dryad opens a portal in her tree and goes inside\n");
      return;
    }
#endif
 
#ifdef LOCMIN_RHOME 
  if ((onum(ob) == OBJ_RHOME_INVRING) && (EQ(pname(mynum), "Rex"))) {
    setpvis(mynum,0);
    return;
  }
#endif
  
#ifdef LOCMIN_VALLEY
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
#endif
  
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
  char x[128];

  if (EMPTY(item1))
    {
      bprintf("Steal what?\n");
      return;
    }
  strcpy(x, item1);
  if (EMPTY(item2))
    {
      bprintf("From whom?\n");
      return;
    }
  if ((c = pl2) == -1)
    {
      bprintf("Who is that?\n");
      return;
    }
  if (mynum == c)
    {
      bprintf("A true kleptomaniac.\n");
      return;
    }
  if ((a = fobncb(x, c)) == -1)
    {
      bprintf("They don't have it.\n");
      return;
    }
  if (plev(mynum) < LVL_APPREN && ploc(c) != ploc(mynum))
    {
      bprintf("They're not here.\n");
      return;
    }
  if (ocarrf(a) == WORN_BY)
    {
      bprintf("They're wearing it.\n");
      return;
    }
  if (pwpn(c) == a)
    {
      bprintf("They have it firmly in hand... for KILLING people with!\n");
      return;
    }
  if (pnum(c) == MOB_CATACOMB_DEFENDER || mtstflg(c, MFL_NOSTEAL))
    {
      sendf(ploc(c), "%s says 'How dare you steal from me, %s!'\n", pname(c), pname(mynum));
      hit_player(c, mynum, -1);
      return;
    }
  if (!do_okay(mynum, c, PFL_NOSTEAL))
    {
      int i = randperc() % 3;
      switch (i) 
        {
	  case 0:
	    bprintf("%s is too watchful.\n", he_or_she(c));
	    return;
	  case 1:
	    bprintf("%s is too alert.\n", he_or_she(c));
	    break;
	  case 2:
	    bprintf("%s is too crafty.\n", he_or_she(c));
            return;
       	}
    }
  if (!cancarry(mynum))
    {
      bprintf("You can't carry any more.\n");
      return;
    }
  f = randperc();
  e = (10 + plev(mynum) - plev(c)) * 5;
  if (f < e || plev(mynum) >> LVL_APPREN)
    {
      bprintf("Got it!\n");
      sendf(c, "%s steals the %s from you!\n", see_name(c, mynum), oname(a));
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
      onum(obj) == OBJ_WINDOW_SHIELD1 ||
	onum(obj) == OBJ_WINDOW_SHIELD2 ||
	  onum(obj) == OBJ_CATACOMB_SHIELD  ||
	    onum(obj) == OBJ_EFOREST_SHIELD   ||
	      onum(obj) == OBJ_FROBOZZ_SHIELD_ATTIC ||
		onum(obj) == OBJ_ANCIENT_BSHIELD;
}

Boolean wears_shield(int pl)
{
  int i;
  for (i = 0; i < pnumobs(pl); i++) 
    {
      if (iswornby(pobj_nr(i, pl), pl) && is_shield(pobj_nr(i, pl)))
      return True;
    }
  return False;
}

Boolean is_armor(int obj)
{
  return onum(obj) == OBJ_CAVE_ARMOR ||
    onum(obj) == OBJ_ANCIENT_ACLOAK ||
      onum(obj) == OBJ_ORCHOLD_CHAINMAIL ||
	onum(obj) == OBJ_FROBOZZ_MITHRIL_JACKET;
}

Boolean wears_armor(int pl)
{
  int i;

  for (i = 0; i < pnumobs(pl); i++) 
    {
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
  
  for (i = 0; i < pnumobs(pl); i++) 
    {
    if (iswornby(pobj_nr(i, pl), pl) && is_mask(pobj_nr(i, pl)))
      return True;
    }
  return False;
}

Boolean is_boat(int obj)
{
  if (otstbit(obj, OFL_BOAT))
    return True;
  return False;
}

Boolean carries_boat(int pl)
{
  int i;
  
  for (i = 0; i < pnumobs(pl); i++) 
    {
      if (iscarrby(pobj_nr(i, pl), pl) && is_boat(pobj_nr(i, pl)))
      return True;
    }
  return False;
}

/* Does pl carry object type or a clone of it ?  */
int carries_obj_type(int pl, int type)
{
  int i;
  
  for (i = 0; i < pnumobs(pl); i++) 
    {
      if (iscarrby(pobj_nr(i, pl), pl) &&
	onum(pobj_nr(i, pl)) == type)
      return pobj_nr(i, pl);
    }
  return -1;
}

/* Does pl wear object type or a clone of it ? */
int wears_obj_type(int pl, int type)
{
  int i;
  
  for (i = 0; i < pnumobs(pl); i++) 
    {
      if (iswornby(pobj_nr(i, pl), pl) &&
	onum(pobj_nr(i, pl)) == type)
      return pobj_nr(i, pl);
    }
  return -1;
}

void wearcom(void)
{
  int a;
  
  if ((a = ohereandget()) == -1) return;
  if (!iscarrby(a, mynum)) 
    {
      bprintf("You don't have it.\n");
      return;
    }
  if (iswornby(a, mynum)) 
    {
      bprintf("You're already wearing it.\n");
      return;
    }
  trywear(a, True);
}

static void trywear(int a, Boolean print_stuff)
{
  int b;
  int_set *inv = pinv(mynum);
  int obj;
  
#ifdef LOCMIN_ANCIENT
  if (onum(a) == OBJ_ANCIENT_HEALBALSAM)
    {
      if(pstr(mynum) < maxstrength(mynum) - 20)
        {
	  setpstr(mynum, pstr(mynum) + 20);
	  bprintf("You feel some of your wounds disappear.\n");
    	}
      else if (pstr(mynum) < maxstrength(mynum))
	{
	  setpstr(mynum, maxstrength(mynum));
	  bprintf("The balsam heals all your wounds!\n");
	}
      else
	{
	  bprintf("The balsam has a nice cooling effect.\n");
	}
      destroy(a);
      return;
    }
  if (onum(a) == OBJ_ANCIENT_EMBBALSAM) 
    {
      bprintf("You start applying the embalming balsam ...\n");
      bprintf("You begin to feel sleepy, and after a while some mummies\n"
	    "turn up helping you with the embalming ...\n");
      destroy(a);
      crapup("\tThe mummies carry you away to a safe restingplace."
	   " You are dead...", SAVE_ME);
      return;
    }
#endif

#ifdef LOCMIN_RHOME
  if ((onum(a) == OBJ_RHOME_INVRING) && (EQ(pname(mynum), "Rex")))
    {
      send_msg(ploc(mynum),0,pvis(mynum), LVL_MAX,mynum,NOBODY,
	     "%s wears the %s.\n", pname(mynum),oname(a));
       setpvis (mynum, 100000);
       setcarrf(a,WORN_BY);
       bprintf ("You slip the ring onto your finger.\n");
       return;
     }
#endif
  
  if (!otstbit(a, OFL_WEARONHEAD) &&
      !otstbit(a, OFL_WEARONFACE) &&
      !otstbit(a, OFL_WEARONNECK) &&
      !otstbit(a, OFL_WEARONFACE) &&
      !otstbit(a, OFL_WEARONARMS) &&
      !otstbit(a, OFL_WEARONHANDS) &&
      !otstbit(a, OFL_WEARONBODY) &&
      !otstbit(a, OFL_WEARONLEGS) &&
      !otstbit(a, OFL_WEARONFEET) &&
      !otstbit(a, OFL_WEARONBACK) &&
      !otstbit(a, OFL_WEARABLE))
    {
      bprintf("Is this a new fashion?\n");
      return;
    }
 /**************************************************-=-=-=-=-=-=-******/ 
  if (otstbit(a,OFL_EXCLUDEWEAR)) {
    for (obj = first_obj(inv); obj != SET_END; obj = next_obj(inv)) {
      if (otstbit(obj,OFL_EXCLUDEWEAR) && obj != a && isworn(obj)) {
	if ((otstbit(a,OFL_WEARONARMS) && otstbit(obj,OFL_WEARONARMS)) ||
	    (otstbit(a,OFL_WEARONHANDS)&& otstbit(obj,OFL_WEARONHANDS)) ||
	    (otstbit(a,OFL_WEARONBODY) && otstbit(obj,OFL_WEARONBODY)) ||
	    (otstbit(a,OFL_WEARONBACK) && otstbit(obj,OFL_WEARONBACK)) ||
	    (otstbit(a,OFL_WEARONLEGS) && otstbit(obj,OFL_WEARONLEGS)) ||
	    (otstbit(a,OFL_WEARONFEET) && otstbit(obj,OFL_WEARONFEET)) ||
	    (otstbit(a,OFL_WEARONNECK) && otstbit(obj,OFL_WEARONNECK)) ||
	    (otstbit(a,OFL_WEARONFACE) && otstbit(obj,OFL_WEARONFACE)) ||
	    (otstbit(a,OFL_WEARONHEAD) && otstbit(obj,OFL_WEARONHEAD))) {
	  if (!strcasecmp(oname(a),oname(obj)) && print_stuff) {
	    bprintf("You can't wear the two %ss at the same time.\n",
		    oname(a));
	    return;
	  } else if(print_stuff) {
	    bprintf("You can't wear the %s and the %s at the same time.\n",
		    oname(a),oname(obj));
	    return;
	  }
	  return;
	}
      }
    }
  }
  if((player_armor(mynum) + oarmor(a)) > MAX_ARMOR && otstbit(a, OFL_ARMOR)) {
    if(print_stuff)
      bprintf("You can't wear that with all that armor you're wearing.\n");
    return;
  } 
  if(ocarrf(a) == IN_CONTAINER) return;
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
  
  if ((a = ohereandget()) == -1) return;
  if (!iswornby(a, mynum)) {
    bprintf("You're not wearing it.\n");
    return;
  }
#ifdef LOCMIN_RHOME
  if ((onum(a) == OBJ_RHOME_INVRING) && EQ(pname(mynum), "Rex")) {
    setpvis(mynum, 0);
    setcarrf(a,CARRIED_BY);
    send_msg( ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
	     "%s removes the %s.\n",pname(mynum), oname(a));
    bprintf("You slip the ring off of your finger.\n");
    return;
  }
#endif
  b = (ocarrf(a) == BOTH_BY) ? WIELDED_BY : CARRIED_BY;
  setcarrf(a, b);
  
  send_msg( ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
	   "%s removes the %s.\n", pname(mynum), oname(a));
  
  bprintf("Ok\n");
}


/* VALUE command:
 *****************************************************************/

int value_class(CLASS_DATA *cl, int plx, Boolean silent)
{
  int obj, sum = 0;
  
  for (obj = 0; obj < numobs; obj++ ) {
    
    if (in_inventory(obj, plx) && classmatch(obj, cl)) {
      sum += ovalue(obj);
      if (!silent) {
	if (plev(plx) >= LVL_APPREN) bprintf("[%3d]", obj);
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
	bprintf("\n&+GTotal value:&+g%*d points.&*\n",
		plev(mynum) >= LVL_APPREN ? 11 : 6,
		value_class(c, mynum, False));
      } else if ((a = fobn(wordbuf)) == -1) {
	bprintf("%s: no such object\n", wordbuf);
      } else {
	if (plev(mynum) >= LVL_APPREN) bprintf("[%3d]", a);
	bprintf("%12.12s:%5d points\n", oname(a), ovalue(a));
      }
    } while (brkword() != -1);
  }
}
/*End, VALUE command
 ****************************************************************/


void putcom(void)
{
  int a, i;
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
 if (ststflg(mynum, SFL_GHOST)) {
 bprintf("Your ghostly fingers pass right through solid objects!\n");
 return; } 
#ifdef LOCMIN_ABYSS
  
  if (c == OBJ_ABYSS_FORGE && state(c)==1)
    if (a == OBJ_ABYSS_LANCE && state(a)) {
      setobjstate(a, 0);
      bprintf("The lance heats up quickly and begins to glow.\n");
      osetdamage(a,26);
      return;
    }
  if (c == OBJ_ABYSS_BOX) {
    if ((a == OBJ_EFOREST_PURSE_COIN) || (a == OBJ_ABYSS_COIN1)) {
      bprintf("An unseen magical force whisks you away...\n");
      destroy(a);
      teletrap(LOC_HEAVEN_1);
      return;
    }
    if ((a == OBJ_EFOREST_TRASH_COIN) || (a == OBJ_ABYSS_COIN2)) {
      bprintf("An unseen magical force whisks you away...\n");
      destroy(a);
      teletrap(LOC_WASTE_BEFORE);
      return;
    }
  }
  if (c == OBJ_ABYSS_SLOT1)
    if (a == OBJ_ABYSS_CARD2) {
      setobjstate(OBJ_ABYSS_HOMEDOOR, 0);
      bprintf("You slide the card into the slot, and the front door opens.\n");
      return;
    }
  if (c == OBJ_ABYSS_SLOT2)
    if (a == OBJ_ABYSS_CARD1) {
      setobjstate(OBJ_ABYSS_CELLWALL, 0);
      bprintf("You slide the card into the slot, and hear a soft click.\n");
      bprintf("The west wall slides back!\n");
      return;
    }
#endif
 
#ifdef LOCMIN_OAKTREE
  if (c == OBJ_OAKTREE_MOUTH1)
    {
      if (a != OBJ_OAKTREE_QUARTZ)
	{
	  bprintf("It won't fit.\n");
	  return;
	}
      else if (state(OBJ_OAKTREE_MOUTH1) == 0)
	{
	  bprintf("Nothing happens.\n");
	  return;
	}
      setobjstate(OBJ_OAKTREE_MOUTH1, 0);
      destroy(OBJ_OAKTREE_QUARTZ);
      bprintf("The quartz slides into its mouth and fits perfectly.\n");
      send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
	       "%s slides the quartz into the lion's mouth.\n",pname(mynum));
      send_msg(ploc(mynum), 0, LVL_MIN, pvis(mynum), mynum, NOBODY,
	       "Someone slides the quartz into the lion's mouth.\n");
      return;
    }
#endif

#ifdef LOCMIN_JBS /* code written by Rendor(ken@esu.edu) Nov 1993 */
  if( c==OBJ_JBS_POND && a==OBJ_JBS_LINE ) {
    if( state(OBJ_JBS_POND) ) {
      bprintf("Your line thrashes about!  You caught a fish!\n");
      create(OBJ_JBS_FISH);
      setoloc(OBJ_JBS_FISH,mynum,CARRIED_BY);
      setobjstate(c,0);
      return;
    }
    bprintf("The line just hangs there, I think the pond is empty.\n");
    return;
  }
  if( c==OBJ_JBS_HOLE_GROUND && a==OBJ_JBS_SEEDS ) {
    bprintf("The seeds sprout into a giant beanstalk!\n");
    setobjstate(OBJ_JBS_STALK_BOTTOM,0);
    destroy(OBJ_JBS_SEEDS);
    return;
  }
  if( c==OBJ_JBS_HOLE_BED && a==OBJ_JBS_STAFF && state(OBJ_JBS_PANEL_BED) ) {
    bprintf("A panel clicks open in the floor!\n");
    setobjstate(OBJ_JBS_PANEL_BED,0);
    return;
  }
#endif
  
#ifdef LOCMIN_MOOR
  if ((c == OBJ_MOOR_ALTAR) && (a == OBJ_MOOR_CHALICE))
    {
      setobjstate(OBJ_MOOR_ALTAR, 1);
      destroy(OBJ_MOOR_CHALICE);
      bprintf("Ok.\n");
      return;
    }
#endif
  
#ifdef LOCMIN_PIRATE
  if ((c == OBJ_PIRATE_TABLE) && (a == OBJ_VILLAGE_MAP))
    {
      if (state(OBJ_PIRATE_PLANK1) == 0)
	{
	  bprintf("Nothing happens...\n");
	  return;
	}
      if (state(OBJ_PIRATE_SAIL) == 1)  /* Sail raised, switch exits */
	{
	  send_msg(ploc(mynum), 0, LVL_MIN, LVL_MAX, NOBODY, NOBODY,
		   "A ghost pirate fades into view.  After carefully studying "
		   "the map, he goes\n"
		   "outside and sails the ship to a new land.\n");
	  setobjstate(OBJ_PIRATE_LADDER2COVE, 1);
	  setobjstate(OBJ_PIRATE_PLANK1, 0);
	  destroy(OBJ_VILLAGE_MAP);
	}
      else                 /* if sail NOT raised... */
	{
	  send_msg(ploc(mynum), 0, LVL_MIN, LVL_MAX, NOBODY, NOBODY,
		   "A ghostly pirate fades into view.  After testing the wind "
		   "carefully, he\n"
		   "shakes his head sadly and fades away.\n");
	}
      return;
    }
  if ((c == OBJ_PIRATE_SABRE) && (a == OBJ_PIRATE_RUBY))  /* Repair sabre */
    {
      bprintf("You put the ruby in the hilt, the sabre glows and eerie green.\n")
	;
      send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
	       "%s puts the ruby in the hilt, the sabre glows and eerie green.\n",
	       pname(mynum));
      send_msg(ploc(mynum), 0, LVL_MIN, pvis(mynum), mynum, NOBODY,
	       "Someone puts the ruby in the hilt, the sabre glows an eerie green.\n");
      
      osetdamage(OBJ_PIRATE_SABRE, 25);  /* aberIV normal is 28 */
      destroy(OBJ_PIRATE_RUBY);
      questsdone[Q_SABRE] = True;
      if (!qtstflg(mynum, Q_SABRE))
	{
	  qsetflg(mynum, Q_SABRE);
	  send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, LVL_MAX,
		   NOBODY, NOBODY, "[%s solved the Sabre Quest]\n", pname(mynum));
	  bprintf("You have solved the Sabre Quest!\n");
	}

      return;
    }
#endif  /* PIRATE.ZONE */
  
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
      questsdone[Q_SUNDISC] = True;
      if (!qtstflg(mynum, Q_SUNDISC))
        {
          qsetflg(mynum, Q_SUNDISC);
          send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, LVL_MAX,
                   NOBODY, NOBODY, "[%s solved the SunDisc Quest]\n", pname(mynum));
          bprintf("You have solved the SunDisc Quest!\n");
        }
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
      if (alive(i = max_players + MOB_TOWER_SHAZARETH) == -1 &&
	  pscore(i) == mynum) 
	{
	  questsdone[Q_INVISWAND] = True;
	  if (!qtstflg(mynum, Q_INVISWAND)) 
	    {
	      qsetflg(mynum, Q_INVISWAND);
	      send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, LVL_MAX,
		       NOBODY, NOBODY, "[%s solved the InvisWand quest]\n",pname(mynum));
	      bprintf("You have solved the InvisWand Quest!\n");
	    }
	}
      bprintf("The wand seems to soak up energy.\n");
      osetarmor(a, 4);
      return;
    }
    bprintf("Nothing happens.\n");
    return;
  }
  
  if (c == OBJ_WINDOW_SLIME_PIT) {
    if (state(c) == 0) {
      setoloc(a, LOC_WINDOW_SLIME, IN_ROOM);
      bprintf("Ok\n");
      return;
    }
    destroy(a);
    bprintf("It dissappears with a fizzle into the slime.\n");
    if (onum(a) == OBJ_WINDOW_SOAP) {
      bprintf("The soap dissolves the slime away!\n");
      setobjstate(OBJ_WINDOW_SLIME_PIT, 0);
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
    bprintf("However much &+rblood&* you drink from the chalice it "
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
  case OBJ_NARNIA_TURKISHDELIGHT:
    bprintf("You dive eagerly into the dish of turkish delight.\n");
    bprintf("It tastes so good!  Unfortunately, because it is magical, it\n");
    bprintf("replenishing itself, and you keep eating until your stomach bursts.\n");
    eat(b);
    p_crapup(mynum, "In your dying moments you recall that gluttony is one of the seven deadly sins.", CRAP_SAVE);
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
  case OBJ_NARNIA_CORDIAL:
    setpstr(mynum, maxstrength(mynum));
    bprintf("You drink a few drops of the potion and instantly feel much stronger.\n");
    setoloc(b, LOC_DEAD_EATEN, IN_ROOM);
    destroy(b);
    break;
  case OBJ_TREEHOUSE_WAYBREAD1:
    if (plev(mynum) < LVL_APPREN && cur_player->pretend < 0) {
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
    if (plev(mynum) >= LVL_ONE && plev(mynum) < LVL_FOUR) {
      setpscore(mynum, pscore(mynum) + 40);
      calibme();
      bprintf("&+CYou feel a wave of energy sweeping through you.&*\n");
    } else {
      bprintf("Faintly magical by the taste.\n");
      if (plev(mynum) >= LVL_FOUR && pstr(mynum) < 10)
        setpstr(mynum, pstr(mynum) + 4);
      calibme();
    }
    break;
#ifdef LOCMIN_JBS /* code written by Rendor(ken@esu.edu) Nov 1993 */
  case OBJ_JBS_SEEDS:
    bprintf("A &+gbeanstalk&* starts growing in your stomach!\n");
    eat(b);
    crapup("Oh dear, your stomach seems to have burst!\n", SAVE_ME );
    return;
  case OBJ_JBS_SHROOMS:
    bprintf("Everything shimmers as the world around you melts and you are\n\
transported to a world of infinite alls and nothings.\n" );
    teletrap(LOC_JBS_FGATE);
    eat(b);
    return;
  case OBJ_JBS_COFFEE:
    bprintf("Everything twists around and resumes normal dimensions.\n");
    teletrap(LOC_JBS_SEPASTURE);
    eat(b);
    return;
#endif

#ifdef LOCMIN_NIBELUNG  /* Potion must eat to go down from bench */
  case OBJ_NIBELUNG_POTION:
    setpstr(mynum, ((randperc() % 21) + 1)); /* strength from 1 - 22 */
    osetbit(b, OFL_DESTROYED);
    setoloc(b, mynum, CARRIED_BY);
    break;
#endif
    
#ifdef LOCMIN_ABYSS
  case OBJ_ABYSS_SODA:
    setpstr(mynum, maxstrength(mynum));
    bprintf("As the &+Ysoda&* gurgles down your dry throat you "
            "feel invigourated!\n");
    setoloc(OBJ_ABYSS_SODA, LOC_DEAD_EATEN, IN_ROOM);
    eat(OBJ_ABYSS_SODA);
    break;
  case OBJ_ABYSS_POTION1:
    osetarmor(OBJ_TOWER_WAND, oarmor(OBJ_TOWER_WAND) - 1);
    cur_player->me_ivct = 60;
    setpvis(mynum, 10);
    bprintf("You seem to &+Cs&+Wh&+ci&+wm&+Cm&+We&+cr and blur.\n");
    setoloc(OBJ_ABYSS_POTION1, LOC_DEAD_EATEN, IN_ROOM);
    eat(OBJ_ABYSS_POTION1);
    break;
  case OBJ_ABYSS_POTION2:
    if (pstr(MOB_ABYSS_QBERT + max_players) < 0)
      {
   	bprintf("There is a sudden feeling of failure...\n");
	break;
      }
    else {
      polymorph(max_players + MOB_ABYSS_QBERT, 10);
    }
    setpstr(mynum, pstr(mynum) + 16);
    setoloc(OBJ_ABYSS_POTION2, LOC_DEAD_EATEN, IN_ROOM);
    eat (OBJ_ABYSS_POTION2);
    break;
#endif
    
#ifdef LOCMIN_ANCIENT
  case OBJ_ANCIENT_FOUNTAIN_OF_YOUTH:
    if (pscore(mynum) >= 20) {
      setpscore(mynum, pscore(mynum) - 20);
      calibme();
      bprintf("You feel younger and less experienced...\n");
    }
    break;
#endif
#ifdef LOCMIN_MITHDAN
  case OBJ_MITHDAN_FOUNTAIN:
    bprintf("As you drink from the fountain, you feel the world altering\n");
    bprintf("around you, and you wake up in a new realm.\n");
    teletrap(LOC_MITHDAN_51);
    break;
#endif
  default:
    if (otstbit(b, OFL_FOOD)) 
      {
        sprintf(s, "\001P%s\003 greedily devours the %s.\n", pname(mynum), oname(b));
        eat(b);
        bprintf("Delicious!\n");
        setpstr(mynum, pstr(mynum) + 12);
        calibme();
        sillycom(s);
      }
    else if (otstbit(b, OFL_DRINK))
      {
	sprintf(s, "\001P%s\003 guzzles the %s.\n", pname(mynum), oname(b));
        eat(b);
        bprintf("Refreshing!\n");
	setpstr(mynum, pstr(mynum) + 8);
        calibme();
        sillycom(s);
      }
    else
      {
        bprintf("I think I've lost my appetite.\n");
        return;
      }
    break;
  }
}

void inventory(void)
{
if (ststflg(mynum, SFL_GHOST)) {
 bprintf("You have no means of carrying items!\n");
 return; }
  if (plev(mynum) < LVL_APPREN) {
    send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
	     "%s rummages through %s backpack.\n", 
	     pname(mynum), his_or_her(mynum));
  }
  
  print_inventory(mynum);
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
      
      if ((d = (otstbit(obj, OFL_DESTROYED)))) {
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
	  strcpy(s,"<worn ");
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
  
  if (plev(mynum) < LVL_APPREN && otstbit(o1, OFL_DESTROYED))
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
  int l,i;
  char *s;
  char bf[81];

 if (ststflg(mynum, SFL_GHOST)) {
 bprintf("Your ghostly fingers pass right through solid objects!\n");
 return; } 
#ifdef LOCMIN_HEAVEN
  if( ob == OBJ_HEAVEN_VASE && state(ob) )
    {
      bprintf("The vase slips from your hands and crashes to the floor.\n");
      bprintf("The reverberations caused by the crash cause the ceiling to "
	      "cave in!\n");
      destroy(ob);
      send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
	       "%s has left.",pname(mynum));
      setploc(mynum,LOC_HEAVEN_1);
      lookin(ploc(mynum),0);
      broad("You hear ambulance sirens in the distance.\n");
      return 0;
    }
  if ((ob == OBJ_HEAVEN_MJOLNIR) && ((alive((max_players + MOB_HEAVEN_THOR))) == -1) &&
 	(pscore(max_players + MOB_HEAVEN_THOR) == mynum))
  {
  questsdone[Q_MJOLNIR] = True;
    if (!qtstflg(mynum, Q_MJOLNIR))
        {
          qsetflg(mynum, Q_MJOLNIR);
          send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, LVL_MAX,
                   NOBODY, NOBODY, "[%s solved the Mjolnir Quest]\n", pname(mynum));
          bprintf("You have solved the Mjolnir Quest!\n");
        }
}
#endif

#ifdef LOCMIN_NEWBIE
  if (ob == OBJ_NEWBIE_DIAMOND)
    {
      bprintf("The diamond is packed into the wall a little better than you thought.\n");
      bprintf("By pulling it out you cause the mine to settle, sealing the tunnel.\n");
      setoloc(ob, mynum, CARRIED_BY);
      setexit(LOC_NEWBIE_MINE, 0, 0);
    }
#endif
 
#ifdef LOCMIN_LEDGE
  if ((ploc(mynum) == LOC_LEDGE_7) && (ob == OBJ_LEDGE_ORB))
    {
      questsdone[Q_ORB] = True;
      if (!qtstflg(mynum, Q_ORB))
        {
          qsetflg(mynum, Q_ORB);
          send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, LVL_MAX,
                   NOBODY, NOBODY, "[%s solved the Orb Quest]\n", pname(mynum));
          bprintf("You have solved the Orb Quest!\n");
        }
    }
#endif

#ifdef LOCMIN_WINDOW
if (ob == OBJ_WINDOW_ROD)
  {
    if (alive(i = max_players + MOB_WINDOW_RIATHA) == -1 && pscore(i) == mynum)
      {
        questsdone[Q_ROD] = True;
        if (!qtstflg(mynum, Q_ROD))
          {
            qsetflg(mynum, Q_ROD);
            send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, LVL_MAX, NOBODY, NOBODY, "[%s solved the Rod Quest]\n", pname(mynum));
            bprintf("You have solved the Rod Quest!\n");
          }
      }
  }
#endif
     
#ifdef LOCMIN_ABYSS
  if (ob == OBJ_ABYSS_BANK1TIMBER) {
    setobjstate(OBJ_ABYSS_BANK1TIMBER, 1);
    ob = OBJ_ABYSS_TIMBER;
    create(ob);
  }
  if (ob == OBJ_ABYSS_BANK2TIMBER) {
    setobjstate(OBJ_ABYSS_BANK2TIMBER, 1);
    ob = OBJ_ABYSS_TIMBER;
    create(ob);
  }
#endif
  
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
  
#ifdef LOCMIN_OAKTREE  /* Ithor. Orig zone from aberIV */
  if ((ob == OBJ_OAKTREE_PICTURE) && (state(OBJ_OAKTREE_PICTURE) == 1))
    {
      bprintf("You uncover a wall safe behind the picture.\n");
      send_msg(LOC_OAKTREE_OAKTREE37, 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
	       "%s uncovers a wall safe behind the picture.\n",pname(mynum));
      send_msg(LOC_OAKTREE_OAKTREE37, 0, LVL_MIN, pvis(mynum), mynum, NOBODY,
	       "Someone uncovers a wall safe behind the picture.\n");
      create(OBJ_OAKTREE_SAFE);
      setoloc(OBJ_OAKTREE_SAFE, LOC_OAKTREE_OAKTREE37, IN_ROOM);
    }
  if ((ob == OBJ_OAKTREE_SPIKE) && (!iswornby(OBJ_OAKTREE_GLOVE, mynum)))
    {
      crapup("    As you touch the spike, you feel a powerful spirit take "
	     "control of\n    your body very briefly before your life winks "
	     "out like a light...",SAVE_ME);
      return 0;
    }
#endif
  
  if (ob == OBJ_WINDOW_SHIELD) {
    if (ishere(OBJ_WINDOW_SHIELD1)) ob = OBJ_WINDOW_SHIELD1;
    else if (ishere(OBJ_WINDOW_SHIELD2)) ob = OBJ_WINDOW_SHIELD2;
    else if (container == -1) {
      if (otstbit(OBJ_WINDOW_SHIELD1, OFL_DESTROYED))
	ob = OBJ_WINDOW_SHIELD1;
      else if (otstbit(OBJ_WINDOW_SHIELD2, OFL_DESTROYED))
	ob = OBJ_WINDOW_SHIELD2;
      if (ob == OBJ_WINDOW_SHIELD1 || ob == OBJ_WINDOW_SHIELD2)
	create(ob);
      else {
	bprintf("The shields are too firmly secured to the walls.\n");
	return 0;
      }
    }
  }
 
  if ((otstbit(ob, OFL_NEWBIE)) && (plev(mynum) > LVL_SEVEN)  && (plev(mynum) < LVL_EMERITUS))
    {
      bprintf("A strange force prevents you from picking it up.\n");
      return -1;
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
 
  if (!cancarry(mynum)) 
   {
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
    questsdone[Q_GRAIL] = True;
    if (!qtstflg(mynum, Q_GRAIL)) {
      qsetflg(mynum, Q_GRAIL);
      send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, LVL_MAX, NOBODY,
	       NOBODY, "[%s solved the FindGrail quest]\n",pname(mynum));
      bprintf("You have solved the FindGrail Quest!\n");
    }
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
  int i, m;
  int loc;
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
  loc = ploc( mynum );
  for (i=0; i <lnumchars(loc); i++)
    {
      m = lmob_nr(i, loc);
      if (m >= max_players && m != mynum && alive(m) != -1)
        {
	  if (mtstflg(m, MFL_GRABH))
	    {
	      bprintf("%s looks at you and shouts: 'You can't take that!'\n", pname(m));
	      send_msg(ploc(mynum), MODE_NODEAF, pvis(mynum), LVL_MAX, mynum, NOBODY, "%s looks at %s and shouts: 'You can't take that!'\n",pname (m), pname(mynum));
	      setpfighting(m, mynum);
	      setpfighting(mynum, m);
	      return -1;
	    }
	  else if (mtstflg(m, MFL_NOGRAB))
	    { 
	      bprintf("%s won't let you near it.\n", pname(m));
	      return -1;
            }
        }
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
  
  /* Mithdan Druids get PO'd if you take their statue. */
#ifdef LOCMIN_MITHDAN
  
  if ((ob == OBJ_MITHDAN_CRYSTAL) && (state (OBJ_MITHDAN_CRYSTAL) == 1))
    {
      setploc(MOB_MITHDAN_DRUID + max_players, oloc(OBJ_MITHDAN_CRYSTAL));
      setploc(MOB_MITHDAN_DRUID2 + max_players, oloc(OBJ_MITHDAN_CRYSTAL));
      bprintf("The druids, angered at you for defiling their sacred relic, shout an\n");
      bprintf("ancient oath and leap towards you.\n");
      send_msg (ploc(mynum),0,pvis(mynum), LVL_MAX,mynum,NOBODY,
		"A pair of druids appear on each side of %s and attack!\n",pname(mynum));
      setpagg (MOB_MITHDAN_DRUID + max_players, 100);
      setpagg (MOB_MITHDAN_DRUID2 + max_players, 100);
      setpspeed (MOB_MITHDAN_DRUID + max_players, 0);
      setpspeed (MOB_MITHDAN_DRUID2 + max_players, 0);
      setobjstate (OBJ_MITHDAN_CRYSTAL, 0);
      hit_player (MOB_MITHDAN_DRUID + max_players, mynum, -1);
      hit_player (MOB_MITHDAN_DRUID2 + max_players, mynum, -1);
    }
  
#endif
  
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
      if (dropobj(-1) == -1)
	return;
    }
  }
}


/* Is the item in the same room as the player ?
 */
Boolean p_ishere(int plr,int item)
{
  if (plev(plr) < LVL_APPREN && otstbit(item, OFL_DESTROYED))
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
  /*****************8
    if (plev(mynum) < LVL_APPREN && otstbit(item, OFL_DESTROYED))
    return False;
    *******************/
  if (user < max_players && plev(user) < LVL_APPREN && otstbit(item, OFL_DESTROYED))
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



int dropobj(int obj)
{
  CLASS_DATA *cl;
  int a, i, l, j;
  if(obj == -1) {
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
  } else a = obj;
  if (plev(mynum) < LVL_APPREN && onum(a) == OBJ_CASTLE_RUNESWORD) {
    bprintf("You can't let go of it!\n");
    return 0;
  }
  if (a == OBJ_CATACOMB_CUPSERAPH
      && ploc((max_players + MOB_CATACOMB_SERAPH)) == LOC_START_CHURCH)
    setplev((max_players + MOB_CATACOMB_SERAPH), -2);
  l = ploc(mynum);
  /* MINE LADDER */
  if (l == LOC_QUARRY_4 || l == LOC_QUARRY_5) 
    {
    bprintf("The %s falls down the ladder.\n", oname(a));
    l = LOC_QUARRY_9;
    }
  if ((l >= LOC_QUARRY_16 && l <= LOC_QUARRY_14) ||
      (l >= LOC_QUARRY_10 && l <= LOC_QUARRY_9)) 
    {
    bprintf("The %s falls down the ladder.\n", oname(a));
    l = LOC_QUARRY_8;
    }
  /* ALL AT SEA */
  if (ltstflg(l, LFL_ON_WATER) && onum(a) != OBJ_VILLAGE_BOAT && 
      onum(a) != OBJ_VILLAGE_RAFT && onum(a) != OBJ_ANCIENT_CANOE) {
    bprintf("The %s sinks into the sea.\n", oname(a));
    l = LOC_SEA_7;
  }
#ifdef LOCMIN_RHOME
  if ((onum(a) == OBJ_RHOME_INVRING) && (EQ(pname(mynum), "Rex"))) {
    send_msg (ploc(mynum),0,pvis(mynum), LVL_MAX,mynum,NOBODY,
	      "%s drops the %s.\n",pname(mynum),oname(a));
    setpvis (mynum, 0);
    setoloc(a, ploc(mynum), IN_ROOM);
    bprintf("You drop the ring.\n");
    return 0;
  }
#endif

#ifdef LOCMIN_ABYSS
  if (a == OBJ_ABYSS_TIMBER)
    if (ploc(mynum) == LOC_ABYSS_BANK1) {
      setobjstate(OBJ_ABYSS_BANK1TIMBER, 0);
      setoloc(a, ploc(mynum), IN_ROOM);
      destroy(a);
      bprintf("You place the timber across the river of slime.\n");
      return 0;
    }
  if (a == OBJ_ABYSS_TIMBER)
    if (ploc(mynum) == LOC_ABYSS_BANK2) {
      setobjstate(OBJ_ABYSS_BANK2TIMBER, 0);
      setoloc(a, ploc(mynum), IN_ROOM);
      destroy(a);
      bprintf("You place the timber across the river of slime.\n");
      return 0;
    }
#endif
  
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
  if ((i >= 0 && state(i) == 0) || oloc(OBJ_SEA_HOLE) == l) {
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
  if ((l = (ploc(mynum))))
    bprintf("Ok\n");
  return 0;
}

void dropinpit(int o)
{
  int i;
  
  setpscore(mynum, pscore(mynum) + ovalue(o));
  calib_player(mynum);
  if (otstbit(o, OFL_PITRESET))
    {
      osetbaseval(o, 0);
      setoloc(o, LOC_START_DONATION, IN_ROOM);
    }
/*
  if (otstbit(o, OFL_PITRESET))
    {
      if(inorigloc(o))
   	{
	  osetbaseval(o, 0);
	}
      else if (!reset_object(o))
	{
	  osetbit(o, OFL_DESTROYED);
	  setoloc(o, LOC_PIT_PIT, IN_ROOM);
	}
     else osetbaseval(o, 0);
    }
*/
  else
    {
      osetbit(o, OFL_DESTROYED);
      setoloc(o, LOC_PIT_PIT, IN_ROOM);
    }
#ifdef LOCMIN_WASTE
  if (o == OBJ_WASTE_THRONE)
    {
      if (alive(i = max_players + MOB_WASTE_DJINNI) == -1 &&
	  pscore(i) == mynum)
	{
	  questsdone[Q_FIERY_KING] = True;
	  if (!qtstflg(mynum, Q_FIERY_KING))
	    {
	      qsetflg(mynum, Q_FIERY_KING);
	      send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, LVL_MAX, NOBODY,
		       NOBODY, "[%s solved the Greatsword Quest]\n", pname(mynum));
	      bprintf("You have solved the Greatsword Quest!\n");
	    }
	}
    }
#endif

#ifdef LOCMIN_OAKTREE
  else if (o == OBJ_OAKTREE_MARBLEBUST)
    {
  questsdone[Q_BUST] = True;
    if (!qtstflg(mynum, Q_BUST))
        {
          qsetflg(mynum, Q_BUST);
          send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, LVL_MAX,
                   NOBODY, NOBODY, "[%s solved the Bust Quest]\n", pname(mynum));
          bprintf("You have solved the Bust Quest!\n");
        }
    }
#endif

  else if (o == OBJ_OAKTREE_SPIKE)
    {
      if (alive(i = max_players + MOB_OAKTREE_SAPLING) == -1 &&
	  pscore(i) == mynum)
	{
	  questsdone[Q_SPIKE] = True;
	  if (!qtstflg(mynum, Q_SPIKE))
	    {
	      qsetflg(mynum, Q_SPIKE);
	      send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, LVL_MAX,
		       NOBODY, NOBODY, "[%s solved the Spike Quest]\n", pname(mynum));
	      bprintf("You have solved the Spike Quest!\n");
	    }
	  
	}
    }
  if (otstbit(o, OFL_CONTAINER)) 
    {
      for (i = ofirst_obj(o); i != SET_END; i = onext_obj(o))
	if (iscontin(i, o)) 
	  {
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
  
  /* Erase any weapon we were already wielding:
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


CLASS_DATA *findclass(char *n)
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
	  ((st = cl->class_state) < 0 || ((st == state(ob)) &&
					  otstmask(ob,cl->class_mask))));
}


/* Can player 'plyr' carry any more objects now ?
 */
Boolean cancarry(int plyr)
{
  int i, a;
  int num = 0;
  
  if (plev(plyr) >= LVL_APPREN || plyr >= max_players)
    return True;
  
  for (i = 0; i < pnumobs(plyr); i++) {
    
    a = pobj_nr(i, plyr);
    
    if (iscarrby(a, plyr) && !iswornby(a, plyr)) num++;
  }
  
  return num < plev(plyr) + 5;
}


int iswornby( int ob, int plr)
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
  
  loc = 0;
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
  
  if (plev(mynum) >= LVL_APPREN)
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
  
  if (plev(mynum) < LVL_APPREN && cf == IN_ROOM && loc == LOC_LIMBO_LIMBO) {
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
  if (plev(mynum) >= LVL_APPREN)
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

/*
   void print_inventory(int who)
   {
   int wielded = -1;
   char outline[256];
   
   int obj;
   int_set *inv = pinv(who);
   int len = 0;
   int xen = 0;
   Boolean first = True;
   int o1, o2;
   
   wielded = pwpn(who);
   
   if (first_obj(inv) == SET_END) {
   if (who == mynum) bprintf("You are not carrying anything.\n");
   return;
   }
   o1 = first_obj(inv);
   o2 = next_obj(inv);
   
   if ((o1 == wielded && o2 > -1) || (o1 != wielded))
   bprintf("%s %s carrying:\n",
   who == mynum ? "You" : psex(who) ? "She" : "He",
   who == mynum ? "are" : "is");
   
   bprintf("   ");
   for (obj = first_obj(inv); obj != SET_END; obj = next_obj(inv)) {
   if (obj == wielded && iswielded(obj)) continue;
   sprintf(outline,"%s%s", otstbit(obj,OFL_DESTROYED) ? "(" : "",
   oname(obj));
   if (isworn(obj)) {
   strcat(outline, " <worn> ");
   outline[strlen(outline) - 1] = '\0';
   }
   
   xen = strlen(outline);
   len += xen;
   len += 2;
   if (len >= 70) {
   bprintf("\n   ");
   len = xen + 3;
   first = True;
   }
   if (otstbit(obj, OFL_LIT)) strcat(outline, " <lit>");
   if (otstbit(obj,OFL_DESTROYED)) strcat(outline,")");
   if (first) bprintf("%s",outline);
   else bprintf(", %s",outline);
   first = False;
   
   }
   bprintf("\n");
   if (wielded >= 0 && iswielded(wielded)) {
   bprintf("%s %s %s %s drawn and ready for battle.\n",
   who == mynum ? "You" : pname(who),
   who == mynum ? "have" : "has",
   who == mynum ? "your" : psex(who) ? "her" : "his",
   oname(wielded));
   }
   }
   */

void print_inventory(int who)
{
  
  int obj, len=0, xen, weapon=-1, found=0;
  char outline[256];
  int_set *inv = pinv(who);
  
  for (obj=first_obj(inv); obj!=SET_END; obj=next_obj(inv)) {
    int started = 0;
    
    if (otstbit(obj, OFL_DESTROYED) && plev(mynum) < LVL_APPREN)
      continue;
    if (obj == pwpn(who) && iswielded(obj)) {
      weapon = pwpn(who);
      continue;
    }
    sprintf (outline,"%s%s", otstbit(obj,OFL_DESTROYED) ? "(" : "",
             oname(obj));
    if (isworn(obj)) {
      strcat (outline, " &+G<worn&*");
      started = 1;
    }
    if (otstbit(obj, OFL_LIT)) {
      if (!started)
        strcat (outline, " &+Y<&*");
      else
        strcat (outline, ",");
      started = 1;
      strcat (outline, "&+Ylit&*");
    }
    if (started)
      strcat (outline, "&+Y>&*");
    if (otstbit(obj,OFL_DESTROYED)) strcat(outline,")");
    
    xen = strlen(outline);
    if (!found) {
      bprintf ("%s %s carrying:\n   %s", who == mynum ? "You" : he_or_she(who),
               who == mynum ? "are" : "is", outline);
      len = 3 + xen;
      found = 1;
    } else if (len + xen+2 >= 75) {
      bprintf (",\n   %s", outline);
      len = 3 + xen;
    } else {
      bprintf (", %s", outline);
      len += xen+2;
    }
  }
  
  if (found)
    bprintf ("\n");
  else if (who == mynum)
    bprintf ("You are not carrying anything.\n");
  
  if (found && (who == mynum || plev(mynum) >= LVL_APPREN)) {
    int obj2;
    int_set *inv2;
    
    for (obj=first_obj(inv); obj!=SET_END; obj=next_obj(inv)) {
      if (otstbit(obj, OFL_DESTROYED) && plev(mynum) < LVL_APPREN)
        continue;
      if (otstbit(obj, OFL_CONTAINER)) {
        inv2 = oinv(obj);
        found = 0;
        for (obj2=first_obj(inv2); obj2!=SET_END; obj2=next_obj(inv2)) {
          if (otstbit(obj2, OFL_DESTROYED) && plev(mynum) < LVL_APPREN)
            continue;
          sprintf (outline, "%s%s%s", otstbit(obj2, OFL_DESTROYED) ? "(" : "",
                   oname(obj2), otstbit(obj2, OFL_DESTROYED) ? ")" : "");
          xen = strlen (outline);
          if (!found) {
            bprintf ("   The %s contains:\n      %s", oname(obj), outline);
            len = 6 + xen;
            found = 1;
          } else if (len + xen+2 >= 75) {
            bprintf (",\n      %s", outline);
            len = 6 + xen;
          } else {
            bprintf (", %s", outline);
            len += xen+2;
          }
        }
        if (found)
          bprintf ("\n");
      }
    }
  }
  
  if (weapon != -1)
    bprintf("&+R%s %s %s %s drawn and ready for battle.&*\n",
            who == mynum ? "You" : pname(who), who == mynum ? "have" : "has",
            who == mynum ? "your" : his_or_her(who), oname(weapon));
}
void
equipcom ()
{
  int who;
  int weapon = -1, light = -1, olight = 0, found = 0;
  int num_ofl = WEAR_LAST - WEAR_OFFSET + 1,
  wornset[WEAR_LAST - WEAR_OFFSET + 1][10];
  int i, j, obj;
  int_set *inv;
  
  if (EMPTY(item1))
    who = mynum;
  else if (pl1 == -1) {
    bprintf ("No one with that name is playing.\n");
    return;
  } else if (plev(mynum) < LVL_APPREN && ploc(pl1) != ploc(mynum)) {
    bprintf ("%s's not here!\n", he_or_she(pl1));
    return;
  } else
    who = pl1;
  inv = pinv(who);
  
  for (i=0; i<num_ofl; i++)
    for (j=0; j<10; j++)
      wornset[i][j] = -1;
  
  for (obj = first_obj(inv); obj != SET_END; obj = next_obj(inv))
    if (obj == pwpn(who) && iswielded(obj)) {
      weapon = obj;
      found = 1;
    } else if (isworn(obj)) {
      for (i=0; i<num_ofl && !otstbit(obj,WEAR_OFFSET+i); i++);
      if (i >= num_ofl)
        mudlog ("case slip in equipcom, no match for wear type");
      else {
        for (j=0; j<10 && wornset[i][j]!=-1; j++);
        if (j<10)
          wornset[i][j] = obj;
        found = 1;
      }
    } else if (light == -1 && otstbit(obj, OFL_LIT)) {
      light = obj;
      found = 1;
    }
  
  if (!found) {
    bprintf ("&+B%s %s nothing equipped.&*\n",
             who == mynum ? "You" : pname(who),
             who == mynum ? "have" : "has");
    return;
  }
  bprintf ("&+B%s %s equipped:&*\n",
           who == mynum ? "You" : pname(who),
           who == mynum ? "have" : "has");
  
  if (weapon != -1) {
    bprintf ("  &+G<wielded>&*          : %s", oname(weapon));
    if (otstbit(weapon, OFL_LIT)) {
      bprintf (" (providing light)");
      olight = 1;
    }
    bprintf ("\n");
  }
  for (i=0; i<num_ofl; i++)
    for (j=0; j<10 && wornset[i][j]!=-1; j++) {
      int valid = 1;
      
      switch (i + WEAR_OFFSET) {
      case OFL_WEARONHEAD:
        bprintf ("  <worn on head>     : ");
        break;
      case OFL_WEARONFACE:
        bprintf ("  <worn over face>   : ");
        break;
      case OFL_WEARONNECK:
        bprintf ("  <worn around neck> : ");
        break;
      case OFL_WEARONARMS:
        bprintf ("  <worn on arms>     : ");
        break;
      case OFL_WEARONHANDS:
        bprintf ("  <worn on hands>    : ");
        break;
      case OFL_WEARONBODY:
        bprintf ("  <worn on body>     : ");
        break;
      case OFL_WEARONBACK:
        bprintf ("  <worn over back>   : ");
        break;
      case OFL_WEARONLEGS:
        bprintf ("  <worn on legs>     : ");
        break;
      case OFL_WEARONFEET:
        bprintf ("  <worn on feet>     : ");
        break;
      default:
        mudlog ("case slip in equipcom");
        valid = 0;
      }
      if (valid) {
        bprintf ("%s", oname(wornset[i][j]));
        if (otstbit(wornset[i][j], OFL_LIT)) {
          bprintf (" (providing light)");
          olight = 1;
        }
        bprintf ("\n");
      }
    }
  if (light != -1 && !olight)
    bprintf ("  &+Y<providing light>&*  : %s\n", oname(light));
}

void lockcom()
{
  if (ob1 == -1)
    {
      bprintf("Lock what?\n");
      return;
    }
if (ststflg(mynum, SFL_GHOST)) {
 bprintf("Your ghostly fingers pass right through solid objects!\n");
 return; }
  switch (ob1)
    {
    default:
      if (!otstbit(ob1, OFL_LOCKABLE))
	bprintf("You can't lock that!\n");
      else if (state(ob1) > 1)
 	bprintf("It's already locked.\n");
      else if (!ohany (1<<OFL_KEY))
 	bprintf("You have no key.\n");
      else
	{
	  setobjstate(ob1, 2);
	  bprintf("OK.\n");
	}
    }
}

void unlockcom()
{
  if (ob1 == -1)
    {
      bprintf("What do you want to unlock?\n");
      return;
    }
if (ststflg(mynum, SFL_GHOST)) {
 bprintf("Your ghostly fingers pass right through solid objects!\n");
 return; }
  switch (ob1)
    {
    default:
      if (!otstbit(ob1, OFL_LOCKABLE))
 	bprintf("You can't unlock that!\n");
      else if (state(ob1) == 1)
 	bprintf("It's already unlocked.\n");
      else if (!ohany (1<<OFL_KEY))
	bprintf("You have no key.\n");
      else
	{
	  setobjstate(ob1, 1);
	  bprintf("Ok.\n");
	}
    }
}

void closecom()
{
  if (ob1 == -1)
    {
      bprintf("What would you like to close?\n");
      return;
    }
if (ststflg(mynum, SFL_GHOST)) {
 bprintf("Your ghostly fingers pass right through solid objects!\n");
 return; }
  switch (ob1)
    {
    case OBJ_START_UMBRELLA:
      bprintf("You close the umbrella.\n");
      setobjstate(OBJ_START_UMBRELLA, 0);
      break;
    default:
      if (!otstbit (ob1, OFL_OPENABLE))
	bprintf("You can't close that!\n");
      else if (state(ob1) > 0)
	bprintf("It's already closed.\n");
      else
	{
	  setobjstate(ob1, 1);
	  bprintf("You close the %s.\n", oname(ob1));
	}
    }
}

void opencom()
{
  char *cant_open ="You can't open that!\n";
  if (ob1 == -1)
    {
      bprintf("What would you like to open?\n");
      return;
    }
if (ststflg(mynum, SFL_GHOST)) {
 bprintf("Your ghostly fingers pass right through solid objects!\n");
 return; }
  switch(ob1)
    {
    case OBJ_FOREST_TREEEATING:
    case OBJ_FOREST_INSIDETREE:
      bprintf("You can't shift the tree!\n");
      break;
    case OBJ_VILLAGE_BOT_BOARDS:
    case OBJ_VILLAGE_TOP_BOARDS:
      bprintf("You shift the floorboards, with much heaving and tugging, to "
	      "reveal an exit\nbeyond.\n");
      setobjstate(OBJ_VILLAGE_TOP_BOARDS, 0);
      break;
    case OBJ_TOWER_DOOR_SHAZARETH:
      if (state(ob1) >= 1)
	bprintf("It seems to be magically closed.\n");
      else
	bprintf("It's already open.\n");
      break;
    case OBJ_TOWER_DOOR_TREASURE:
      bprintf("You can't shift the door at all from this side.\n");
      break;
    case OBJ_START_UMBRELLA:
      bprintf("You open the umbrella.\n");
      setobjstate(OBJ_START_UMBRELLA, 1);
      break;
    default:
      if (!otstbit (ob1, OFL_OPENABLE))
	bprintf("%s", cant_open);
      else if (state(ob1) == 0)
	bprintf("It's already open.\n");
      else if (state(ob1) == 1)
	{
	  bprintf("You open the %s.\n", oname(ob1));
	  setobjstate(ob1, 0);
	}
      else if (state(ob1) == 2)
	if (!ohany (1<<OFL_KEY))
	  bprintf("It seems to be locked.\n");
	else
	  {
	    bprintf("Ok.\n");
	    setobjstate(ob1, 0);
	  }
      break;
    }
}
void wearall(void)
{
  int reg[1024], arm[1024], reg_len = 0, arm_len = 0;
  int ob;
  
  for(ob = 0; ob < numobs; ob++)
    if(in_inventory(ob, mynum) && is_wearable(ob) && !iswornby(ob, mynum))
      if(otstbit(ob, OFL_ARMOR)) arm[arm_len++] = ob;
      else reg[reg_len++] = ob;
  
  /*  qsort(reg, reg_len, sizeof(int), cmp_reg_obj);
   *  qsort(arm, arm_len, sizeof(int), cmp_arm_obj);   :P Rex
   */ 
  for(ob = 0; ob < arm_len; ob++)
    trywear(arm[ob], False);
  for(ob = 0; ob < reg_len; ob++)
    trywear(reg[ob], False);
}





