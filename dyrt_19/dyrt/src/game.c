#include "verbs.h"
#include "kernel.h"
#include "stdinc.h"
#include "objects.h"
#include "mobiles.h"
#include "locations.h"
#include "game.h"

void becom (char *passwd)
{
  char x[128];
  PERSONA p;
  
   if (in_group(mynum))
    {
      bprintf("You can not use this command while you are in a group.\n");
      return;
    }
  if (passwd == NULL)
    {
      if (cur_player->polymorphed != -1 || cur_player->aliased)
        {
          bprintf ("Not when aliased.");
          return;
        }
      if (cur_player->writer != NULL)
        {
          bprintf ("Finish whatever you're writing first!\n");
          return;
        }
      if (pfighting (mynum) != -1)
        {
          bprintf ("Not while fighting!\n");
          return;
        }
      if (brkword () == -1)
        {
          bprintf ("Become what?  Inebriated?\n");
          return;
        }
      strcpy (cur_player->work2, wordbuf);
      strcpy (cur_player->cprompt, "Password: ");
      cur_player->no_echo = True;
      /*          IAC WILL ECHO */
      bprintf ("\n\377\373\001\001Password: ");
      push_input_handler (becom);
    }
  else
    {
      bprintf ("\377\374\001\001");
      cur_player->no_echo = False;
      pop_input_handler ();
      if (!getuaf (cur_player->work2, &p))
        {
          bprintf ("No such player.\n");
        }
      else if (is_player_banned(cur_player->work2)) {
        bprintf("Sorry, %s is banned.\n", cur_player->work2);
        return;
        }
      else if (!EQ (p.p_passwd, my_crypt (x, passwd, strlen (p.p_passwd) + 1)))
        {
          bprintf ("Incorrect password!\n");
        }
      else if (fpbns (cur_player->work2) >= 0)
        {
          bprintf ("That player is already on the game.\n");
        }
      else
        {
  cur_player->ghostcounter = 0;
  sclrflg(mynum, SFL_GHOST);
   strcpy (cur_player->setin, cur_player->ghostsetin);
   strcpy (cur_player->setout, cur_player->ghostsetout);
/* Again, in newer dyrt versions, need to use setstand and setsit */
/*   strcpy (cur_player->sethere, cur_player->ghostsethere); */
   strcpy (cur_player->setstand, cur_player->ghostsetstand);
   strcpy (cur_player->setsit, cur_player->ghostsetsit);
          saveme ();
          remove_entry (mob_id (mynum), &id_table);
	  
          pers2player (&p, mynum);
          strcpy (cur_player->prompt, cur_player->prompt);
          mudlog ("%s has BECOME %s", pname (mynum), p.p_name);
	  send_msg (DEST_ALL, MODE_QUIET, max (LVL_APPREN, pvis (mynum)),
		    LVL_MAX, mynum, NOBODY,
		    "[%s has BECOME %s]\n", pname (mynum), p.p_name);
          send_msg (ploc (mynum), 0, pvis (mynum), LVL_MAX, mynum, NOBODY,
                    "%s disintegrates, and reappears as %s.\n",
                    pname (mynum), p.p_name);
	  setpname (mynum, p.p_name);
          fetchprmpt (mynum);
          insert_entry (mob_id (mynum), mynum, &id_table);
	  bprintf ("You are now %s.\n", pname (mynum));
          if (phome(mynum) == LOC_JAIL_JAIL) {
            bprintf ("Gee..why didn't I think of that?\n");
            bprintf ("You are still jailed :)\n");
            setploc (mynum, LOC_JAIL_JAIL);
           }
        }
      get_command (NULL);
    }
}

