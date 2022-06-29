#include "kernel.h"
#include "verbs.h"
#include "mobiles.h"
#include "objects.h"
#include "locations.h"
#include "stdinc.h"

struct _spell
{
  int verb;
  char mob_chance;	/* If caster is a mobile, chance to cast the spell */
  char mob_cast_mflag;	/* mflag to see if mobile can cast this spell */
  short mob_dam;	/* If caster is mobile, damage is mob_dam */
  short chance;		/* Chance to succeed */
  short like_mflag;	/* mflag to see if victim likes the spell. */
  short fear_mflag;	/* mflag to see if factor 6 should be used */
  short maxlev;		/* If caster is above this level, treat him as this */
  short obj3first;	/* First index in spell_obj[] for factor 3 object */
  short obj3next;	/* first index beyond list */
  char *like_it;	/* Text to tell caster if victim likes the spell */
  char *to_room;	/* Msg to all in room, including victim, excl caster */
  char *to_victim;	/* Msg to victim */
  char *to_others;	/* Msg to others in room */
};

Boolean cast_spell (int caster, int victim, int spell_type);

struct _spell spell_table[] =
{
  {VERB_FIREBALL, 15, MFL_CFIREBALL, 10,
     5, MFL_NFIREBALL, MFL_NFROST, -1,
     0, 2,
     "&+R%s enjoys the heat and gets STRONGER!&*\n",
     "\001A\033[31m\003\001p%s\003 casts a &+Rfireball&*.\001A\033[0m\003\n",
     "&+RYou are struck!&*\n",
     "&+R\001p%s\003 is struck!&*\n"},
  {VERB_FROST, 25, MFL_CFROST, 10,
     5, MFL_NFROST, MFL_NFIREBALL, -1,
     2, 3,
     "&+C%s likes the cold and gets STRONGER!&*\n",
     "\001A\033[31m\003\001p%s\003 fires an &+Cicy&* ray!\001A\033[0m\003\n",
     "&+CYou are chilled to the bone!&*\n",
     "&+C\001p%s\003 is struck!&*\n"},
  {VERB_MISSILE, 20, MFL_CMISSILE, 10,
     7, MFL_NMISSILE, -1, LVL_SIX,
     3, 4,
     "&+M%s likes your magic missile and gets STRONGER!&*\n",
     "Bolts of &+Mfire&* leap from the fingers of \001p%s\003\n",
     "&+MYou are struck!&*\n",
     "&+M\001p%s\003 is struck!&*\n"},
  {VERB_SHOCK, 15, MFL_CSHOCK, 8,
     5, MFL_NSHOCK, -1, -1,
     4, 5,
     "&+Y%s likes the shock and gets STRONGER!&*\n",
     "&+YSparks&* fly from the fingers of \001p%s\003!\n",
     "&+Y\001A\033[1;36m\003\001p%s\003 touches you, giving you a "
       "strong electric shock!&*\001A\033[0m\003\n",
       "&+Y\001p%s\003 is struck!&*\n"},
       {-1}
};

short spell_objs[] =
{
  OBJ_ISLAND_SULPHUR, 
  OBJ_EFOREST_SULPHUR,
  OBJ_BLIZZARD_WINTERGREEN,
  OBJ_BLIZZARD_TALISMAN,
  OBJ_VALLEY_LODESTONE
};


/* Calculate damage done by a player 'pl' carrying item 'it'.
 * If no item, return damage of player. If it is not a weapon, return -1
 * else return damage of player + damage of weapon.
 */
int dambyitem (int pl, int it)
{
  if (it == -1)
    {
      return pdam (pl);
    }
  return !otstbit (it, OFL_WEAPON) ? -1 : odamage (it) + pdam (pl);
}

/* Command function block to handle wielding of weapons. */
void wieldcom (void)
{
  int a;
  
  if ((a = ob1) == -1)
    {
      bprintf ("What's that?\n");
      return;
    }
  else if (!iscarrby (a, mynum))
    {
      bprintf ("You're not carrying the %s!\n", item1);
      set_weapon (mynum, -1);
      return;
    }
  else if (!otstbit (a, OFL_WEAPON))
    {
      bprintf ("It's not a weapon.\n");
      set_weapon (mynum, -1);
      return;
    }
  set_weapon (mynum, a);
  send_msg (ploc (mynum), 0, pvis (mynum), LVL_MAX, mynum, NOBODY,
	    "%s wields the %s.\n", pname (mynum), oname (a));
  bprintf ("You are now wielding the %s.\n", oname (a));
}

