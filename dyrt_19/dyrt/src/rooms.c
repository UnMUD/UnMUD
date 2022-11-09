#include "kernel.h"
#include "locations.h"
#include "objects.h"
#include "exitnames.h"
#include "stdinc.h"

/* A room's short description (title) */
char *sdesc (int room)
{
  return exists (room) ? lshort (room) : "Where no man has gone before";
}

/* A room's long description */
char *ldesc (int room)
{
  return exists (room) ? llong (room) : "";
}

int getexit (int room, int ex)
{
  return lexit (room, ex);
}

/* Set the <dir>-most exit in <room> to <dest>.
 * Return False if the new exit was a room which didn't exist.
 */
void setexit (int room, int dir, int dest)
{
  int i, n;
  
  if (exists (lexit (room, dir)))
    {
      
      for (i = n = 0; i < NEXITS && n <= 1; i++)
	if (lexit (room, i) == lexit (room, dir))
	  ++n;
      
      /* If we are removing the only exit to that room from this: */
      if (n == 1)
	{
	  remove_int (room, lexits_to_me (lexit (room, dir)));
	}
    }
  if (exists (dest))
    {
      add_int (room, lexits_to_me (dest));
    }
  lexit (room, dir) = dest;
  ltouched (room) = True;
}

/*
* return the room number an object is in
* even if it is carried by someone or in something
* [code by Twizzly, July 1995]
*/
int roomobjin(int o)
{
  int i;

  i = oloc(o);
  while( ocarrf(o) == IN_CONTAINER)  /* work our way out of the container */
    {
      o = i;
      i = oloc(o);
    }
  if( ocarrf(o) != IN_ROOM)
    {
      return(ploc(i));
    }
  return i;
}

/* Get a random exit direction from a location, or -1 if none exists. */
int get_rand_exit_dir (int room)
{
  int i, ex[NEXITS], newch, n_exits = 0;
  
  for (i = 0; i < NEXITS; i++)
    {
      if (!exists (lexit (room, i)) &&
	  (lexit (room, i) < DOOR || lexit (room, i) >= EDOOR))
	{
	  continue;
	}
      if (!exists (newch = lexit (room, i)))
	{
	  if (state (newch -= DOOR) > 0)
	    {
	      continue;
	    }
	  else
	    {
	      newch = obj_loc (olinked (newch));
	    }
	}
      /* ex[n_exits++] = newch;  */
      ex[n_exits++] = i;
    }
  return n_exits == 0 ? -1 : ex[my_random () % n_exits];
}

Boolean checklevel (int player, int room)
{
  if (ltstflg(room, LFL_WIZONLY) && (plev(player) < LVL_APPREN)) 
    {
      sendf(player, "I'm sorry, that room is reserved for Immortals.\n");
      return False;
    }
  if (ltstflg(room, LFL_AWIZONLY) && (plev(player) < LVL_ARCHWIZARD))
    {
      sendf(player, "I'm sorry, that room is reserved for Powers.\n");
      return False;
    }
  if (ltstflg(room, LFL_DGODONLY) && (plev(player) < LVL_DEMI))
    {
      sendf(player, "I'm sorry, that room is reserved for Dgod and up.\n");
      return False;
    }
  if (ltstflg(room, LFL_GODONLY) && (plev(player) < LVL_GOD))
    {
      sendf(player, "I'm sorry, that room is reserved for God and up.\n");
      return False;
    }
  return True;
}

/* Get a random exit (the room itself) from a location, or 0 if none exists. */
int get_rand_exit (int room)
{
  int newch;
  int i = get_rand_exit_dir (room);
  
  if (i == -1)
    {
      return 0;
    }
  newch = lexit (room, i);
  return !exists (newch) ? obj_loc (olinked (newch - DOOR)) : newch;
}