void blowcom (void)
{
  int a;
  char s[100];
  
  if ((a = ohereandget ()) == -1)
    return;
  if (onum (a) == OBJ_CAVE_BAGPIPES)
    {
      broad ("\001dA &+yhideous&* wailing sounds echos all around.\n\003");
      return;
    }
  if (onum (a) == OBJ_OAKTREE_WHISTLE)
    {
      broad ("\001dA strange ringing fills your head.\n\003");
      if (alive (max_players + MOB_OAKTREE_OTTIMO) != -1)
        {
          bprintf ("A small dachshund bounds into the room "
                   "and leaps on you playfully.\n");
          sprintf (s, "A small dachshund bounds into the room "
                   "and leaps on %s playfully.\n", pname (mynum));
          sillycom (s);
          setploc (max_players + MOB_OAKTREE_OTTIMO, ploc (mynum));
        }
      if (ploc(mynum) == (oloc(OBJ_OAKTREE_TOPFOXHOLE)))
        {
          bprintf("Ottimo begins to dig, wagging his tail happily.\n");
          bprintf("Ottimo has uncovered a foxhole at the bottom of the oak!\n");
          setobjstate(OBJ_OAKTREE_TOPFOXHOLE, 0);
        }
      return;
    }
  
  if (onum (a) == OBJ_LABYRINTH_HORN)
    {
      broad ("\001dA mighty horn blast echoes around you.\n\003");
      if (ploc (mynum) >= LOC_SEA_12 && ploc (mynum) <= LOC_SEA_1 &&
          oarmor (OBJ_SEA_EXCALIBUR) == 0)
        {
          setoloc (OBJ_SEA_EXCALIBUR, ploc (mynum), IN_ROOM);
          setobjstate (OBJ_SEA_EXCALIBUR, 1);
          bprintf ("A hand breaks through the water holding up "
                   "the sword &+WExcalibur!&*\n");
          osetarmor (OBJ_SEA_EXCALIBUR, 1);
          questsdone[Q_EXCALIBUR] = True;
          if (!qtstflg(mynum, Q_EXCALIBUR)) 
	    {
	      qsetflg (mynum, Q_EXCALIBUR);
	      send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, LVL_MAX, 
		       NOBODY, NOBODY, "[%s solved the Excalibur quest]\n",pname(mynum));
	      bprintf("Congratulations!  You have solved the FindExcalibur Quest\n");
	    }
        }
      return;
    }
  bprintf ("You can't blow that.\n");
}

void beepcom (void)
{
  int vic;
  
  if (brkword () == -1)
    {
      bprintf ("Beep who?\n");
      return;
    }
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD)) {
    bprintf ("I think you should cool off a bit first.\n");
    return;
   }

  if ((vic = pl1) == -1)
    {
      bprintf ("There is no one with that name.\n");
      return;
    }
  if (vic == mynum)
    {
      bprintf ("You are funny.\n");
      return;
    }
  
  /* no beep to BUSY, DEAF, DUMB, or people in SOUNDPROOF rooms */
  
  if (ststflg (vic, SFL_BUSY) || ststflg (vic, SFL_DEAF) ||
      ststflg (vic, SFL_DUMB) ||
      ltstflg (ploc (vic), LFL_SOUNDPROOF))
    {
      bprintf ("Your spell fizzles and dies...\n");
      return;
    }
  sendf (vic, "%s is &+Ydesperately&* trying to get your attention.\007\n",
         see_player (vic, mynum) ? pname (mynum) : "Someone");
  bprintf ("You send %s annoying &+Gbeeps&*.\n", psex (vic) ? "her" : "him");
}


void homecom (void)
{
  if (phome (mynum) == 0 || plev (mynum) < LVL_APPREN || !exists (phome (mynum)))
    {
      bprintf ("You are but without a home, my friend.\n");
      return;
    }
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD)) {
    bprintf ("Until you are let out, this IS your home.\n");
    return;
    }
  if (ltstflg (phome (mynum), LFL_PRIVATE))
    {
      if (plev (mynum) < LVL_GOD)
	if (count_players (phome (mynum), LVL_MIN, LVL_MAX, COUNT_PLAYERS) > 1)
	  {
	    bprintf ("Your home seems to be full at the moment.\n");
	    return;
	  }
    }
  bprintf ("You suddenly feel homesick... and click your heels three times.\n");
  teletrap (phome (mynum));
  return;
}

