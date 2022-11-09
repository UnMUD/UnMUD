#include <unistd.h>
#include "kernel.h"
#include "locations.h"
#include "objects.h"
#include "mobiles.h"
#include "stdinc.h"

#ifndef R_OK
#define R_OK 4
#endif

extern char *WizLevels[];
extern char *MLevels[];
extern char *FLevels[];


/** Rope code, by Rhadamanthus (7/1/95) **/
void untiecom(void)
{
  int o, ob;
  if((o = ob1) == -1 || state(o) != 0) {
    bprintf("There's no rope here.\n");
    return;
  }
if (ststflg(mynum, SFL_GHOST)) {
 bprintf("Your ghostly fingers pass right through solid objects!\n");
 return; }
  switch(o) {
  case OBJ_TREEHOUSE_LADDER:
  case OBJ_VALLEY_LADDER2ELF:
    if(pstr(MOB_TREEHOUSE_ELF + max_players) >= 0) {
      bprintf("The Elf shouts 'Hey, leave my rope alone!'\n");
      return;
    }
    ob = clone_object(OBJ_LIMBO_ROPE, -1, NULL);
    if(ob >= 0) {
      bprintf("You untie the rope.\n");
      setobjstate(OBJ_VALLEY_LADDER2ELF, 1);
      send_msg(ploc(mynum), MODE_NSFLAG|MS(SFL_BLIND), LVL_MIN,
	       LVL_MAX, mynum, NOBODY,
	       "\001p%s\003 unties the rope.\n", pname(mynum));
      setoloc(ob, ploc(mynum), IN_ROOM);
      return;
    }
    break;
  case OBJ_VALLEY_ROPEEAST:
  case OBJ_LEDGE_ROPEWEST:
    ob = clone_object(OBJ_LIMBO_ROPE, -1, NULL);
    if(ob >= 0) {
      bprintf("You untie the rope.\n");
      setobjstate(o, 1);
      osetbit(OBJ_VALLEY_ROPEEAST, OFL_DESTROYED);
      osetbit(OBJ_LEDGE_ROPEWEST, OFL_DESTROYED);
      setexit(LOC_VALLEY_STEPS, 1, 0);
      setexit(LOC_LEDGE_1, 3, 0);
      send_msg(ploc(mynum), MODE_NSFLAG|MS(SFL_BLIND), LVL_MIN,
	       LVL_MAX, mynum, NOBODY,
	       "\001p%s\003 unties the rope.\n", pname(mynum));
      setoloc(ob, ploc(mynum), IN_ROOM);
      return;
    }
    break;
  case OBJ_LEDGE_ROPEBOTPIT:
  case OBJ_MOOR_ROPETOPPIT:
    ob = clone_object(OBJ_LIMBO_ROPE, -1, NULL);
    if(ob >= 0) {
      bprintf("You untie the rope.\n");
      setobjstate(o, 1);
      osetbit(OBJ_LEDGE_ROPEBOTPIT, OFL_DESTROYED);
      osetbit(OBJ_MOOR_ROPETOPPIT, OFL_DESTROYED);
      setexit(LOC_MOOR_3, 5, 0);
      setexit(LOC_LEDGE_8, 4, 0);
      send_msg(ploc(mynum), MODE_NSFLAG|MS(SFL_BLIND), LVL_MIN,
	       LVL_MAX, mynum, NOBODY,
	       "\001p%s\003 unties the rope.\n", pname(mynum));
      setoloc(ob, ploc(mynum), IN_ROOM);
      return;
    }
    break;
  case OBJ_ABYSS_L664ROPE:
  case OBJ_ABYSS_L665ROPE:
    ob = clone_object(OBJ_LIMBO_ROPE, -1, NULL);
    if(ob >= 0) {
      bprintf("You untie the rope.\n");
      setobjstate(o, 1);
      send_msg(ploc(mynum), MODE_NSFLAG|MS(SFL_BLIND), LVL_MIN,
	       LVL_MAX, mynum, NOBODY,
	       "\001p%s\003 unties the rope.\n", pname(mynum));
      setoloc(ob, ploc(mynum), IN_ROOM);
      return;
    }
    break;
  case OBJ_NARNIA_ASLAN:
    if (otstbit(OBJ_NARNIA_ASLAN, OFL_DESTROYED))
      {
	bprintf("Aslan is not here.\n");
	return;
      }
    if (alive(max_players + MOB_NARNIA_FENRIS) != -1)
      {
        if (ploc(max_players + MOB_NARNIA_FENRIS) == LOC_NARNIA_TABLE)
  	  {
	    bprintf("Fenris Ulf growls mercilessly, and manages to keep you "
                    "just out of reach of\nAslan's bindings.\n");
	    return;
	  }
        else
          {
	    bprintf("You untie Aslan, but his poor wretched body lies still, "
                    "no life left in it.\n");
            osetbit(OBJ_NARNIA_ASLAN, OFL_DESTROYED);
            return;
	  }
      }
    else
      {
        bprintf("You untether Aslan gently and wait for some reaction.\n");
        bprintf("Suddenly Aslan's fur grows back, and he jumps to his feet.\n");
        bprintf("Aslan thanks you for your help and hands you a silver shield.\n");
        bprintf("Aslan says &+C'This shield belongs to the great King Peter of Narnia.&*\n");
        bprintf("&+CPlease return it to him.'&*\n");
        setoloc(OBJ_NARNIA_PETERSHIELD, mynum, CARRIED_BY);
        bprintf("With those words, Aslan takes off running.  You feel suddenly sad that\n");
        bprintf("you will probably never see him again.\n");
        destroy(OBJ_NARNIA_ASLAN);
        break;
      }
  default:
    bprintf("--There's no rope here.\n");
    break;
  }
}

void tiecom(void)
{
  int o;
  if(onum(o = ob1) != OBJ_LIMBO_ROPE && onum(o) != OBJ_ABYSS_ROPE) {
    bprintf("You can't tie that!\n");
    return;
  }
if (ststflg(mynum, SFL_GHOST)) {
 bprintf("Your ghostly fingers pass right through solid objects!\n");
 return; }
  switch(ploc(mynum)) {
  case LOC_ABYSS_L664:
  case LOC_ABYSS_L665:
    setobjstate(OBJ_ABYSS_L664ROPE, 0);
    bprintf("You tie the rope over the gap.\n");
    destroy(o);
    send_msg(ploc(mynum), MODE_NSFLAG|MS(SFL_BLIND), LVL_MIN, LVL_MAX,
	     mynum, NOBODY, "\001p%s\003 ties the rope over the gap.\n",
	     pname(mynum));
    break;
  case LOC_VALLEY_ESIDE:
  case LOC_TREEHOUSE_1:
    setobjstate(OBJ_TREEHOUSE_LADDER, 0);
    bprintf("You tie the rope to the tree.\n");
    destroy(o);
    send_msg(ploc(mynum), MODE_NSFLAG|MS(SFL_BLIND), LVL_MIN, LVL_MAX,
	     mynum, NOBODY, "\001p%s\003 ties the rope to the tree.\n",
	     pname(mynum));
    break;
  case LOC_VALLEY_STEPS:
  case LOC_LEDGE_1:
    setobjstate(OBJ_VALLEY_ROPEEAST, 0);
    oclrbit(OBJ_VALLEY_ROPEEAST, OFL_DESTROYED);
    oclrbit(OBJ_LEDGE_ROPEWEST, OFL_DESTROYED);
    setexit(LOC_VALLEY_STEPS, 1, LOC_LEDGE_1);
    setexit(LOC_LEDGE_1, 3, LOC_VALLEY_STEPS);
    bprintf("You tie the rope over the gap.\n");
    destroy(o);
    send_msg(ploc(mynum), MODE_NSFLAG|MS(SFL_BLIND), LVL_MIN, LVL_MAX,
	     mynum, NOBODY, "\001p%s\003 ties the rope over the gap.\n",
	     pname(mynum));
    break;
  case LOC_LEDGE_8:
  case LOC_MOOR_3:
    setobjstate(OBJ_MOOR_ROPETOPPIT, 0);
    oclrbit(OBJ_MOOR_ROPETOPPIT, OFL_DESTROYED);
    oclrbit(OBJ_LEDGE_ROPEBOTPIT, OFL_DESTROYED);
    setexit(LOC_MOOR_3, 5, LOC_LEDGE_8);
    setexit(LOC_LEDGE_8, 4, LOC_MOOR_3);
    bprintf("You tie the rope to the pit.\n");
    destroy(o);
    send_msg(ploc(mynum), MODE_NSFLAG|MS(SFL_BLIND), LVL_MIN, LVL_MAX,
	     mynum, NOBODY, "\001p%s\003 ties the rope over the gap.\n",
	     pname(mynum));
    break;
  default:
    bprintf("You have nothing to tie it to.\n");
    break;
  }
}

void throwcom (void)
{
  if (!iscarrby(ob1, mynum))
    {
      bprintf("You don't have it.\n");
      return;
    }  
  if (ob2 == OBJ_LIMBO_ROPE || ob1 == OBJ_ABYSS_ROPE)
    {
      if (ploc(mynum) == LOC_LEDGE_1)
	{
	  bprintf("The rope fails to catch on the ledge.\n");
	  return;
	}
      if (ploc(mynum) == LOC_VALLEY_STEPS)
	{
	  if (state(OBJ_VALLEY_ROPEEAST) == 1)
	    {
	      bprintf("The rope catches on the other side of the ledge and seems secure.\n");
	      destroy(ob1);
	      setobjstate(OBJ_VALLEY_ROPEEAST, 0);
	      return;
	    }
	  else
	    {
	      bprintf("Why do you want to do that?  There is already a rope here.\n");
	      return;
	    }
	}
      if (ploc(mynum) == LOC_LEDGE_8 || ploc(mynum) == LOC_MOOR_3)
	{
	  if (state(OBJ_LEDGE_ROPEBOTPIT) == 1)
	    {
	      if (ploc(mynum) == LOC_LEDGE_8)
		bprintf("The rope catches high on the lip of the pit.\n");
	      else
		bprintf("The rope now leads down the pit.\n");
	      destroy(ob1);
	      setobjstate(OBJ_LEDGE_ROPEBOTPIT, 0);
	      return;
	    }
	  else
	    {
	      bprintf("Why do you want to do that?  There is already a rope here.\n");
	      return;
	    }
	}
    }
#ifdef LOCMIN_NIBELUNG
  if ((ob1 == OBJ_NIBELUNG_ARROW)
      || (ob1 == OBJ_NIBELUNG_ARROW2)
      || (ob1 == OBJ_NIBELUNG_ARROW3))
    {
      if (ploc(mynum) == oloc(OBJ_NIBELUNG_HOLE))
	{
	  if (   ((state(OBJ_NIBELUNG_FIRE) == 1) && (ob1 == OBJ_NIBELUNG_ARROW))
	      || ((state(OBJ_NIBELUNG_FIRE) == 2) && (ob1 == OBJ_NIBELUNG_ARROW2))
	      || ((state(OBJ_NIBELUNG_FIRE) == 3) && (ob1 == OBJ_NIBELUNG_ARROW3)) )
	    {
	      bprintf("The arrow arcs cleanly through the air into the hole.\n");
	      sillycom("\001s%s\002%s throws the arrow into the hole.\n\003");
	      send_msg(ploc(mynum), 0, LVL_MIN, LVL_MAX, NOBODY, NOBODY,
		       "Suddenly a door opens in the south wall.\n");
	      setobjstate(OBJ_NIBELUNG_HOLE, 0);
	    }
	  else
	    {
	      bprintf("The arrow crumbles to dust as it encounters the wall.\n");
	      sillycom("\001s%s\002%s throws the arrow at the wall, breaking it."
		       "\n\003");
	      destroy(ob1);
	    }
	  return;
	}
      else
	{
	  bprintf("You shoot the arrow into the air, it comes to earth you know not where.\n");
	  sillycom("\001s%s\002%s throws an arrow into the air. It sails away."
		   "\n\003");
	  destroy(ob1);
	  return;
	}
    }
#endif
  bprintf ("You can't throw that!\n");
}

