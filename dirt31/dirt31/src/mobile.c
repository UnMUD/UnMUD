#include "kernel.h"
#include "pflags.h"
#include "mflags.h"
#include "sflags.h"
#include "oflags.h"
#include "lflags.h"
#include "quests.h"
#include "cflags.h"
#include "levels.h"
#include "locations.h"
#include "timing.h"
#include "jmp.h"
#include "sendsys.h"
#include "objects.h"
#include "mobiles.h"
#include "rooms.h"
#include "mobile.h"
#include "fight.h"


extern char *WizLevels[];



/* Compare players (for the qsort() function)
 */
static int cmp_player(int *a, int *b)
{
	return plev(*b) - plev(*a);
}



/* The USERS command. Show names and locations of users. Possibly also
 * their local host if PFL_SHUSERS is set.
 */
void usercom(void)
{
	char locnam[256], wiznam[128], src[256], buff[128];
	int  i;
	int  a[256], a_len = 0;

	if (plev(mynum) < LVL_WIZARD && !the_world->w_tournament) {
		erreval();
		return;
	}

	for (i = 0; i < max_players; i++)
	        if (is_in_game(i) && (pvis(i) <= plev(mynum) || i == mynum)){

			a[a_len++] = i;
		}

	qsort(a, a_len, sizeof(int), cmp_player);

	for ( i = 0 ; i < a_len ; ++i ) {

		*wiznam = '\0';

		if (!exists(ploc(a[i]))) {
			sprintf(locnam,"NOT IN UNIVERSE[%d]", ploc(a[i]));
		} else {
			strcpy(locnam, sdesc(ploc(a[i])));
			if (plev(mynum) >= LVL_WIZARD)
			  sprintf(wiznam, "| %s", xshowname(buff, ploc(a[i])));
		}



		if (ptstflg(mynum,PFL_SHUSER)) {
			sprintf(src,"[%s]", players[a[i]].hostname);
			bprintf("%-*s %-22.22s %c%-27.27s%s\n", PNAME_LEN, 
				pname(a[i]),
				src,
				ststflg(a[i], SFL_OCCUPIED) ? '*' : ' ',
				locnam,
				wiznam );
		} else {
			bprintf("%-*s %c%-45.45s%s\n", PNAME_LEN + 5,
				pname(a[i]),
				ststflg(a[i], SFL_OCCUPIED) ? '*' : ' ',
				locnam,
				wiznam );
		}
	}
}





/* The WHO command
 */
void whocom(void)
{
	int  i;
	int  a[256], a_len = 0;

	for (i = 0; i < max_players; i++)
		if (is_in_game(i) && (pvis(i) <= plev(mynum) || i == mynum)){

			a[a_len++] = i;
		}

	qsort(a, a_len, sizeof(int), cmp_player);

	bprintf("Players\n-------\n");

	for ( i = 0 ; i < a_len ; ++i ) {

		if (pvis(a[i]) > 0)  bprintf("(");

		bprintf( "%s", make_title(ptitle(a[i]), pname(a[i])) );

		if (pvis(a[i]))  bprintf(")");

		if (plev(mynum) >= LVL_WIZARD) {
			if (players[a[i]].aliased
			    || players[a[i]].polymorphed >= 0) {
				bprintf(" [out of %s head]", his_or_her(a[i]));
			}
			if (ststflg(a[i], SFL_QUIET)) bprintf(" <Quiet>");
			if (ststflg(a[i], SFL_NOWIZ)) bprintf(" <NoWiz>");
		}
		bprintf("\n");
	}
	bprintf("\nThere %s %d visible player%s currently on the game.\n",
		(a_len > 1 ? "are" : "is"), a_len, (a_len > 1 ? "s" : "") );
}


/* The MWHO command
 */
#define COLUMNS (72/MNAME_LEN)
void mwhocom(void)
{
	char buff[64];
	int  i;
	int  ct;
	int zone;
	int nr;

	Boolean list_all = True;
	int first = max_players;
	int last = numchars;

	if (plev(mynum) < LVL_WIZARD) {
		erreval();
		return;
	}

	if (brkword() != -1) {
		list_all = False;
  
		if ((zone = get_zone_by_name(wordbuf)) == -1) {

			bprintf("No such zone: %s\n", wordbuf);
			return;
		}

		first = 0;
		last = znumchars(zone);
	}

	bprintf("Live Mobiles\n------------\n\n");

	for (ct = first, nr = 0; ct < last; ++ct ) {

		i = list_all ? ct : zmob_nr(ct, zone);
 
		if (!EMPTY(pname(i)) && pstr(i) >= 0 &&
		    ( pvis(i) <= 0 || pvis(i) <= plev(mynum))) {

			++nr;

			if (pvis(i) > 0)
			        sprintf(buff, "(%s)", pname(i));
			else
			        strcpy(buff, pname(i));

			bprintf("%-*s%c",
				PNAME_LEN+3, buff, (nr%COLUMNS) ? ' ' : '\n');
		}
	}

	bprintf("\n\nThere are %d mobiles left (from %d)\n",
		nr, last - first);
}


int player_damage(int player)
{
	int w;
	int damage = pdam(player);

	if ( (w = pwpn(player)) != -1 && iscarrby(w,player) && iswielded(w) ) {

		damage += odamage(w);
	}
	return damage;
}

int player_armor(int player)
{
	int i, j;
	int armor = parmor(player);

	for (j = 0; j < pnumobs(player) && (i = pobj_nr(j, player),1); j++) {

		if (iswornby(i, player) && otstbit(i, OFL_ARMOR)) {
			armor += oarmor(i);
		}
	}
	return armor;
}




/* The STATS command WITHOUT arguments.
 */
static void ustats(void)
{
	int  i, j;
	int  a[256], a_len = 0;
	char buff[64];

	if (plev(mynum) < LVL_WIZARD) {
		erreval();
		return;
	}

	for (i = 0; i < max_players; i++)
	        if (is_in_game(i) && (pvis(i) <= plev(mynum) || i == mynum)){

			a[a_len++] = i;
		}

	qsort(a, a_len, sizeof(int), cmp_player);

	bprintf("Level     Name         Str/Max Dam Arm Vis"
		"     Idle    On For  Location\n-----------------------"
		"-------------------------------------------------------\n");

	for ( j = 0 ; i = a[j], j < a_len ; ++j ) {

		if (plev(i) <= LVL_WIZARD)
		        bprintf("[%7d]", plev(i));
		else    bprintf("[%7.7s]", WizLevels[wlevel(plev(i))]);

		bprintf(" %-*s", PNAME_LEN, pname(i));

		if (plev(i) < LVL_WIZARD) {

			bprintf(" %3d/%-3d %3d %3d",
				pstr(i), pmaxstrength(plev(i)),
				player_damage(i),
				player_armor(i));
		}
		else bprintf(" ---/--- --- ---");

		bprintf(pvis(i) < LVL_WIZARD ? " %3d" : " inv", pvis(i));

		strcpy(buff, sec_to_hhmmss( global_clock - plast_cmd(i)));

		bprintf("%9.9s%10.10s  %s\n",
			buff,
			sec_to_hhmmss( global_clock - plogged_on(i) ),

			showname( ploc(i) ));
	}
	bprintf("\n");
}



