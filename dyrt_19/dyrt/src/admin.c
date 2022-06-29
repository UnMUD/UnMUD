#include "verbs.h"
#include "kernel.h"
#include "stdinc.h"
#include "admin.h"
#include "locations.h"

void bugcom (void)
{
  char x[MAX_COM_LEN];
  
  getreinput (x);
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD))
   {
    bprintf ("Obviously, YOU'RE the bug, or you wouldn't be in here\n");
    return;
   }

  if (EMPTY (txt1))
    {
      bprintf ("What do you want to bug me about?\n");
    }
  else
    {
      mudlog ("BUG BY %s: %s", pname (mynum), x);
      bprintf ("Your bug message has been reported.  Thank you.\n");
    }
}

void suggestcom (void)
{
  char x[MAX_COM_LEN];
  
  getreinput (x);
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD))
   {
    bprintf ("Obviously, YOU'RE the one who needs some suggestions.\n");
    return;
   }

  if (EMPTY (txt1))
    {
      bprintf ("What would you like to suggest?\n");
    }
  else
    {
      mudlog ("SUGGEST BY %s: %s", pname (mynum), x);
      bprintf ("Your suggestion has been noted.  Thank you.\n");
    }
}

/* The JAIL command
 */

void
jailcom ()
{
  PERSONA d;
  char buff[80];


  if (plev (mynum) < LVL_ARCHWIZARD)
   {
    bprintf ("Pardon?\n");
    return;
   }

  if ((pl1 > max_players) || (pl1 < 0) || EMPTY(item1)) {
    bprintf ("Jail who?\n");
    return;
   }

 if (getuaf (strcpy (buff, item1), &d))
  {
    if (d.p_level >= LVL_ARCHWIZARD) {
      bprintf ("I don't think that would be effective\n");
      return;
     }
    if (d.p_home != LOC_JAIL_JAIL) {
      d.p_home = LOC_JAIL_JAIL;
      bprintf ("You have jailed %s\n", d.p_name);
      if (pl1 > -1)
        setphome (pl1, LOC_JAIL_JAIL);
     }
    else
     {
      if (plev(mynum) < LVL_ARCHWIZARD) {
        bprintf ("They are already jailed and you cannot let them out\n");
        return;
       }
      bprintf ("You have unjailed %s\n", d.p_name);
      d.p_home = 0;
      if (pl1 > -1)
        setphome (pl1, 0);
     }
    putuaf (&d);
   }
  else {
    bprintf ("No such players %s\n", buff);
    return;
  }
  if (pl1 != -1)
   {
    if (phome(pl1) == LOC_JAIL_JAIL) {
      sendf (pl1, "You have been jailed!\n");
      mudlog ("%s has jailed %s", pname(mynum), buff);
      send_msg (DEST_ALL, MODE_COLOR | MODE_QUIET, LVL_ARCHWIZARD, LVL_MAX,
                 mynum,NOBODY,
                "[%s has jailed %s]\n", pname(mynum), buff);
      setploc (pl1, LOC_JAIL_JAIL);
    }
   else
    {

     sendf (pl1, "You have been set free!\n");
     mudlog ("%s has freed %s from jail", pname(mynum), buff);
     send_msg (DEST_ALL, MODE_COLOR | MODE_QUIET, LVL_ARCHWIZARD, LVL_MAX,
              mynum, NOBODY,
              "[%s has let %s out of jail]\n", pname(mynum), buff);
     setploc (pl1, LOC_START_TEMPLE);
    }
  }
}

void typocom (void)
{
  int l = ploc (mynum);
  char x[MAX_COM_LEN], y[80];
  
  getreinput (x);
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD)) {
     bprintf ("You don't like the accomidations?  We keep our jails spotless!\n");
     return;
   }

  if (EMPTY (x))
    {
      bprintf ("What typo do you wish to inform me of?\n");
    }
  else
    {
      mudlog ("TYPO BY %s in %s [%d|%s]: %s", pname (mynum),
              xshowname (y, l), l, sdesc (l), x);
      bprintf ("Ok.\n");
    }
}