/*  The killing and hitting command */
static void wreckdoor (void);
void killcom (void)
{
  if (pfighting (mynum) >= 0)
    {
      bprintf ("You're already fighting!\n");
      return;
    }
  if (pl1 == -1 && ob1 == -1)
    {
      bprintf ("Kill who?\n");
      return;
    }
  if (ststflg(mynum, SFL_GHOST)) 
    {
      bprintf("Your hands slide right through them!\n");
      return; 
    }
  if (EQ (item1, "door"))
    {
      if (!ptstflg (mynum, PFL_WRECK))
	{
	  bprintf ("What are you trying to do?  Wreck the door?\n");
	}
      else
 	{
	  wreckdoor ();
	}
      return;
    }
  if (ob1 != -1)
    {
      breakitem (ob1);
      return;
    }
  if (pl1 == -1)
    {
      bprintf ("You can't do that.\n");
      return;
    }
  if (pl1 == mynum)
    {
      bprintf ("Come on, it'll look better tomorrow...\n");
      return;
    }
  if (ploc (pl1) != ploc (mynum))
    {
      bprintf ("They aren't here.\n");
      return;
    }
  if (testpeace (mynum))
    {
      bprintf ("Sorry, it's too peaceful for violence.\n");
      return;
    }
  if (ptstflg (pl1, PFL_NOHASSLE))
    {
      bprintf ("Something interferes with your attack.\n");
      return;
    }
#ifdef LOCMIN_PIRATE
  if ((pl1 == MOB_PIRATE_MERMAID + max_players) && (!ststflg (mynum, SFL_DEAF)))
    {
      bprintf ("Her music is too beautiful.  You can't bring yourself to "
	       "attack her.\n");
      return;
    }
#endif /* LOCMIN_PIRATE */
  
  setpfighting (mynum, pl1);
  hit_player (mynum, pl1, (ob2 == -1) ? pwpn (mynum) : ob2);
  if (pl1 < max_players && plev (mynum) < LVL_APPREN && plev (pl1) < LVL_APPREN)
    {
      send_msg (DEST_ALL, MODE_QUIET | MODE_COLOR, LVL_APPREN, LVL_MAX, NOBODY,
                NOBODY, "[%s attacked %s]\n", pname (mynum), pname (pl1));
    }
  if (pl1 < max_players && plev(mynum) < LVL_APPREN && plev(pl1) < LVL_APPREN &&
     (!the_world->w_tournament))
    {
      send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, LVL_MAX,
	       NOBODY, NOBODY, "[Peace declared by Game Driver]\n");
      the_world->w_peace = 1;
      mudlog("JFK: %s attacked %s", pname(mynum), pname(pl1));
      bprintf("\nThe Game Driver tells you 'I cannot allow you to attack "
              "%s.'\n", pname(pl1));
      bprintf("The Game Driver tells you 'Try to behave yourself.'\n");
      setpfighting(mynum, -1);
      setpfighting(pl1, -1);
      the_world->w_peace = 0;
      send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, LVL_MAX,
	       NOBODY, NOBODY, "[War declared by Game Driver]\n");
    }
  return;
}

void breakitem (int x)
{
  switch (x)
    {
    case OBJ_BLIZZARD_RESET_STONE:
      sys_reset ();
      break;
    case OBJ_VILLAGE_PEBBLE:
      bprintf("The pebble gets annoyed and goes to philosophize elsewhere.\n");
      setoloc(OBJ_VILLAGE_PEBBLE, LOC_DEAD_DESTROYED, IN_ROOM);
      return;
    case OBJ_QUARRY_ROCK:
      bprintf ("You smash it apart to reveal a gem inside.\n");
      create (OBJ_QUARRY_GEM);
      setoloc (OBJ_QUARRY_GEM, oloc (OBJ_QUARRY_ROCK), ocarrf (OBJ_QUARRY_ROCK));
      destroy (OBJ_QUARRY_ROCK);
      break;
    case -1:
      bprintf ("What's that?\n");
      break;
    default:
      bprintf ("You can't do that.\n");
      break;
    }
}

static void wreckdoor (void)
{
  int a;
  char ms[80];
  
  if ((a = fobna ("door")) == -1)
    {
      bprintf ("Sorry, no doors here.\n");
    }
  else if (state (a) == 0)
    {
      bprintf ("It's already open.\n");
    }
  else
    {
      setobjstate (a, 0);
      bprintf ("The door flies open!\n");
      sprintf (ms, "%s hits the door...  It flies open!\n", pname (mynum));
      sillycom (ms);
      sprintf (ms,
	       "\001dYou hear the distinctive crunch of %s meeting a door.\n\003",
	       pname (mynum));
      broad (ms);
    }
}

Boolean testpeace (int player)
{
  return (the_world->w_peace || ltstflg (ploc (player), LFL_PEACEFUL));
}

void fleemob (int plx)
{
  static char *fleemsg[]=
    {
	"%s says &+C'Yikes, I'm outta here.'&*\n",
	"%smake a frantic attempt to flee.\n",
	"%s says &+C'It's been nice talking to you, but I gotta go.'&*\n",
	"%s ducks the attack and heads for one of the exits.\n"
    };
      sendf(ploc(plx), fleemsg[my_random() %arraysize(fleemsg)], pname(plx));
      movemob(plx);
}