void 
lookcom (void)
{
  int a;
  
  if (brkword() == -1)
    {
      lookin(ploc(mynum), SHOW_LONG);
    }
  else 
    {
      if (EQ(wordbuf, "at"))
	{
	  strcpy(item1, item2);
	  ob1 = ob2;
	  examcom();
	}
      else if (!EQ(wordbuf, "in") && !EQ(wordbuf, "into"))
	{
	  bprintf("I don't understand.  Are you trying to LOOK, LOOK AT, or LOOK IN something?\n");
	}
      else if (EMPTY(item2))
	{
	  bprintf("In what?\n");
	}
      else if ((a = ob2) == -1)
	{
	  bprintf("What?\n");
	}
      else if (a == OBJ_WINDOW_TUBE ||
	       a == OBJ_TREEHOUSE_BEDDING ||
	       a == OBJ_ORCHOLD_TOPCOVER)
	{
	  strcpy(item1, item2);
	  ob1 = ob2;
	  examcom();
	}
      else if (!otstbit(a, OFL_CONTAINER))
	{
	  bprintf("That isn't a container.\n");
	}
      else if (otstbit(a, OFL_OPENABLE) && state(a) != 0)
	{
	  bprintf("It's closed.\n");
	}
#ifdef LOCMIN_NIBELUNG
      else if ((a == OBJ_NIBELUNG_COFFIN) &&
	       (alive(max_players+MOB_NIBELUNG_VAMPIRE) != -1))
	{
	  bprintf("A Vampire leaps out of the coffin onto you!\n");
	  sillycom("\001s%s\003A Vampire leaps out of the coffin "
		   "onto %s!\n\003");
	  setploc(max_players+MOB_NIBELUNG_VAMPIRE, ploc(mynum));
	  hit_player(max_players+MOB_NIBELUNG_VAMPIRE, mynum, -1);
	}
#endif
      else
	{
	  bprintf ("The %s contains:\n", oname (a));
	  aobjsat (a, IN_CONTAINER, 8);
	}
    }
}

void wherecom ()
{
  int cha, rnd, num_obj_found = 0, num_chars_found = 0;
  
  if (plev (mynum) < LVL_APPREN && pstr (mynum) < 10)
    {
      bprintf ("You're too weak to cast any spells.\n");
      return;
    }
  if (ltstflg (ploc (mynum), LFL_NO_MAGIC) && plev (mynum) < LVL_APPREN)
    {
      bprintf ("Something about this location has drained your mana.\n");
      return;
    }
  if (plev (mynum) < LVL_APPREN)
    setpstr (mynum, pstr (mynum) - 2);
  rnd = randperc ();
  cha = 6 * plev (mynum);
  if (carries_obj_type (mynum, OBJ_WINDOW_POWERSTONE) > -1 ||
      carries_obj_type (mynum, OBJ_WINDOW_POWERSTONE1) > -1 ||
      carries_obj_type (mynum, OBJ_WINDOW_POWERSTONE2) > -1 ||
      plev (mynum) >= LVL_SIXTEEN)
    cha = 100;
  if (rnd > cha)
    {
      bprintf ("Your spell fails.\n");
      return;
    }
  
  if (!item1[0])
    {
      bprintf ("What's that?\n");
      return;
    }
  
  for (cha = 0; cha < numobs; cha++)
    {
      
      if (cha == num_const_obs && plev (mynum) < LVL_APPREN)
	break;
      
      if (EQ (oname (cha), item1)
	  || (plev (mynum) >= LVL_APPREN && EQ (oaltname (cha), item1)))
	{
	  num_obj_found++;
	  if (plev (mynum) >= LVL_APPREN)
	    bprintf ("[%3d] ", cha);
	  bprintf ("%16.16s - ", oname (cha));
	  if (plev (mynum) < LVL_APPREN && ospare (cha) == -1)
	    bprintf ("Nowhere.\n");
	  else if (otstbit(cha, OFL_NOLOCATE) && plev(mynum) < LVL_APPREN)
		bprintf("Unknown...\n");
	  else
	    desloc (oloc (cha), ocarrf (cha));
	}
    }
  
  for (cha = 0; cha < numchars; cha++)
    {
      
      if (cha == num_const_chars && plev (mynum) < LVL_APPREN)
	break;
      
      if (EQ (xname (pname (cha)), item1))
	{
   	  if (pvis(mynum) >= pvis(cha))
	    {	  
	       num_chars_found++;
	  
	       if (plev (mynum) >= LVL_APPREN)
	          bprintf ("[%d]", GLOBAL_MAX_OBJS + cha);
	  
	       bprintf ("%16.16s - ", pname (cha));
	       desloc (ploc (cha), 0);
	    }
	}
    }
  
  
  if (num_obj_found == 0 && num_chars_found == 0)
    bprintf ("I don't know what that is.\n");
}

static Boolean find_stuff (int s, int o, char *t)
{
  if (odamage (s) == 0)
    {
      osetdamage (s, 1);
      bprintf (t);
      create (o);
      setoloc (o, ploc (mynum), IN_ROOM);
      return True;
    }
  return False;
}