/* The STATS command
 */
void showplayer(void)
{
  extern    char *Mflags[];
  char      buff[80];
  PERSONA   d;
  int       b;
  int       max_str;
  int       i, j, w, armor;
  char      *title, *name;
  Boolean   in_file, is_mobile;

  if (!ptstflg(mynum, PFL_STATS)) {
    erreval();
    return;
  }

  if (brkword() == -1) {
	  ustats();
	  return;
  }

  if ((b = fpbns(wordbuf)) != -1 && seeplayer(b)) {
    in_file = False;
    is_mobile = b >= max_players;
    player2pers(&d,NULL,b);
    name = pname(b);
    title = is_mobile ? NULL : ptitle(b);
    d.p_home = find_loc_by_id( is_mobile ? ploc_reset(b) : phome(b) );
    max_str  = is_mobile ? pstr_reset(b) : maxstrength(b);
  } else if (getuaf(wordbuf, &d)) {
    in_file = True;
    is_mobile = False;
    title = d.p_title;
    name  = d.p_name;
    max_str = pmaxstrength(d.p_level);
  } else {
    bprintf("Who's that?\n");
    return;
  }

  bprintf("Name       : %s", name);
  if (!is_mobile) {
    bprintf("\nTitle      : %s", make_title(title,"<name>"));
    bprintf("\nScore      : %d", d.p_score);
    bprintf("\nLevel      : %d", d.p_level);
  }

  bprintf("\nStrength   : %d / %d", d.p_strength, max_str);

  bprintf("\nDamage     : %d", d.p_damage);

  if (!in_file && (w = pwpn(b)) != -1 && iscarrby(w,b) && iswielded(w)) {

    int w_damage = is_mobile ? odamage(w) / 2 : odamage(w);

    bprintf(" + %s(%d) = %d",
	    oname(w), w_damage, w_damage + d.p_damage);
  }

  bprintf("\nArmor      : %d", armor = d.p_armor);

  if (!in_file) {

    for (j = 0; j < pnumobs(b) && (i = pobj_nr(j, b),1); j++) {

      if (iswornby(i, b) && otstbit(i, OFL_ARMOR)) {
	armor += oarmor(i);
	bprintf(" + %s(%d)", oname(i), oarmor(i));
      }
    }
    if (armor != d.p_armor)
      bprintf(" = %d", armor);
  }

  bprintf("\nVisibility : %d ", d.p_vlevel);

  if (is_mobile) {
    bprintf("\nAggression : %d %%", pagg(b));
    bprintf("\nSpeed      : %d", pspeed(b));
    bprintf("\n%s      : %s",
	    zpermanent(pzone(b)) ? "Zone " : "Owner", zname(pzone(b)));
  }

/*  bprintf("\nWimpy      : %d", d.p_wimpy); */

  if ( exists(w = find_loc_by_id(d.p_home)) )
    bprintf("\nStart      : %s \t(%s)", sdesc(w), xshowname(buff, w));

  if (!in_file) {
    bprintf("\nLocation   : %s \t(%s)",
	    sdesc(ploc(b)), xshowname(buff, ploc(b)));

    bprintf("\nCondition  :");

    if (pfighting(b) >= 0) 
        bprintf(" Fighting %s", pname(pfighting(b)));

    if (phelping(b) >= 0)
	bprintf(" Helping %s", pname(phelping(b)));

    if (psitting(b))
        bprintf(" Sitting");

    if (is_mobile) {
	bprintf("\nBehavior   : ");
	show_bits( (int *)&mflags(b), sizeof(MFLAGS)/sizeof(int), Mflags );
    }
    else if (ptstflg(mynum, PFL_SHUSER)) {
	bprintf("\nFrom Host  : %s", players[b].hostname);
    }

  } else {
    bprintf("\nLast on    : %s", time2ascii(d.p_last_on));
  }

  if (d.p_sflags != 0) {
    if (!is_mobile) bprintf("\n");
    bprintf("Various    : ");
    show_bits( (int *)&(d.p_sflags), sizeof(SFLAGS)/sizeof(int), Sflags );
  }

  bprintf("\n");
}


/* The MOBILE command
 */
void mobilecom(void)
{
	int  i;
	int  ct;
	int zone;
	int live = 0;
	int dead = 0;

	Boolean list_all = True;
	int first = max_players;
	int last = numchars;

	if (plev(mynum) < LVL_WIZARD) {
		erreval();
		return;
	}

	if (brkword() != -1) {
		list_all = False;
  
		if ((zone = get_zone_by_name(wordbuf)) == -1) {

			bprintf("No such zone: %s\n", wordbuf);
			return;
		}

		first = 0;
		last = znumchars(zone);
	}

	bprintf("Mobiles");

	if (the_world->w_mob_stop)  bprintf("   [Currently STOPped]");

	bprintf("\n\n");

	for (ct = first; ct < last; ++ct ) {

		i = list_all ? ct : zmob_nr(ct, zone);
 
		bprintf("[%d] %-*s %c", i + GLOBAL_MAX_OBJS, MNAME_LEN,
			EMPTY(pname(i)) ? pname_reset(i) : pname(i),
			ststflg(i, SFL_OCCUPIED) ? '*' : ' ');

		if (EMPTY(pname(i))) {
		        bprintf("<QUIT or KICKED OFF>\n");
			++dead;
		}
		else {
		        desrm(ploc(i), IN_ROOM);

			if (pstr(i) < 0)
			        ++dead;
			else
			        ++live;
		}
	}

	bprintf("\nA total of %d live mobile(s) + %d dead = %d.\n",
		live, dead, live + dead);
}


/* List the people in a room as seen from myself
 */