int count_players (int loc, int min_lvl, int max_lvl, int flags)
{
  /* Count number of players in a room */
  /* loc == 0 is legal and signify all rooms */
  
  int plx;
  int count = 0;
  int pla, plb;
  Boolean i;
  
  pla = ((flags & COUNT_PLAYERS) != 0) ? 0 : max_players;
  plb = ((flags & COUNT_MOBILES) != 0) ? numchars : max_players;
  i = ((flags & INVERT_LEVELS) != 0);
  
  for (plx = pla; plx < plb; plx++)
    {
      if ((loc == 0 || ploc (plx) == loc) && !EMPTY (pname (plx)))
	{
	  if (plx >= max_players ||
	      ((plev (plx) >= min_lvl && plev (plx) < max_lvl) != i))
	    {
	      ++count;
	    }
	}
    }
  return count;
}

void gotocom (Boolean tiptoe)
{
  char xx[SETIN_MAX];
  int a, pc;
  
  if (!ptstflg (mynum, PFL_GOTO))
    {
      erreval ();
      return;
    }
  if (ltstflg(ploc(mynum),LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD)) 
    {
      bprintf ("Just where do you think yer goin...?\n");
      erreval ();
      return;
    }
  if (brkword () == -1)
    {
      if (!exists (a = find_loc_by_id (phome (mynum))))
	{
	  a = randperc () > 50 ? LOC_START_TEMPLE : LOC_START_CHURCH;
	}
    }
  else if ((a = findroomnum (wordbuf)) == 0)
    {
      bprintf ("Unknown Player, object or room\n");
      return;
    }
  if (ltstflg(a, LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD)) 
    {
      bprintf ("I don't think that would be a good idea...\n");
      return;
    }
#ifdef LOCMIN_RHOME
  if ((a == LOC_RHOME_15) && (!EQ(pname(mynum), "Rex")))
    {
      bprintf("I'm sorry but you must be invited to this room.\n");
      return;
    }
#endif
  if (!checklevel(mynum,a))
    {
      return;
    }
  if (plev (mynum) < LVL_GOD)
    {
      if (ltstflg (a, LFL_PRIVATE))
	{
	  pc = count_players (a, LVL_MIN, LVL_MAX, COUNT_PLAYERS | COUNT_MOBILES);
	  if (pc > 1)
	    {
	      bprintf ("I'm sorry.  There's a private conference in that "
                       "location.\n");
	      return;
	    }
	}
    }
  if (!tiptoe)
    {
      send_msg (ploc (mynum), 0, pvis (mynum), LVL_MAX, mynum, NOBODY, "%s\n", 
                build_setin (xx, cur_player->setmout, pname (mynum), NULL));
    }
  setploc (mynum, a);
  trapch (a);
  if (!tiptoe)
    {
      send_msg (ploc (mynum), 0, pvis (mynum), LVL_MAX, mynum, NOBODY, "%s\n", 
                build_setin (xx, cur_player->setmin, pname (mynum), NULL));
    }
}

int exists (int room)
{
  return (room < 0 && convroom (room) < numloc);
}