void examcom (void)
{
  int a, foo, z;
  FILE *x;
  char ch;
  char *t;
  char text[80];
  
  if (!item1[0])
    {
      bprintf ("Examine what?\n");
      return;
    }
  
  if ((a = fpbn (item1)) != -1 && ploc (a) == ploc (mynum))
    {				/*220692 */
      if (pstr (a) < 1)
	{
	  bprintf ("You see the worm-infested corpse of %s.\n", pname (a));
	  return;
	}
if (pstr (a) < 1 && a < max_players && ststflg(a, SFL_GHOST)) {
        bprintf ("You see the fading ghost of %s.\n", pname(a));
        return; }
      if (a >= max_players && (t = pexam (a)) != NULL)
	{
	  bprintf ("%s\n", t);
	}
      else
	{
	  
	  if (a != mynum)
	    {
	      sendf (a, "%s examines you closely.\n", pname (mynum));
	    }
	  
	  sprintf (text, "%s/%s", DESC_DIR, pname (a));
	  if ((x = fopen (text, "r")) == NULL)
	    {			/*ACCESS! */
	      bprintf ("A typical, run of the mill %s.\n", pname (a));
	      return;
	    }
	  fclose (x);
	  bprintf ("\001f%s\003", text);
	}
      return;
    }
  
  if ((a = ob1) == -1)
    {
      bprintf ("You see nothing special.\n");
      return;
    }
  
  if (isa_board (a))
    {
      lat_board (a);
      return;
    }
  
  send_msg (ploc (mynum), 0, pvis (mynum), LVL_MAX, mynum, NOBODY,
	    "%s examines the %s closely.\n", pname (mynum), oname (a));
  
  switch (onum (a))
    {
      
#ifdef LOCMIN_FAIRYBOOK
    case OBJ_FAIRYBOOK_TRAP_DOOR:
      bprintf("You look at the trap door and try to open it.  As you heave "
	      "you break\nthe door open and fall through.\n");
      setploc(mynum, LOC_FAIRYTALE_FAIRYTALE99);
      lookin(ploc(mynum), 0);
      return;
      break;
#endif
    
#ifdef LOCMIN_MONOPOLY
  case OBJ_VILLAGE_GAMEBOARD:
    bprintf("As you examine the board you realize it is a game of Monopoly!\n");
    bprintf("You feel yourself being sucked into the game.\n");
    setploc(mynum, LOC_MONOPOLY_GO);
    lookin(ploc(mynum), 0);
    return;
    break;
#endif

#ifdef LOCMIN_ISLAND
    case OBJ_ISLAND_BONE:
      bprintf("There is a flash and you are teleported...\n");
      setploc(mynum, LOC_LABYRINTH_K);
      lookin(ploc(mynum), 0);
      return;
      break;
#endif
      
#ifdef LOCMIN_KASTLE
    case OBJ_KASTLE_LEDGE:
      if (find_stuff (OBJ_KASTLE_LEDGE, OBJ_KASTLE_NEST_EGG,
		      "Feeling along the ledge you find a small nest with an egg in it.\n"))
	return;
      break;
#endif
     
#ifdef LOCMIN_JBS /* code written by Rendor(ken@esu.edu) Nov 1993 */
  case OBJ_JBS_TABLE:
    if( find_stuff(OBJ_JBS_TABLE,OBJ_JBS_STEAKNIFE,
                "You find a knife taped to the bottom of the table!\n") )
      return;
    break;
  case OBJ_JBS_BAIL:
    if( find_stuff(OBJ_JBS_BAIL,OBJ_JBS_KEY,"You find a key in the hay!\n") )
      return;
    break;
#endif
 
#ifdef LOCMIN_DOOM
    case OBJ_DOOM_BOOK:
      if (ploc(mynum) == LOC_DOOM_14)
        {
	  bprintf("As you look at the book, a strange feeling comes over you.\n"
		  "You feel your surroundings change to a darker place.\n");
	  trapch(LOC_DOOM_15);
	  return;
	  break;
        }
#endif
      
#ifdef LOCMIN_ABYSS
    case OBJ_ABYSS_HUSKS:
      if (find_stuff (OBJ_ABYSS_HUSKS, OBJ_ABYSS_RING,
		      "Under one of the husks you find a ring.\n"))
	return;
      break;
      
    case OBJ_ABYSS_MAT:
      if (find_stuff (OBJ_ABYSS_MAT, OBJ_ABYSS_CARD2,
		      "Under the mat you find a plastic card.\n"))
	return;
      break;
#endif
      
#ifdef LOCMIN_ANCIENT
      
    case OBJ_ANCIENT_SKYMAP:
      if (ploc (mynum) == LOC_ANCIENT_ANC53)
	{
	  bprintf ("You feel like you are flying....\n"
		   "     ....and land on a desolate planet,"
		   "obviously far from the sun.\n\n\n");
	  trapch (LOC_ANCIENT_ANC56);
	}
      else
	{
	  bprintf ("You ain't lost, Mac... yet!\n");
	}
      break;
      
    case OBJ_ANCIENT_LBOOK:
      if (oarmor (a) > 0)
	{
	  bprintf ("The Book contains old writings. You can make out\n"
		   "the names of old mages like 'Elmo' and 'Infidel'.\n");
	}
      else
	{
	  bprintf ("Ancient wisdom fills your head, you feel much more capable...\n");
	  osetarmor (a, 1);
	}
      break;
#endif
      
#ifdef LOCMIN_NIBELUNG /* Zone given by Alhana, Budapest, thanks */
    case OBJ_NIBELUNG_COFFIN:
      if ((state(a) == 0) && (alive(max_players+MOB_NIBELUNG_VAMPIRE) != -1))
	{
	  bprintf("A Vampire leaps out os the coffin onto you!\n");
	  sprintf(text,"A Vampire leaps out of the coffin onto %s!\n",pname(mynum));
	  sillycom(text);
	  setploc(max_players+MOB_NIBELUNG_VAMPIRE, ploc(mynum));
	  hit_player(max_players+MOB_NIBELUNG_VAMPIRE, mynum, -1);
	  return;
	}
      
    case OBJ_NIBELUNG_FIRE:
      if (iscarrby(OBJ_NIBELUNG_FIRESTONE, mynum)
	  && (state(OBJ_NIBELUNG_FIRE) == 0))
	{
	  setobjstate(OBJ_NIBELUNG_FIRE, randperc() % 3 + 1);
	  switch (state(OBJ_NIBELUNG_FIRE))
	    {
	    case 1:
	      bprintf("The fire appears a muddy brown through the firestone.\n");
	      break;
	    case 2:
	      bprintf("The fire burns with a purple incandescence.\n");
	      break;
	    case 3:
	      bprintf("A mauve haze tints the edges of the flames.\n");
	      break;
	    }
	}
      else
	bprintf("You see nothing special.\n");
      return;
      break;
#endif
      
#ifdef LOCMIN_EFOREST
    case OBJ_EFOREST_TREE:
      if (odamage (a) == 0)
	{
	  osetdamage (a, 1);
	  bprintf ("You find a door in the tree.\n");
	  setobjstate (a, 0);
	  return;
	}
      break;
    case OBJ_EFOREST_DESK:
      if (find_stuff (OBJ_EFOREST_DESK, OBJ_EFOREST_BUTTON,
		      "You find a button hidden in a recess of the desk.\n"))
	return;
      break;
    case OBJ_EFOREST_THRONE_WOOD:
      if (find_stuff (OBJ_EFOREST_THRONE_WOOD, OBJ_EFOREST_CROWN,
		      "You find a crown hidden under the throne.\n"))
	return;
      break;
    case OBJ_EFOREST_COAT:
      if (find_stuff (OBJ_EFOREST_COAT, OBJ_EFOREST_COIN,
		      "You find a coin in the pocket.\n"))
	return;
      break;
    case OBJ_EFOREST_TABLE:
      if (find_stuff (OBJ_EFOREST_TABLE, OBJ_EFOREST_GAUNTLETS,
		      "You find some gauntlets admidst the rubble "
		      "of this table.\n"))
	return;
      break;
    case OBJ_EFOREST_PAINTING:
      if (find_stuff (OBJ_EFOREST_PAINTING, OBJ_EFOREST_NOTE,
		      "There was a note behind the painting.\n"))
	return;
      break;
    case OBJ_EFOREST_CHAIR:
      if (find_stuff (OBJ_EFOREST_CHAIR, OBJ_EFOREST_RING,
		      "A ring was between the cushions of that chair!\n"))
	return;
      break;
    case OBJ_EFOREST_RACK:
      if (find_stuff (OBJ_EFOREST_RACK, OBJ_EFOREST_SCARAB,
		      "You found a scarab in the rack.\n"))
	return;
      break;
    case OBJ_EFOREST_PAPERS:
      if (find_stuff (OBJ_EFOREST_PAPERS, OBJ_EFOREST_TREATY,
		      "Among the papers, you find a treaty.\n"))
	return;
      break;
    case OBJ_EFOREST_DESK_LICH:
      if (find_stuff (OBJ_EFOREST_DESK_LICH, OBJ_EFOREST_EMERALD,
		      "Inside the desk is a beautiful emerald.\n"))
	return;
      break;
    case OBJ_EFOREST_ALTAR:
      if (find_stuff (OBJ_EFOREST_ALTAR, OBJ_EFOREST_STATUE,
		      "Inside the altar is a statue of a dark elven deity.\n"))
	return;
      break;
    case OBJ_EFOREST_MATTRESS:
      if (find_stuff (OBJ_EFOREST_MATTRESS, OBJ_EFOREST_PURSE,
		      "Hidden under the mattress is a purse.\n"))
	return;
      break;
    case OBJ_EFOREST_TRASH:
      if (find_stuff (OBJ_EFOREST_TRASH, OBJ_EFOREST_TRASH_COIN,
		      "In the trash is a silver coin.\n"))
	return;
      break;
#endif
      
#ifdef LOCMIN_WINDOW
    case OBJ_WINDOW_TUBE:
      if (oarmor (a) == 0)
	{
	  int obj = clone_object (OBJ_WINDOW_SCROLL, -1, NULL);
	  
	  if (obj >= 0)
	    {
	      osetarmor (a, 1);
	      bprintf ("You take a scroll from the tube.\n");
	      setoloc (obj, mynum, CARRIED_BY);
	      return;
	    }
	}
      break;
      
    case OBJ_WINDOW_SCROLL:
      if (iscarrby (OBJ_CATACOMB_CUPSERAPH, mynum))
	{
	  bprintf ("Funny, I thought this was a teleport scroll, but "
		   "nothing happened.\n");
	  return;
	}
      bprintf ("As you read the scroll you are teleported!\n");
      destroy (a);
      teletrap (LOC_WINDOW_PENTACLE);
      return;
      
    case OBJ_WINDOW_BLACKROBE:
      if (oarmor (a) == 0)
	{
	  int obj = clone_object (OBJ_WINDOW_KEY, -1, NULL);
	  if (obj >= 0)
	    {
	      bprintf ("You take a key from one pocket.\n");
	      osetarmor (a, 1);
	      setoloc (obj, mynum, CARRIED_BY);
	      return;
	    }
	}
      break;
#endif
      
    case OBJ_TOWER_WAND:
      if (oarmor (a) != 0)
	{
	  bprintf ("It seems to be charged.\n");
	  return;
	}
      break;
      
    case OBJ_WINDOW_BALL:
      setobjstate (a, randperc () % 3 + 1);
      switch (state (a))
	{
	case 1:
	  bprintf ("It glows red.");
	  break;
	case 2:
	  bprintf ("It glows blue.");
	  break;
	case 3:
	  bprintf ("It glows green.");
	  break;
	}
      bprintf ("\n");
      return;
      
    case OBJ_TOWER_SCROLL:
      foo = carries_obj_type (mynum, OBJ_WINDOW_BALL);
      if (foo < 0)
	foo = OBJ_WINDOW_BALL;
      
      if (((z = carries_obj_type (mynum, OBJ_TOWER_RED_CANDLE)) > -1 &&
	   state (foo) == 1 &&
	   otstbit (z, OFL_LIT)) ||
	  
	  ((z = carries_obj_type (mynum, OBJ_TOWER_BLUE_CANDLE)) > -1 &&
	   state (foo) == 2 &&
	   otstbit (z, OFL_LIT)) ||
	  
	  ((z = carries_obj_type (mynum, OBJ_TOWER_GREEN_CANDLE)) > -1 &&
	   state (foo) == 3 &&
	   otstbit (z, OFL_LIT)))
	{
	  
	  bprintf ("Everything shimmers and then solidifies into a different "
		   "view!\n");
	  destroy (a);
	  teletrap (LOC_TOWER_POTION);
	  return;
	}
      break;
      
    case OBJ_VALLEY_BED:
      if (!odamage (a))
	{
	  int c = clone_object (OBJ_VALLEY_LOAF, -1, NULL);
	  int b = clone_object (OBJ_VALLEY_PIE, -1, NULL);
	  
	  if (c >= 0 && b >= 0)
	    {
	      bprintf ("Aha!  Under the bed you find a loaf and a "
		       "rabbit pie.\n");
	      setoloc (c, ploc (mynum), IN_ROOM);
	      setoloc (b, ploc (mynum), IN_ROOM);
	      osetdamage (a, 1);
	      return;
	    }
	}
      break;
      
#ifdef LOCMIN_ORCHOLD
    case OBJ_ORCHOLD_GARBAGE:
      if (state (a) == 0)
	{
	  
	  int x = clone_mobile (max_players + MOB_ORCHOLD_MAGGOT, -1, NULL);
	  
	  if (x >= 0)
	    {
	      
	      bprintf ("In the garbage you find a gold plate... "
		       "with a maggot on it!\n");
	      
	      sillycom ("\001s%s\003A maggot leaps out of the garbage "
			"onto %s!\n\003");
	      
	      setploc (x, ploc (mynum));
	      
	      /* Make sure maggot attacks the right person. */
	      hit_player (x, mynum, -1);
	      
	      setobjstate (a, 1);
	    }
	  else
	    {
	      bprintf ("In the garbage you find a gold plate with some "
		       "slime on it.\n");
	    }
	  
	  if ((x = clone_object (OBJ_ORCHOLD_ORCGOLD, -1, NULL)) > -1)
	    {
	      
	      setoloc (x, ploc (mynum), IN_ROOM);
	      setobjstate (a, 1);
	    }
	  
	  return;
	}
      break;
#endif
      
    case OBJ_TREEHOUSE_BEDDING:
      if (a != OBJ_TREEHOUSE_BEDDING)
	break;
      
      if (!odamage (OBJ_TREEHOUSE_AMULET))
	{
	  create (OBJ_TREEHOUSE_AMULET);
	  bprintf ("You pull an amulet from the bedding.\n");
	  osetdamage (OBJ_TREEHOUSE_AMULET, 1);
	  return;
	}
      break;
      
    case OBJ_OAKTREE_BOARSKIN:
      if (a != OBJ_OAKTREE_BOARSKIN)
	break;
      
      if (!odamage (OBJ_OAKTREE_WHISTLE))
	{
	  create (OBJ_OAKTREE_WHISTLE);
	  bprintf ("Under the boarskin you find a silver whistle.\n");
	  osetdamage (OBJ_OAKTREE_WHISTLE, 1);
	  return;
	}
      break;
    }
  
  if (oexam_text (a) != NULL)
    {
      bprintf ("%s", oexam_text (a));
    }
  else if (oexamine (a) != 0 && (x = fopen (OBJECTS, "r")) != NULL)
    {
      
      fseek (x, oexamine (a), 0);
      while ((ch = fgetc (x)) != '^')
	bprintf ("%c", ch);
      fclose (x);
    }
  else
    {
      bprintf ("You see nothing special.\n");
      return;
    }
}

void incom (Boolean inv)
{
  int x, y;
  char st[MAX_COM_LEN];
  
  if (!ptstflg (mynum, PFL_GOTO))
    {
      /* Mortals who doesn't have goto will assume that in = inventory... */
      /* Thus hiding the fact that there IS a command called in... */
      /* Must fix it so that at this should only be the case when IN is used */
      /* and not when AT is used */
      
      if (inv)
	inventory ();
      else
	bprintf ("You must be high.\n");
      return;
    }
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD)) {
    bprintf ("This is where you are AT, Jail you are IN.. deal with it.\n");
    return;
    }
  if ((x = getroomnum ()) == 0 || !exists (x))
    {
      bprintf ("Unknown Player Or Room\n");
      return;
    }
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD)) {
    bprintf ("You aren't authorized for that room\n");
    return;
   }