void list_people(void)
{
	int i, j;
	int room = ploc(mynum);

	for (j = 0; j < lnumchars(room) && (i = lmob_nr(j, room) ,1); j++) {

		if (i != mynum && is_in_game(i) && seeplayer(i)) {

			if (plev(mynum) >= LVL_WIZARD && pvis(i) > 0)
			        bprintf("(");

			if (i >= max_players) {
				bprintf("%s", pftxt(i));
			} else {
				bprintf("%s", make_title(ptitle(i), pname(i)));

				if (psex(i))
				        cur_player->wd_them =
					strcpy(cur_player->wd_her, pname(i));
				else
				        cur_player->wd_them =
					strcpy(cur_player->wd_him, pname(i));
			}

			if (i < max_players) {
				bprintf(" is %shere.",
					psitting(i) ? "sitting " : "");
			}

			if (plev(mynum) >= LVL_WIZARD && pvis(i) > 0)
			        bprintf(")");

			bprintf("\n");

			if (gotanything(i)) {
				bprintf("%s is carrying:\n", pname(i));
				mlobjsat(i, 8);
			}
		}
	}
}

void move_mobiles(void)
{
  int mon;

  for (mon = max_players; mon < numchars; mon++) {
    if (!ststflg(mon, SFL_OCCUPIED) && pstr(mon) > 0)
      consid_move(mon);		/* Maybe move it */
  }

  onlook();
}


/*  Fight controll
 */
void onlook(void)
{
	int i, j;

	for (i = numchars - 1; i >= 0; i--) {

		if (is_in_game(i) && (j = pfighting(i)) >= 0) {

			if (testpeace(i) || !is_in_game(j) ||
			    ploc(i) != ploc(j)) {

				pfighting(i) = pfighting(j) = -1;

			} else {

				hit_player(i, j, pwpn(i));

                                #if 0
				/* "Wimpy" code, make victim flee if his
				 * strength is less then his "wimpy" val.
				 */
				if (j < max_players && 
				    pstr(j) >= 0 && pstr(j) < pwimpy(j)) {

					int x = mynum;

					setup_globals(j);
					gamecom("flee");
					setup_globals(x);
				}
				#endif
			}
		}
	}

	for (i = max_players; i < numchars; i++) {
		chkfight(i);
	}

	if (ocarrf(OBJ_CASTLE_RUNESWORD) >= CARRIED_BY) {
		dorune(oloc(OBJ_CASTLE_RUNESWORD));
	}

	for (i = 0; i < max_players; i++) {
		if (is_in_game(i) && phelping(i) != -1) {
			helpchkr(i);
		}
	}
}


void chkfight(int mon)
{
  int plx;
  int i;

  if (mon < max_players || mon >= numchars || pagg(mon) == 0 ||
      ststflg(mon, SFL_OCCUPIED) || EMPTY(pname(mon)) ||
      pstr(mon) < 0 || testpeace(mon) || pfighting(mon) >= 0) /* Already? */
	return;

  /* See if we can hit someone.... */

  for (plx=lfirst_mob(ploc(mon)); plx != SET_END; plx=lnext_mob(ploc(mon))) {

    if (plx < max_players && is_in_game(plx)
	    && !ptstflg(plx, PFL_NOHASSLE) && pvis(plx) == 0) {

		if (mtstflg(mon,MFL_CROSS) &&
		    carries_obj_type(plx, OBJ_CHURCH_CROSS) > -1)
		      continue;

		if (randperc() >= pagg(mon))
		      continue;

	        if (mtstflg(mon,MFL_NOHEAT) && p_ohany(plx,(1 << OFL_LIT))) {
		 sendf(plx,
	        "%s seems disturbed by naked flame, and keeps its distance.\n",
		  pname(mon));
		  continue;
	        }

		hit_player(mon,plx,pwpn(mon)); /* Start the fight */
    }
    }
}

void consid_move(int mon)
{
  static char *m[] = {
    "Nicknack hasn't fed me today.",
    "Look at the pretty colors!",
    "My god!  It's full of stars!",
    "To iterate is human.  To recurse, divine.",
    "How'd those fish get up there?",
    "Hi!",
    "Shoikana has beautiful sparkling eyes!",
    "Alf doesn't respond, he must be hacking."
    };

  int  plx;

  if (EMPTY(pname(mon)))
    return;

    for (plx = 0; plx < lnumchars(ploc(mon)); plx++) {

	    if (pfighting( lmob_nr(plx, ploc(mon)) ) == mon)  return;
    }

  if (randperc()*6/(10*TIMER_INTERRUPT) < pspeed(mon))
      movemob(mon);

  if (pnum(mon) == MOB_VALLEY_CHICKEN && randperc() < 8)
    sendf(ploc(mon), "\001p%s\003 says 'The sky is about to fall in.'\n",
	  pname(mon));
  if (pnum(mon) == MOB_CATACOMB_GHOST && randperc() < 12)
    sendf(ploc(mon), "The Ghost moans, sending chills down your spine.\n");
  if (pnum(mon) == MOB_START_PUFF && randperc() < 8) {
    sendf(ploc(mon), "\001p%s\003 says '%s'\n",
	  pname(mon), m[my_random()%arraysize(m)]);
  }

  if (mtstflg(mon, MFL_THIEF) && randperc() < 20 && stealstuff(mon)
      || mtstflg(mon, MFL_PICK) && randperc() < 40 && shiftstuff(mon))
    return;
}    

/* Handle Runesword */
void dorune(int plx)
{
  int ply, i;

  if (pfighting(plx) >= 0 || testpeace(plx))
    return;

    for (ply=lfirst_mob(ploc(plx)); ply != SET_END; ply= lnext_mob(ploc(plx))){

    if (ply != plx && !EMPTY(pname(ply)) &&
	plev(ply) < LVL_WIZARD && fpbns(pname(ply)) >= 0 &&
	randperc() >= 9 * plev(plx)) {
      sendf(plx,"The Runesword twists in your hands, lashing out savagely!\n");
      hit_player(plx, ply, OBJ_CASTLE_RUNESWORD);
      return;
    }
  }
}

Boolean dragget(void)
{
  int l;

  if (plev(mynum) >= LVL_WIZARD)
    return False;
  if ((l = alive((max_players + MOB_CAVE_DRAGON))) != -1 &&
      ploc(l) == ploc(mynum)) {
    bprintf("The dragon makes it quite clear that he doesn't want "
	    "his treasure borrowed!\n");
    return True;
  }
  if ((l = alive(max_players + MOB_OAKTREE_COSIMO)) != -1 &&
      ploc(l) == ploc(mynum)) {
    bprintf("Cosimo guards his treasure jealously.\n");
/*    hit_player(l,mynum,pwpn(l));*/
    return True;
  }
  return False;
}