void helpcom (void)
{
  int a, b;
  
  if (item1[0] != 0)
    {
      if (showhelp (item1) < 0)
        return;
      if ((a = vichere ()) < 0)
        return;
      if ((b = phelping (mynum)) >= 0)
        {
          sillytp (b, "stopped helping you.");
          bprintf ("Stopped helping \001p%s\003.\n", pname (b));
        }
      if (a == mynum)
        {
          bprintf ("You are beyond help.\n");
          return;
        }
      setphelping (mynum, a);
      sillytp (a, "has offered to help you.");
      bprintf ("Started helping \001p%s\003.\n", pname (phelping (mynum)));
      return;
    }
  
  
  if (plev (mynum) >= LVL_GOD)
    read_file (HELP, NULL, False, NULL);
  else if (plev (mynum) >= LVL_DEMI)
    read_file (HELP, "GOD", False, NULL);
  else if (plev (mynum) >= LVL_ARCHWIZARD)
    read_file (HELP, "DEMIGOD", False, NULL);
  else if (plev (mynum) >= LVL_APPREN)
    read_file (HELP, "ARCHWIZARD", False, NULL);
  else
    read_file (HELP, "WIZARD", False, NULL);
  
  return;
  
}

int showhelp (char *verb)
{
  char file[256];
  char line[80];
  char ext;
  int scanreturn = EOF;
  int v;
  FILE *fp;
  
  ext = tolower(verb[0]);
  if(!isalpha(ext)) ext = '1';
  sprintf(line,"%s.%c",FULLHELP,ext);
  sprintf (file, "../bin/pfilter %d 0x%08lx:0x%08lx:0x%08lx 0x%08lx:0x%08lx:0x%08lx %s",
           plev (mynum), pflags (mynum).b3, pflags (mynum).b2, pflags(mynum).b1,
           pmask (mynum).b3, pmask (mynum).b2, pmask(mynum).b1,line);
  
  if ((fp = popen (file, "r")) == NULL)
    {
      bprintf ("Someone's editing the help file.\n");
      return -1;
    }
  ssetflg (mynum, SFL_BUSY);
  pbfr ();
  v = strlen (verb);
  while (True)
    {
      if (fgets (line, sizeof (line), fp) == NULL)
        {
          scanreturn = EOF;
          break;
        }
      if ((scanreturn = strncasecmp (line, verb, v)) == 0)
        break;
      do
        {
          if (fgets (line, sizeof (line), fp) == NULL)
            {
              scanreturn = EOF;
              break;
            }
        }
      while (strcmp (line, "^\n") != 0);
    }
  
  if (scanreturn != 0)
    {                           /* command not found in extern list */
      pclose (fp);
      sclrflg (mynum, SFL_BUSY);
      return 0;
    }
  
  bprintf ("\nUsage: %s\n", line);
  while (fgets (line, sizeof (line), fp) && strcmp (line, "^\n") != 0)
    {
      bprintf ("%s", line);
    }
  bprintf ("\n");
  
  pclose (fp);
  sclrflg (mynum, SFL_BUSY);
  return -1;
}