#ifdef LOCMIN_RHOME
  if ((x == LOC_RHOME_15) && (!EQ(pname(mynum), "Rex")))
    {
      bprintf("I'm sorry but you just can't barge in like that.\n");
      return;
    }
#endif
  if (!checklevel(mynum, x)) return;
  getreinput (st);
  if (EQ (st, "!"))
    {
      bprintf ("What exactly do you want to do?\n");
      return;
    }
  y = ploc (mynum);
  setploc (mynum, x);
  gamecom (st, False);
  setploc (mynum, y);
}

/* Hit the reset-stone. */
void sys_reset (void)
{
  int i;
  
  for (i = 0; i < max_players; i++)
    {
      if (is_in_game (i) && plev (i) < LVL_APPREN && ploc (i) != ploc (mynum))
	{
	  bprintf ("There are other players on who are not at this location,"
		   " so it won't work.\n");
	  return;
	}
    }
  
  if (last_reset < global_clock && global_clock < last_reset + 3600)
    {
      bprintf ("Sorry, at least an hour must pass between resets.\n");
      return;
    }
  resetcom (0);
}


/* The RESET command. */
void resetcom (int flags)
{
       register int i;
      extern CALENDAR calendar;

  if ((flags & RES_TEST) != 0 && !ptstflg (mynum, PFL_RESET))
    {
      erreval ();
      return;
    }
   if (ltstflg(ploc(mynum), LFL_JAIL)) {
     bprintf ("You can't reset from here!\n");
     return;
   }
  if (brkword () != -1 && plev (mynum) >= LVL_APPREN)
    {
      int zone, r_locs, r_mobs, r_objs, nlocs, nmobs, nobjs, d_locs, d_mobs,
      d_objs;
      
      if ((zone = get_zone_by_name (wordbuf)) == -1)
	{
	  
	  bprintf ("No such zone: %s\n", wordbuf);
	  return;
	}
      
      if (EQ(zname(zone),"eforest"))
	questsdone[Q_EFOREST] = False;
      else if (EQ(zname(zone),"tower"))
	questsdone[Q_INVISWAND] = False;
      else if (EQ(zname(zone),"sea"))
	questsdone[Q_EXCALIBUR] = False;
      else if (EQ(zname(zone),"catacomb"))
	questsdone[Q_GRAIL] = False;
      else if (EQ(zname(zone),"oaktree"))
	questsdone[Q_BUST] = False;
      else if (EQ(zname(zone),"oaktree"))
	questsdone[Q_SPIKE] = False;
      else if (EQ(zname(zone),"waste"))
	questsdone[Q_FIERY_KING] = False;
      
      nlocs = znumloc (zone);
      nobjs = znumobs (zone);
      nmobs = znumchars (zone);
      
      reset_zone (zone, &global_clock, &d_locs, &d_mobs, &d_objs,
		  &r_locs, &r_mobs, &r_objs);
      
      if (!ztemporary (zone))
	{
#ifdef LOG_RESET
	  mudlog ("RESET (zone %s) by %s", zname (zone), pname (mynum));
#endif
	  
	  bprintf ("Zone %s: Reset Completed.\n", zname (zone));
	  
	  bprintf ("\n%3d (from %d) "
		   "Room(s) were successfully reset.\n",
		   r_locs, nlocs - d_locs);
	  
	  bprintf ("%3d (from %d) "
		   "Object(s) were successfully reset.\n",
		   r_objs, nobjs - d_objs);
	  
	  bprintf ("%3d (from %d) "
		   "Mobile(s) were successfully reset.\n",
		   r_mobs, nmobs - d_mobs);
	}
      else
	{
#ifdef LOG_RESET
	  mudlog ("RESET (tempzone %s) by %s", zname (zone), pname (mynum));
#endif
	  
	  bprintf ("Zone %s:\n\n"
		   "%d Room(s) destroyed.\n"
		   "%d Objects(s) destroyed.\n"
		   "%d Mobiles(s) destroyed.\n",
		   zname (zone),
		   nlocs, nobjs, nmobs);
	}
    }
  else
    {
     if (the_world->w_delayed == 1) {
      sendf(DEST_ALL, "Delayed Reboot Activated.\n");
      mudlog("DELAYED REBOOT ACTIVATED");
      sysreboot(True);
      the_world->w_delayed == 0;
      return; }
      last_reset = global_clock;
      broad ("\001A\033[31m\003&+RReset in progress....&*\n");
      
#ifdef LOG_RESET
      mudlog ("RESET (all zones) by %s", pname (mynum));
#endif
      
      calendar.daytime = 0;
      
      for (i = 0; i <= LAST_QUEST; i++)
	questsdone[i] = False;
      
      for (i = 0; i < numzon; i++)
	{
	  
	  reset_zone (i, &global_clock,
		      NULL, NULL, NULL, NULL, NULL, NULL);
	}
      
      broad ("&+RReset Completed....&*\001A\033[0;37m\003\n");
    }
}

void praycom (void)
{
/* ghost code. -Dragorn */
if (ploc(mynum) == LOC_START_CHURCH && ststflg(mynum, SFL_GHOST)) {
 sclrflg(mynum, SFL_GHOST);
 cur_player->ghostcounter = 0;
 bprintf("As you pray, you feel your body solidify and your strength return.\n ");
   setpscore (mynum, pscore(mynum) + pscore(mynum) / 2);
   calib_player(mynum);

 setpstr(mynum, 40);
/* again, to cover rainbow's spell points. */
#ifdef setpspellpts
 setpspellpts(mynum, pmaxspellpts(plev(mynum)));
#endif

   strcpy (cur_player->setin, cur_player->ghostsetin);
   strcpy (cur_player->setout, cur_player->ghostsetout);
/* Again, in newer dyrt versions, need to use setstand and setsit */
/*   strcpy (cur_player->sethere, cur_player->ghostsethere); */
   strcpy (cur_player->setstand, cur_player->ghostsetstand);
   strcpy (cur_player->setsit, cur_player->ghostsetsit);
   return;
}

  if (ploc(mynum) == oloc(OBJ_MOOR_ALTAR))
    if (state(OBJ_MOOR_ALTAR))
      {
	bprintf("Some mighty force hurls you across the universe.\n"
		"The stars seem to spin and whirl around you, whipping into a fiery storm of\n"
		"multihued flaming light, spinning, twisting, half blinding you in its wild\n"
		"crazy flickerings.\n"
		"Suddenly, everything seems to jolt back to a kind of almost sanity.\n");
	trapch(LOC_WASTE_BEFORE);
	if (iscarrby(OBJ_ICECAVE_DAGGER, mynum))
	  {
	    bprintf("The icy dagger melts under the extreme heat.\n");
	    destroy(OBJ_ICECAVE_DAGGER);
	  }
	setobjstate(OBJ_MOOR_ALTAR, 0);
	return;
      } 
  if (loc2zone(ploc(mynum)) == loc2zone(LOCMAX_WASTE))
    {
      if (pfighting(mynum) != -1)
	{
	  bprintf("Not while fighting!\n");
	  return;
	}
      if ((my_random()%100) < 50)
	{
	  bprintf("You feel as if some force is offended.  There is a terrific lurch as if the\n"
		  "very ground is alive, and then everything clears.\n");
	  trapch(LOC_CHURCH_BEHIND);
 	}
      else
	bprintf("No one seems to hear you, perhaps later?\n");
      return;
    } 
#ifdef LOCMIN_DOOM
  if (ploc(mynum )== LOC_DOOM_15) {
    bprintf("You kneel down and pray to a less evil god.\n");
    bprintf("You feel your surroundings change.\n");
    teletrap(LOC_START_TEMPLE);
    send_msg (ploc(mynum), 0, LVL_MIN, LVL_MAX, mynum, NOBODY,
              "\001p%s\003 drops back into this dimension.\n", pname(mynum));
    return;
  }
  else 
#endif
    
#ifdef LOCMIN_MITHDAN
    if (ploc(mynum) == LOC_MITHDAN_35)
      {
	bprintf("You bow down before the Guardian Dragons...\n");
	bprintf("Pleased with your reaction, the dragons allow you to pass.\n");
	teletrap(LOC_MITHDAN_37);
	return;
      } 
#endif
  bprintf("You fall down and grovel in the dirt.\n");
  sillycom ("\001s%s\003%s falls down and grovels in the dirt.\n\003");
}


/* Brian Preble -- shows current set*in/set*out settings.
 * (Improved by Alf. Extended to include other players setins by Nicknack.)
 */
void 
reviewcom (void)
{
  char xx[SETIN_MAX + 1];
  SETIN_REC s;
  PERSONA p;
  int x;
  Boolean me = False;
  PLAYER_REC *v = NULL;
  char *in_ms, *out_ms, *min_ms, *mout_ms;
  char *vin_ms, *vout_ms, *qin_ms, *qout_ms;
  char *pro;
  char *name;
  char *sit_ms, *stand_ms;
  char *trenter_ms, *trvict_ms, *trroom_ms;
  
  if (brkword () == -1 || EQ (wordbuf, pname (mynum)))
    {
      me = True;
      v = cur_player;
      x = mynum;
    }
  else if (plev (mynum) < LVL_APPREN)
    {
      erreval ();
      return;
    }
  else if ((x = fpbns (wordbuf)) >= 0 && x < max_players &&
	   plev (x) >= LVL_APPREN)
    {
      v = players + x;
    }
  else if (!getuaf (wordbuf, &p) || p.p_level < LVL_APPREN)
    {
      bprintf ("I know no wizard with the name '%s'.\n", wordbuf);
      return;
    }
  else if (!getsetins (wordbuf, &s))
    {
      bprintf ("%s has no non-standard travel-messages.\n", p.p_name);
      return;
    }
  if (v != NULL)
    {
      in_ms = v->setin;
      out_ms = v->setout;
      min_ms = v->setmin;
      mout_ms = v->setmout;
      vin_ms = v->setvin;
      vout_ms = v->setvout;
      qin_ms = v->setqin;
      qout_ms = v->setqout;
      pro = v->prompt;
      sit_ms = v->setsit;
      stand_ms = v->setstand;
      trenter_ms = v->settrenter;
      trvict_ms = v->settrvict;
      trroom_ms = v->settrroom;
      
      name = pname (x);
      
    }
  else
    {
      in_ms = s.setin;
      out_ms = s.setout;
      min_ms = s.setmin;
      mout_ms = s.setmout;
      vin_ms = s.setvin;
      vout_ms = s.setvout;
      qin_ms = s.setqin;
      qout_ms = s.setqout;
      pro = s.prompt;
      sit_ms = s.setsit;
      stand_ms = s.setstand;
      trenter_ms = s.settrenter;
      trvict_ms = s.settrvict;
      trroom_ms = s.settrroom;
      name = p.p_name;
    }
  
  bprintf ("&+RCurrent travel messages&*");
  if (!me)
    bprintf (" &+Rfor %s&*", name);
  bprintf (":\n&+W------------------------&*\n");
  bprintf ("&+BSETIN\t\t&+W:&* %s\n", build_setin (xx, in_ms, name, NULL));
  bprintf ("&+BSETOUT\t\t&+W:&* %s\n", build_setin (xx, out_ms, name, "<dir>"));
  bprintf ("&+BSETMIN\t\t&+W:&* %s\n", build_setin (xx, min_ms, name, NULL));
  bprintf ("&+BSETMOUT\t\t&+W:&* %s\n", build_setin (xx, mout_ms, name, NULL));
  bprintf ("&+BSETVIN\t\t&+W:&* %s\n", build_setin (xx, vin_ms, name, NULL));
  bprintf ("&+BSETVOUT\t\t&+W:&* %s\n", build_setin (xx, vout_ms, name, NULL));
  bprintf ("&+BSETQIN\t\t&+W:&* %s\n", build_setin (xx, qin_ms, name, NULL));
  bprintf ("&+BSETQOUT\t\t&+W:&* %s\n", build_setin (xx, qout_ms, name, NULL));
  bprintf ("&+BSETSIT\t\t&+W:&* %s\n", build_setin (xx, sit_ms, name, NULL));
  bprintf ("&+BSETSTAND\t&+W:&* %s\n", build_setin (xx, stand_ms, name, NULL));
  bprintf ("&+BSETTRENTER\t&+W:&* %s\n", build_setin (xx, trenter_ms, "<player>", NULL));
  bprintf ("&+BSETTRVICT\t&+W:&* %s\n", build_setin (xx, trvict_ms, name, NULL));
  bprintf ("&+BSETTRROOM\t&+W:&* %s\n", build_setin (xx, trroom_ms, name, NULL));
  if (!me)
    bprintf ("&+BPROMPT\t\t&+W:&* %s\n", pro);
}