void comparecom()
{
  if (EMPTY(item1) || EMPTY(item2)) 
    {
      bprintf ("You must specify two items to compare.\n");
      return;
    }
  if (!iscarrby(ob1, mynum)) 
    {
      bprintf ("You don't have a %s.\n", item1);
      return;
    }
  if (!iscarrby(ob2, mynum)) 
    {
      bprintf ("You don't have a %s.\n", item2);
      return;
    }
  if ((oarmor(ob1) < 1 && odamage(ob1) < 1) ||
      (oarmor(ob2) < 1 && odamage(ob2) < 1)) 
    {
      bprintf("The gods save you points by telling you that either %s or %s is "
	      "neither\narmor nor weapon.\n", oname(ob1), oname(ob2));
      return;
    }
  if (pscore(mynum) < 2500) 
    {
      bprintf ("You don't have enough score to compare objects.\nYou need"
	       "2500 to use this command.\n");
      return;
    } 
  else 
    {
      setpscore (mynum, pscore (mynum) - 500);
      bprintf("You offer your 500 score sacrifice in hopes of answered"
	      " prayers.\n");
      bprintf("After a few moments you hear their reply and jot it down.\n");
      bprintf("Your notes look like this...\n\n");
      if (oarmor(ob1) != oarmor(ob2)) {
	bprintf("Better Armor == ");
	if (oarmor(ob1) > oarmor(ob2))
	  bprintf("&+G%s&*\n", oname(ob1));
	else
	  bprintf("&+G%s&*\n", oname(ob2));
      } else
	bprintf("&+GBoth same armor value.&*\n");
      if (odamage(ob1) != odamage(ob2)) 
	{
	  bprintf("Better Damage == ");
	  if (odamage(ob1) > odamage(ob2))
	    bprintf("&+C%s&*\n", oname(ob1));
	  else
	    bprintf("&+C%s&*\n", oname(ob2));
	} 
      else
	bprintf("&+CBoth same damage value.&*\n");
    }
}

void sitcom (void)
{
  if (psitting (mynum))
    {
      bprintf ("You're already sitting.\n");
      return;
    }
  else if (pfighting (mynum) >= 0)
    {
      bprintf ("You want to sit down while fighting?  Do you have a death "
               "wish or something?\n");
      return;
    }
#ifdef LOCMIN_ICECAVE
  if (ploc(mynum) == LOC_ICECAVE_THRONE)
    {
      if (oloc(OBJ_ICECAVE_WESTICETHRONE))
	{
	  bprintf("The throne collapses under your weight, revealing a passage east.\n");
	  setobjstate(OBJ_ICECAVE_WESTICETHRONE, 0);
	  return;
	}
    }
#endif

#ifdef LOCMIN_JBS
  if (ploc(mynum) == LOC_ICECAVE_THRONE)
    {
      bprintf("When you look into the mugwumps beady eyes,"
                 " you are enslaved to the interzone.\n");
      crapup("You spend eternity sitting here, and eventually die", SAVE_ME);
      return;
    }
#endif
  else
    {
      bprintf ("You assume the lotus position.\n");
      send_msg (ploc (mynum), 0, pvis (mynum), LVL_MAX, mynum, NOBODY,
                "%s sits down.\n", pname (mynum));
      setpsitting (mynum, 1);
    }
}

void standcom (int player)
{
  if (!psitting (player))
    {
      sendf (player, "You're already standing.\n");
      return;
    }
  else
    {
      sendf (player, "You clamber to your feet.\n");
      send_msg (ploc (player), 0, pvis (player), LVL_MAX, player, NOBODY,
		"%s clambers to %s feet.\n", pname (player), his_or_her (player));
      setpsitting (player, 0);
    }
}