void fleecom (void)
{
  int pf,oldloc,newloc;

  if ((pf = pfighting(mynum)) < 0)
    {
      dogocom ();
	return;
    }
  if (carries_obj_type (mynum, OBJ_CASTLE_RUNESWORD) > -1)
    {
      bprintf ("The sword won't let you!\n");
      return;
    }
  set_hate(pfighting(mynum), mynum);
  setpfighting(mynum,-1);
  oldloc = ploc(mynum);
  dogocom();
  if(oldloc == ploc(mynum)) /* we never moved... */
    {
       setpfighting(mynum, pf);
       return;
    }
  newloc=ploc(mynum);
  setploc(mynum,oldloc);
    send_msg (ploc (mynum), 0, pvis (mynum), LVL_MAX, mynum, NOBODY,
		"%s drops things as %s make a frantic attempt to escape.\n",
		pname (mynum), psex (mynum) ? "she" : "he");
      drop_some_objects (mynum);
      setpscore (mynum, pscore (mynum) - pscore (mynum) / 33);	/* lose 3% */
      calibme ();
      setpfighting (mynum, -1);
      if (cur_player->i_follow >= 0)
	{
	  bprintf ("You stopped following %s.\n", pname (cur_player->i_follow));
	  cur_player->i_follow = -1;
	}
  setploc(mynum,newloc);
}

int mob_cast_spell (int caster)
{
  struct _spell *s;
  
  /* caster is a mobile, see if we let him cast a spell... */
  
  for (s = spell_table; s->verb >= 0; s++)
    {
      if (mtstflg (caster, s->mob_cast_mflag) == 0)
 	{
	  continue;
	}
      if (randperc () < s->mob_chance)
	{
	  return s->verb;
	}
    }
  return -1;
}