void helpchkr(int plx)
{
  int x = phelping(plx);

  if (!is_in_game(x)) {
	  sendf(plx, "You can no longer help.\n");
	  setphelping(plx, -1);
  } else
  if (ploc(x) != ploc(plx)) {
	  sendf(plx, "You can no longer help %s.\n", pname(x));
	  sendf(x, "%s can no longer help you.\n", pname(plx));
	  setphelping(plx, -1);
  }	  
}



void movemob(int x)
{
  int n, r;

  if (the_world->w_mob_stop)
    return;
  r = randperc() % 6;   /* change this.... here chance is less if few exits*/
  if ((n = getexit(ploc(x),r)) >= EX_SPECIAL)
    return;
  if (n >= DOOR) {
    if (state(n - DOOR) > 0)
      return;
    n = /*oloc((n - DOOR) ^ 1);*/ obj_loc(olinked(n - DOOR));
  }
  if (n >= 0 || ltstflg(n, LFL_NO_MOBILES) || ltstflg(n, LFL_DEATH))
    return;

  send_msg(ploc(x), 0, pvis(x), LVL_MAX, x, NOBODY,
	   "%s has gone %s.\n", pname(x), exits[r]);

  setploc(x, n);

  send_msg(ploc(x), 0, pvis(x), LVL_MAX, x, NOBODY, 
	   "%s has arrived.\n", pname(x));
}



void stopcom(void)
{
  if (plev(mynum) < LVL_WIZARD) {
    erreval();
    return;
  }
  the_world->w_mob_stop = 1;

  send_msg(DEST_ALL, MODE_QUIET, LVL_WIZARD, LVL_MAX, NOBODY, NOBODY,
	  "[Mobiles STOPped]\n");
}

void startcom(void)
{
  if (plev(mynum) < LVL_WIZARD) {
    erreval();
    return;
  }
  the_world->w_mob_stop = 0;

  send_msg(DEST_ALL, MODE_QUIET, LVL_WIZARD, LVL_MAX, NOBODY, NOBODY,
	  "[Mobiles STARTed]\n");
}



/*
 * Steal random object from random player in the same room
 * -Nicknack  Sep. 1990
 * improved by Alf Oct, 1991
 * improved by Nicknack May, 1993
 */
Boolean stealstuff( int m )
{
	int p[50]; /* Room for mortals and objects */
	int i, j, k;
	int nr = 0;

	/* Count the number of mortals in the same room:
	 */
	for (j = 0; j < lnumchars(ploc(m)) && nr < 50; j++) {
		i = lmob_nr(j, ploc(m));

		if (is_in_game(i) && i < max_players && plev(i) < LVL_WIZARD) {
			p[nr++] = i;
		}
	}
	if (nr == 0)  return False;

	/* select a random one of those in the same room:
	 */
	i = p[randperc() % nr];

	/* Count the number of objects he carries that we can take:
	 */
	for (nr = k = 0; k < pnumobs(i) && nr < 50; k++) {
		j = pobj_nr(k, i);

		if (iscarrby(j,i)) {
			if (((ocarrf(j) == WORN_BY || ocarrf(j) == BOTH_BY)
			     && !mtstflg(m, MFL_SWORN)) 
			    ||
			    ((ocarrf(j) == WIELDED_BY || ocarrf(j) == BOTH_BY)
			     && !mtstflg(m, MFL_SWPN)))
			  continue;
			else 
			  p[nr++] = j;
		}
	}

	if (nr == 0)  return False;

	/* Select random object from those he carries
	 */
	j = p[randperc() % nr];

	sendf(i, "\001p%s\003 steals the %s from you!\n", pname(m), oname(j));

	send_msg(ploc(i), 0, LVL_MIN, LVL_MAX, i, NOBODY,
		 "\001p%s\003 steals the %s from \001p%s\003!\n",
		 pname(m), oname(j), pname(i));
 
	if ( otstbit(j, OFL_WEARABLE)) {
		setoloc(j, m, WORN_BY);
	} else {
		setoloc(j, m, CARRIED_BY);
	}

	if ( otstbit(j, OFL_WEAPON) &&
	    (pwpn(m) == -1 || odamage(pwpn(m)) < odamage(j))) {
		set_weapon(m, j);
	}
	return True;
}


Boolean shiftstuff(int m)
{
	Boolean  took = False;
	int      a, b;
	int      maxdam = 0, w  = -1;
	int      maxarm = 0, ww = -1;

	for (b = 0; b < lnumobs(ploc(m)); b++) {

		a = lobj_nr(b, ploc(m));

		if (!oflannel(a)) {
			took = True;

			sendf(ploc(m), "\001p%s\003 takes the %s.\n",
			      pname(m), oname(a));

			/* Wield the best weapon, wear the best armor:
			 */
			if (otstbit(a, OFL_WEAPON) && odamage(a) > maxdam) {
				w = a;
				maxdam = odamage(a);
			}
			if (otstbit(a, OFL_ARMOR) && oarmor(a) > maxarm) {
				ww = a; 
				maxarm = oarmor(a);
			}
			setoloc(a, m, CARRIED_BY);
		}

		if (w >= 0) set_weapon(m, w);
		if (ww >= 0) {
			setoloc(ww, m, (w == ww) ? BOTH_BY : WORN_BY);
		}
	}
	return took;
}


char *xname(char *n)
{
  char *t;

  if (n != NULL && (t = strrchr(n,' ')) != NULL) {
    return t + 1;
  }
  return n;
}



void setname(int plx)
{
  register PLAYER_REC *p = cur_player;

  if (psex(plx)) p->wd_them = strcpy(p->wd_her, pname(plx));
  else p->wd_them = strcpy(p->wd_him, pname(plx));
}

Boolean see_player(int pla, int plb)
{
  if (pla < 0 || pla >= numchars) return False;
  if (plb == pla || plb < 0 || plb >= numchars) return True;
  if (pvis(plb) > 0 && plev(pla) < pvis(plb)) return False;
  if (ststflg(pla, SFL_BLIND)) return False;
  if (ploc(pla) == ploc(plb) && r_isdark(ploc(pla), pla)) return False;
  return True;
}


char *see_name(int pla, int plb)
{
  return see_player(pla,plb) ? pname(plb) : "Someone";
}

Boolean seeplayer(int plx)
{
  if (plx == mynum || plx < 0 || plx >= numchars) return True;
  if (!see_player(mynum,plx)) return False;
  setname(plx);
  return True;
}




/* Return a player index given a target name on one of the forms:
 * 1) <player-number>
 * 2) <player-name>
 * 3) <player-name><number-in-sequence-with-that-name>
 *
 * Return -1 if not found.
 */
