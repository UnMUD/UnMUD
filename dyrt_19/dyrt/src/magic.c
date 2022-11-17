#include "kernel.h"
#include "objects.h"
#include "locations.h"
#include "stdinc.h"

#define MIN_HEALTIME 600

void healallcom(void) /* heals all non-fighting mortals */
{
  int i;
  Boolean healed = False;

  if ((!ptstflg(mynum, PFL_HEALALL)))
  {
    bprintf("The spell fails.\n");
    return;
  }
  if (healalltime() < MIN_HEALTIME)
  {
    bprintf("It hasn't been 10 minutes.\n");
    bprintf("Time since last healall: %s\n", sec_to_str(healalltime()));
    if ((plev(mynum) < LVL_ARCHWIZARD || (EMPTY(item1))))
      return;
    else
      bprintf("Time limit over-ridden.\n");
  }
  for (i = 0; i < max_players; i++)
  {
    if (plev(i) < LVL_APPREN && players[i].iamon && (pfighting(i) == -1) && pstr(i) < maxstrength(i) && (!ststflg(i, SFL_GHOST)))
    {
      setpstr(i, maxstrength(i));
      sendf(i, "%s releases a massive healing flux upon the mortals of this land.\n", see_name(i, mynum));
      if (!healed)
      {
        bprintf("You healed the following players:\n");
        last_healall = time(0);
        mudlog("%s HEALALLed.", pname(mynum));
        send_msg(DEST_ALL, MODE_COLOR | MODE_QUIET, LVL_APPREN, LVL_MAX, mynum, NOBODY,
                 "[\001p%s\003 has HEALALLed]\n", pname(mynum));
        healed = True;
      }
      bprintf("\t%s\n", pname(i));
    }
  }
  if (healed)
    bprintf("\n");
  saveallcom();
  return;
}

void healcom(void)
{
  int a;
  Boolean f;
  PERSONA p;

  if (EMPTY(item1))
  {
    bprintf("Heal who?\n");
    return;
  }
  if (!ptstflg(mynum, PFL_HEAL))
  {
    bprintf("The spell fails.\n");
    return;
  }
  if (brkword() == -1 || (a = find_player(wordbuf, &p, &f)) == -1)
  {
    bprintf("Heal who?\n");
    return;
  }
  if (pfighting(a) != -1)
  {
    bprintf("I'm sorry...%s is in a fight right now.  Try again later.\n", pname(a));
    return;
  }
  if (ststflg(a, SFL_GHOST))
  {
    bprintf("The spell cannot focus on the ghostly form of %s.\n", pname(a));
    return;
  }
  if (a == mynum)
  {
    bprintf("&+WYou feel much better.&*\n");
    setpstr(mynum, maxstrength(mynum));
    return;
  }
  else if (a >= max_players)
  {
    setpstr(a, pstr_reset(a));
  }
  else if (a >= 0)
  {
    setpstr(a, maxstrength(a));
  }
  else if (f)
  {
    bprintf("%s will feel much better now!\n", p.p_name);
    p.p_strength = pmaxstrength(p.p_level);
    putuaf(&p);
    return;
  }
  bprintf("Power radiates from your hands to heal %s.\n", pname(a));
  sendf(a, "&+W%s heals all your wounds.&*\n", see_name(a, mynum));
  mudlog("%s has HEALed %s.", pname(mynum), pname(a));
  send_msg(DEST_ALL, MODE_COLOR | MODE_QUIET, LVL_SENIOR, LVL_MAX, NOBODY, NOBODY,
           "[\001p%s\003 has HEALed %s.]\n", pname(mynum), pname(a));
}

void forcecom(void)
{
  int rme = real_mynum;
  int me = mynum;
  int a;
  char z[MAX_COM_LEN];

  if ((a = vicf2(SPELL_REFLECTS, 4)) < 0)
    return;
  if (a >= max_players)
  {
    bprintf("You can only force players to do things.\n");
    return;
  }
  if (plev(mynum) < LVL_APPREN && ploc(mynum) != ploc(a))
  {
    bprintf("There is no %s here.\n", pname(a));
    return;
  }
  getreinput(z);
  if (do_okay(me, a, PFL_NOFORCE))
  {
    send_msg(DEST_ALL, MODE_QUIET, LVL_APPREN, LVL_MAX, NOBODY, NOBODY,
             "&+M[\001p%s\003 forced \001p%s\003]&*\n&+M[Force:%s]&*\n", pname(rme), pname(a), z);
    setup_globals(a);
    bprintf("%s has &+Mforced&* you to %s\n", see_name(a, me), z);
    cur_player->isforce = True;
    gamecom(z, True);
    cur_player->isforce = False;
    setup_globals(rme);
  }
  else
  {
    sendf(a, "%s tried to force you to %s\n", see_name(a, me), z);
  }
}