void hit_player (int attacker, int victim, int weapon)
{
  int x, cth, i, d, w;
  int mob = victim - max_players;
  
  if ((x = attacker) >= max_players)
    {
      x = victim;
    }
  if (attacker >= numchars || victim >= numchars || attacker < 0 ||
      victim < 0 || testpeace (victim) || ploc (attacker) != ploc (victim)
      || !is_in_game (attacker) || !is_in_game (victim))
    {
      if (attacker >= numchars)
	{
	  set_hate(attacker, victim);
	}
      return;
    } 
  /* if the attacker is played by a human: */
  if (attacker < max_players || ststflg (attacker, SFL_OCCUPIED) != 0)
    {
      /* if the attacker is a mortal: */
      if (plev (attacker) < LVL_APPREN || ststflg (attacker, SFL_OCCUPIED) != 0)
	{
	  /* if the victim is a mobile: */
	  if (victim >= max_players)
	    {
	      if (mtstflg (victim, MFL_BLIND) != 0 &&
                  !ststflg (attacker, SFL_BLIND))
		{
		  sendf (attacker, "%s scratches your eyes out! You are "
                         "blind!\n", pname (victim));
		  ssetflg (attacker, SFL_BLIND);
		}
	      if (mob == MOB_ABYSS_GUXX && weapon == OBJ_ABYSS_LANCE &&
		  (!state (OBJ_ABYSS_LANCE)))
		{
		  sendf (attacker, "Guxx says 'Guxx will win, this demon knows,\n");
		  sendf (attacker, "	       'for with every rhyme...Errrk!\n");
		  sendf (attacker, "Guxx collapses in a heap as you hit him "
                         " with your lance.\n");
		  setpscore (attacker, pscore (attacker) + 750);
                  questsdone[Q_GUXX] = True;
                  if (!qtstflg(attacker, Q_GUXX))
                    {
                      qsetflg(attacker, Q_GUXX);
                      send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, 
                               LVL_MAX, NOBODY, NOBODY, 
                               "[%s solved the Guxx Quest]\n", pname(attacker));
                      sendf(attacker, "\nYou have solved the Guxx Quest!\n");
                    }
		  wound_player (attacker, victim, pstr (victim) + 1, -1);
		  return;
		}
              if (mob == MOB_NARNIA_FENRIS && weapon == OBJ_NARNIA_RHINDON)
                {
                  sendf(attacker, "You drive the blade of King Peter's sword "
                        "deep into Fenris Ulf's side, and he collapses to the "
                        "ground, gasping for air.  After a few seconds, he "
                        "dies silently.\n");
                  wound_player(attacker, victim, pstr(victim) + 1, -1);
                  return;
	        } 
	      if (mob == MOB_CAVE_DRAGON)
		{
		  if (iswielded(OBJ_ICECAVE_DAGGER) && 
                      iscarrby (OBJ_ICECAVE_DAGGER, attacker))
		    {
		      sendf(attacker, "The Dragon glares at you and prepares "
                            "to blast you out of existence with his fiery "
                            "breath... but the icy blade sinks deep into the "
                            "dragon's hide.  The Dragon shudders.\n");
		      setpscore(attacker, pscore(attacker) + 500);
                      questsdone[Q_DRAGON] = True;
                      if (!qtstflg(attacker, Q_DRAGON))
                        {
                          qsetflg(attacker, Q_DRAGON);
                          send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN,
                                   LVL_MAX, NOBODY, NOBODY, 
                          "[%s solved the Dragon Quest]\n", pname(attacker));
                          bprintf("\nYou have solved the Dragon Quest!\n");
                        }
		      wound_player(attacker, victim, pstr(victim) +1, -1);
		      return;
		    }
		} 
	      if (mob == MOB_VALLEY_CHICKEN && !psex (victim))
		{
		  sendf (attacker, "As you look up you see the sky is somehow "
                         "different. The next second you're crushed to death "
                         "as the sky hits you!\n");
		  send_msg (ploc (attacker), 0, 
                            pvis (attacker) > 0 ? pvis (attacker) : LVL_MIN, 
                            LVL_MAX, attacker, NOBODY, "%s is squashed as %she "
                            "attacks Chicken Licken.\n", pname (attacker), 
                            psex (attacker) ? "s" : "");
		  p_crapup (attacker, "\t\tChicken Licken was right!", 
                            CRAP_SAVE | CRAP_RETURN);
		  return;
		}
	      else if (mob == MOB_CATACOMB_SERAPH)
		{
		  sendf (attacker, "The Seraph is amused by your "
                         "foolhardiness.  For your impudence, he draws his "
                         "flaming sword and slays you with a single blow.\n");
		  send_msg (ploc (attacker), 0, 
                            pvis (attacker) > 0 ? pvis (attacker) : LVL_MIN, 
                            LVL_MAX, attacker, NOBODY, "%s is sliced in half "
                            "by the Seraph!\n\003", pname (attacker));
		  p_crapup (attacker, "\t\tNever pick a fight with an angel..",
			    CRAP_SAVE | CRAP_RETURN);
		  return;
		}
	      else if (mob == MOB_CATACOMB_BEGGAR)
		{
		  sendf (attacker,
			 "You drop everything as you are summoned by The Seraph.\n"
			 "The Seraph scowls at you contemptuously.  For your lack of "
			 "compassion, he\n"
			 "draws his flaming sword and slays you with a single "
			 "blow.\n");
		  send_msg (ploc (attacker), 0,
			    pvis (attacker) > 0 ? pvis (attacker) : LVL_MIN, LVL_MAX,
			    attacker, NOBODY,
			    "%s is sliced in half by the Seraph!\n\003",
			    pname (attacker));
		  p_crapup (attacker,
			    "\tRemember the ten commandments: Thou Shalt Not Kill",
			    CRAP_SAVE | CRAP_RETURN);
		  return;
		}
	      
	    }			/* end if victim is a mobile */
	  
	}			/* end, the attacker is a mortal */
      
      
      if (testpeace (attacker))
	return;
      if ((w = weapon) < 0)
	w = pwpn (attacker);
      if (w >= 0 && !iscarrby (w, attacker))
	{
	  sendf (attacker,
		 "You belatedly realize you don't have the %s,\nand are forced "
		 "to use your hands instead.\n", oname (w));
	  w = -1;
	}
      set_weapon (attacker, w);
      if (w >= 0 && (onum(w) == OBJ_CASTLE_RUNESWORD) && 
	  carries_obj_type(victim, OBJ_TOWER_STAFF) > -1)
	{
	  sendf (attacker,
		 "The Runesword flashes back from its target, growling in "
		 "anger!\n");
	  return;
	}
      
      if (weapon >= 0 && dambyitem (attacker, weapon) < 0)
	{
	  sendf (attacker, "You can't attack %s with a %s!\n",
		 pname (victim), oname (weapon));
	  set_weapon (attacker, -1);
	  return;
	}
      
      if (psitting (attacker))
	{
	  standcom (attacker);
	}
      
      /* end, attacker is played by a human */
      
    }
  else if (weapon < 0 &&
	   carries_obj_type (victim, OBJ_CATACOMB_SHIELD) == -1 &&
	   (x = mob_cast_spell (attacker)) >= 0)
    {
      
      if (cast_spell (attacker, victim, x))
	return;
      
    }				/* end, the attacker was a mobile */
  
  
  x = randperc ();
  cth = 57 - parmor (victim);
  if (carries_obj_type (attacker, OBJ_CASTLE_HORSESHOE) > -1)
    cth += 5;
  
  for (i = 0; i < pnumobs (victim); i++)
    {
      if (iswornby (pobj_nr (i, victim), victim) &&
	  otstbit (pobj_nr (i, victim), OFL_ARMOR))
	{
	  cth -= oarmor (pobj_nr (i, victim));
	}
    }
  
  if(cth<10) cth = 10;
  /* Add 1/2 damage of wielded weapon for mobiles..full for players.. */
  if ((w = weapon) < 0)
    w = pwpn (attacker);
  if (w >= 0 && iscarrby (w, attacker) && iswielded (w) &&
      otstbit (w, OFL_WEAPON))
    {
      d = odamage (w);
      if (attacker >= max_players)
	d /= 2;
    }
  else
    {
      d = 0;
    }
  
  d += pdam (attacker);
  d = (x < cth) ? my_random () % (d + 1) : -1;	/* damage done */
  
  combatmessage (victim, attacker, weapon, d);
  wound_player(attacker, victim, d, -1);
  should_hate(victim, attacker, d);
}