void pncom (void)
{
  int plx = real_mynum;
  char *p;
  
  if (((brkword () == -1 || (plx = fpbn (wordbuf)) == -1 ||
      plx >= max_players || plx != mynum) && plev (mynum) < LVL_APPREN))
    {
      plx = real_mynum;
    }
  if (plx == real_mynum)
    {
      bprintf ("&+YCurrent pronouns are:&*\n");
    }
  else
    {
      bprintf ("&+YCurrent pronouns for %s are:&*\n", pname (plx));
    }
  bprintf ("Me    : %s\n", pname (plx));
  bprintf ("It    : %s\n", (p = players[plx].wd_it) == NULL ? "<none>" : p);
  bprintf ("Him   : %s\n", (p = players[plx].wd_him) == NULL ? "<none>" : p);
  bprintf ("Her   : %s\n", (p = players[plx].wd_her) == NULL ? "<none>" : p);
  bprintf ("Them  : %s\n", (p = players[plx].wd_them) == NULL ? "<none>" : p);
}

/* The QUESTS-Command. Mortals may find out which quests they have
 * comleted and which are left. Arch-Wizards may in addition set or
 * clear quests for a mortal, Wizards only for themselves.
 * Usage: QUESTS <player> <questname> <true/false>
 */
void questcom (void)
{
  int a, b, c, l;
  char *n;
  Boolean f, all;
  QFLAGS q, *p;
  PERSONA d;
  
  f = False;
  
  if (brkword () == -1)
    {
      a = mynum;
      p = &(qflags (a));
      n = pname (a);
      l = plev (a);
    }
  else if ((a = fpbn (wordbuf)) != -1)
    {
      if (a != mynum && plev (mynum) < LVL_APPREN)
	{
	  bprintf ("You can only check your own Quest-stats.\n");
	  return;
	}
      p = &(qflags (a));
      n = pname (a);
      l = plev (a);
    }
  else if (!getuaf (wordbuf, &d))
    {
      bprintf ("No such persona in system.\n");
      return;
    }
  else if (plev (mynum) < LVL_APPREN)
    {
      bprintf ("You can only check your own Quest-stats.\n");
      return;
    }
  else
    {
      f = True;
      p = &(d.p_quests);
      n = d.p_name;
      l = d.p_level;
    }
  
  if (brkword () == -1)
    {
      bprintf ("\nPlayer: %s\n\n", n);
      
      all = False;
      bprintf ("&+BCompleted Quests:&*\n");
      if ((*p & Q_ALL) == Q_ALL)
	{
	  bprintf ("All!\n");
	  all = True;
	}
      else if ((*p & Q_ALL) != 0)
	{
	  show_bits ((int *) p, sizeof (QFLAGS) / sizeof (int), Quests);
	}
      else
	{
	  bprintf ("None!\n");
	}
      if (!all)
	{
	  bprintf ("\n&+BStill to do:&*\n");
	  q = (~*p & Q_ALL);
	  show_bits ((int *) &q, sizeof (QFLAGS) / sizeof (int), Quests);
	}
      return;
    }
  else if ((b = tlookup (wordbuf, Quests)) == -1)
    {
      bprintf ("%s: No such Quest.\n", wordbuf);
      return;
    }
  else if (brkword () == -1 || plev (mynum) < LVL_APPREN)
    {
      c = xtstbit (*p, b) ? 1 : 0;
      bprintf ("Value of %s is %s\n", Quests[b], TF[c]);
      return;
    }
  else if (plev (mynum) < LVL_ARCHWIZARD && !EQ (n, pname (mynum)))
    {
      bprintf ("You can't change other players Quest-stats.\n");
      return;
    }
  else if ((c = tlookup (wordbuf, TF)) == -1)
    {
      bprintf ("Value must be True or False.\n");
      return;
    }
  
  mudlog ("Quest: %s by %s, %s := %s", n, pname (mynum), Quests[b], TF[c]);
  
  if (c == 0)
    {
      xclrbit (*p, b);
    }
  else
    {
      xsetbit (*p, b);
    }
  if (f)
    {
      putuaf (&d);
    }
}

void qdonecom(void)
{
  int q;
  
  bprintf("&+RQuest information for this reset&*\n");
  bprintf("&+W--------------------------------&*\n");
  for (q = 0; q <= LAST_QUEST; q++)
    bprintf("%15s     %s\n", Quests[q], questsdone[q] ? "&+Bcompleted&*" : "&+Yyet to do!&*");
  bprintf("&+W--------------------------------&*\n");
}

/* The INFO command. */
void infocom (void)
{
  char file[100];
  char file1[110];
  
  if (brkword () == -1)
    {
      strcpy (file, INFO);
    }
  else
    {
      if (strlen(wordbuf) > 15) {
	bprintf("Filename too long.\n");
	return; }
      sprintf (file, "INFO/%s.i", lowercase (wordbuf));
      sprintf (file1, "INFO/%s.i.Z", lowercase (wordbuf));
      
      if (access (file, R_OK) < 0 && access (file1, R_OK) < 0)
	{
	  bprintf ("No info available on that topic.\n");
	  return;
	}
    }
  read_file (file, NULL, True, NULL);	/* use pager */
}

static void do_pretend (int plx)
{
  int p;
  
  if ((p = cur_player->pretend) >= 0)
    {
      /* We are someone else, unalias him */
      sclrflg (p, SFL_OCCUPIED);
      mynum = real_mynum;
      cur_player->pretend = -1;
    }
  if (plx < 0)
    {
      /* Back to ourselves */
      if (p < 0)
	return;			/* we already are ourselves */
      
      strcpy (cur_player->setin, cur_player->o_setin);
      strcpy (cur_player->setout, cur_player->o_setout);
    }
  else
    {
      /* We will pretend to be plx from now */
      mynum = plx;
      cur_player->pretend = plx;
      ssetflg (plx, SFL_OCCUPIED);
      if (p < 0)
	{
	  strcpy (cur_player->o_setin, cur_player->setin);
	  strcpy (cur_player->o_setout, cur_player->setout);
	}
      if (plx >= max_players)
	{
	  strcpy (cur_player->setin, "%n has arrived.");
	  strcpy (cur_player->setout, "%n has gone %d.");
	}
      else
	{
	  strcpy (cur_player->setin, players[plx].setin);
	  strcpy (cur_player->setout, players[plx].setout);
	}
    }
}

int find_pretender (int plx)
{
  int p;
  
  if (ststflg (plx, SFL_OCCUPIED))
    {
      for (p = 0; p < max_players; p++)
	{
	  if (players[p].pretend == plx && is_in_game (p))
	    {
	      return p;
	    }
	}
    }
  return -1;
}

void aliascom (void)
{
  if (pl1 == -1)
    {
      if (ptstflg (mynum, PFL_ALIAS) || ptstflg (mynum, PFL_ALIASP))
	{
	  bprintf ("Who?\n");
	}
      else
	{
	  erreval ();
	}
      return;
    }
  if (cur_player->polymorphed >= 0)
    {
      bprintf ("A mysterious force stops you.\n");
      return;
    }
  
  if (ststflg (pl1, SFL_OCCUPIED))
    {
      bprintf ("Already occupied!\n");
      return;
    }
  
  if (pl1 < max_players)
    {
      if (!ptstflg (mynum, PFL_ALIASP))
	{
	  bprintf ("You can't become another player!\n");
	  return;
	}
      if (!do_okay (mynum, pl1, PFL_NOALIAS))
	{
	  if (players[pl1].asmortal > 0 && plev (mynum) <= LVL_APPREN)
	    bprintf ("Already occupied!\n");
	  else
	    bprintf ("They don't want to be aliased!\n");
	  return;
	}
    }
  else if (!ptstflg (mynum, PFL_ALIAS))
    {
      bprintf ("You can't become a mobile!\n");
      return;
    }
  mudlog("%s ALIASed %s",pname(real_mynum), pname(pl1));
  do_pretend (pl1);
  cur_player->aliased = True;
  bprintf ("Aliased to %s.\n", pname (pl1));
}

void polymorph (int plx, int turns)
{
  /* Polymorph to PLX for TURNS turns. */
  if (plx < 0 || turns < 0)
    {
      unpolymorph (real_mynum);
      return;
    }
  do_pretend (plx);
  cur_player->polymorphed = turns;
  bprintf ("You pass out.....\n..... and wake up.\n");
}


void unalias (int pl)
{
  int me = real_mynum;
  
  setup_globals (pl);
  if (cur_player->aliased)
    {
      do_pretend (-1);
      cur_player->aliased = False;
      bprintf ("&+RBack to good old %s....&*\n", pname (mynum));
    }
  setup_globals (me);
}

void unpolymorph (int pl)
{
  int me = real_mynum;
  
  setup_globals (pl);
  if (cur_player->polymorphed >= 0)
    {
      do_pretend (-1);
      cur_player->polymorphed = -1;
      setpfighting (mynum, -1);
      bprintf ("Suddenly you awake... were you dreaming or what?\n");
    }
  setup_globals (me);
}

Boolean disp_file (char *fname, FILE * f)
{
  char buff[BUFSIZ];
  
  if (f == NULL && (f = fopen (fname, "r")) == NULL)
    {
      return False;
    }
  
  while (fgets (buff, BUFSIZ, f) != NULL)
    {
      bprintf ("%s", buff);
    }
  fclose (f);
  return True;
}