int find_player_by_name(char *name)
{
	char b[MNAME_LEN + 1], *p = b;
	int n, i;

	if (name == NULL || strlen(name) > MNAME_LEN)  return -1;

	name = xname(name);         /* Skip the "The " if there. */

	while (*name != '\0' && isalpha(*name)) *p++ = *name++;
	*p = '\0';

	if (isdigit(*name)) {
	        n = atoi(name);

		while (isdigit(*++name));
		if (*name != '\0') return -1;
	}
	else if (*name != '\0') {
	        return -1;
	}
	else n = 1;

	if (*b == '\0') {

		if (n >= GLOBAL_MAX_OBJS && n < GLOBAL_MAX_OBJS + numchars
		                         && is_in_game(n - GLOBAL_MAX_OBJS))
		        return n - GLOBAL_MAX_OBJS;
		else
		        return -1;
	} else {
		/* Try the mobiles in the players location first:
		 */
		if (is_in_game(mynum)) {
			int m = n;
			int loc = ploc(mynum);

			for (i = 0; i < lnumchars(loc); i++) {

				if ( EQ(b, xname(pname(lmob_nr(i, loc))))
				    && is_in_game(lmob_nr(i, loc))
				    && --m == 0)

				        return lmob_nr(i, loc);
			}

			if (m < n) return -1;
		}

		/* Now try anyone.
		 */
		for (i = 0; i < numchars; i++) {

			if (EQ(b,xname(pname(i))) && is_in_game(i) && --n == 0)
			        return i;
		}
	}

	return -1;
}


/* Find player by name, if visible to mynum:
 */
int fpbn(char *name)
{
	int n = find_player_by_name(name);

	return n < 0 || seeplayer(n) ? n : -1;
}


/* Find mobile's in-game index from its ID. Return -1 if not found.
 */
int find_mobile_by_id(long int id)
{
	long int x;

	if (id >= max_players && id < num_const_chars) return id;

	return (x = lookup_entry(id, &id_table)) == NOT_IN_TABLE
	  || x < 0 || x >= numchars ? -1 : x;
}



/* Find player or mobile, return number if in game.
 * Set f to true if in file, False if in game. If exists, put
 * the data in the object pointed to by p.
 */
int find_player(char *name,PERSONA *p,Boolean *f)
{
  int plr;

  *f = False;
  if ((plr = fpbns(name)) >= 0) {
    if (!seeplayer(plr)) return -1;
    player2pers(p,NULL,plr);
    return plr;
  } else if (ptstflg(mynum,PFL_UAF) != 0 && getuaf(name,p)) {
    *f = True;
    return -2;
  }
  return -1;
}

int alive(int i)
{
  if (pstr(i) < 0 || EMPTY(pname(i)))
    return -1;
  else
    return i;
}

int wlevel(int lev)
{
  if (lev < LVL_GUEST)
    return LEV_NEG;  /* Negative level */
  if (lev < LVL_WIZARD)
    return LEV_MORTAL; /* Mortal */
  if (lev == LVL_WIZARD)
    return LEV_APPRENTICE; /* Apprentice */
  if (lev < LVL_ARCHWIZARD)
    return LEV_WIZARD; /* Wizard */
  if (lev < LVL_DEMI)
    return LEV_ARCHWIZARD; /* Arch wizard */
  if (lev < LVL_GOD)
    return LEV_DEMI; /* Demi god */
  return LEV_GOD;     /* God */
}

Boolean do_okay_l( int p, int v, Boolean c )
{
  int lev_p = wlevel(p);
  int lev_v = wlevel(v);

  if (lev_v > lev_p && lev_p > LEV_NEG)  {
    return False;
  }

  return (c || lev_v < lev_p && lev_p > LEV_WIZARD || lev_p >= LEV_GOD);
}


/* Can p(layer) do XX to v(ictim) ?
** prot_flag protects against it.
*/

Boolean do_okay( int p, int v, int prot_flag )
{
  return do_okay_l(plev(p),plev(v),
                   (prot_flag < PFL_MAX && !ptstflg(v,prot_flag)));
}


void setpsex(int chr, Boolean v)
{
  if (v)
    ssetflg(chr, SFL_FEMALE);
  else
    sclrflg(chr, SFL_FEMALE);
}


/* SET Player LOCation.
 */
void setploc(int plr, int room)
{
	int i;

	/* First remove plr from his current location:
	 */
	if (exists(ploc(plr)))  remove_int(plr, lmobs(ploc(plr)));

	/* Then add him to the new room:
	 */
	if (exists(room)) add_int(plr, lmobs(room));

	ploc(plr) = room;
}

int ptothlp(int pl)
{
  int ct;

  for (ct = 0; ct < numchars; ct++) {
    if (ploc(ct) == ploc(pl) && phelping(ct) == pl)
      return ct;
  }
  return -1;
}

int maxstrength(int p)
{
  return pmaxstrength(plev(p));
}

char *make_title( char *title, char *name )
{
  static char buff[ PNAME_LEN + TITLE_LEN + 20];

  sprintf(buff, (EMPTY(title) ? "%s the Unknown" : title), name);
  return buff;
}

char *std_title(int level, Boolean sex)
{
  extern char *MLevels[];
  extern char *FLevels[];

  static char buff[ TITLE_LEN + 10 ];
  int    wl = wlevel(level);

  strcpy(buff, "%s the ");

  switch (wl) {
  case LEV_NEG:
    strcat(buff, "Mobile");
    break;
  case LEV_MORTAL:
  case LEV_APPRENTICE:
    strcat(buff, (sex ? FLevels : MLevels)[level]);
    break;
  default:
    strcat(buff, WizLevels[wl]);
  }
  return buff;
}


/* Try to reset a mobile.
 */
Boolean reset_mobile(int m)
{
	int loc;

	setpstr(m, pstr_reset(m));
	setpvis(m, pvis_reset(m));
	setpflags(m, pflags_reset(m));
	setmflags(m, mflags_reset(m));
	setsflags(m, sflags_reset(m));
	setpmskh(m, 0);
	setpmskl(m, 0);
	setpsitting(m, 0);
	setpfighting(m, -1);
	setphelping(m, -1);
	setpwpn(m, -1);
	setplev(m, plev_reset(m));
	setpagg(m, pagg_reset(m));
	setpspeed(m, pspeed_reset(m));
	setpdam(m, pdam_reset(m));
	setparmor(m, parmor_reset(m));
	setpwimpy(m, pwimpy_reset(m));

	if (EMPTY(pname(m))) setpname(m, pname_reset(m));

	if ((loc = find_loc_by_id(ploc_reset(m))) == 0) {
		setpstr(m, -1);
		setploc(m, LOC_DEAD_DEAD);
		return False;
	}
	else {
		setploc(m, loc);
		return True;
	}
}