/* High level procedure to cast a spell.
 * spell_type = verb of spell.
 */

void 
spellcom (int spell_verb)
{
  int victim = brkword () < 0 ? pfighting (mynum) : fpbn (wordbuf);
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_DEMI)) {
    bprintf ("You can't cast from here.\n");
    return;
   }
  if (!is_in_game (victim))
    bprintf ("Who?\n");
  else
    cast_spell (mynum, victim, spell_verb);
}


/* Procedure to cast a spell
 * caster   = player index of the spell caster (may be a mobile).
 * victim   = player index of the victim.
 * spell_type = key in spell_table.
 *
 * Return true if the spell succeeded.
 */

Boolean 
cast_spell (int caster, int victim, int spell_type)
{
  struct _spell *spell;
  int clev;
  int i;
  int ob = 0;
  int f;
  
  if ((caster < max_players) && (victim < max_players) && (plev(mynum) < LVL_SENIOR) && !the_world->w_tournament) {
    bprintf("A shield of the Gods can not be surpassed by your wimpy magical spells.\n");
    return 0;
  }
  
  /* Failure to cast the spell? */
  if (plev (caster) < LVL_APPREN && ploc (caster) != ploc (victim))
    {
      sendf (caster, "%s is not here.\n", pname (victim));
      return False;
    }
if (ststflg(victim, SFL_GHOST)) {
 bprintf("Your spell cannot target %s ghostly form.\n", his_or_her(victim));
 return False; }
  
  /* Find spell */
  for (spell = spell_table; spell->verb != spell_type; spell++)
    {
      if (spell->verb < 0)
	return False;		/* No such spell */
    }
  
#ifdef LOCMIN_PIRATE
  if ((victim == MOB_PIRATE_MERMAID + max_players) && (!ststflg (caster, SFL_DEAF))
      )
    {
      bprintf ("Her music is too beautiful.  You can't bring yourself to "
	       "attack her.\n");
      return False;
    }
#endif /* PIRATE.ZONE */
  
  if (plev (caster) < LVL_APPREN)
    {
      
      if (pstr (caster) < 10)
	{
	  sendf (caster, "You are too weak to cast magic.\n");
	  return False;
	}
      
      setpstr (caster, pstr (caster) - 2);
      
      if (testpeace (caster))
	{
	  sendf (caster, "No, that's violent!\n");
	  return False;
	}
      else if (ltstflg (ploc (victim), LFL_NO_MAGIC))
	{
	  sendf (caster, "Something about this location has drained your mana.\n");
	  return False;
	}
      
      i = (caster >= max_players) ? 90 + spell->chance : spell->chance;
      
      if (carries_obj_type (caster, OBJ_WINDOW_POWERSTONE) > -1)
	++i;
      if (carries_obj_type (caster, OBJ_WINDOW_POWERSTONE1) > -1)
	++i;
      if (carries_obj_type (caster, OBJ_WINDOW_POWERSTONE2) > -1)
	++i;
      if (caster < max_players && randperc () > i * plev (caster))
	{
	  sendf (caster, "You fumble the magic.\n");
	  return False;
	}
      
      if (ptstflg (victim, PFL_NOHASSLE) != 0)
	{
	  sendf (caster, "The magic flashes harmlessly past.\n");
	  return False;
	}
      else if (carries_obj_type (victim, OBJ_CATACOMB_SHIELD) > -1)
	{
	  sendf (caster, "The spell is absorbed by %s shield!\n", his_or_her (victim));
	  send_msg (ploc (caster), 0, pvis (caster), LVL_MAX, caster, victim,
		    "%s casts a spell on %s. It is absorbed by %s shield!\n",
		    pname (caster), pname (victim), his_or_her (victim));
	  
	  sendf (victim, "%s casts a spell on you. It is absorbed by your shield!\n",
		 see_name (victim, caster));
	  return False;
	}
    }
  
  if (caster == victim)
    {
      sendf (victim, "You're supposed to be killing others, not yourself.\n");
      return False;
    }
  if (!do_okay (caster, victim, PFL_NOHASSLE))
    {
      sendf (caster, "The magic flashes harmlessly past %s.\n", pname (victim));
      return False;
    }
  
  clev = plev (caster);
  if (spell->maxlev >= 0 && clev > spell->maxlev)
    clev = spell->maxlev;
  
  f = 0;
  if (victim >= max_players)
    {
      /* victim likes the spell? */
      if (spell->like_mflag >= 0 && mtstflg (victim, spell->like_mflag))
	{
	  sendf (caster, spell->like_it, pname (victim));
	  if (pstr (victim) < pstr_reset (victim))
	    setpstr (victim, pstr (victim) + 5);
	  f = -1;
	}
      else if (spell->fear_mflag >= 0 && mtstflg (victim, spell->fear_mflag))
	{
	  /* victim fears the spell, give factor 6 */
	  f = 6;
	}
    }
  if (f == 0)
    {
      /* Find if player is carrying any magical object for this spell */
      for (i = spell->obj3first; i < spell->obj3next; i++)
	{
	  if (iscarrby (ob = spell_objs[i], caster))
	    break;
	  ob = -1;
	}
      f = (ob >= 0 ? 3 : 2);
    }
  i = (caster >= max_players ? spell->mob_dam : f > 0 ? f * clev : 0);
  
  send_msg (ploc (victim), MODE_NOBATTLE, LVL_MIN, LVL_MAX, caster, NOBODY,
	    spell->to_room, pname (caster));
  if (ststflg(victim, SFL_NOBATTLE)) {
    sendf (victim, spell->to_room, pname (caster));}
  sendf (victim, spell->to_victim, pname (caster));
  send_msg (ploc (victim), MODE_NOBATTLE, LVL_MIN, LVL_MAX, victim, NOBODY,
            spell->to_others, pname (victim));
  wound_player (caster, victim, i, spell_type);
  return True;
}