/* The GLOBAL command. */
void globalcom (void)
{
  char buff[80];
  
  if (plev (mynum) < LVL_APPREN)
    {
      erreval ();
      return;
    }
  else if (plev (mynum) >= LVL_GOD && dump_pflags ())
    return;
  
  bprintf ("&+RGlobal Settings:&*\n&+W===============&*\n");
  bprintf ("&+BWeather           &+W: &+B%s&*\n", wthr_type (the_world->w_weather));
  bprintf ("&+BLock-status       &+W: &+B%slocked&*\n", lev2s (buff, the_world->w_lock, 0));
  bprintf ("&+BMobiles           &+W: &+B%s&*\n",
           (the_world->w_mob_stop) ? "&+BStopped" : "Started&*");
  bprintf ("&+BWar/Peace         &+W: &+B%s&*\n", (the_world->w_peace) ? "Peace" : "War");
  bprintf ("&+BTournament-Mode   &+W: &+B%s&*\n",
           (the_world->w_tournament) ? "&+BOn" : "Off&*");
  bprintf ("&+BDelayed Reboot    &+W: %s&*\n", the_world->w_delayed ? "&+rEnabled" : "&+bDisabled" );
  bprintf ("&+BMax. Users        &+W: &+B%d&*\n", max_players);
  bprintf ("&+BNumber of Rooms   &+W: "
           "&+B%4d permanent +%4d wiz-made = %4d (max %d)&*\n",
           num_const_locs, numloc - num_const_locs, numloc, GLOBAL_MAX_LOCS);
  bprintf ("&+BNumber of Mobiles &+W: "
           "&+B%4d permanent +%4d wiz-made = %4d (max %d)&*\n",
           num_const_chars - max_players, numchars - num_const_chars,
           numchars - max_players, GLOBAL_MAX_MOBS);
  bprintf ("&+BNumber of Objects &+W: "
           "&+B%4d permanent +%4d wiz-made = %4d (max %d)&*\n",
           num_const_obs, numobs - num_const_obs, numobs, GLOBAL_MAX_OBJS);
}

void pocketcom (void)
{
  int x = 0;
  char roomname[80];
  int pocketloc;

  if (plev(mynum) < LVL_ARCHWIZARD)
    {
      bprintf ("Say what?\n");
      return;
    }
  if (EMPTY (item1))
    {
      bprintf ("Pocket who?\n");
      return;
    }
  if ((x = pl1) == -1)
    {
      bprintf ("No such player on the game.\n");
      return;
    }
  if (x >= max_players)
    {
      bprintf("You cannot pocket a mobile.\n");
      return;
    }
  if (wlevel (plev (mynum)) < wlevel (plev (x)))
    {
      bprintf ("You can't pocket %s.\n", pname (x));
    }
  else
    { 
      int y = real_mynum;
      sprintf(roomname,"&+W%s's Pocket&*",pname(mynum));
      for(pocketloc = LOCMAX_POCKET;pocketloc<LOCMIN_POCKET;pocketloc++)
        {
          if(strcasecmp(roomname,sdesc(pocketloc))) continue;
          setup_globals (x);
          bprintf("\r%s picks you up and stuffs you in %s pocket.\n", pname(y),his_or_her(y));
          send_msg(ploc(x),0,pvis(x),LVL_MAX,x,y,
               "\001p%s\003 picks up %s and stuffs them in %s pocket!\n",
               pname(y),pname(x),his_or_her(y));
          send_msg(pocketloc,0,pvis(x),LVL_MAX,x,y,
               "You hear a muffled yelp as %s is dropped into the pocket with you.\n",
               pname(x));
          setploc(x, pocketloc);
     	  lookin(ploc(x), 0);
          setup_globals(y);
          setphome(x, pocketloc);
          bprintf("\rYou pick up %s and stuff %s in your pocket.\n",
                  pname(x),him_or_her(x));
          return;
        }
      bprintf("But you don't have a pocket!\n");
    }
  return;
}

void freecom (void)
{
  int x = 0;
  int pocketloc = 0;

  if (plev(mynum) < LVL_ARCHWIZARD)
    {
      bprintf ("Say what?\n");
      return;
    }
  if (EMPTY (item1))
    {
      bprintf ("Free who?\n");
      return;
    }
  if ((x = pl1) == -1)
    {
      bprintf ("No such player on the game.\n");
      return;
    }
  if (x >= max_players)
    {
      bprintf("You cannot free a mobile.\n");
      return;
    }
  if (wlevel (plev (mynum)) < wlevel (plev (x)))
    {
      bprintf ("You can't free %s.\n", pname (x));
    }
  else
    { 
      int y = real_mynum;
      if (loc2zone(ploc(x)) == loc2zone(LOCMAX_POCKET))
        {
          setup_globals (x);
          bprintf("\rA hand comes into the pocket, picks you up, and sets you back on solid ground.\n");
          send_msg(pocketloc,0,pvis(x),LVL_MAX,x,y,
               "A hand reaches into the pocket and fishes around for %s.\n", pname(x));
          setploc(x, LOC_START_TEMPLE);
     	  lookin(ploc(x), 0);
          send_msg(ploc(x),0,pvis(x),LVL_MAX,x,y,
               "\001p%s\003 sets %s on the ground in front of you.\n",
               pname(y),pname(x));
          setup_globals(y);
          setphome(x, 0);
          bprintf("\rYou reach in and grab %s.\n", pname(x));
          return;
        }
      bprintf("But you don't have a pocket!\n");
    }
  return;
}