void ticklecom (void)
{
  int a;
  
  if (ob1==OBJ_FOREST_INSIDETREE)
    {
      bprintf("You tickle the tree into paroxyms of giggles and manage to escape.\n");
      trapch(LOC_FOREST_F4);
     /*
mortals cheating with this.
 setpscore(mynum, pscore(mynum)+100);
*/
      ++oarmor(OBJ_FOREST_TREEEATING);
      return;
    }
  if ((a = vichere ()) < 0)
    return;
  if (a == mynum)
    {
      bprintf ("You tickle yourself.\n");
      return;
    }
  sillytp (a, "tickles you.");
  bprintf ("You tickle %s.\n", him_or_her (a));
}

void diagnosecom(void)
{
  static char *t[] = 
    {
      "&+R%s near death.&*\n",
      "&+R%s near death.&*\n",
      "&+R%s mortally wounded.&*\n",
      "&+R%s seriously wounded.&*\n",
      "&+R some wounds, but %s still fairly strong.&*\n",
      "&+R minor cuts and abrasions.&*\n",
      "&+R minor cuts and abrasions.&*\n",
      "&+R feels a bit dazed.&*\n",
      "&+R%s in better than average condition.&*\n",
      "&+R%s in exceptional health.&*\n",
      "&+R%s in exceptional health.&*\n"
    };
  
  int p, x, plx=pfighting (mynum);
  
  if (pfighting(mynum) < 0) 
    {
      bprintf ("You aren't fighting anyone!\n");
    } 
  else 
    {
      if (plx >= max_players) 
	{
	  x = pstr_reset (plx);
	} 
      else 
	{
	  x = maxstrength (plx);
	}
      if (pstr(plx) < 0)
	p = -1;
      else if (pstr(plx) > x)
	p = 11;
      else
	p = (int) (pstr(plx) * 10 / x);
      bprintf("&+R%s&* ", pname(plx));
      if (p >= 4 && p <= 6)
	bprintf ("&+R%s&*", "&+Rhas");
      if (p >= 0 && p <= 10)
	bprintf (t[p], "&+Ris");
      else
	bprintf ("&+Rmust %s&*", p < 0 ? "&+Rbe undead!&*\n" :
		 "&+Rhave had Wheaties for breakfast!&*\n");
    }
}