/* The EXITS command */
void exitcom ( int check_args )
{
  int a, v, newch;
  int drnum, droff;
  int b = 0;
  char st[64];
  
  int room, exit;
  Boolean gotroom = False, gotexit = False, gotarg;
  
  if( check_args )
  {
    room = !(gotarg = brkword () != -1) ? ploc (mynum) : find_loc_by_name (wordbuf);
  }
  else
  {
    room = ploc( mynum );
    gotarg = False;
  }
  
  if (gotarg && !ptstflg (mynum, PFL_GOTO))
    {
      bprintf ("You aren't powerful enough to give arguments to EXITS.\n");
      return;
    }
  gotroom = exists (room);
  if (!gotroom && !(gotexit = ((exit = tlookup (wordbuf, Exits)) != -1)))
    {
      bprintf ("Non-existant room or exit.\n");
      return;
    }
  if (!check_args || ( !gotexit && brkword () == -1) )
    {
      if (r_isdark (room, mynum))
	{
	  bprintf ("It is dark.....\n");
	  return;
	}
      bprintf ("&+WObvious exits are:&*\n");
      for (a = 0; a < NEXITS; a++)
	{
	  newch = getexit (room, a);
	  if (newch >= DOOR && newch < EDOOR)
	    {
	      /* look through special exits */
	      drnum = newch - DOOR;
	      droff = olinked (drnum);	/*other side */
	      if (!state (drnum))
	        {
		  newch = obj_loc (droff);
		}
	    }
	  if (newch >= 0)
	    {
	      continue;
	    }
	  if (plev (mynum) < LVL_APPREN || (ltstflg(ploc(mynum), LFL_MAZE) &&
	      plev(mynum) < LVL_HIGHARCH))
 	    {
	      bprintf ("%9s : %s\n", Exits[a], sdesc (newch));
	    }
	  else
	    {
	      v = findzone (newch, st);
	      bprintf ("%d:%9s : %-45s : %s%d\n", a, Exits[a], sdesc (newch), st, v);
	    }
	  b = 1;
	}
      if (b == 0)
        {
	  bprintf ("None....\n");
	}
      return;
    }
  if (!gotroom)
    {
      room = ploc (mynum);
    }
  if (lpermanent (room) && !ptstflg (mynum, PFL_ROOM))
    {
      bprintf ("Your are only powerful enough to change exits on\n"
	       "non-permanent (wizard-created) rooms.\n");
      return;
    }
  if (!gotexit && (exit = tlookup (wordbuf, Exits)) == -1)
    {
      bprintf ("Illegal exit-name, use north, east, etc...\n");
      return;
    }
  if (brkword () == -1)
    {
      newch = 0;
    }
  else if (!exists (newch = find_loc_by_name (wordbuf)))
    {
      bprintf ("I don't know that destination.\n");
      return;
    }
  setexit (room, exit, newch);
  bprintf ("Exit %s from %s is now %s.\n", Exits[exit],
	   sdesc (room), (newch == 0) ? "removed" : sdesc (newch));
}

/* Is a room dark or not? It's dark if the Lflag DARK is set but
 * not if there is someone in the room carrying somthing lit.
 */
Boolean roomdark (int room)
{
  int i, j;

  /* ghosts can see in the dark */
  if (ststflg(mynum, SFL_GHOST))
    {
      return False;
    }
  /* if there is a lit wiz in the room, it is light */
  for (i=0; i< lnumchars(room); i++)
    if (ststflg( lmob_nr(i,room), SFL_GLOWING))
      {
        return False;
      }
    /* if LFL_LIGHT, it is never dark */
    if ((xlflags (room) & LFL_L_MASK) == LFL_LIGHT)
      {
        return False;
      }
    /* if REALTIME and not night, it is light */
    if (ltstflg (room, LFL_REALTIME) && !is_real_dark ())
      {
        return False;
      }
    for (i = 0; i < lnumobs (room); i++)
      {
        if (otstbit (lobj_nr (i, room), OFL_LIT) &&
  	   !otstbit (lobj_nr (i, room), OFL_DESTROYED))
 	  {
	    return False;
	  }
      }
    for (i = 0; i < lnumchars (room); i++)
      {
        for (j = 0; j < pnumobs (lmob_nr (i, room)); j++)
  	  {
	    if (otstbit (pobj_nr (j, lmob_nr (i, room)), OFL_LIT) &&
	        !otstbit (pobj_nr (j, lmob_nr (i, room)), OFL_DESTROYED))
	      {
	        return False;
 	      }
 	  }
      }
  return True;
}

/* Determine if a player can see in a room or not. Same as above with the
 * difference that if they are wiz, they will allways be able to see. */
Boolean r_isdark (int room, int plr)
{
  return roomdark (room) && plev (plr) < LVL_APPREN;
}

Boolean isdark ()
{
  return r_isdark (ploc (mynum), mynum);
}

void teletrap (int newch)
{
  if (ltstflg(ploc(mynum),LFL_JAIL) && (plev(mynum) < LVL_ARCHWIZARD)) 
    {
      bprintf ("For some reason, you are still in the jail\n");
      return;
    }
  send_msg (ploc (mynum), 0, pvis (mynum), LVL_MAX, mynum, NOBODY,
	    "%s has left.\n", pname (mynum));
  send_msg (newch, 0, pvis (mynum), LVL_MAX, mynum, NOBODY,
	    "%s has arrived.\n", pname (mynum));
  trapch (newch);
}