void disconnectcom (void)
{
  int x = 0;
  char name[80];
  
  if (!ptstflg (mynum, PFL_DISCONNECT))
    {
      bprintf ("Huh?\n");
      return;
    }
  
  if (EMPTY (item1))
    {
      bprintf ("Disconnect who?\n");
      return;
    }
  
  if ((x = pl1) == -1)
    {
      bprintf ("No such player or mobile on the game.\n");
      return;
    }
  
  
  if (x >= max_players)
    {
      bprintf("You cannot disconnect a mobile.\n");
      return;
    }
  if (wlevel (plev (mynum)) < wlevel (plev (x)))
    {
      bprintf ("You can't disconnect %s.\n", pname (x));
    }
  else
    {                           /* player */
      int y = real_mynum;
      sprintf (name, "player %s", pname (x));
      setup_globals (x);
      crapup ("\tSocket error:  Connection Reset by Foreign Host.",
	      CRAP_SAVE | CRAP_RETURN);
      setup_globals (y);
    }
  return;
}

void shutdowncom (char crash)
{
  FILE *nologin_file;
  char s[MAX_COM_LEN];
  int i;
  char *t = "  DYRT is closed for hacking.  Please try again later.\n";
  
  if (crash != 100)
    {
      if (!ptstflg (mynum, PFL_SHUTDOWN))
        {
          erreval ();
          return;
        }
    }
  getreinput (s);
  if (!EMPTY (s))
    t = s;
  if (!crash)
    {
      if ((nologin_file = fopen (NOLOGIN, "w")) == NULL)
        {
          bprintf ("(Unable to write NOLOGIN file.)");
        }
      else
        {
          fprintf (nologin_file, "%s\n", t);
          fclose (nologin_file);
        }
      bprintf ("&+YMilliways is now closed.&*\n");
    }
  else
    {
      bprintf ("&+YMilliways is going down HARD.&*\n");
      rm_pid_file ();
    }
  mudlog ("SHUTDOWN%s by %s", crash ? " +CRASH" : "",
          crash == 100 ? "DUE TO FATAL ERROR" : pname (mynum));
  send_msg (DEST_ALL, MODE_COLOR, LVL_APPREN, LVL_MAX, NOBODY, NOBODY,
            "[%s %s %s]\n",
            crash ? "CRASH" : "SHUTDOWN",
            crash == 100 ? "" : "by",
            crash == 100 ? "DUE TO FATAL ERROR" : pname (mynum));
  
  /* kick people off here */
  for (i = 0; i < max_players; i++)
    {
      if (is_in_game (i) && !players[i].isawiz)
        {
          p_crapup (i, t, CRAP_SAVE | CRAP_UNALIAS | CRAP_RETURN);
        }
    }
  if (crash)
    exit (0);
}

void opengamecom()
{
  if (!ptstflg(mynum, PFL_SHUTDOWN))
    {
      erreval();
      return;
    }
  if (unlink(NOLOGIN) < 0)
    {
      bprintf("The game is already open.\n");
    }
  else
    {
      mudlog("OPENGAME by %s", pname(mynum));
      bprintf("&+YMilliways is now open.&*\n");
    }
}

void tournamentcom()
{
  if (plev(mynum) < LVL_ARCHWIZARD)
    {
      erreval();
      return;
    }
  bprintf("Tournament mode is now %s.\n",
          (the_world->w_tournament = !the_world->w_tournament) ?
	  "ON" : "OFF");
}