void 
rollcom (void)
{
  int a;
  
  if ((a = ohereandget ()) == -1)
    return;
  switch (a)
    {
    case OBJ_WINDOW_PILLAR_WEST:
    case OBJ_WINDOW_PILLAR_EAST:
      gamecom ("push pillar", True);
      break;
    case OBJ_WINDOW_BOULDER:
      gamecom ("push boulder", True);
      break;
    default:
      bprintf ("You can't roll that.\n");
    }
}

void emptycom (void)
{
  int a, b;
  
  if ((a = ohereandget ()) == -1)
    return;
  
  if (otstbit (a, OFL_LOCKABLE) && state (a) == 2 && !ohany (1 << OFL_KEY))
    {
      bprintf ("The %s is locked, and you have no key.\n", oname (a));
      return;
    }
  else if ((otstbit (a, OFL_OPENABLE) || otstbit (a, OFL_LOCKABLE))
	   && state (a) > 0)
    {
      bprintf ("You open the %s.\n", oname (a));
      setobjstate (a, 0);
    }
  
  for (b = ofirst_obj (a); b != SET_END; b = onext_obj (a))
    {
      if (iscontin (b, a))
	{
	  setoloc (b, mynum, CARRIED_BY);
	  bprintf ("You empty the %s from the %s.\n", oname (b), oname (a));
	  /*	  sprintf (x, "drop %s", oname (b));
		  gamecom (x, False);*/
	  dropobj(b);
	}
    }
}

void dircom (void)
{
  int a, c, oc = 0;
  char b[40], d[40];
  
  if (plev (mynum) < LVL_APPREN)
    {
      erreval ();
      return;
    }
  for (a = 0; a < numobs; a++)
    {
      oc++;
      c = findzone (oloc (a), b);
      sprintf (d, "%s%d", b, c);
      if (ocarrf (a) >= CARRIED_BY)
	strcpy (d, "Carried");
      else if (ocarrf (a) == IN_CONTAINER)
	strcpy (d, "In item");
      bprintf ("%-13s%-13s", oname (a), d);
      if (a % 3 == 2)
	bprintf ("\n");
      /*    if (a % 18 == 17)
       * pbfr(); */
    }
  bprintf ("\nTotal of %d objects.\n", oc);
}


void treasurecom (void)
{
  int a, c, tc = 0;
  char b[40], d[40];
  
  if (plev (mynum) < LVL_APPREN)
    {
      erreval ();
      return;
    }
  for (a = 0; a < numobs; a++)
    {
      if (!oflannel (a) && !otstbit (a, OFL_DESTROYED) && ocarrf (a) == IN_ROOM)
	{
	  tc++;
	  c = findzone (oloc (a), b);
	  sprintf (d, "%s%d", b, c);
	  bprintf ("%-13s%-13s", oname (a), d);
	  if (tc % 3 == 0)
	    bprintf ("\n");
	  if (tc % 18 == 17)
	    pbfr ();
	}
    }
  bprintf ("\nApproximately %d treasures remain.\n", tc);
}

/* The LEVELS command. Display the number of points required to reach each
 * new level together with the corresponding male and female titles.
 */
void levelscom (void)
{
  int i;
  
  bprintf ("&+RLevel   &+BPoints        &+WMale                    Female&*\n");
  bprintf ("=====   ======        ====                    ======\n");
  
  for (i = 1; i <= LVL_APPREN; i++)
    {
      
      bprintf ("&+R%-5d&*   &+B%6d&*        &+Wthe %-17s   the %-20s&*\n",
	       i, levels[i], MLevels[i], FLevels[i]);
    }
  bprintf ("\n");
}


void promptcom (void)
{
  char buff[MAX_COM_LEN];
  
  getreinput (buff);
  if (strlen (buff) > PROMPT_LEN)
    {
      bprintf ("Max. prompt length is %d.\n", PROMPT_LEN);
      return;
    }
  strcpy (cur_player->prompt, buff);
  bprintf ("Ok\n");
}


void kooshcom(void)
{
  if (pl1 == -1)
    {
       bprintf("Toss the koosh to who?\n");
        return;
     }
  else if (iscarrby(OBJ_LIMBO_KOOSH, mynum))
    {
       bprintf ("You toss %s the koosh ball.\n", him_or_her (pl1));
       sillytp(pl1, "tosses you a koosh ball.\n");
  send_msg(ploc(pl1), 0, LVL_MIN, LVL_MAX, pl1, mynum, "\001p%s\003 tosses \001p%s\003 the koosh ball.\n", pname(mynum), pname(pl1));
       setoloc(OBJ_LIMBO_KOOSH, pl1, CARRIED_BY);
       return;
    }
  else
    {
      bprintf("You do not have the koosh ball.\n");
      return;
    }
}
void flowercom (void)
     
{
  int a;
  
  if ((a = vichere ()) < 0)
    return;
  sillytp (a, "sends you flowers.\n\n         \001A\033[1;33m\002(*)\n"
	   "     \001A\033[35m\002*    \001A\033[32m\002|"
	   "\001A\033[1;36m\002    *\n    \001A\033[32m\002"
	   "\\|/  \\|/  \\|/\n\001A\033[0;33m\002"
	   "   ---------------\001A\033[0m\002");
  bprintf ("You send %s flowers.\n", him_or_her (a));
}

void rosecom (void)
{
  int a;
  if ((a = vichere ()) < 0)
    return;
  sillytp (a, "sends you a beautiful rose.\n"
	   "&+R        _____ &*\n"
	   "&+R      /  ___  \\ &*\n"
	   "&+R    /  /  _  \\  \\ &*\n"
	   "&+R  /( /( /(_)\\ )\\ )\\ &*\n"
	   "&+R (  \\  \\ ___ /  /  ) &*\n"
	   "&+R (    \\ _____ /    ) &*\n"
	   "&+R /(               )\\ &*\n"
	   "&+R|  \\             /  | &*\n"
	   "&+R|    \\ _______ /    | &*\n"
	   "&+R \\    / &+G\\   /&+R \\    / &*\n"
	   "&+R   \\/    &+G| |&+R    \\/ &*\n"
	   "&+G         | |/ &*\n"
	   "&+G         | | &*\n"
	   "&+G        \\| | &*\n");
  bprintf ("You send %s a beautiful rose.\n", him_or_her (a));
}

void spitcom(void)
{
  if (pl1 == -1)
    {
      bprintf("You spit.\n");
      sillycom("\001s%s\003%s &+yspits&* on the ground.\n\003");
    }
  else
    {
      sillytp(pl1, "&+yspits&* on you!");
      bprintf("You &+yspit&* on %s.\n", him_or_her(pl1));
    }
  /***************************************************************************
    A bit I wrote for an annoying player who spits a LOT.  If you ever get
    someone who spits a lot it might be funny to sic this on them.
    ***************************************************************************/
  if (EQ(pname(mynum), "Hellraiser"))
    {
      broad("You hear a splash and a gurgle as Hellraiser drowns in his own slobber.\n");
      crapup("Suddenly you find yourself wallowing in a pool of your own slobber.\n"
	     "\tIt rises quickly over your head and you drown.\n", CRAP_SAVE);
    }
}

void petcom (void)
{
  int a;
  
  if ((a = vichere ()) < 0)
    return;
  if (a == mynum)
    {
      bprintf ("No petting yourself in public, please.\n");
      return;
    }
  if (a == MOB_OAKTREE_OTTIMO)
    {
      bprintf("Ottimo rubs up against your leg and licks your hand.\n");
      return;
    }
  sillytp (a, "pats you on the head.");
  bprintf ("You pat %s on the head.\n", him_or_her (a));
}

void wavecom (void)
{
  int a;
  
  if (pfighting (mynum) >= 0)
    {
      bprintf ("What are you trying to do?  Make 'em die laughing?\n");
      return;
    }
  if (EMPTY (item1))
    {
      sillycom ("\001s%s\002%s waves happily.\n\003");
      bprintf ("You wave happily.\n");
    }
  
  if ((a = ob1) < 0)
    return;
  
  switch (onum (a))
    {
      
    case OBJ_WINDOW_BRIDGE_WAND:
      if ((state (OBJ_WINDOW_BRIDGE_FIRE) == 1)
	  && (oloc (OBJ_WINDOW_BRIDGE_FIRE) == ploc (mynum)))
	{
	  setobjstate (OBJ_WINDOW_BRIDGE_HALL, 0);
	  sendf (ploc (mynum), "The drawbridge is lowered!\n");
	  return;
	}
      break;
      
    case OBJ_WINDOW_ROD:
      if (iscarrby (OBJ_CATACOMB_CUPSERAPH, mynum))
	{
	  bprintf ("Something prevents the rod's functioning.\n");
	  return;
	}
      if (oarmor (a) >= 3)
	{
	  bprintf ("The rod crumbles to dust!\n");
	  destroy (a);
	  return;
	}
      oarmor (a)++;
      bprintf ("You are teleported!\n");
      teletrap (LOC_WINDOW_PENTACLE);
      return;
      
    case OBJ_TOWER_WAND:
      
      if (oarmor (a) > 0)
	{
	  osetarmor (a, oarmor (a) - 1);
	  cur_player->me_ivct = 60;
	  setpvis (mynum, 10);
	  bprintf ("You seem to &+Cs&+Wh&+Ci&+Wm&+Cm&+We&+Cr&* and blur.\n");
	  return;
	}
    }
  bprintf ("Nothing happens.\n");
}

void lightcom ()
{
  int a;
  char s[100];
  
  if (pl1 == mynum && plev(mynum) >= LVL_APPREN) 
    {
      ssetflg(mynum, SFL_GLOWING);
      bprintf("You &+Ylight&* yourself!\n");
      return;
    }
  if ((a = ohereandget ()) == -1)
    return;
  if (!ohany (1 << OFL_LIT))
    {
      bprintf ("You have nothing to light things from.\n");
      return;
    }
#ifdef LOCMIN_FOREST
  if (a==OBJ_FOREST_INSIDETREE)
    {
      bprintf("The tree screams and thrashes around.  In its dying throes you dash free!\n");
      trapch(LOC_FOREST_F4);
      destroy(OBJ_FOREST_TREEEATING);
      setpscore(mynum,pscore(mynum)+100);
      return;
    }
#endif
  
#ifdef LOCMIN_OAKTREE
  if (((a == OBJ_OAKTREE_B_ROOTS) ||  (a == OBJ_OAKTREE_ROOTS)) && state(a) == 1)
    {
      send_msg(ploc(mynum), 0, LVL_MIN, LVL_MAX, NOBODY, NOBODY, 
	       "The roots burn away to reveal a hidden passageway.\n");
    }
#endif
  
#ifdef LOCMIN_EFOREST
  
  if (a == OBJ_EFOREST_THORNS || a == OBJ_EFOREST_THORNSEAST)
    {
      if (state (OBJ_EFOREST_THORNS) == 0)
	{
	  bprintf ("The thorns have already been burned away.\n");
	  return;
	}
      setobjstate (OBJ_EFOREST_THORNS, 0);
      bprintf ("You burn the wall of thorns away!\n");
      bprintf ("Behind them you can see the entrance to a cave.\n");
      sprintf (s, "%s burns the wall of thorns away!\n", pname (mynum));
      
      send_msg (LOC_EFOREST_THORNY, 0, LVL_MIN, LVL_MAX, mynum, NOBODY, s);
      send_msg (LOC_EFOREST_CAVE, 0, LVL_MIN, LVL_MAX, mynum, NOBODY, s);
      
      return;
    }
#endif
  
#ifdef LOCMIN_PIRATE
  if (a == OBJ_PIRATE_KEG)	/* Light keg */
    {
      if (oloc (a) == LOC_PIRATE_PIRATE30)	/* blow up boulder */
	{
	  send_msg (oloc (a), 0, LVL_MIN, LVL_MAX, NOBODY, NOBODY,
		    "The explosion shatters the boulder!\n");
	  setobjstate (OBJ_PIRATE_BOULDER1, 0);
	  destroy (OBJ_PIRATE_KEG);
	}
      bprintf ("You are hurt by the flying pieces of debris!\n");
      if (pstr (mynum) <= 10)
	{
	  setpstr (mynum, 1);
	}
      else
	{
	  setpstr (mynum, (pstr (mynum) - 10));
	}
      return;
    }
#endif /* PIRATE.ZONE */
  
  if (!otstbit (a, OFL_LIGHTABLE))
    bprintf ("You can\'t light that!\n");
  else if (otstbit (a, OFL_LIT))
    bprintf ("It\'s already lit.\n");
  else
    {
      setobjstate (a, 0);
      osetbit (a, OFL_LIT);
      bprintf ("Ok\n");
    }
}