void digcom(void)
{
if (ststflg(mynum, SFL_GHOST)) {
 bprintf("Your ghostly fingers pass right through the ground!\n");
 return; }

#ifdef LOCMIN_ICECAVE
  if (ploc (mynum) == oloc (OBJ_ICECAVE_HOLE_SNOW_CAVE) ||
      ploc (mynum) == oloc (OBJ_ICECAVE_HOLE_G_HALL))
    {
      if (state (OBJ_ICECAVE_HOLE_G_HALL) == 1)
	{
	  if (iscarrby (ob1, mynum) && otstbit (ob1, OFL_WEAPON))
	    {
	      bprintf ("You rapidly dig through to another passage.\n");
	      setobjstate (OBJ_ICECAVE_HOLE_G_HALL, 0);
	      return;
	    }
	  else
	    {
	      bprintf ("The ice and snow are thick, you'll need something "
		       "to hack through it with.\n");
	      return;
	    }
	}
      else
	{
	  bprintf ("You widen the hole but with little effect.");
	  return;
	}
    }
#endif
  
#ifdef LOCMIN_KASTLE
  if (ploc(mynum) == oloc(OBJ_KASTLE_SNOW_ICE))
    {
      bprintf("You dig a passage up through the snow and ice.\n");
      setobjstate(OBJ_KASTLE_SNOW_ICE, 0);
      setobjstate(OBJ_BLIZZARD_SNOW_ICE, 0);
      return;
    }
  if (ploc(mynum) == oloc(OBJ_KASTLE_HOLE))
    {
      bprintf("You dig through the thick snow to form a passage leading down.\n");
      setobjstate(OBJ_KASTLE_HOLE, 0);
      setobjstate(OBJ_KASTLE_HOLE_1, 0);
      return;
    }
#endif
  
#ifdef LOCMIN_LEDGE
  if ((ploc (mynum) == oloc (OBJ_LEDGE_TUNNWEST) ||
       ploc (mynum) == oloc (OBJ_LEDGE_TUNNEAST)) &&
      state (OBJ_LEDGE_TUNNEAST) == 1)
    {
      bprintf ("You dig your way through the rockfall, and soon clear "
	       "the passage.\n");
      setobjstate (OBJ_LEDGE_TUNNEAST, 0);
      return;
    }
#endif
  
#ifdef LOCMIN_ABYSS
  if ((ploc (mynum) == oloc (OBJ_ABYSS_BONESSTAIRS) ||
       ploc (mynum) == oloc (OBJ_ABYSS_PLAINSTAIRS)) &&
      state (OBJ_ABYSS_PLAINSTAIRS) == 1)
    {
      bprintf ("You dig your way through the rockfall, and soon clear "
	       "the passage.\n");
      setobjstate (OBJ_ABYSS_PLAINSTAIRS, 0);
      return;
    }
#endif
  
#ifdef LOCMIN_CHURCH
  if (ploc (mynum) == oloc (OBJ_CHURCH_SLAB_TOP) &&
      otstbit (OBJ_CHURCH_SLAB_TOP, OFL_DESTROYED))
    {
      bprintf ("You uncover a stone slab!\n");
      oclrbit (OBJ_CHURCH_SLAB_TOP, OFL_DESTROYED);
      return;
    }
#endif
  
#ifdef LOCMIN_OAKTREE
  if (ploc (mynum) == oloc (OBJ_OAKTREE_TOPFOXHOLE) &&
      state (OBJ_OAKTREE_TOPFOXHOLE) == 1)
    {
      bprintf ("Although there are fox tracks in the grass, you can't "
	       "find the fox hole.\n");
      return;
    }
#endif
  
#ifdef LOCMIN_NIBELUNG
  if (((ploc(mynum) == oloc(OBJ_NIBELUNG_AZURITE)) == LOC_NIBELUNG_5)
      
      && (otstbit(OBJ_NIBELUNG_AZURITE, OFL_DESTROYED)))
    {
      bprintf("You dig up the azurite\n");
      oclrbit(OBJ_NIBELUNG_AZURITE, OFL_DESTROYED);
      return;
    }
#endif
#ifdef LOCMIN_JBS /* code written by Rendor(ken@esu.edu) Nov 1993 */
  if( ploc(mynum) == oloc(OBJ_JBS_FLOP_PASTURE) ) {
    if( state(OBJ_JBS_FLOP_PASTURE) == 1 ) {
      if( iscarrby(ob1, mynum) && ob1==OBJ_JBS_SHOVEL ) {
        bprintf("You shovel the flop out of the way and reveal a tunnel!\n");
        setobjstate(OBJ_JBS_FLOP_PASTURE,0);
        return;
      }
      bprintf("Your hands sink deep into the flop and all you manage to do \
is smell bad.\n");
      return;
    }
    else {
      bprintf("You make the path a little wider.\n");
      return;
    }
  }
#endif

  bprintf ("You find nothing.\n");
  return;
}