/*
 * procedure to wound a player.
 * attacker = player index of the attacker.
 * victim   = player index of the victim.
 * damage   = damage caused by the attacker on the victim.
 * hit_type >= 0  verb that caused the magical hit.
 *          <  0  non-magical hit.
 *
 * Return true if victim died.
 */
Boolean 
wound_player (int attacker, int victim, int damage, int hit_type)
{
  int x, q;
  int storenum;
 
  if (victim < 0 || victim >= numchars || attacker < 0 ||
      attacker >= numchars)
    {
      return False;
    }
  
  pfighting (victim) = attacker;
  
  if (damage <= 0)
    return False;
  
  if (victim >= max_players)
    {
      setpstr (victim, pstr (victim) - damage);
    }
  else if (plev (victim) < LVL_APPREN)
    {
      setpstr (victim, pstr (victim) - damage);
      if (attacker >= max_players)
	{
	  if (mtstflg (attacker, MFL_DSCORE) != 0)
	    {
	      sendf (victim, "You feel weaker as %s's icy touch drains your "
		     "very life force!\n", pname (attacker));
	      setpscore (victim, pscore (victim) - 100 * damage);
	      if ((int)pscore (victim) <= 0)
		{
		  setpstr (victim, -1);
		}
	    }
	}
    }
  
  if (pstr (victim) >= 0)
    {
      if (attacker < max_players &&
	  (plev (victim) > LVL_FOUR || victim >= max_players))
	{
	  setpscore (attacker, pscore (attacker) + damage * 2);
	}
      return False;
    }
  
  
  /* Victim has died */
  
  pfighting (victim) = -1;
  
  if (hit_type >= 0 && hit_type != VERB_ZAP)
    {
      sendf (attacker, "&+WYour last spell did the trick.&*\n");
    }
  setpstr (victim, -1);
  setpfighting (victim, -1);

/*
  sendf (ploc (victim), "%s has died.\n", pname (victim));
*/
  send_msg (DEST_ALL, 0, pvis(victim), LVL_MAX, victim, NOBODY,
     	"%s has died.\n",pname(victim));
  send_msg (DEST_ALL, MODE_COLOR | MODE_QUIET | MODE_NOSLAIN, LVL_APPREN, LVL_MAX, victim, NOBODY,
	    "[%s has been slain by %s]\n", pname (victim), pname (attacker));
  if (the_world->w_tournament)
    {
      send_msg (DEST_ALL, MODE_COLOR, LVL_MIN, LVL_APPREN, victim, NOBODY,
		"[%s has died]\n", pname (victim));
    }
  dumpstuff (victim, ploc (victim));
  
  if (victim >= max_players)
    {
      /* Kill mobile */
      if (attacker < max_players)
	{
	  setpscore (attacker, pscore (attacker) + 10 * pdam (victim));
	}
      if ((q = find_pretender (victim)) >= 0)
	{
	  sendf (victim, "*** You have died. ***\n");
	  unalias (q);
	  unpolymorph (q);
	}
      setploc (victim, LOC_DEAD_DEAD);
      if (attacker < max_players)
	setpscore (victim, attacker);	/* remember who killed the mobile */
      else
	setpscore (victim, -1);	/* Mobile killing mobile */
      
      if ((q = victim - max_players) == MOB_EFOREST_LICH)
	x = max_players + MOB_EFOREST_ASMODEUS;
      else if (q == MOB_EFOREST_ASMODEUS)
	x = max_players + MOB_EFOREST_LICH;
      else
	x = -1;
      if (x >= 0)
	{
	  if (alive (x) == -1)
	    {
              send_msg (DEST_ALL, MODE_NODEAF, LVL_MIN, LVL_MAX, NOBODY, NOBODY,
                        "A Great Evil has departed from the land.\n");
              questsdone[Q_EFOREST] = True;
              if (pscore (x) == attacker && !qtstflg(attacker, Q_EFOREST))
                {
                  sendf (attacker, "You have avenged the Elven Forest!\n");
                  qsetflg (attacker, Q_EFOREST);
                  send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, LVL_MAX,
			   NOBODY, NOBODY, "[%s solved the Elven Forest quest]\n",pname(attacker));
		  
		}
	    }
	}
#ifdef LOCMIN_OAKTREE
      if (victim == MOB_OAKTREE_SAPLING + max_players)
	{
	  send_msg(ploc(attacker), 0, LVL_MIN, LVL_MAX, NOBODY, NOBODY,
		   "The Sapling bursts into flames and crumbles to ashes.\n"
		   "A silvery object falls to the ground.\n");
	  create(OBJ_OAKTREE_SPIKE);
	  setoloc(OBJ_OAKTREE_SPIKE, ploc(attacker), IN_ROOM);
	}
#endif

#ifdef LOCMIN_VALLEY
  if (victim == MOB_VALLEY_CHICKEN + max_players)
    {
      questsdone[Q_CHICKEN] = True;
      if (!qtstflg(attacker, Q_CHICKEN))
        {
          qsetflg(attacker, Q_CHICKEN);
          send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, LVL_MAX,
                   NOBODY, NOBODY, "[%s solved the Chicken Quest]\n", pname(attacker));
          sendf(attacker,"\nYou have solved the Chicken Quest!\n");
        }
    }