void p_crapup(int player, char *str, int flags)
{
  int m = real_mynum;

  setup_globals(player);
  crapup(str, flags|CRAP_RETURN);
  setup_globals(m);
}

void crapup(char *str, int flags)
{
  if ((flags & CRAP_UNALIAS) != 0) {
    unalias(real_mynum);
    unpolymorph(real_mynum);
  }
  xcrapup(str,(flags & CRAP_SAVE) != 0);
  if ((flags & CRAP_RETURN) == 0) {
    longjmp(to_main_loop,JMP_QUITTING);
  }
}

void xcrapup(char *str, Boolean save_flag)
{
  char x[80];
  static char *dashes =
  "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=";

  if (cur_player->aliased) {
    bprintf("%s\n", str);
    unalias(real_mynum);
    error();
    return;
  } else if (cur_player->polymorphed >= 0) {
    bprintf("%s\n", str);
    unpolymorph(real_mynum);
    error();
    return;
  }
  quit_player(); /* So we don't get a prompt after exit */

  loseme(save_flag);
  if (str != NULL) {
    bprintf("\n%s\n\n%s\n\n%s\n", dashes, str, dashes);
    bflush();
  }
  setpname(mynum,"");
  setploc(mynum, 0);

  remove_entry(mob_id(mynum), &id_table);

  cur_player->iamon = False;
}

/* Remove me from the game. Dump objects, send messages, save etc..
 * May only be used after has been called.
 */
void loseme( Boolean save_flag )
{
  char   bk[128], b[80];
  int    x, y;
  Boolean emp = EMPTY(pname(mynum));

  if (cur_player->aliased || cur_player->polymorphed >= 0)
    return;

  if (cur_player->iamon) {
    if (!emp) {

      if (save_flag)
	saveme();

      if ((x = the_world->w_lock) > 0
	  && (y = plev(mynum)) >= x && y >= LVL_WIZARD)
	bprintf("\nDon't forget the game is locked....\n");

      send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, max(pvis(mynum), LVL_WIZARD),
	       LVL_MAX, mynum, NOBODY,
	       "[%s has departed from MUD in %s (%s)]\n",
	       pname(mynum), sdesc(ploc(mynum)),xshowname(b,ploc(mynum)));
	       

    }

    dumpitems();


    if (!emp) {
      mudlog("EXIT:  %s [lev %d, scr %d]",
	     pname(mynum), plev(mynum), pscore(mynum));

      setpname(mynum, "");
      setploc(mynum, 0);
    }

    cur_player->iamon = False;

    snoop_off(mynum);
  }
}

char *lev2s(char *b,int lvl)
{
  switch (lvl) {
  case 0: strcpy(b,"Un"); return b;
  case LVL_WIZARD: strcpy(b,"Apprentice-Wizard "); return b;
  case LVL_WIZARD + 1:strcpy(b,"Wizard "); return b;
  case LVL_ARCHWIZARD:strcpy(b,"Arch-Wizard "); return b;
  case LVL_DEMI:strcpy(b,"Demi-God "); return b;
  case LVL_GOD:strcpy(b,"God "); return b;
  case LVL_LEGEND:strcpy(b,"Legend "); return b;
  case LVL_WARLOCK:strcpy(b,"Warlock "); return b;
  case LVL_SORCERER:strcpy(b,"Sorcerer "); return b;
  case LVL_ENCHANTER:strcpy(b,"Enchanter "); return b;
  case LVL_MAGICIAN:strcpy(b,"Magician "); return b;
  case LVL_CONJURER:strcpy(b,"Conjurer "); return b;
  case LVL_CHAMPION:strcpy(b,"Champion "); return b;
  case LVL_WARRIOR:strcpy(b,"Warrior "); return b;
  case LVL_HERO:strcpy(b,"Hero "); return b;
  case LVL_ADVENTURE:strcpy(b,"Adventurer "); return b;
  case LVL_NOVICE:strcpy(b,"Novice "); return b;
  }
  switch(wlevel(lvl)) {
  case LEV_WIZARD: sprintf(b,"Wizard [level %d] ", lvl); return b;
  case LEV_ARCHWIZARD: sprintf(b, "Arch-Wizard [level %d] ", lvl); return b;
  case LEV_DEMI: sprintf(b, "Demi-God [level %d] ", lvl); return b;
  }
  sprintf(b, "Level %d ", lvl);
  return b;
}

int tscale(void)
{
  int a = 0;
  int b;

  for (b = 0; b < max_players; b++)
    if (is_in_game(b) && plev(b) < LVL_WIZARD)
      a++;
  if (a < 2)
    return 1;
  else
    return (a > 9 ? 9 : a);
}

Boolean chkdumb(void)
{
  if (!ststflg(mynum, SFL_DUMB))
    return False;
  bprintf("You are mute.\n");
  return True;
}

Boolean chkcrip(void)
{
  if (!ststflg(mynum, SFL_CRIPPLED))
    return False;
  bprintf("You are crippled.\n");
  return True;
}

Boolean chksitting(void)
{
  if (!psitting(mynum))
    return False;
  bprintf("You'll have to stand up, first.\n");
  return True;
}


void calib_player(int pl)
{
  extern char *MLevels[];
  extern char *FLevels[];

  int b;
  char sp[128];

  if (pl >= max_players || !players[pl].iamon || players[pl].aliased ||
      players[pl].polymorphed >= 0)
    return;
  if ((b = levelof(pscore(pl),plev(pl))) != plev(pl) &&
      plev(pl) > 0) {

    if (b == LVL_WIZARD && (!tstbits(qflags(pl), Q_ALL))) {
      return;
    }

    setplev(pl, b);
    setptitle(pl, std_title(b, psex(pl)) );

    sendf(pl, "You are now %s\n", make_title(ptitle(pl), pname(pl)));

    mudlog("%s to level %d", pname(pl), b);

    send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, max(pvis(pl), LVL_WIZARD),
	     LVL_MAX, NOBODY, NOBODY,
	     "[%s is now level %d]\n", pname(pl), plev(pl));

    switch (b) {
    case LVL_MAGICIAN:
      sendf(pl, "\nWelcome, %s.  You may now use the POSE command.\n",
               (psex(pl) ? FLevels : MLevels)[LVL_MAGICIAN]);
      break;
    case LVL_SORCERER:
      sendf(pl, "\nNice work, %s.  You may now use the BANG command.\n",
               (psex(pl) ? FLevels : MLevels)[LVL_SORCERER]);
      break;
    case LVL_LEGEND:
      set_xpflags(LVL_LEGEND,&pflags(pl),&pmask(pl));
      sendf(pl, "\nCongratulations %s!  You may now use EMOTE anywhere.\n",
               (psex(pl) ? FLevels : MLevels)[LVL_LEGEND]);
      break;
    case LVL_WIZARD:
      set_xpflags(LVL_WIZARD,&pflags(pl),&pmask(pl));

      sendf(DEST_ALL, "Trumpets sound to praise %s, the new Wizard.\n",
	    pname(pl));

      sendf(pl, "\001f%s\003", GWIZ);

      update_wizlist( pname(pl), LEV_APPRENTICE);

      break;
    }
  }

  if (pstr(pl) > (b = maxstrength(pl)))
    setpstr(pl, b);
}