void sumcom(void)
{
  int a, me = mynum, rme = real_mynum;
  int c, d, x;
  char buf[64];
  char xx[SETIN_MAX];

  if (brkword() == -1)
  {
    bprintf("Summon who?\n");
    return;
  }
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD))
  {
    bprintf("Can't summon from here!\n");
    return;
  }
  do
  {
    if ((a = fpbn(wordbuf)) != -1)
    {
      if (plev(mynum) < LVL_APPREN)
      {
        if (a == mynum)
        {
          bprintf("You're already here.\n");
          return;
        }
        /* ghost code: ghosts can't be summoned. -dragorn */
        if ((ststflg(a, SFL_GHOST)) && (plev(mynum) < LVL_ARCHWIZARD))
        {
          bprintf("You cannot locate %s ghostly form.\n", his_or_her(a));
        }
        if (ltstflg(ploc(a), LFL_CANT_SUMMON))
        {
          bprintf("Something about that location blocks your summon.\n");
          return;
        }
        if (pstr(mynum) < 10)
        {
          bprintf("You're too weak.\n");
          return;
        }
        setpstr(mynum, pstr(mynum) - 2);
        c = plev(mynum) * 2;
        if (carries_obj_type(mynum, OBJ_WINDOW_POWERSTONE) > -1 ||
            carries_obj_type(mynum, OBJ_WINDOW_POWERSTONE1) > -1 ||
            carries_obj_type(mynum, OBJ_WINDOW_POWERSTONE2) > -1)
          c += plev(mynum);
        d = randperc();
        if (ltstflg(ploc(mynum), LFL_ONE_PERSON))
        {
          bprintf("It's too restricted in here.\n");
          return;
        }
        if (ltstflg(ploc(mynum), LFL_NO_SUMMON))
        {
          bprintf("Something prevents your summoning from succeeding.\n");
          return;
        }
        if (ltstflg(ploc(mynum), LFL_ON_WATER))
        {
          bprintf("The boat is rolling too much.\n");
          return;
        }
        if (a >= max_players)
        {
          bprintf("You can't summon mobiles.\n");
          return;
        }
        else if (c < d || players[a].asmortal > 0)
        {
          bprintf("The spell fails.\n");
          return;
        }
        if (wears_obj_type(a, OBJ_CATACOMB_SHIELD) > -1 || wears_obj_type(a, OBJ_TREEHOUSE_AMULET) > -1 || carries_obj_type(a, OBJ_CASTLE_RUNESWORD) > -1 || wears_obj_type(a, OBJ_BLIZZARD_TALISMAN) > -1 || wears_obj_type(a, OBJ_ICECAVE_PENDANT) > -1)
        {
          bprintf("Something prevents your summoning from succeeding.\n");
          return;
        }
      } /* end; if not wiz */
      if (ltstflg(ploc(a), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD))
      {
        bprintf("They are in jail."
                "  You cannot summon someone from there\n");
        return;
      }

      if (!do_okay(mynum, a, PFL_NOSUMMON))
      {
        bprintf("%s doesn't want to be summoned.\n", pname(a));
        sendf(a, "%s tried to summon you!\n", see_name(a, mynum));
        continue;
      }
      bprintf("You cast the summoning......\n");
      if (a < max_players)
      {
        mudlog("%s &+gSUMMON&*ed player %s from %s to %s.", pname(mynum), pname(a), xshowname(buf, ploc(a)), showname(ploc(mynum)));
        send_msg(DEST_ALL, MODE_COLOR | MODE_QUIET,
                 pvis(mynum) < LVL_ARCHWIZARD ? LVL_ARCHWIZARD : pvis(mynum), LVL_MAX, mynum, a,
                 "[%s has summoned player %s from %s to %s]\n",
                 pname(mynum), pname(a), xshowname(buf, ploc(a)),
                 showname(ploc(mynum)));
      }
      if (a >= max_players)
      {
        mudlog("%s &+gSUMMON&*ed mobile %s from %s to %s.", pname(mynum), pname(a), xshowname(buf, ploc(a)), showname(ploc(mynum)));
      }
      if (plev(mynum) < LVL_APPREN && !the_world->w_tournament)
      {
        sendf(a, "You drop everything as you are summoned by %s.\n", see_name(a, mynum));
      }
      else
      {
        sendf(a, "%s\n", build_setin(xx, cur_player->settrvict, pname(mynum), NULL));
      }
      send_msg(ploc(a), 0, pvis(a), LVL_MAX, a, NOBODY,
               "%s disappears in a puff of smoke.\n", pname(a));
      if (a < max_players)
      {
        sprintf(buf, "%s\n", build_setin(xx, cur_player->settrroom, pname(mynum), NULL));
        sillycom(buf);
        send_msg(ploc(mynum), 0, pvis(a), LVL_MAX, a, NOBODY,
                 "%s\n", build_setin(xx, cur_player->settrenter, pname(a), NULL));
        setup_globals(a);
        trapch(ploc(me));
        setup_globals(rme);
      }
      else
      {

        send_msg(ploc(mynum), 0, pvis(a), LVL_MAX, a, NOBODY,
                 "%s arrives.\n", pname(a));

        setploc(a, ploc(mynum));
      }

      continue;
    }
    else if ((a = fobn(wordbuf)) != -1)
    {

      if (!ptstflg(mynum, PFL_SUMOBJ))
      {
        bprintf("You can only summon people.\n");
        continue;
      }
      x = a;
      while (ocarrf(x) == IN_CONTAINER)
        x = oloc(x);

      if (ocarrf(x) >= CARRIED_BY)
        x = ploc(oloc(x));
      else
        x = oloc(x);
#ifdef LOCMIN_RHOME
      if ((a == OBJ_RHOME_INVRING) && (!EQ(pname(mynum), "Rex")))
      {
        bprintf("You can not summon that object, sorry.\n");
        return;
      }
#endif
      send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
               "%s fetches something from another dimension.\n", pname(mynum));

      sendf(x, "The %s vanishes!\n", oname(a));
      mudlog("%s &+gSUMMON&*ed object %s from %s to %s.", pname(mynum), oname(a), xshowname(buf, x), showname(ploc(mynum)));
      bprintf("The %s flies into your hand.\nIt was:", oname(a));
      desrm(oloc(a), ocarrf(a));
      setoloc(a, mynum, CARRIED_BY);
      continue;
    }
    else
    {
      bprintf("Who or what is %s?\n", wordbuf);
    }
  } while (plev(mynum) >= LVL_APPREN && brkword() != -1);
}