void extinguishcom ()
{
  int a;
  
  if (pl1 == mynum && plev(mynum) >= LVL_APPREN) 
    {
      sclrflg(mynum, SFL_GLOWING);
      bprintf("You put yourself out before you accidently set a mortal alight!\n");
      return;
    }
  if ((a = ohereandget ()) == -1)
    return;
  if (!otstbit (a, OFL_LIT))
    bprintf ("It\'s not lit!\n");
  else if (!otstbit (a, OFL_EXTINGUISH))
    bprintf ("You can\'t extinguish that!\n");
  else
    {
      setobjstate (a, 1);
      oclrbit (a, OFL_LIT);
      bprintf ("Ok\n");
    }
}

void pushcom (void)
{
  int x;
  char s[128];
  
  if (brkword () == -1)
    {
      bprintf ("Push what?\n");
      return;
    }
  if ((x = fobna (wordbuf)) == -1)
    {
      bprintf ("That is not here.\n");
      return;
    }
  
  
  /* If it's the original object OR a copy:
   */
  switch (onum (x))
    {
#ifdef LOCMIN_KASTLE
    case OBJ_KASTLE_GOLD_CHAIN:
      bprintf("As you tug on the chain a trap door opens beneath your feet.\n");
      bprintf("You fall with a loud thud to the room below as the trap door closes above you.\n");
      teletrap(LOC_KASTLE_15);
      return;
      break;
#endif
      
#ifdef LOCMIN_OAKTREE
    case OBJ_OAKTREE_ROCK:
      if (state(OBJ_OAKTREE_ROCK) == 1)
	{
	  sprintf(s,"moves the rock, uncovering a depression.");
	  bprintf("You uncover a depression.\n");
	  setobjstate(OBJ_OAKTREE_ROCK, 0);
	}
      else
	{
	  sprintf(s,"pushes the rock back into the depression.\n");
	  bprintf("You push the rock back into the depression.\n");
	  setobjstate(OBJ_OAKTREE_ROCK, 1);
	}
      send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
	       "%s %s\n", pname(mynum), s);
      send_msg(ploc(mynum), 0, LVL_MIN, pvis(mynum), mynum, NOBODY,
	       "Someone %s\n", s);
      return;
      break;
#endif
      
    case OBJ_WINDOW_TRIPWIRE:
      bprintf ("The tripwire moves and a huge stone crashes down from above!\n");
      broad ("\001dYou hear a thud and a squelch in the distance.\n\003");
      crapup ("             S   P    L      A         T           !", SAVE_ME);
      break;
    case OBJ_WINDOW_BOOKCASE:
      bprintf ("A trapdoor opens at your feet and you plumment downwards!\n");
      sillycom ("\001p%s\003 disappears through a trapdoor!\n");
      teletrap (LOC_WINDOW_FISSURE);
      return;
    }
  
  
  /* If it's the original object (only), not a copy:
   */
  switch (x)
    {
      
#ifdef LOCMIN_ABYSS
    case OBJ_ABYSS_BUTTON1:
      if (!state (OBJ_ABYSS_BUTTON1))
	{
	  setobjstate (OBJ_ABYSS_CLOSETPANEL, 0);
	  setobjstate (OBJ_ABYSS_BUTTON1, 1);
	}
      else
	{
	  setobjstate (OBJ_ABYSS_CLOSETPANEL, 1);
	  setobjstate (OBJ_ABYSS_BUTTON1, 0);
	}
      bprintf ("You hear a soft swish come from below.\n");
      return;
    case OBJ_ABYSS_BUTTON2:
      if (state (OBJ_ABYSS_HALLELEVATORDOOR))
	{
	  setobjstate (OBJ_ABYSS_HALLELEVATORDOOR, 0);
	  bprintf ("The elevator door swishes open.\n");
	}
      return;
    case OBJ_ABYSS_BUTTON3:
      if (!state (OBJ_ABYSS_ELEVATORDOOR))
	{
	  setobjstate (OBJ_ABYSS_HALLELEVATORDOOR, 0);
	  bprintf ("The elevator door swishes closed.\n");
	  bprintf ("The elevator zips upward at an astonishing pace and deposits "
		   "you here.\n");
	  teletrap (LOC_CHURCH_N_BARROW);
	}
      return;
    case OBJ_ABYSS_DUST:
      if (!state (OBJ_ABYSS_DUST))
	{
	  setobjstate (OBJ_ABYSS_DUST, 1);
	  create (OBJ_ABYSS_BUTTON1);
	  bprintf ("You sweep the dust aside to reveal a square button on the shelf.\n");
	  return;
	}
      break;
    case OBJ_ABYSS_L663BOULDER:
      if (ptothlp (mynum) == -1)
	{
	  bprintf ("You can't seem to get enough leverage to move it.\n");
	  break;
	}
      sillytp (ptothlp (mynum), "pushes the boulder aside with your help.");
      setobjstate (x, 1 - state (x));
      oplong (x);
      return;
#endif
      
#ifdef LOCMIN_PIRATE
    case OBJ_PIRATE_MASTROPE:
      if (state (OBJ_PIRATE_SAIL) == 0)
	{
	  bprintf ("You raise the sail, which promptly catches the wind.\n");
	  setobjstate (OBJ_PIRATE_SAIL, 1);
	}
      else
	{
	  bprintf ("You lower and furl the sail.\n");
	  setobjstate (OBJ_PIRATE_SAIL, 0);
	}
      return;
#endif /* PIRATE.ZONE */
      
#ifdef LOCMIN_EFOREST
      
    case OBJ_EFOREST_BUTTON:
      setobjstate (OBJ_EFOREST_BUTTON, 0);
      setobjstate (OBJ_EFOREST_THRONE_CATHEDRAL,
		   1 - state (OBJ_EFOREST_THRONE_CATHEDRAL));
      
      sendf (ploc (mynum),
	     "You hear a grinding sound from near the entrance of the caves.\n");
      
      sprintf (s, "You hear a grinding sound as a mysterious force moves "
	       "the throne!\n");
      
      sendf (LOC_EFOREST_EASTEND, s);
      sendf (LOC_EFOREST_BOTTOM, s);
      break;
#endif
      
#ifdef LOCMIN_FROBOZZ
    case OBJ_FROBOZZ_BUTTON_OUTSIDE:
      if (state (OBJ_FROBOZZ_VAULTDOOR_OUTSIDE) == 1)
	{
	  setobjstate (OBJ_FROBOZZ_VAULTDOOR_OUTSIDE, 0);
	  
	  sendf (LOC_FROBOZZ_OUTSIDE, "Without a sound the whole southern "
		 "wall moves some feet westwards!\n");
	  
	  sendf (LOC_FROBOZZ_VAULT, "Without a sound the whole northern wall "
		 "moves some feet westwards!\n");
	  break;
	}
      if (state (OBJ_FROBOZZ_VAULTDOOR_OUTSIDE) == 0)
	{
	  setobjstate (OBJ_FROBOZZ_VAULTDOOR_OUTSIDE, 2);
	  sendf (LOC_FROBOZZ_OUTSIDE, "The southern wall glides some feet "
		 "to the east closing the vault!\n");
	  
	  sendf (LOC_FROBOZZ_VAULT, "The northern wall glides some feet to "
		 "the east closing the vault!\n");
	  break;
	}
      if (state (OBJ_FROBOZZ_VAULTDOOR_OUTSIDE) == 2)
	{
	  bprintf ("Nothing happens.\n");
	  break;
	}
      
    case OBJ_FROBOZZ_RUG_LIVING:
      if (state (OBJ_FROBOZZ_RUG_LIVING) == 1)
	{
	  setobjstate (OBJ_FROBOZZ_RUG_LIVING, 0);
	  setobjstate (OBJ_FROBOZZ_TRAPDOOR_LIVING, 2);
	  bprintf ("You move the rug and uncover a trapdoor!\n");
	  sprintf (s, "%s moves the heavy rug and uncovers a trapdoor!\n",
		   pname (mynum));
	  sillycom (s);
	}
      else
	{
	  bprintf ("You move the rug, but find nothing.\n");
	  sprintf (s, "%s moves the rug.\n", pname (mynum));
	  sillycom (s);
	}
      break;
#endif
#ifdef LOCMIN_ANCIENT
    case OBJ_ANCIENT_MOONCROSS:
      if (state (OBJ_ANCIENT_MOONCROSS) > 0)
	{
	  setobjstate (OBJ_ANCIENT_MOONCROSS, 0);
	  if (state (OBJ_ANCIENT_BIGCROSS) > 0)
	    {
	      bprintf ("You move the cross to one side, but nothing happens.\n");
	    }
	  else
	    {
	      bprintf ("You hear a loud hollow sound from a room nearby!\n");
	      setobjstate (OBJ_ANCIENT_HOLE_ANC31, 0);
	    }
	}
      else
	{
	  setobjstate (OBJ_ANCIENT_MOONCROSS, 1);
	  setobjstate (OBJ_ANCIENT_HOLE_ANC31, 1);
	  bprintf ("You move the cross back into place.\n");
	}
      break;
    case OBJ_ANCIENT_BIGCROSS:
      if (state (OBJ_ANCIENT_BIGCROSS) > 0)
	{
	  setobjstate (OBJ_ANCIENT_BIGCROSS, 0);
	  if (state (OBJ_ANCIENT_MOONCROSS) > 0)
	    {
	      bprintf ("You move the cross to one side, but nothing happens.\n");
	    }
	  else
	    {
	      bprintf ("You hear a loud hollow sound from a room nearby!\n");
	      setobjstate (OBJ_ANCIENT_HOLE_ANC31, 0);
	    }
	}
      else
	{
	  setobjstate (OBJ_ANCIENT_BIGCROSS, 1);
	  setobjstate (OBJ_ANCIENT_HOLE_ANC31, 1);
	  bprintf ("You move the cross back into place.\n");
	}
      break;
#endif
      
    case OBJ_WINDOW_IRONBAR:
      if (state (OBJ_WINDOW_PASS_STONE) == 1)
	{
	  setobjstate (OBJ_WINDOW_PASS_FISSURE, 0);
	  bprintf ("A secret panel opens in the east wall!\n");
	  break;
	}
      bprintf ("Nothing happens.\n");
      break;
    case OBJ_WINDOW_BOULDER:
      bprintf ("With a mighty heave you manage to move the boulder a few feet\n");
      if (state (OBJ_WINDOW_HOLE_CRACK) == 1)
	{
	  bprintf ("uncovering a hole behind it.\n");
	  setobjstate (OBJ_WINDOW_HOLE_CRACK, 0);
	}
      else
	{
	  bprintf ("covering a hole behind it.\n");
	  setobjstate (OBJ_WINDOW_HOLE_CRACK, 1);
	}
      break;
    case OBJ_WINDOW_LEVER:
      if (ptothlp (mynum) == -1)
	{
	  bprintf ("It's too stiff.  Maybe you need help.\n");
	  return;
	}
      bprintf ("Ok\n");
      if (state (OBJ_WINDOW_SLIME_PIT) != 0)
	{
	  sillycom ("\001s%s\002%s pulls the lever.\n\003");
	  sendf (oloc (OBJ_WINDOW_LEVER),
		 "\001dYou hear a gurgling noise and then silence.\n\003");
	  setobjstate (OBJ_WINDOW_SLIME_PIT, 0);
	  sendf (oloc (OBJ_WINDOW_SLIME_PIT),
		 "\001cThere is a muffled click and the slime drains away.\n\003");
	}
      break;
    case OBJ_WINDOW_THRONE_CURTAINS:
    case OBJ_WINDOW_ROUGH_CURTAINS:
      setobjstate (OBJ_WINDOW_THRONE_CURTAINS,
		   1 - state (OBJ_WINDOW_THRONE_CURTAINS));
      bprintf ("Ok\n");
      break;
    case OBJ_CASTLE_LEVER:
      setobjstate (OBJ_CASTLE_PORT_INSIDE, 1 - state (OBJ_CASTLE_PORT_INSIDE));
      if (state (OBJ_CASTLE_PORT_INSIDE))
	{
	  sendf (oloc (OBJ_CASTLE_PORT_INSIDE), "\001cThe portcullis falls.\n\003");
	  sendf (oloc (OBJ_CASTLE_PORT_OUTSIDE), "\001cThe portcullis falls.\n\003");
	}
      else
	{
	  sendf (oloc (OBJ_CASTLE_PORT_INSIDE), "\001cThe portcullis rises.\n\003");
	  sendf (oloc (OBJ_CASTLE_PORT_OUTSIDE), "\001cThe portcullis rises.\n\003");
	}
      break;
    case OBJ_WINDOW_BRIDGE_LEVER:
      setobjstate (OBJ_WINDOW_BRIDGE_HALL, 1 - state (OBJ_WINDOW_BRIDGE_HALL));
      if (state (OBJ_WINDOW_BRIDGE_HALL))
	{
	  sendf (oloc (OBJ_WINDOW_BRIDGE_HALL), "\001cThe drawbridge rises.\n\003");
	  sendf (oloc (OBJ_WINDOW_BRIDGE_FIRE), "\001cThe drawbridge rises.\n\003");
	}
      else
	{
	  sendf (oloc (OBJ_WINDOW_BRIDGE_HALL),
		 "\001cThe drawbridge is lowered.\n\003");
	  sendf (oloc (OBJ_WINDOW_BRIDGE_FIRE),
		 "\001cThe drawbridge is lowered.\n\003");
	}
      break;
    case OBJ_CASTLE_TORCH:
      if (state (OBJ_CASTLE_DOOR_GOLEM) == 1)
	{
	  setobjstate (OBJ_CASTLE_DOOR_GOLEM, 0);
	  sendf (oloc (OBJ_CASTLE_DOOR_GOLEM),
		 "A secret door slides quietly open in the south wall!\n");
	}
      else
	bprintf ("It moves but nothing seems to happen.\n");
      return;
    case OBJ_CHURCH_ROPE:
      if (oarmor (OBJ_CHURCH_ROPE) >= 12)
	bprintf ("\001dChurch bells ring out around you.\n\003");
      else
	{
	  if (++oarmor (OBJ_CHURCH_ROPE) == 12)
	    {
	      bprintf ("A strange ghostly guitarist shimmers briefly before you.\n");
	      setpscore (mynum, pscore (mynum) + 300);
	      broad ("\001dA faint ghostly guitar solo "
		     "floats through the air.\n\003");
	    }
	}
      break;
    case OBJ_CATACOMB_DUST:
      bprintf ("Great clouds of dust billow up, causing you to sneeze "
	       "horribly.\nWhen you're finished sneezing, you notice "
	       "a message carved into one wall.\n");
      
      broad ("\001dA loud sneeze echoes through the land.\n\003");
      destroy (OBJ_CATACOMB_DUST);
      create (OBJ_CATACOMB_KOAN);
      break;
    case OBJ_ORCHOLD_BOTCOVER:
      bprintf ("You can't seem to get enough leverage to move it.\n");
      return;
    case OBJ_ORCHOLD_TOPCOVER:
      if (ptothlp (mynum) == -1)
	{
	  bprintf ("You try to shift it, but it's too heavy.\n");
	  break;
	}
      sillytp (ptothlp (mynum), "pushes the cover aside with your help.");
      setobjstate (x, 1 - state (x));
      oplong (x);
      return;
    case OBJ_ORCHOLD_SWITCH:
      if (state (x))
	{
	  bprintf ("A hole slides open in the north wall!\n");
	  setobjstate (x, 0);
	}
      else
	bprintf ("You hear a little 'click' sound.\n");
      return;
    case OBJ_WINDOW_STATUE_DOWN:
      if (ptothlp (mynum) == -1)
	{
	  bprintf ("You can't shift it alone, maybe you need help.\n");
	  break;
	}
      sillytp (ptothlp (mynum), "pushes the statue with your help.");
      /* FALLTHROUGH */
    default:
      if (otstbit (x, OFL_PUSHABLE))
	{
	  setobjstate (x, 0);
	  oplong (x);
	  return;
	}
      if (otstbit (x, OFL_PUSHTOGGLE))
	{
	  setobjstate (x, 1 - state (x));
	  oplong (x);
	  return;
	}
      bprintf ("Nothing happens.\n");
    }
}