void calibme()
{
  calib_player(mynum);
}



void destroy_mobile(int mob)
{
	setploc(mob, LOC_DEAD_DEAD);
}



int levelof(int score,int lev)
{
  int i;

  if (lev > LVL_WIZARD || lev < LVL_NOVICE)
    return lev;
  for (i = LVL_WIZARD; i > LVL_GUEST; i--)
    if (score >= levels[i])
      return i;
  return 0;
}

Boolean check_setin(char *s, Boolean d)
{
  char *p, *q, *r;
  int nn = 0, dd = 0;

  for (p = s; (p = strchr(p,'%')) != NULL;) {
    switch (*++p) {
    case 'n':
    case 'N': ++nn; break;
    case 'd': ++dd; break;
    }
  }
  return (nn > 0 && (dd == 0 || d) && strlen(s) < SETIN_MAX);
}

Boolean check_busy(int plx)
{
  if (ststflg(plx,SFL_BUSY)) {
    bprintf("%s is busy, try later!\n", pname(plx));
    return True;
  }
  return False;
}

char *build_prompt(int plx)
{
  static char b[PROMPT_LEN + 30];

  *b = 0;
  if (pvis(plx)) strcat( b, "(");
  if (players[plx].aliased) strcat(b,"@");
  strcat(b, players[plx].prompt);
  if (pvis(plx)) strcat(b,")");
  return b;
}

int vicf2(int fl, int i)
{
  int plr;

  if (ltstflg(ploc(mynum), LFL_NO_MAGIC) != 0 && plev(mynum) < LVL_WIZARD) {
    bprintf("Something about this location has drained your mana.\n");
    return -1;
  }
  if (fl >= SPELL_VIOLENT && plev(mynum) < LVL_WIZARD && testpeace(mynum)) {
    bprintf("No, that's violent!\n");
    return -1;
  }
  if ((plr = vicbase()) < 0)
    return -1;

  if (pstr(mynum) < 10) {
    bprintf("You are too weak to cast magic.\n");
    return -1;
  }
  if (plev(mynum) < LVL_WIZARD)
    setpstr(mynum, pstr(mynum) - 1);

  if (carries_obj_type(mynum, OBJ_BLIZZARD_POWERSTONE) > -1) i++;
  if (carries_obj_type(mynum, OBJ_BLIZZARD_POWERSTONE1) > -1) i++;
  if (carries_obj_type(mynum, OBJ_BLIZZARD_POWERSTONE2) > -1) i++;

  if (plev(mynum) < LVL_WIZARD && randperc() > i * plev(mynum)) {
    bprintf("You fumble the magic.\n");
    if (fl == SPELL_REFLECTS) {
      bprintf("The spell reflects back.\n");
      return mynum;
    }
    return -1;
  }
  if (plev(mynum) < LVL_WIZARD) {
    if ((fl >= SPELL_VIOLENT) && wears_obj_type(plr, OBJ_CATACOMB_SHIELD) >-1){
      bprintf("The spell is absorbed by %s shield!\n", his_or_her(plr));
      return -1;
    }
    else
      bprintf("The spell succeeds!\n");
  }
  return plr;
}

int vichfb(int cth)
{
  int a;

  if ((a = vicf2(SPELL_VIOLENT, cth)) < 0) return a;
  if (ploc(a) != ploc(mynum)) {
    bprintf( "%s isnt here.\n", psex(a) ? "She" : "He" );
    return -1;
  }
  return a;
}

int vichere(void)
{
  int a;
  if ((a = vicbase()) == -1) return -1;
  if (ploc(a) != ploc(mynum)) {
    bprintf( "They aren't here.\n" );
    return -1;
  }
  return a;
}

int vicbase(void)
{
  int a;

  do {
    if (brkword() == -1) {
      bprintf("Who?\n");
      return -1;
    }
  } while (EQ(wordbuf, "at"));
  if ((a = fpbn(wordbuf)) < 0) {
    bprintf( "That person isn't playing now.\n" );
    return -1;
  }
  return a;
}







/* The JUMP command
 */