static void vis_proc(int new)
{
  char xx[SETIN_MAX];

  int oldvis = pvis(mynum);

  setpvis(mynum, new);

  bprintf("Ok\n");

  if (new < oldvis)
  {
    send_msg(ploc(mynum), 0, new, oldvis, mynum, NOBODY,
             "%s\n", build_setin(xx, cur_player->setvin, pname(mynum), NULL));
  }
  else
  {
    send_msg(ploc(mynum), 0, oldvis, new, mynum, NOBODY,
             "%s\n", build_setin(xx, cur_player->setvout, pname(mynum), NULL));
  }
}

void viscom(void)
{
  if (plev(mynum) < LVL_APPREN)
  {
    bprintf("You can't do that sort of thing at will, you know.\n");
    return;
  }
  if (pvis(mynum) == 0)
  {
    bprintf("You're already visible.\n");
    return;
  }
  vis_proc(0);
}

void inviscom()
{
  int x, y;

  switch (wlevel(plev(mynum)))
  {
  case LEV_MASTER:
    x = LVL_GOD;
    break;
  case LEV_GOD:
    x = LVL_GOD;
    break;
  case LEV_HIGHDEMI:
    x = LVL_GOD;
    break;
  case LEV_DEMI:
    x = LVL_HIGHDEMI;
    break;
  case LEV_HIGHARCH:
    x = LVL_DEMI;
    break;
  case LEV_ARCHWIZARD:
    x = LVL_HIGHDEMI;
    break;
  case LEV_SENIOR:
    x = LVL_ARCHWIZARD;
    break;
  case LEV_WIZARD:
    x = LVL_SENIOR;
    break;
  case LEV_EMERITUS:
    x = LVL_WIZARD;
    break;
  case LEV_APPRENTICE:
    x = LVL_APPREN;
    break;
  default:
    bprintf("You can't turn invisible at will, you know.\n");
    return;
  }
  if (brkword() == -1 || (y = atoi(wordbuf)) < 0 || y > x)
    y = x;

  if (pvis(mynum) == y)
  {
    bprintf("You're already %svisible.\n", (y == 0 ? "" : "in"));
    return;
  }

  vis_proc(y);
}