#endif

#ifdef LOCMIN_ICECAVE
  if (victim == MOB_ICECAVE_UNICORN + max_players)
    {
      questsdone[Q_UNICORN] = True;
      if (!qtstflg(attacker, Q_UNICORN))
        {
          qsetflg(attacker, Q_UNICORN);
          send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, LVL_MAX,
                   NOBODY, NOBODY, "[%s solved the Unicorn Quest]\n", pname(attacker));
          sendf(attacker, "\nYou have solved the Unicorn Quest!\n");
        }
    }
#endif

#ifdef LOCMIN_NIBELUNG
  if (victim == MOB_NIBELUNG_FAFFNER + max_players)
    {
      questsdone[Q_FAFFNER] = True;
      if (!qtstflg(attacker, Q_FAFFNER))
        {
          qsetflg(attacker, Q_FAFFNER);
          send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, LVL_APPREN, LVL_MAX,
                   NOBODY, NOBODY, "[%s solved the Faffner Quest]\n", pname(attacker));
          sendf(attacker, "\nYou have solved the Faffner Quest!\n");
        }
    }
#endif

      if (ptemporary (victim))
	destruct_mobile (victim, NULL);
      
    }
  else if (plev (victim) >= LVL_APPREN)
    return False;		/* Wizards don't die */
  else
    {
      /* Kill mortal */
#ifdef LOG_SLAIN
      mudlog ("%s SLAIN %sby %s", pname (victim),
	      (hit_type < 0 ? "" : "magically "),
	      pname (attacker));
#endif
      
      if (attacker < max_players &&
	  (the_world->w_tournament || plev (victim) > LVL_FOUR))
	{
	  x = plev (victim);
	  setpscore (attacker, pscore (attacker) + x * x * x * 10);
	}
/* Bulk of ghost code follows. -Dragorn */
if (!the_world->w_tournament) /* No ghosts in a tournament */
{
/* For rainbow's spell pt system */
#ifdef setpspellpts
      setpspellpts(victim, 0);
#endif

  if (pscore(mynum) != 0)
      setpscore (victim, pscore (victim) / 2);

      x = real_mynum;
      
      storenum = mynum;
      setup_globals (victim);

      calibme ();
      setpstr (victim, -1);
      ssetflg(victim, SFL_GHOST);
      bprintf("You scream in pain as %s strikes the death blow. As your body\ncrumples beneath you, you feel yourself rising. You gaze around to find\nyourself standing above your body. As you look through your transparent hands,\nyou feel yourself slowly fading.\n", pname(attacker));

   strcpy (cur_player->ghostsetin, cur_player->setin);
   strcpy (cur_player->ghostsetout, cur_player->setout);
/* Again, in newer dyrt versions, need to use setstand and setsit */
/*   strcpy (cur_player->ghostsethere, cur_player->sethere); */
   strcpy (cur_player->ghostsetsit, cur_player->ghostsetsit);
   strcpy (cur_player->ghostsetstand, cur_player->ghostsetstand);   

   strcpy (cur_player->setin, "The ghost of %n has arrived.");
   strcpy (cur_player->setout, "The ghost of %n has gone %d.");
/* Again, in newer versions... Gee, do you get the point? *grin* */
/*   strcpy (cur_player->sethere, "The ghost of %n stands here, slowly fading."); */
   strcpy (cur_player->setsit, "The ghost of %n is sitting here.");
   strcpy (cur_player->setstand, "The ghost of %n is here, slowly fading.");
   
   setup_globals(storenum);
}
      if (the_world->w_tournament) /* No ghosts in a tourney */
      {
      setpstr (victim, 40);
      setpscore (victim, pscore (victim) / 2);
      
      x = real_mynum;
      setup_globals (victim);
      
      calibme ();
      crapup ("\t\tOh dear... you seem to be slightly dead.",
	      CRAP_SAVE | CRAP_RETURN);
      setup_globals (x);
     } 
    }
  return True;
}