void jumpcom()
{
  int a, b, i, j, x;
  char ms[128];
  extern int pits[];

/* Where is it possible to jump from, and where do
 * we land...
 */
static int jumtb[] = {
  /* FROM,                    TO */
  LOC_CASTLE_LEAP,            LOC_VALLEY_NPATH,
  LOC_TOWER_LEDGE,            LOC_CASTLE_WALLS,
  LOC_TREEHOUSE_PORCH,        LOC_VALLEY_ESIDE,
  LOC_MOOR_PIT,               LOC_LEDGE_PIT,
  LOC_BLIZZARD_WINDOW,        LOC_BLIZZARD_CHAMBER,
  LOC_BLIZZARD_CHAMBER,       LOC_BLIZZARD_WINDOW,
  LOC_CATACOMB_CHASM1,        LOC_CATACOMB_CAVERN,
  LOC_CATACOMB_CHASM2,        LOC_CATACOMB_CAVERN,
  0,                  0 };

  if (psitting(mynum)) {
    bprintf("You have to stand up first.\n");
    error();
    return;
  }

#ifdef LOCMIN_FROBOZZ
  if(ploc(mynum) == LOC_FROBOZZ_ENDING){
    sprintf(ms,"%s jumps into the hole and disappears!\n",pname(mynum));
    sillycom(ms);
    setploc(mynum,LOC_BLIZZARD_GLITTERING);
    bprintf("You jump into the hole...\n\n\n\nAnd fall....."
	    "Until the tunnel stops abruptly\n"
	    "and ends as a hole in a ceiling, "
	    "through which you fall and land\n");
    sprintf(ms,"%s comes falling through a hole in the ceiling and lands\n",
            pname(mynum));
    sillycom(ms);
    if (ploc(mynum) == ploc(max_players + MOB_BLIZZARD_GIANT)) {
      bprintf("on the stomach of a snoring giant!\n");
      sillycom("on the stomach of the the snoring giant!\n");
      hit_player(max_players + MOB_BLIZZARD_GIANT, mynum, -1);
    }
    else {
      bprintf("on the ground with a thud, hurting yourself badly!\n");
      sillycom("on the ground with a loud thud, being badly hurt!\n");
      if (plev(mynum) < LVL_WIZARD) setpstr(mynum,pstr(mynum)/2);
    }
    return;
  }
#endif


/* Search the jump-table for special locations..
 */
    for (a = 0, b = 0; jumtb[a]; a += 2) {
        if (jumtb[a] == ploc(mynum)) {
            b = jumtb[a + 1];
	    break;
	}
    }

/* Are we by a pit ? If so we'll jump into it.
 */
    for (i = 0; (j = pits[i++]) != -1 && oloc(j) != ploc(mynum); )
        ;

    if (j >= OBJ_CATACOMB_PIT_NORTH && j <= OBJ_CATACOMB_PIT_WEST
	&& state(i) == 0) {
        b = LOC_CATACOMB_RIPS;
    }
    else if (j >= OBJ_START_PIT && j <= OBJ_START_CHURCH_PIT
           || oloc(OBJ_ORCHOLD_HOLEORCS) == ploc(mynum)) {
        b = LOC_PIT_PIT;
    }

    if (b == 0) {
        bprintf("Wheeeeee....\n");
	return;
    }

    if ((x = carries_obj_type(mynum, OBJ_START_UMBRELLA)) > -1
	&& state(x) != 0) {
        sprintf(ms, "%s jumps off the ledge.\n", pname(mynum));
        bprintf("You grab hold of the %s and fly down like "
		"Mary Poppins.\n", oname(x));
    }
    else if (plev(mynum) < LVL_WIZARD) {
        sprintf(ms, "%s makes a perfect swan dive off the ledge.\n",
		pname(mynum));
        if (b != LOC_PIT_PIT) {
            send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY, ms);
            setploc(mynum, b);
            bprintf("Wheeeeeeeeeeeeeeeee       <<<<SPLAT>>>>\n");
            bprintf("You seem to be splattered all over the place.\n");
            crapup("\t\tI suppose you could be scraped up with a spatula.",
		   SAVE_ME);

        }
    }
    else
        sprintf(ms, "%s dives off the ledge and floats down.\n", pname(mynum));


    send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY, ms);

    setploc(mynum, b);

    if (iscarrby(OBJ_START_UMBRELLA, mynum))
        sprintf(ms, "%s flys down, clutching an umbrella.\n", pname(mynum));
    else
        sprintf(ms, "%s has just dropped in.\n", pname(mynum));

    send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY, ms);

    trapch(b);
}




/* Stuff that should be done both after every command and at every
 * i/o-interrupt at the latest.
 */
void special_events(int player)
{
  int start = (player == SP_ALL) ? 0 : player;
  int stop = (player == SP_ALL) ? max_players - 1 : player;   /*max_chars ?*/
  int i;

  if (player >= max_players) return;

  for(i = start; i <= stop; ++i)
    if (is_in_game(i)) {

      if (ploc(i) == LOC_CASTLE_MAIDEN && state(OBJ_CASTLE_IN_MAIDEN) == 1) {
	broad("\001dThere is a long drawn out scream in the distance\n\003");
	p_crapup(i, "\t\tThe iron maiden closes....... S Q U I S H !",
		 CRAP_SAVE);
	continue;
      }
      if (plev(i) < LVL_WIZARD && ltstflg(ploc(i), LFL_ON_WATER)) {
	      if (!carries_boat(i)) {
		p_crapup(i, "\t\tYou plunge beneath the waves....", CRAP_SAVE);
	      continue;
	}
      }

      /* 10% chance of the tree swallowing you
       */
      if (ploc(i) == LOC_FOREST_INTREE
	  && plev(i) < LVL_WIZARD && randperc() < 10) {

	p_crapup(i, "You have been absorbed, and crushed to death by "
		    "the tree....", CRAP_SAVE);
	broad("There is a hideous scream and a grinding of bone...\n");
	continue;
      }

      /* 40% chance that tree will suck you in
       */
      if (ploc(i) == oloc(OBJ_FOREST_TREEEATING) && randperc() < 40) {
	int me = real_mynum;
	sendf(i, "You are suddenly grabbed and taken into the tree!\n");
	setup_globals(i);
	teletrap(LOC_FOREST_INTREE);
	setup_globals(me);
      }


      /* If the snoop-victim has disappeard or the snooper is (only) a wizard 
       * and the target has gone into a private room, then stop the snoop.
       */
      if (players[i].snooptarget != -1) {

	if (!is_in_game(players[i].snooptarget)) {
	  sendf(i, "You can no longer snoop.\n");
	  snoop_off(i);
	}
	else if(ltstflg(ploc(players[i].snooptarget), LFL_PRIVATE)
		&& plev(i) < LVL_ARCHWIZARD) {

	  sendf(i, "%s went into a PRIVATE room, you can no longer snoop.\n",
		pname(players[i].snooptarget));

	  snoop_off(i);
	}
      }

      /* count down if polymorphed
       */
      if (players[i].polymorphed == 0) unpolymorph(i);
      if (players[i].polymorphed > -1) players[i].polymorphed--;

      /* if invis, count down
       */
      if (players[i].me_ivct > 0 && --players[i].me_ivct == 0) { 
        setpvis(i, 0);
      }

      if (pstr(i) < 0 && (players[i].aliased
			  || players[i].polymorphed != -1)) {
	sendf(i, "You've just died.\n");
	if (players[i].polymorphed != -1) unpolymorph(i);
	else unalias(i);
      }

    } /* end for each player */
}



void regenerate(void)
{
  /* chance of heal is now 15 and 30 instead of 20 and 40 since
   * the interrupt now occurs every 2 secs. instead of 3
   */
  int i;

  for(i = 0; i < max_players; ++i)
    if (is_in_game(i) && pfighting(i) < 0 && pstr(i) < maxstrength(i)) {

      Boolean has_ring = wears_obj_type(i, OBJ_TOWER_RING) > -1;

      if (!has_ring && (randperc() < 15 || psitting(i) && randperc() < 30)
	  || has_ring && randperc() < 67) {

	  if (maxstrength(i) == pstr(i) + 1) {
	    sendf(i, "You feel fully healed.\n");
	  }

	  setpstr(i, pstr(i) + 1);

	  if (players[i].iamon)
	    calib_player(i);
      }
    }
}