void followcom (void)
{
  int i;
  
  if (cur_player->i_follow >= 0)
    {
      bprintf ("Stopped following \001P%s\003.\n", pname (cur_player->i_follow));
      cur_player->i_follow = -1;
    }
  if (pfighting (mynum) != -1)
    {
      bprintf ("Not in a fight!\n");
      return;
    }
  if (EMPTY (item1))
    {
      bprintf ("Who?\n");
      return;
    }
  if (pl1 == -1)
    {
      bprintf ("Who?\n");
      return;
    }
  if (ploc (pl1) != ploc (mynum))
    {
      bprintf ("They aren't here.\n");
      return;
    }
  if (mynum == pl1)
    {
      bprintf ("You run round in circles!\n");
      return;
    }
  i = pl1;
  if ((i != -1) && (i < max_players)) {
    i = players[i].i_follow;
    if (i == mynum) {
      bprintf("You chase each other in circles!\n");
      return;
    }
  }
  
  sendf (pl1, "%s is following you.\n", see_name (pl1, mynum));
  cur_player->i_follow = pl1;
  bprintf ("Ok\n");
}

void losecom (void)
{
  if (cur_player->i_follow == -1)
    {
      bprintf ("You aren't following anyone.\n");
      return;
    }
  cur_player->i_follow = -1;
  bprintf ("Ok\n");
}

/*****************************************************
 *                                                   *
 *               The BULLETIN command                *
 *                                                   *
 *****************************************************/

static void 
bull3 (char *cont)
{
  bprintf ("\001f%s\003", BULLETIN3);
  get_command (NULL);
}

static void 
bull2 (char *cont)
{
  bprintf ("\001f%s\003", BULLETIN2);
  if (plev (mynum) < LVL_ARCHWIZARD)
    {
      get_command (NULL);
    }
  else
    {
      bprintf (pwait);
      replace_input_handler (bull3);
    }
}
void bulletincom (void)
{
  bprintf ("\001f%s\003", BULLETIN1);
  if (plev (mynum) < LVL_APPREN)
    {
      return;
    }
  bprintf (pwait);
  replace_input_handler (bull2);
}


void boot_reset ()
{
  register int i;
  extern CALENDAR calendar;
  
  calendar.daytime = 0;
  
  global_clock = time (0);
  for (i = 0; i < numzon; i++)
    {
      reset_zone (i, &global_clock,
		  NULL, NULL, NULL, NULL, NULL, NULL);
    }
  mudlog ("--------  SYSTEM RELOAD  --------");
  last_reset = time (0);
}

void timeoutcom (void)
{
  int x, p;
  char name[80];
  
  if (!ptstflg (mynum, PFL_TIMEOUT))
    {
      bprintf ("You can't do that now.\n");
      return;
    }
  
  if (EMPTY (item1))
    {
      bprintf ("Timeout who?\n");
      return;
    }
  
  if ((x = pl1) == -1)
    {
      bprintf ("No such player or mobile on the game.\n");
      return;
    }
  
  
  if (x >= max_players)
    {				/* mobile */
      dumpstuff (x, ploc (x));
      setpfighting (x, -1);
      if (!ppermanent (x))
	{
	  sprintf (name, "non-permanent mobile %s", pname (x));
	  destruct_mobile (x, NULL);
	  
	}
      else
	{
	  sprintf (name, "permanent mobile %s", pname (x));
	  setpname (x, "");
	}
    }
  else if (wlevel (plev (mynum)) < wlevel (plev (x)))
    {
      bprintf ("You can't time out %s.\n", pname (x));
    }
  else
    {				/* player */
      int y = real_mynum;
      sprintf (name, "player %s", pname (x));
      setup_globals (x);
      crapup ("\tYou have timed out.", CRAP_SAVE | CRAP_RETURN);
      setup_globals (y);
    }
  
  
#ifdef LOG_TIMEOUT
  mudlog ("%s TOUTed %s", pname (mynum), name);
#endif
  
  p = LVL_ARCHWIZARD;
  if (pvis (x) > p)
    p = pvis (x);
  send_msg (DEST_ALL, MODE_QUIET | MODE_COLOR, p, LVL_MAX, x, mynum,
	    "[%s timed out %s]\n", pname (mynum), (char *)name);
  return;
  
}

void asmortalcom(void)
{
  int old_lev;

  if (!ptstflg(mynum, PFL_ASMORTAL))
    {
      bprintf("You can't do that now.\n");
      return;
    }
  old_lev = plev(mynum);
  if (brkword() == -1)
    {
      bprintf("Do what as a mortal?\n");
      return;
    }
  cur_player->asmortal = 1;
  cur_player->isforce = True;
  setplev(mynum, 10);
  gamecom(wordbuf, True);
  setplev(mynum, old_lev);
  cur_player->asmortal = 0;
  cur_player->isforce = False;
}