void 
combatmessage (int victim, int attacker, int wpn, int ddn)
{
  static char *form = "\001p%s\003";
  
  char vic_name[64];
  char att_name[64];
  char weap[64];
  char his[20];
  char hard[64];
  
  if (victim < 0 || attacker < 0 || victim >= numchars ||
      attacker >= numchars)
    return;
  sprintf (vic_name, form, pname (victim));
  sprintf (att_name, form, pname (attacker));
  
  strcpy (his, his_or_her (attacker));
  
  if (wpn >= 0)
    {
      strcpy (weap, oname (wpn));
    }
  else
    {
      strcpy (weap, "bare hands");
    }
  
  if (ddn < 0)
    {				/* Miss */
      sendf (attacker, "You missed %s.\n", vic_name);
      sendf (victim, "%s missed you.\n", att_name);
      send_msg (ploc (victim), MODE_NOBATTLE, LVL_MIN, LVL_MAX, victim, attacker,
		"%s missed %s.\n", att_name, vic_name);
    }
  else if (ddn < 4)
    {
      sendf (attacker, "You hit %s very lightly.\n", vic_name);
      sendf (victim, "%s hit you very lightly.\n", att_name);
      send_msg (ploc (victim), MODE_NOBATTLE, LVL_MIN, LVL_MAX, victim, attacker,
		"%s hit %s very lightly.\n", att_name, vic_name);
    }
  else if (ddn < 7)
    {
      sendf (attacker, "You hit %s lightly.\n", vic_name);
      sendf (victim, "%s hit you lightly.\n", att_name);
      send_msg (ploc (victim), MODE_NOBATTLE, LVL_MIN, LVL_MAX, victim, attacker,
		"%s hit %s lightly.\n", att_name, vic_name);
    }
  else if (ddn < 13)
    {
      sendf (attacker, "You hit %s.\n", vic_name);
      sendf (victim, "%s hit you.\n", att_name);
      send_msg (ploc (victim), MODE_NOBATTLE, LVL_MIN, LVL_MAX, victim, attacker,
		"%s hit %s.\n", att_name, vic_name);
    }
  else
    {
      if (ddn < 17)
	strcpy (hard, " hard");
      else if (ddn < 21)
	strcpy (hard, " very hard");
      else
	strcpy (hard, " extremely hard");
      sendf (attacker, "You hit %s%s with your %s.\n", vic_name, hard, weap);
      sendf (victim, "%s hit you%s with %s %s.\n", att_name, hard, his, weap);
      send_msg (ploc (victim), MODE_NOBATTLE, LVL_MIN, LVL_MAX, victim, attacker,
		"%s hit %s%s with %s %s.\n",
		att_name, vic_name, hard, his, weap);
    }
}

void 
setpfighting (int x, int y)
{
  int ct, loc = ploc (x);
  
  pfighting (x) = y;
  
  if (y == -1)
    {
      if (exists (loc))
	{
	  for (ct = 0; ct < lnumchars (loc); ct++)
	    {
	      
	      if (pfighting (lmob_nr (ct, loc)) == x)
		{
		  pfighting (lmob_nr (ct, loc)) = -1;
		}
	    }
	}
    }
  else
    {
      pfighting (y) = x;
    }
}


















