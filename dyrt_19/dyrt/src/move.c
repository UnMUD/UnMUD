#include "kernel.h"
#include "objects.h"
#include "mobiles.h"
#include "locations.h"
#include "stdinc.h"

static char *exittxt[] =
{
  "north", "east", "south", "west", "up", "down",
  "northeast", "southeast", "southwest", "northwest",
  "n", "e", "s", "w", "u", "d", "ne", "se", "sw", "nw",
  0
};


static int exitnum[] =
{
  1, 2, 3, 4, 5, 6, 7, 8, 9,
  1, 2, 3, 4, 5, 6, 7, 8, 9
};                            

int 
dodirn (int vb)
{
  char block[SETIN_MAX];
  int i, pc, n;
  int save_mynum, save_real_mynum, myloc;
  int newch, drnum, droff;
 /*** 
  if (cur_player->aliased || cur_player->polymorphed != -1)
    {
      bprintf("I'm sorry, but you can't move while aliased.\n");
      return 0;
    }
*****/
  if (pfighting (mynum) >= 0)
    {
      bprintf ("You can't just stroll out of a fight!\n");
      bprintf ("If you wish to leave, you must FLEE in a direction.\n");
      return -1;
    }
  if (iscarrby (OBJ_CATACOMB_CUPSERAPH, mynum)
      && (i = alive ((max_players + MOB_CATACOMB_SERAPH))) != -1
      && ploc (i) == ploc (mynum))
    {
      bprintf ("The Seraph says 'I cannot allow you to leave this place "
	       "with the Holy Relic.'\n");
      return -1;
    }
  if (iscarrby (OBJ_CASTLE_RUNESWORD, mynum)
      && ploc ((max_players + MOB_CASTLE_GOLEM)) == ploc (mynum)
      && !EMPTY (pname ((max_players + MOB_CASTLE_GOLEM))))
    {
      bprintf ("\001cThe Golem\003 bars the doorway!\n");
      return -1;
    }
  
  n = vb - 2;			/* Since VERB_NORTH = 2 etc....stupid */
  
  if (chkcrip () || chksitting ())
    return -1;
  
  switch (newch = getexit (ploc (mynum), n))
    {
    case EX_DOWN_SLIME:
      if (state (OBJ_WINDOW_SLIME_PIT) != 0)
	{
	  bprintf ("That doesn't look like a very good idea!\n");
	  return -1;
	}
      newch = LOC_WINDOW_SLIME;
      break;
    case EX_DEFENDER:
      newch = LOC_CATACOMB_HALL;
      break;
    case EX_GET_STUFF:
      newch = LOC_CATACOMB_CHAMBER;
      break;
      
    default:
      
      if (newch >= DOOR && newch < EDOOR)
	{
	  drnum = newch - DOOR;
	  droff = /*drnum ^ 1 */ olinked (drnum);	/* other door side */
	  if (state (drnum))
	    {
	      if (!EQ (oname (drnum), "door") || isdark ()
		  || EMPTY (olongt (drnum, state (drnum))))
		bprintf ("You can't go that way.\n");
	      else
		bprintf ("The door is closed.\n");
	      return -1;
	    }
	  newch = /*oloc(droff) */ obj_loc (droff);
	}
      break;
    }
  
  if (!exists (newch))
    {
      bprintf ("You can't go that way.\n");
      return -1;
    }
  
  if (!checklevel(mynum, newch)) return -1;
  
#ifdef LOCMIN_HEAVEN
  switch (newch)
    {
    case LOC_HEAVEN_36:
    case LOC_HEAVEN_27:
    case LOC_HEAVEN_16:
    case LOC_HEAVEN_17:
    case LOC_HEAVEN_18:
    case LOC_HEAVEN_14:
    case LOC_HEAVEN_15:
      newch = LOC_VILLAGE_HOSPITAL;
      bprintf ("You faintly hear voices, then feel an irresistable tugging.\n");
      setploc (mynum, LOC_VILLAGE_HOSPITAL);
      lookin (ploc (mynum), 0);
      bprintf ("A nurse tells you they almost didn't pull you through.\n");
      return 0;
    default:;
    }
#endif
  
#ifdef LOCMIN_PIRATE
  if ((!ststflg (mynum, SFL_DEAF)) &&
      (alive (MOB_PIRATE_MERMAID + max_players) != -1))
    {
      if ((ploc (mynum) == LOC_PIRATE_PIRATE27) && (n != 3))	/* ignore west */
	{
	  bprintf ("The Mermaid's song draws you west!\n");
	  newch = LOC_PIRATE_PIRATE28;
	}
      else if ((ploc (mynum) == LOC_PIRATE_PIRATE28) && (n != 2))	/* South */
	{
	  bprintf ("You try to leave but the Mermaid's song draws you back.\n");
	  return -1;
	}
    }
#endif /* PIRATE.ZONE */
  
#ifdef LOCMIN_MITHDAN
  if (n == EX_NORTH)
    {
      if ((ploc(mynum) == LOC_MITHDAN_30) && (newch == LOC_MITHDAN_29))
	{
	  bprintf("You slip on the ice and slide uncontrolably down the side of\n");
	  bprintf("mountain, landing in a soft pile of snow.\n");
	  setploc(mynum, LOC_MITHDAN_19);
	  lookin(ploc(mynum), 0);
	  return 0;
	}
    }
#endif
  
  
#ifdef LOCMIN_NARNIA
  if (alive (MOB_NARNIA_FEN + max_players) != -1)
    {
      if (ploc(mynum) == LOC_NARNIA_CAST7)
	{
	  bprintf("You have awakened Fenris Ulf!\n");
	  bprintf("Fenris gets to his feet and growls at you.\n");
	  bprintf("Suddenly he lunges at you, hurting you badly!\n");
	  setpstr(mynum, (pstr(mynum) - ((pstr(mynum)*20)/100)));
	  bprintf("Fenris takes off into the courtyard.\n");
	  setpstr((MOB_NARNIA_FEN + max_players), -1);
	  setploc((MOB_NARNIA_FEN + max_players), LOC_DEAD_DEAD);
          setploc((MOB_NARNIA_FENRIS + max_players), LOC_NARNIA_TABLE);
	  return 0;
	}
    }
  if ((ploc(mynum) == LOC_NARNIA_CAIR6) && (n == EX_DOWN))
    {
      bprintf("You feel your body being pulled and twisted in many different ways\n");
      bprintf("at once by the darkness.  Just as you begin to wonder what you've done\n");
      bprintf("you find yourself safely in the company of coats.  You find your feet\n");
      bprintf("and begin to run, hurling yourself against the front door of the wardrobe.\n");
      bprintf("Under your weight and speed the door flies open and you stumble into...\n\n\n");
      setploc(mynum, LOC_NARNIA_WARDROBE);
      lookin(ploc(mynum), 0);
      return 0;
    }
#endif
  
#ifdef LOCMIN_ORCHOLD
  if (n == EX_NORTH)
    {
      if (ploc(mynum) == LOC_ORCHOLD_DAMP)
	{
	  bprintf("You fall through a trap door, plummeting to the ground below!\n");
	  bprintf("Dazed, you pick yourself up from the floor to find yourself...\n");
	  setploc(mynum, LOC_ORCHOLD_TRAP);
	  lookin(ploc(mynum), 0);
	  return 0;
	}
    }
#endif
  
  if (n == EX_WEST)
    {
      if (ploc(mynum) == LOC_BLIZZARD_GATE)
	if (plev(mynum) > 6)
	  {
	    bprintf("You are too mighty a warrior to enter here.\n");
	    return -1;
	  }
    }
  
  if (ltstflg (newch, LFL_PRIVATE) || ltstflg (newch, LFL_ONE_PERSON))
    {
      pc = 0;
      for (i = 0; i < max_players; i++)
	if (!EMPTY (pname (i)) && ploc (i) == newch)
	  pc++;
      if (pc > (ltstflg (newch, LFL_PRIVATE) ? 1 : 0))
	{
	  bprintf ("I'm sorry, that room is currently full.\n");
	  return -1;
	}
    }
  if (newch == LOC_WINDOW_LAVA_PATH2)
    {
      if (!iswornby(OBJ_WINDOW_SHIELD1, mynum)
	  && !iswornby (OBJ_WINDOW_SHIELD2, mynum)
	  && !iswornby (OBJ_TREEHOUSE_SHIELD, mynum)
	  && !iswornby (OBJ_CATACOMB_SHIELD, mynum)
	  && !iswornby (OBJ_EFOREST_SHIELD, mynum))
	{
	  bprintf ("The intense heat drives you back.\n");
	  return -1;
	}
      bprintf ("The shield protects you from the worst of the lava's heat.\n");
    }
  if (n == EX_NORTH)
    {
      for (i = max_players; (!mtstflg (i, MFL_BAR_N) || ploc (i) != ploc (mynum)
			     || alive (i) == -1) && i < numchars; i++);
      if (mtstflg (i, MFL_BAR_N) && alive (i) != -1 && ploc (i) == ploc (mynum)
	  && plev (mynum) < LVL_APPREN)
	{
	  bprintf ("\001p%s\003 says 'None shall pass!'\n", pname (i));
	  if (i == (max_players + MOB_CATACOMB_DEFENDER))
	    {
#if 0
	      woundmn (i, 0);
#endif
	    }
	  return -1;
	}
      
      if (iscarrby (OBJ_EFOREST_HOPE, mynum) && ploc (mynum) == LOC_EFOREST_STONE)
	{
	  bprintf ("A mysterious force prevents you from going that way.\n");
	  return -1;
	}
      
    }
  if (n == EX_WEST)
    {
      
      for (i = max_players; (!mtstflg (i, MFL_BAR_W) || ploc (i) != ploc (mynum)
			     || alive (i) == -1) && i < numchars; i++);
      if (mtstflg (i, MFL_BAR_W) && alive (i) != -1 && ploc (i) == ploc (mynum)
	  && plev (mynum) < LVL_APPREN)
	{
	  bprintf ("\001p%s\003 gives a warning growl.\n", pname (i));
	  bprintf ("\001p%s\003 won't let you go West!\n", pname (i));
	  return -1;
	}
    }
  if ((n == EX_NORTH) || (n == EX_EAST ) || (n == EX_SOUTH) || (n == EX_DOWN))
    {
      if ((i = alive ((max_players + MOB_EFOREST_ASMADEUS))) != -1
	  && plev (mynum) < LVL_APPREN
	  && ploc (i) == ploc (mynum))
	{
	  bprintf ("\001pAsmadeus\003 refuses to let you enter his museum.\n");
	  return -1;
	}
    }
  
  if (n == EX_DOWN)
    {				/* can't go down unless empty-handed */
      if ((ploc (mynum) == LOC_CATACOMB_BEGGAR || ploc (mynum) == LOC_VALLEY_FALLS)
	  && gotanything (mynum))
	if (ploc (mynum) == LOC_CATACOMB_BEGGAR)
	  {
	    bprintf ("A mysterious force blocks your passage.\n");
	    if (ploc ((max_players + MOB_CATACOMB_BEGGAR)) == ploc (mynum))
	      sendf (ploc (mynum),
		     "%s says 'To continue on, you must forego all worldly "
		     "possessions.'\n", pname (max_players + MOB_CATACOMB_BEGGAR));
	    return -1;
	  }
	else
	  {
	    bprintf ("The steep and slippery sides of the pool make it "
		     "impossible to climb down\nwithout dropping everything "
		     "first.\n");
	    return -1;
	  }
      
      for (i = max_players; (!mtstflg (i, MFL_BAR_D) || ploc (i) != ploc (mynum)
			     || alive (i) == -1) && i < numchars; i++);
      if (mtstflg (i, MFL_BAR_D) && alive (i) != -1 && ploc (i) == ploc (mynum)
	  && plev (mynum) < LVL_APPREN)
	{
	  bprintf ("\001p%s\003 refuses to let you go Down!\n", pname (i));
	  return -1;
	}
#ifdef LOCMIN_NIBELUNG /* Stop going down unless drink potion */
      if (ploc(mynum) == LOC_NIBELUNG_84)
	{
	  if ((oloc(OBJ_NIBELUNG_POTION) != mynum)
	      || (!otstbit(OBJ_NIBELUNG_POTION, OFL_DESTROYED)))
	    {
	      bprintf("You are overcome by a forboding feeling of darkness!!\n");
	      return 0;
	    }
	}
#endif
      
#ifdef LOCMIN_KASTLE
      if (newch == LOC_KASTLE_11 && ploc(mynum) == LOC_KASTLE_10)
	if (!iswornby(OBJ_KASTLE_SKIS, mynum)) 
	  {
	    teletrap(LOC_KASTLE_24);
	    return 0;
	  }
	else 
	{
 	bprintf("You shuss down the glacier like an expert on your skis.\n");
      questsdone[Q_SKI] = True;
      if (!qtstflg(mynum, Q_SKI))
        {
          qsetflg(mynum, Q_SKI);
          send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, LVL_MAX,
                   NOBODY, NOBODY, "[%s solved the SkiLesson Quest]\n", pname(mynum));
          bprintf("You have successfully completed your Skiing lesson!\n");
        }
    }
#endif
    }
  
  if (n == EX_UP)
    {
      for (i = max_players; (!mtstflg (i, MFL_BAR_U) || ploc (i) != ploc (mynum)
			     || alive (i) == -1) && i < numchars; i++);
      if (mtstflg (i, MFL_BAR_U) && alive (i) != -1 && ploc (i) == ploc (mynum)
	  && plev (mynum) < LVL_APPREN)
	{
	  bprintf ("\001p%s\003 blocks your way up!\n", pname (i));
	  return -1;
	}
#ifdef LOCMIN_ANCIENT
      if ((i = ploc (mynum)) == ploc ((max_players + MOB_ANCIENT_RATTLESNAKE))
	  && alive ((max_players + MOB_ANCIENT_RATTLESNAKE)) != -1
	  && (!ishere (OBJ_ANCIENT_CHAIN) || !ishere (OBJ_ANCIENT_RBLOCK) ||
	      !ishere (OBJ_ANCIENT_RCOINS) || !ishere (OBJ_ANCIENT_RPLATE)))
	{
	  hit_player (max_players + MOB_ANCIENT_RATTLESNAKE, mynum, -1);
	  return -1;
	}
#endif
    }
  
  if (n == EX_SOUTH)
    {
      
      if ((i = alive ((max_players + MOB_WINDOW_FIGURE))) != mynum && i != -1
	  && ploc (i) == ploc (mynum) && !iswornby (OBJ_WINDOW_BLACKROBE, mynum))
	{
	  bprintf ("\001pThe Figure\003 holds you back!\n");
	  bprintf ("\001pThe Figure\003 says 'Only true sorcerors may pass.'\n");
	  return -1;
	}
      
      for (i = max_players; (!mtstflg (i, MFL_BAR_S) || ploc (i) != ploc (mynum)
			     || alive (i) == -1) && i < numchars; i++);
      if (mtstflg (i, MFL_BAR_S) && alive (i) != -1 && ploc (i) == ploc (mynum)
	  && plev (mynum) < LVL_APPREN)
	{
	  bprintf ("\001p%s\003 holds you back!\n", pname (i));
	  return -1;
	}
#ifdef LOCMIN_KASTLE
      if (newch == LOC_KASTLE_11 && ploc(mynum) == LOC_KASTLE_10)
	if (!iswornby(OBJ_KASTLE_SKIS, mynum)) 
	  {
	    teletrap (LOC_KASTLE_24);
	    return 0;
	  }
	else 
	{
bprintf("You shuss down the glacier like an expert on your skis.\n");
      questsdone[Q_SKI] = True;
      if (!qtstflg(mynum, Q_SKI))
        {
          qsetflg(mynum, Q_SKI);
          send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, LVL_MAX,
                   NOBODY, NOBODY, "[%s solved the SkiLesson Quest]\n", pname(mynum));
          bprintf("You have successfully completed your Skiing lesson!\n");
        }
}
#endif
    }
  if (n == EX_EAST)
    {
      if ((i = alive ((max_players + MOB_OAKTREE_VIOLA))) != mynum && i != -1
	  && ploc (i) == ploc (mynum) &&
	  carries_obj_type (i, OBJ_OAKTREE_FAN) == -1)
	{
	  bprintf ("\001pViola\003 says 'How dare you come to our land!  "
		   "Leave at once!'\n");
	  return -1;
	}
#ifdef LOCMIN_OAKTREE
      if ((alive(max_players + MOB_OAKTREE_ENT) != -1) &&
	  (ploc(max_players + MOB_OAKTREE_ENT) == ploc(mynum)) &&
	  (strcmp(pname(max_players + MOB_OAKTREE_ENT), "") != 0))
	{
	  bprintf("The Ent pushes its branches in your way, preventing you from moving east.\n");
	  return -1;
	}
#endif
      
      for (i = max_players; (!mtstflg (i, MFL_BAR_E) || ploc (i) != ploc (mynum)
			     || alive (i) == -1) && i < numchars; i++);
      if (mtstflg (i, MFL_BAR_E) && alive (i) != -1 && ploc (i) == ploc (mynum)
	  && plev (mynum) < LVL_APPREN)
	{
	  bprintf ("\001p%s\003 won't let you go East!\n", pname (i));
	  return -1;
	}
      
    }
  if (ltstflg (newch, LFL_ON_WATER))
    {
      if (plev (mynum) < LVL_APPREN && (!carries_boat (mynum) && !ststflg(mynum, SFL_GHOST)))
	{
	  
	  bprintf ("You need a boat to go to sea!\n");
	  return -1;
	}
    }
  if (ltstflg (newch, LFL_IN_WATER))
    {
      if (plev (mynum) < LVL_APPREN)
	{
          if ((!iswornby(OBJ_TREEHOUSE_AMULET, mynum)) && 
	      (!iswornby(OBJ_BLIZZARD_TALISMAN, mynum)))
	    {
	      bprintf ("You'd surely drown!\n");
	      return -1;
	    }
	  else
	    {
	      bprintf("You see bubbles rising as you exhale under water.\n");
	    }
        }
    }
  if (n == EX_DOWN && ploc (mynum) == LOC_OAKTREE_LANDING)
    {
      bprintf ("You slide down the banister.  Wheee!\n");
      
      send_msg (ploc (mynum), 0, pvis (mynum), LVL_MAX, mynum, NOBODY,
		"%s slides down the banister shouting 'Yippeee...'\n",
		pname (mynum));
      
      send_msg (newch, 0, pvis (mynum), LVL_MAX, mynum, NOBODY,
		"%s slides down the banister and lands at your feet.\n",
		pname (mynum));
      
      if (oloc (OBJ_OAKTREE_MARBLEBUST) == newch)
	{
	  bprintf ("On your way down, you smash a valuable bust.\n");
	  
	  send_msg (newch, 0, pvis (mynum), LVL_MAX, mynum, NOBODY,
		    "%s smashed a valuable bust on %s way down.\n",
		    pname (mynum), his_or_her (mynum));
	  
	  destroy (OBJ_OAKTREE_MARBLEBUST);
	  create (OBJ_OAKTREE_BUSTBROKEN);
	}
    }
  else if (mynum < max_players && cur_player->asmortal > 0)
    {
      send_msg (ploc (mynum), 0, max (LVL_APPREN, pvis (mynum)), LVL_MAX,
		mynum, NOBODY, "%s\n",
		build_setin (block, cur_player->setout, pname (mynum), exittxt[n]));
      send_msg (newch, 0, max (LVL_APPREN, pvis (mynum)), LVL_MAX,
		mynum, NOBODY, "%s\n",
		build_setin (block, cur_player->setin, pname (mynum), NULL));
      if (pvis (mynum) < LVL_APPREN)
	{
	  send_msg (ploc (mynum), 0, pvis (mynum), LVL_APPREN, NOBODY, NOBODY,
		    "%s has gone %s.\n", pname (mynum), exittxt[n]);
	  send_msg (newch, 0, pvis (mynum), LVL_APPREN, NOBODY, NOBODY,
		    "%s has arrived.\n", pname (mynum));
	}
    }
  else
    {
      
      send_msg (ploc (mynum), 0, pvis (mynum), LVL_MAX, mynum, NOBODY, "%s\n",
		build_setin (block, cur_player->setout, pname (mynum), exittxt[n]));
      
      send_msg (newch, 0, pvis (mynum), LVL_MAX, mynum, NOBODY, "%s\n",
		build_setin (block, cur_player->setin, pname (mynum), NULL));
    }
  setpfighting (mynum, -1);
  myloc = ploc(mynum);
  trapch(newch);
  save_mynum = mynum;
  save_real_mynum = real_mynum;
  for (i=0; i<max_players; i++)
    if (players[i].iamon && (players[i].i_follow == save_mynum) && (ploc(i) == myloc))
      {
	setup_globals(i);
	dodirn(vb);
      }
  setup_globals(save_real_mynum);
  return 0;
}

int 
dogocom ()
{
  int a = (brkword () == -1) ? get_rand_exit_dir (ploc (mynum))
    : chklist (wordbuf, exittxt, exitnum) - 1;
  if (a < 0 || a >= NEXITS)
    {
      bprintf ("Go where?\n");
      return -1;
    }
  
  return dodirn (a + 2);
}