void unveilcom (char *unv_pass)
{
  int lev;
  
  if (cur_player->aliased || cur_player->polymorphed != -1)
    {
      bprintf("Not while aliased.\n");
      return;
    }
  if (!cur_player->isawiz)
    {
      erreval();
      return;
    }
  if (unv_pass == NULL)
    {
      if (brkword() == -1)
	{
	  cur_player->work = LVL_GOD;
	}
      else
	{
	  cur_player->work = atoi(wordbuf);
	}
      strcpy(cur_player->cprompt, "Magic Word: ");
      bprintf("\n\377\373\001\001Magic Word: ");
      cur_player->no_echo = True;
      push_input_handler(unveilcom);
    }
  else
    {
      bprintf("\377\374\001\001");
      cur_player->no_echo = False;
      pop_input_handler();
      if (!EQ(unv_pass, UNVEIL_PASS))
	{
	  bprintf("Eek!  Go away!\a\n");
	  mudlog("BAD UNVEIL: %s", pname(mynum));
	}
      else
	{
	  lev = cur_player->work;
	  if (lev >= LVL_MAX)
	    {
	      mudlog("WRONG UNVEIL: %s to %d", pname(mynum), plev(mynum));
	      bprintf("The maximum level is %d.\n", LVL_MAX - 1);
	    }
	  else
	    {
	      set_xpflags(lev, &pflags(mynum), &pmask(mynum));
	      setplev(mynum, lev);
	      update_wizlist(pname(mynum), wlevel(lev));
	      mudlog("UNVEIL: %s to %d", pname(mynum), plev(mynum));
	      bprintf("Certainly, master!\n");
	    }
	}
      get_command(NULL);
    }
}

void
warcom ()
{
  if (plev (mynum) < LVL_APPREN)
    {
      erreval ();
      return;
    }
  the_world->w_peace = 0;
  broad ("&+rThe air of peace and friendship lifts.&*\n");
  send_msg (DEST_ALL, MODE_QUIET | MODE_COLOR, LVL_APPREN, LVL_MAX, mynum,
	    NOBODY, "&+r[War declared by %s]&*\n", pname (mynum));
#ifdef LOG_WAR
  mudlog ("WAR by %s", pname (mynum));
#endif
  
}

void
peacecom ()
{
  int m;
  if (plev (mynum) < LVL_APPREN)
    {
      erreval ();
      return;
    }
  the_world->w_peace = 1;
  for (m = 0; m < max_players; m++)
    {
      setpfighting (m, -1);
    }
  send_msg (DEST_ALL, MODE_QUIET | MODE_COLOR, LVL_APPREN, LVL_MAX, mynum,
	    NOBODY, "&+c[Peace declared by %s]&*\n", pname (mynum));
  broad ("&+cPeace and tranquility descend upon the world.&*\n");
#ifdef LOG_WAR
  mudlog ("PEACE by %s", pname (mynum));
#endif
  
}

void
wizlock ()
{
  extern char *WizLevels[];
  extern char *MLevels[];
  extern char *FLevels[];
  
  static int k[] =
    {0, LVL_APPREN, LVL_APPREN + 1, LVL_WIZARD, LVL_SENIOR, LVL_ARCHWIZARD, LVL_HIGHARCH,
       LVL_DEMI, LVL_HIGHDEMI,LVL_GOD};
  char s[80];
  char b[50];
  int l, v, n, x, y;
  
  
  l = the_world->w_lock;
  if (brkword () == -1)
    {
      bprintf ("The game is currently %slocked.\n", lev2s (b, l, 0));
      return;
    }
  if (!ptstflg (mynum, PFL_LOCK) || (v = plev (mynum)) <= 0)
    {
      erreval ();
      return;
    }
  if (l > v || (v > LVL_APPREN && l > k[wlevel (v)]))
    {
      bprintf ("Sorry, the game is already %slocked.\n", lev2s (b, l, 0));
      return;
    }
  
  y = strlen (wordbuf);
  
  if (strncasecmp (wordbuf, "Off", y) == 0
      || strncasecmp (wordbuf, "Unlock", y) == 0)
    {
      n = 0;
    }
  else if ((x = tlookup (wordbuf, WizLevels)) > 0)
    {
      n = k[x];
    }
  else if ((x = tlookup (wordbuf, MLevels)) > 0)
    {
      n = x;
    }
  else if ((x = tlookup (wordbuf, FLevels)) > 0)
    {
      n = x;
    }
  else if (isdigit (*wordbuf))
    {
      n = atoi (wordbuf);
    }
  else
    {
      bprintf ("Illegal argument to wizlock command.\n");
      return;
    }
  
  if (n > v || (v > LVL_APPREN && n > k[wlevel (v)]))
    {
      bprintf ("You can't %slock the game!\n", lev2s (b, n, 0));
      return;
    }
  the_world->w_lock = n;
  sprintf (s, "&+Y** SYSTEM : The Game is now %slocked.&*\007\007\n",
	   lev2s (b, n, 0));
  broad (s);
}