Boolean trapch (int loc)
{
  setploc (mynum, loc);
  lookin (loc, 0);

  if (ltstflg (loc, LFL_DEATH))
    {
      if (plev (mynum) < LVL_APPREN)
	{
          if (ststflg(mynum, SFL_GHOST)) 
 	    {
              p_crapup (mynum, "\t\tYou fade to nothing, and vanish.", CRAP_SAVE);
              return False; 
            }
          send_msg(DEST_ALL, MODE_QUIET, LVL_APPREN, LVL_MAX, mynum, NOBODY,
	           "&+Y[%s has discovered a new deathroom.]\n", pname(mynum));
	  crapup ("\t\tYou seem to have died...", CRAP_SAVE);
	  return False;
	}
    }
  if (pfighting (mynum) >= 0 && ploc (mynum) != ploc (pfighting (mynum)))
    {
      setpfighting (mynum, -1);
    }
  return True;
}

void lookin (int loc, int showfl)
{
  char wbuff[200];
  char tbuff[50];

  if ((ptstflg (mynum, PFL_GOTO) || plev (mynum) >= LVL_APPREN))
    {
      sprintf (wbuff, "<%s>", showname (loc));
    }
  if ((!ststflg (mynum, SFL_BRIEF) && plev (mynum) >= LVL_APPREN))
    {
      /* show some of the usefull flags */
      if ((xlflags (loc) & LFL_L_MASK) == LFL_DARK)
	{
          if (roomdark (loc))
            {
              /* location is really dark */
              strcat (wbuff, " DARK ");
            }
          else
            {
              /* location is dark but there is light present */
              strcat (wbuff, " dark ");
 	    }
 	}
      if (ltstflg (loc, LFL_DEATH))
	{
          strcat (wbuff, " DEATH ");
	}
      if (ltstflg (loc, LFL_PRIVATE))
	{
          strcat (wbuff, " PRIVATE ");
	}
      if (ltstflg (loc, LFL_PARTY))
	{
          strcat (wbuff, " PARTY ");
	}
      if (ltstflg (loc, LFL_ON_WATER))
	{
          strcat (wbuff, " ON-WATER ");
	}
      if (ltstflg (loc, LFL_IN_WATER))
	{
          strcat (wbuff, " IN-WATER ");
	}
      /* display weather and temperature at the location */
      strcat (wbuff, " [");
      strcat (wbuff, real_light_state (loc));
      strcat (wbuff, "]");
      if (strncmp (real_temp_state (loc), "Very Hot", 8) == 0)
	{
          strcat (wbuff, " [");
	}
      if (strncmp (real_temp_state (loc), "Hot", 3) == 0)
	{
          strcat (wbuff, " [");
	}
      if (strncmp (real_temp_state (loc), "Warm", 4) == 0)
	{
          strcat (wbuff, " [");
	}
      if (strncmp (real_temp_state (loc), "Cool", 4) == 0)
	{
          strcat (wbuff, " [");
	}
      if (strncmp (real_temp_state (loc), "Cold", 4) == 0)
	{
          strcat (wbuff, " [");
	}
      if (strncmp (real_temp_state (loc), "Very Cold", 9) == 0)
	{
          strcat (wbuff, " [");
	}
      strcat (wbuff, real_temp_state (loc));
      strcat (wbuff, "]");
    }

  /* now print out the buffer that we have built up */
  if ((ptstflg (mynum, PFL_GOTO) || plev (mynum) >= LVL_APPREN))
    {
      strcat (wbuff, "\n");
      bprintf (wbuff);
    }

/* now print out what the mortals see */
  if (ststflg (mynum, SFL_BLIND))
    {
      bprintf ("You're blind, you can't see a thing!\n");
    }
  else
    {
      if (isdark ())
        {
          bprintf ("It's dark.  Be careful or you may be eaten by a Grue!\n");
          return;
        }
      else
        {
          bprintf ("%s\n", sdesc (loc));
        }
    }

  /* print out the description of the room as long as the player can see */
  if (!isdark () && !ststflg (mynum, SFL_BLIND))
    {
      if ((showfl & SHOW_LONG) != 0 || ststflg (mynum, SFL_BRIEF) == 0)
        {
          bprintf ("%s", ldesc (loc));
        }
      list_objects (1 << OFL_NOGET, True);
      show_weather ();
      list_objects (1 << OFL_NOGET, False);
      list_people ();
      bprintf ("\n");
      if (ststflg (mynum, SFL_AUTOEXIT))
 	{
          exitcom ( 0 );
	}
    }
  bprintf ("\n");
}