void playcom (void)
{
#ifdef LOCMIN_FOREST
  if (ob1 == OBJ_FOREST_PIPES)
    {
      bprintf("Much as you try, the pipes only make nasty &=yMsqueak&*ing noises.\n");
      return;
    }
#endif
  
#ifdef LOCMIN_CAVE
  if (ob1 == OBJ_CAVE_BAGPIPES)
    {
      broad("There is a hideous &=mRwailing&* noise.\n");
      return;
    }
#endif
  
#ifdef LOCMIN_WINDOW
  if (ob1 == OBJ_WINDOW_HARP)
    {
      bprintf("A beautiful harp melody &+Cfloats&* to you on a gentle breeze.\n");
      return;
    }
#endif
}
void scorecom (void)
{
  static char *t[] =
    {
      "&+C%s near death.&*\n",
      "&+C%s near death.&*\n",
      "&+C%s mortally wounded.&*\n",
      "&+C%s seriously wounded.&*\n",
      "&+C some wounds, but %s still fairly strong.&*\n",
      "&+C minor cuts and abrasions.&*\n",
      "&+C minor cuts and abrasions.&*\n",
      "&+C feel a bit dazed.&*\n",
      "&+C%s in better than average condition.&*\n",
      "&+C%s in exceptional health.&*\n",
      "&+C%s in exceptional health.&*\n"
    };
  
  int p;
  int plx = mynum;
  
  if (plev(plx) >= LVL_APPREN && brkword() != -1 && (plx = fpbn (wordbuf)) < 0)
    {
      bprintf ("Hmm...%s seems to be hiding...\n", wordbuf);
    }
  else if (plx >= max_players)
    {
      if (plx == mynum)
	{
	  bprintf ("Your ");
	}
      else
	{
	  bprintf ("%s's ", pname (plx));
	}
      bprintf ("strength is %d.\n", pstr (plx));
    }
  
  else
    {
      p = (int) (pstr (plx) * 10 / maxstrength (plx));
      if (p > 10)
	p = 10;
      if (plx == mynum)
	{
	  bprintf("&+Y[&*%s&+Y]&*\n\n", make_title(ptitle(plx), pname(plx)));
	  bprintf("&+BStrength:&*\t\t%d/%d\n", pstr(plx), maxstrength(plx));
	  bprintf("&+BScore:&*\t\t\t%d\n", pscore(plx));
	  if (plev(plx) < LVL_APPREN)
	    {
	      bprintf("&+BLevel:&*\t\t\t%d\n", plev(plx));
	      bprintf("&+BPoints to next level:&*\t%d\n",
		      levels[plev(plx)+1] - pscore(plx));
	    }
	  bprintf("\n");
	  if (ststflg(plx, SFL_GHOST)) {
           bprintf("You are a ghost!\n"); }
	  else {
	  bprintf ("&+CYou &*");
	  if (p >= 4 && p <= 6)
	    bprintf ("have");
	  bprintf (t[p], "are"); }
	  bprintf("&+CThe value of your possessions is %d.&*\n",
		  value_class(findclass("all"), plx, True));
	}
      else
	{
	  bprintf("&+Y[&*%s&*Y]&*\n\n", make_title(ptitle(plx), pname(plx)));
	  bprintf("&+BStrength:&*\t%d/%d\n", pstr(plx), maxstrength(plx));
	  bprintf("&+BScore:&*\t\t%d\n", pscore(plx));
	  if (plev(plx) < LVL_APPREN)
	    {
	      bprintf("&+BLevel:&*\t\t%d\n", plev(plx));
	      bprintf("&+BPoints to next level:&*\t%d\n",
		      levels[plev(plx)+1] - pscore(plx));
	    }
	  bprintf("\n");
          if (ststflg(plx, SFL_GHOST)) {
           bprintf("%s is a ghost!\n", pname(plx)); }
          else {
	  bprintf ("%s ", pname (plx));
	  if (p >= 4 && p <= 6)
	    bprintf ("has ");
	  bprintf (t[p], "is"); }
	  bprintf("The value of %s's possessions is %d.\n", pname(plx),
		  value_class(findclass("all"), plx, True));
	}
    }
}

void meditatecom(void)
{
#ifdef LOCMIN_ANCIENT
  if (ploc (mynum) == LOC_ANCIENT_ANC59)
    {
      if (oarmor (OBJ_ANCIENT_LBOOK) == 0)
	{
	  bprintf ("You &+bmeditate&*, but nothing seems to happen.\n");
	}
      else
	{
	  bprintf ("&+RYou are teleported!&*\n\n\n");
	  trapch (LOC_ANCIENT_ANC35);
	}
    }
#endif
  bprintf ("You &+bmeditate&* quietly in the corner.\n");
}

void ringcom(void)
{
  switch(ob1)
    {
    case OBJ_VILLAGE_BELL:
      broad("There is an almighty &+BBONG!&*\n");
      break;
    default:
      bprintf("You can't ring that.\n");
      break;
    }
}