void
zapcom (void)
{
  int vic, x;
  
  if (!ptstflg (mynum, PFL_ZAP))
    {
      bprintf ("The spell fails.\n");
      return;
    }
  if (ltstflg(ploc(mynum), LFL_JAIL) && (plev(mynum) < LVL_DEMI)) {
    bprintf ("If you're not careful, YOU could be the one being zapped\n");
    return;
   }

  if (brkword () == -1)
    {
      bprintf ("Zap who?\n");
      return;
    }
  if ((vic = pl1) == -1)
    {
      bprintf ("There is no one on with that name.\n");
      return;
    }
  
  if (EQ("Oracle", pname(pl1)))
    {
      bprintf("The Oracle does not want to be zapped.\n");
      return;
    }
  
  /* Message to everyone in the same room */
  send_msg (ploc (vic), 0, pvis (vic), LVL_MAX, vic, NOBODY,
            "\001A\033[1m\003\001c&+RA massive lightning "
            "bolt strikes \003\001D%s\003\001c!\n\003\001A\033[0m\003&*",
            pname (vic));
  
  if (!do_okay (mynum, vic, PFL_NOZAP))
    {
      
      sendf (vic, "&+R%s casts a lightning bolt at you!&*\n",
             see_player (vic, mynum) ? pname (mynum) : "Someone");
      
    }                           /* end ZAP not successful */
  else
    {
      
      sendf (vic, "\001A\033[1m\003&+RA massive lightning bolt arcs down out of "
             "the sky to strike you between&*\n&+Rthe eyes!&*\001A\033[0m\003\n"
             "&+RYou have been utterly destroyed by %s.&*\n",
             see_player (vic, mynum) ? pname (mynum) : "Someone");
      
      
      if (vic < max_players)
        {
#ifdef LOG_ZAP
          mudlog ("%s ZAPped %s", pname (mynum), pname (vic));
#endif
	  
          if (plev (vic) >= LVL_APPREN)
            {
              update_wizlist (pname (vic), LEV_MORTAL);
            }
	  
          deluaf (pname (vic));
	  
          send_msg (ploc (vic), 0, pvis (vic), LVL_MAX, vic, NOBODY,
                    "\001s%s\003%s has just died.\n\003",
                    pname (vic), pname (vic));
	  
          /* Send info-msg. to wizards */
          send_msg (DEST_ALL, 0, LVL_APPREN, LVL_MAX, vic, NOBODY,
                    "[\001p%s\003 has just been zapped by \001p%s\003]\n",
                    pname (vic), pname (mynum));
	  
          x = real_mynum;
          setup_globals (vic);
          crapup ("\t\t&+RBye Bye.... Slain by a Thunderbolt&*", CRAP_RETURN);
          setup_globals (x);
	  
        }                       /* end if PC was zapped */
      else
        {
	  
          wound_player (mynum, vic, pstr (vic) + 1, VERB_ZAP);
	  
        }                       /* end if NPC was zapped */
      
      
    }                           /* end ZAP successful */
  
  broad ("\001dYou hear an ominous clap of thunder in the distance.\n\002");
  
}                               /* end ZAPcom */