char *showname (int loc)
{
  static char a[64];

  return xshowname (a, loc);
}

char *xshowname (char *b, int loc)
{
  char v[64];
  
  sprintf (b, "%s", buildname (v, loc));
  return b;
}

char *buildname (char *b, int loc)
{
  int k;
  char n[64];
  
  k = findzone (loc, n);
  sprintf (b, "%s%d", n, k);
  return b;
}

/* Find Location By Name
 *
 * Name is either <zonename/abbrev><offsett>    (for instance "cat44")
 *             or <absolute room number>        (for instance "-233")
 *
 * Return location, or 0 on error, or 1 if correct zonename (only) was given.
 */
int find_loc_by_name (char *name)
{
  int y;
  char buff[MAX_COM_LEN], *b = buff;
  char *p = name;
  int n;
  
  while (isalpha (*p))
    *b++ = *p++;
  *b = '\0';
  n = atoi (p);
  if (*buff == '\0')
    {
      return exists (n) ? n : 0;
    }
  if ((y = get_zone_by_name (buff)) != -1)
    {
      return n == 0 ? 1 : getlocid (y, n);
    }
  else
    {
      return 0;
    }
}

/*
 * Basic function to find a room number from: 1) zonename<offset>,
 *                                        or  2) mobile/player
 *                                        or  3) object
 *                                        or  4) zonaname (no offset, assumes1)
 * Return room number, or 0 on error.
 */
int findroomnum (char *w)
{
  int loc, a;
  
  while (*w != 0 && *w != '-' && !isalnum (*w))
    ++w;
  if ((loc = find_loc_by_name (w)) < 0)
    {
      return loc;
    }
  else if ((a = fpbn (w)) != -1)
    {
      return ploc (a);
    }
  else if ((a = fobn (w)) != -1)
    {
      return obj_loc (a);
    }
  else if (loc == 1)
    {
      return getlocnum (w, 1);
    }
  else
    return 0;
}

/* Same as findroomnum(), but use brkword() and wordbuf. */
int getroomnum ()
{
  return (brkword () == -1) ? 0 : findroomnum (wordbuf);
}


/* Find location's in-game index from its ID.  Return 0 if not found. */
int find_loc_by_id (long int id)
{
  long int x;
  
  if (id < 0 && convroom (id) < num_const_locs)
    {
      return id;
    }
  return (x = lookup_entry (id, &id_table)) == NOT_IN_TABLE
          || !exists (x) ? 0 : x;
}

Boolean cango (int d)
{
  if (getexit (ploc (mynum), d) == 0)
    {
      return False;
    }
  if (getexit (ploc (mynum), d) < 0)
    {
      return True;
    }
  if (getexit (ploc (mynum), d) < DOOR)
    {
      return False;
    }
  if (state (getexit (ploc (mynum), d) - DOOR) == 0)
    {
      return True;
    }
  return False;
}

Boolean reset_location (int loc)
{
  Boolean missing_exit = False;
  
  xlflags (loc) = xlflags_reset (loc);
  if (ltouched (loc))
    {
      int i, x;
      for (i = 0; i < NEXITS; i++)
	{
	  missing_exit |= (x = lexit_reset (loc, i)) != 0 &&
	                  (x < DOOR || x > EX_MAX) &&
		          (x = find_loc_by_id (x)) == 0;
	  setexit (loc, i, x);
	}
      ltouched (loc) = False;
    }
  return !missing_exit;
}