void resurcom(void)
{
  int a;

  if (!ptstflg(mynum, PFL_RES))
  {
    erreval();
    return;
  }
  if (EMPTY(item1))
  {
    bprintf("Resurrect what?\n");
    return;
  }
  if ((a = fobn(item1)) == -1)
  {
    if ((a = fpbn(item1)) == -1 || a < max_players)
    {
      bprintf("You can only resurrect objects and mobiles.\n");
      return;
    }

    if (!EMPTY(pname(a)))
    {
      /* if alive we should only give them strength and level back :-) */

      if (pfighting(a) != -1)
      {
        bprintf("%s is fighting at the moment.\n", pname(a));
        return;
      }
      else if (pstr(a) >= 0)
      {
        bprintf("%s is resurrected.\n", pname(a));
        setpstr(a, pstr_reset(a));
        return;
      }
    }
    if (ltstflg(ploc(mynum), LFL_ONE_PERSON))
    {
      bprintf("This room is too small.\n");
      return;
    }

    reset_mobile(a);
    clear_hate(a);
    setploc(a, ploc(mynum));

    sendf(ploc(mynum), "%s suddenly appears.\n", pname(a));
    mudlog("%s RESURRECTed mobile %s.", pname(mynum), pname(a));
    return;
  }
  if (ospare(a) != -1)
  {
    bprintf("It already exists.\n");
    return;
  }
  oclrbit(a, OFL_DESTROYED);
  setoloc(a, ploc(mynum), IN_ROOM);

  sendf(ploc(mynum), "The %s suddenly appears.\n", oname(a));
  mudlog("%s RESURRECTed the %s.", pname(mynum), oname(a), onum(a));
}
void posecom(void)
{
  char x[128];
  int n;
  int m;

  char *POSE[] =
      {"gestures", "fireball", "hamster",
       "sizzle", "crackle", TABLE_END};

  if (plev(mynum) < LVL_SEVEN)
  {
    bprintf("You're not up to this yet.\n");
    return;
  }
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD))
  {
    bprintf("You don't look very good right now anyway.\n");
    return;
  }
  if (brkword() != -1)
  {
    if (plev(mynum) < LVL_APPREN)
    {
      bprintf("Usage: POSE\n");
      return;
    }
    if (((n = atoi(wordbuf)) > 5 || n < 1) &&
        (m = tlookup(wordbuf, POSE)) == -1)
    {
      bprintf("Usage: POSE <gestures/fireball/hamster/sizzle/crackle>\n");
      return;
    }
    if (m != -1)
      n = m;
    else
      --n;
  }
  else
    n = randperc() % 5;

  switch (n)
  {
  case 0:
    sprintf(x, "&+R\001s%%s\003%%s raises %s arms in mighty magical "
               "invocations.&*\n\003",
            his_or_her(mynum));
    sillycom(x);
    bprintf("&+RYou make mighty magical gestures.&*\n");
    break;
  case 1:
    sillycom("&+B\001s%s\003%s throws out one arm and sends a huge bolt of "
             "fire high into the sky.&*\n\003");
    bprintf("&+BYou toss a fireball high into the sky.&*\n");
    broad("&+B\001cA massive ball of fire explodes high up in the sky.&*\n\003");
    break;
  case 2:
    sillycom("\001s%s\003%s turns casually into a &+yhamster&* before resuming "
             "normal shape.\n\003");
    bprintf("You casually turn into a &+yhamster&* before resuming normal "
            "shape.\n");
    break;
  case 3:
    sillycom("\001s%s\003%s starts &+Cs&+Wi&+Cz&+Wz&+Cl&+Wi&+Cn&+Wg&* with magical energy.\n\003");
    bprintf("You &+Cs&+Wi&+Cz&+Wz&+Cl&+We&* with magical energy.\n");
    break;
  case 4:
    sillycom("\001s%s\003%s begins to &+Rc&+rr&+Ra&+rc&+Rk&+rl&+Re&* with magical fire.\n\003");
    bprintf("You &+Rc&+rr&+Ra&+rc&+Rk&+rl&+Re&* with magical fire.\n");
    break;
  }
}

void bangcom(void)
{
  if (plev(mynum) < LVL_TEN)
    erreval();
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) <= LVL_ARCHWIZARD))
    erreval();
  else
    broad("\001dA huge crash of &+Wthunder&* echoes throughout the land.\n\003");
}