void deletecom()
{
  if (!ptstflg(mynum, PFL_DELETE))
    {
      erreval();
      return;
    }
  if (brkword() == -1 || strlen(wordbuf) > PNAME_LEN)
    {
      bprintf("Delete who?\n");
      return;
    }
  mudlog("DELETE: %s deleted %s", pname(mynum), wordbuf);
  deluaf(wordbuf);
  update_wizlist(wordbuf, LEV_MORTAL);
  bprintf("Deleted %s.\n", wordbuf);
}

void
stopcom (void)
{
  if (plev (mynum) < LVL_APPREN)
    {
      erreval ();
      return;
    }
  the_world->w_mob_stop = 1;
  
  send_msg (DEST_ALL, MODE_COLOR|MODE_QUIET, LVL_APPREN, LVL_MAX, NOBODY, NOBODY,
            "[Mobiles STOPped]\n");
}

void
startcom (void)
{
  if (plev (mynum) < LVL_APPREN)
    {
      erreval ();
      return;
    }
  the_world->w_mob_stop = 0;
  
  send_msg (DEST_ALL, MODE_COLOR|MODE_QUIET, LVL_APPREN, LVL_MAX, NOBODY, NOBODY,
            "[Mobiles STARTed]\n");
}

/* Soc stuff, by cornwall */
void socinfo(void)
{
  int i;
  int usercount = 0;

  if (!ptstflg(mynum, PFL_SOCKET) && plev (mynum) < LVL_DEMI)
    {
      erreval ();
      return;
    }
  bprintf ("&+YSocket(?) Status      Name        Host&*\n"
           "&+c-----------------------------------------&*\n");
  for (i = 0; i < max_players; i++)
    {
      if ((pvis (i) > plev (mynum) && i != mynum) ||
#ifdef TCP_ANNOUNCE
        !players[i].inp_handler ||
        (plev(mynum) < LVL_DEMI && !players[i].iamon) ||
#else
        !players[i].iamon ||
#endif
       strncasecmp(pname(i), item1, strlen(item1))
        )
      continue;

      bprintf ("&+C[&+W     %d&+C ] ",i);

      if (!players[i].iamon) {
        if (!EMPTY(pname(i)))
          bprintf ("[&+B  *tcp*&+C  ]");
        else
          bprintf ("[&+B  Setup&+C  ]");
      } else {
        bprintf("[&+BConnected&+C]");
      }

      bprintf ("&+Y %-*s", PNAME_LEN, EMPTY(pname(i)) ? "Undecided" :
pname(i));

      bprintf ("&+B%s&*\n", players[i].hostname);

      usercount++;
  }
  bprintf ("\n\nA total of %d visible connection(s).\n", (long)usercount);
}

void killsocket(void)
{
  int s;
  int old_mynum=mynum;

  if (EMPTY(txt1)) {
      bprintf("No input given.\n");
      return;
  }

  if (!ptstflg(mynum, PFL_SOCKET) && plev(mynum) < LVL_DEMI)
    {
      erreval ();
      return;
    }

  s = atoi(txt1);

  if ((s > 31) || (s<0)) {
    bprintf("Invalid input. Please specify socket number.\n");
    return;
  }

  bprintf("Socket chosen: %d. Trying to clean up socket...\n"
          "Socket killed for %s.\n",s,EMPTY(pname(s)) ?
          "John Doe" : pname(s));

  mudlog("SOCKET: Socket %d killed for %s by %s.",s,EMPTY(pname(s)) ?
         "John Doe" : pname(s),pname(mynum));

  send_msg (DEST_ALL, MODE_QUIET, LVL_APPRENTICE, LVL_MAX, NOBODY, NOBODY,
                    "[%s killed socket %d, player %s]\n",
                    pname (mynum), s, EMPTY(pname(s)) ? "John Doe" : pname(s));

  setup_globals (s);
  crapup ("\tConnection killed by foreign host [bad connect].", CRAP_RETURN);
  setup_globals (old_mynum);

}
