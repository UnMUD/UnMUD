#include "kernel.h"
#include "locations.h"
#include "objects.h"
#include "mobiles.h"
#include "objsys.h"
#include "stdinc.h"
#include "mobiles.h"
#include "party.h"

extern char *WizLevels[];
static void _userlist(char mode);
char *make_here(int num);

/* Compare players (for the qsort() function) */
static int cmp_player(const void *a, const void *b)
{
  return (plev(*(int *)a) - plev(*(int *)b));
}
#if 0
static int 
cmp_player (int *a, int *b)
{
  return plev (*b) - plev (*a);
}
#endif
/* The USERS command. Show names and locations of users. Possibly also
 * their local host if PFL_SHUSERS is set.
 */

void usercom()
{

  _userlist(0);
}

/* The WHO command
 */

void whocom(void)
{

  _userlist(1);
}

/* The MLEFT command
 */
#define COLUMNS (72 / MNAME_LEN)
void mleftcom(void)
{
  char buff[64];
  int i;
  int ct;
  int zone;
  int nr;

  Boolean list_all = True;
  int first = max_players;
  int last = numchars;

  zone = 0;
  if (plev(mynum) < LVL_APPREN)
  {
    erreval();
    return;
  }

  if (brkword() != -1)
  {
    list_all = False;

    if ((zone = get_zone_by_name(wordbuf)) == -1)
    {

      bprintf("No such zone: %s\n", wordbuf);
      return;
    }

    first = 0;
    last = znumchars(zone);
  }

  bprintf("Live Mobiles\n------------\n\n");

  for (ct = first, nr = 0; ct < last; ++ct)
  {

    i = list_all ? ct : zmob_nr(ct, zone);

    if (!EMPTY(pname(i)) && pstr(i) >= 0 &&
        (pvis(i) <= 0 || pvis(i) <= plev(mynum)))
    {

      ++nr;

      if (pvis(i) > 0)
        sprintf(buff, "(%s)", pname(i));
      else
        strcpy(buff, pname(i));

      bprintf("%-*s%c",
              PNAME_LEN + 3, buff, (nr % COLUMNS) ? ' ' : '\n');
    }
  }

  bprintf("\n\nThere are %d mobiles left (from %d)\n",
          nr, last - first);
}

int player_damage(int player)
{
  int w;
  int damage = pdam(player);

  if ((w = pwpn(player)) != -1 && iscarrby(w, player) && iswielded(w))
  {

    damage += odamage(w);
  }
  return damage;
}

int player_armor(int player)
{
  int i, j;
  int armor = parmor(player);

  for (j = 0; j < pnumobs(player) && (i = pobj_nr(j, player), 1); j++)
  {

    if (iswornby(i, player) && otstbit(i, OFL_ARMOR))
    {
      armor += oarmor(i);
    }
  }
  return armor;
}

/* The STATS command WITHOUT arguments.
 */
void ustats(int minlvl, int maxlvl)
{
  int i, j;
  int a[256], a_len = 0;
  char buff[64];
  int usercount = 0;

  if (plev(mynum) < LVL_APPREN)
  {
    erreval();
    return;
  }

  for (i = 0; i < max_players; i++)
  {
    if ((pvis(i) > plev(mynum) && i != mynum) ||
#ifdef TCP_ANNOUNCE
        !players[i].inp_handler ||
        (plev(mynum) < LVL_SENIOR && !players[i].iamon) ||
#else
        !players[i].iamon ||
#endif
        strncasecmp(pname(i), item1, strlen(item1)) ||
        (plev(i) < minlvl) || (plev(i) > maxlvl))
      continue;
    a[a_len++] = i;
  }

  if (a_len == 0)
  {
    bprintf("No one by that name on\n");
    return;
  }

  qsort(a, a_len, sizeof(int), cmp_player);

  bprintf("&+GLevel     &+gName     &+BStatus &+YStr/Max &+rDam &+RArm &+WVis"
          "     &+YIdle    &+bOn For  &+GLocation\n-----------------------"
          "-------------------------------------------------------\n");

  for (j = 0; i = a[j], j < a_len; ++j)
  {

    if (!players[i].iamon)
    {
      if (!EMPTY(pname(i)))
        bprintf("[ *tcp* ]");
    }
    else if (plev(i) < LVL_APPREN)
      bprintf("&+G[%7d]", plev(i));
    else
    {
      strncpy(buff, WizLevels[wlevel(plev(i))], 7);
      bprintf("&+G[%7.7s]", buff);
    }

    bprintf(" &+g%-*s", PNAME_LEN, EMPTY(pname(i)) ? "Undecided" : pname(i));

    if (plev(i) < LVL_APPREN)
    {
      if (pfighting(i) >= 0)
      {
        bprintf("&+Bfgt");
      }
      else if (psitting(i))
      {
        bprintf("&+Bsit");
      }
      else
        bprintf(" &+Bup");
    }
    else
      bprintf("&+B---");

    if (plev(i) < LVL_APPREN && players[i].iamon)
      bprintf(" &+Y%3d/%-3d &+r%3d &+R%3d",
              pstr(i), pmaxstrength(plev(i)),
              player_damage(i),
              player_armor(i));
    else
      bprintf(" &+Y---/--- &+r--- ---");

    bprintf(pvis(i) < LVL_APPREN ? " &+W%3d" : " inv", pvis(i));
    if (cur_player->last_cmd >= 0)
      strcpy(buff, sec_to_hhmmss(time(0) - plast_cmd(i)));
    else
      strcpy(buff, sec_to_hhmmss(time(0) + plast_cmd(i)));
    bprintf("&+y%9.9s&+b%10.10s  &+G%s\n",
            buff,
            sec_to_hhmmss(time(0) - plogged_on(i)),
            !ploc(i) ? "Unconnected" : showname(ploc(i)));
    usercount++;
  }
  bprintf("\n&+RA total of %ld visible users.&*\n", (long)usercount);
}

void lastoncom(void)
{
  PERSONA d;
  Boolean is_mobile;
  int b;
  char *name;
  int laston;

  if (brkword() == -1)
  {
    bprintf("Check the last time who was on?\n");
    return;
  }
  if ((b = fpbns(wordbuf)) != -1 && seeplayer(b))
  {
    is_mobile = b >= max_players;
    player2pers(&d, NULL, b);
    laston = 0;
    name = pname(b);
  }
  else if (getuaf(wordbuf, &d))
  {
    is_mobile = False;
    laston = 1;
    name = d.p_name;
  }
  else
  {
    bprintf("Who's that?\n");
    return;
  }
  if (is_mobile)
  {
    bprintf("You can't check when a mobile was last on!\n");
  }
  else
  {
    bprintf("&+BName\t:&+W %s&*", name);
    bprintf("\n&+BLast on\t:&+W %s&*", (laston) ? time2ascii(d.p_last_on) : "&+WCurrently Logged on.&*");
    bprintf("\n");
  }
}

/* The STATS command */
void showplayer(void)
{
  extern char *Mflags[];
  char buff[80];
  PERSONA d;
  int b;
  int max_str;
  int i, j, w, armor;
  char *title, *name;
  Boolean in_file, is_mobile;

  if (!ptstflg(mynum, PFL_STATS))
  {
    erreval();
    return;
  }

  if (brkword() == -1)
  {
    ustats(LVL_MIN, LVL_MAX);
    return;
  }

  if ((b = fpbns(wordbuf)) != -1 && seeplayer(b))
  {
    in_file = False;
    is_mobile = b >= max_players;
    player2pers(&d, NULL, b);
    name = pname(b);
    title = is_mobile ? NULL : ptitle(b);
    d.p_home = find_loc_by_id(is_mobile ? ploc_reset(b) : phome(b));
    max_str = is_mobile ? pstr_reset(b) : maxstrength(b);
  }
  else if (getuaf(wordbuf, &d))
  {
    in_file = True;
    is_mobile = False;
    title = d.p_title;
    name = d.p_name;
    max_str = pmaxstrength(d.p_level);
  }
  else
  {
    bprintf("Who's that?\n");
    return;
  }

  if (d.p_level >= LVL_MAX)
  {
    bprintf("Who's that?\n");
    return;
  }

  bprintf("&+BName       &+W:&* %s", name);
  if (!is_mobile)
  {
    bprintf("\n&+BTitle      &+W:&* %s", make_title(title, "<name>"));
    bprintf("\n&+BEmail      &+W:&* %s", d.p_email);
    bprintf("\n&+BScore      &+W:&* %d", d.p_score);
    bprintf("\n&+BLevel      &+W:&* %d", d.p_level);
  }

  bprintf("\n&+BStrength   &+W:&* %d / %d", d.p_strength, max_str);

  bprintf("\n&+BDamage     &+W:&* %d", d.p_damage);

  if (!in_file && (w = pwpn(b)) != -1 && iscarrby(w, b) && iswielded(w))
  {

    int w_damage = is_mobile ? odamage(w) / 2 : odamage(w);

    bprintf(" + %s(%d) = %d",
            oname(w), w_damage, w_damage + d.p_damage);
  }

  bprintf("\n&+BArmor      &+W:&* %d", armor = d.p_armor);

  if (!in_file)
  {

    for (j = 0; j < pnumobs(b) && (i = pobj_nr(j, b), 1); j++)
    {

      if (iswornby(i, b) && otstbit(i, OFL_ARMOR))
      {
        armor += oarmor(i);
        bprintf(" + %s(%d)", oname(i), oarmor(i));
      }
    }
    if (armor != d.p_armor)
      bprintf(" = %d", armor);
  }

  bprintf("\n&+BVisibility &+W:&* %d ", d.p_vlevel);

  if (is_mobile)
  {
    bprintf("\n&+BAggression &+W:&* %d %%", pagg(b));
    bprintf("\n&+BHates      &+W:&* %s", ((i = hates_who(b)) == -1) ? "No one" : pname(i));
    bprintf("\n&+BSpeed      &+W:&* %d", pspeed(b));
    bprintf("\n&+B%s      &+W:&* %s",
            zpermanent(pzone(b)) ? "Zone " : "Owner", zname(pzone(b)));
  }

  bprintf("\n&+BWimpy      &+W:&* %d", d.p_wimpy);

  if (exists(w = find_loc_by_id(d.p_home)))
    bprintf("\n&+BStart      &+W:&* %s \t(%s)", sdesc(w), xshowname(buff, w));

  if (!in_file)
  {
    bprintf("\n&+BLocation   &+W:&* %s \t(%s)",
            sdesc(ploc(b)), xshowname(buff, ploc(b)));

    bprintf("\n&+BCondition  &+W:&*");

    if (pfighting(b) >= 0)
      bprintf(" Fighting %s", pname(pfighting(b)));

    if (phelping(b) >= 0)
      bprintf(" Helping %s", pname(phelping(b)));

    if (psitting(b))
      bprintf(" Sitting");

    if (is_mobile)
    {
      bprintf("\n&+BBehavior   &+W:&* ");
      show_bits((int *)&mflags(b), sizeof(MFLAGS) / sizeof(int), Mflags);
    }
    else if (ptstflg(mynum, PFL_SHUSER))
    {
      bprintf("\n&+BFrom Host  &+W:&* %s", players[b].hostname);
    }
  }
  else
  {
    bprintf("\n&+BLast on    &+W:&* %s", time2ascii(d.p_last_on));
  }

  if (d.p_sflags != 0)
  {
    if (!is_mobile)
      bprintf("\n");
    bprintf("&+BVarious    &+W:&* ");
    show_bits((int *)&(d.p_sflags), sizeof(SFLAGS) / sizeof(int), Sflags);
  }

  bprintf("\n");
}

/* The MOBILE command
 */
void mobilecom(void)
{
  int i;
  int ct;
  int zone;
  int live = 0;
  int dead = 0;

  Boolean list_all = True;
  int first = max_players;
  int last = numchars;

  zone = 0;
  if (plev(mynum) < LVL_APPREN)
  {
    erreval();
    return;
  }

  if (brkword() != -1)
  {
    list_all = False;

    if ((zone = get_zone_by_name(wordbuf)) == -1)
    {

      bprintf("No such zone: %s\n", wordbuf);
      return;
    }

    first = 0;
    last = znumchars(zone);
  }

  bprintf("Mobiles");

  if (the_world->w_mob_stop)
    bprintf("   [Currently STOPped]");

  bprintf("\n\n");

  for (ct = first; ct < last; ++ct)
  {

    i = list_all ? ct : zmob_nr(ct, zone);

    bprintf("[%d] %-*s %c", i + GLOBAL_MAX_OBJS, MNAME_LEN,
            EMPTY(pname(i)) ? pname_reset(i) : pname(i),
            ststflg(i, SFL_OCCUPIED) ? '*' : ' ');

    if (EMPTY(pname(i)))
    {
      bprintf("<QUIT or KICKED OFF>\n");
      ++dead;
    }
    else
    {
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

  for (j = 0; j < lnumchars(room) && (i = lmob_nr(j, room), 1); j++)
  {

    if (i != mynum && is_in_game(i) && seeplayer(i))
    {

      /* mobile ? */
      if (i >= max_players)
      {
        bprintf("%s%s%s\n",
                pvis(i) > 0 ? "(" : "",
                pftxt(i),
                pvis(i) > 0 ? ")" : "");
      }

      /* player ? */
      else
      {

        bprintf("%s%s%s\n",
                pvis(i) > 0 ? "(" : "",
                make_here(i),
                pvis(i) > 0 ? ")" : "");
      }

      if (gotanything(i) && !ststflg(mynum, SFL_NOINVEN))
      {
        print_inventory(i);
      }
    }
  }
}

void move_mobiles(void)
{
  int mon;

  for (mon = max_players; mon < numchars; mon++)
  {
    if (!ststflg(mon, SFL_OCCUPIED) && pstr(mon) > 0)
      consid_move(mon); /* Maybe move it */
  }

  onlook();
}

/*  Fight control */
void onlook(void)
{
  int i, j, vict;

  for (i = numchars - 1; i >= 0; i--)
  {

    if (is_in_game(i) && (j = pfighting(i)) >= 0)
    {

      if (testpeace(i) || !is_in_game(j) ||
          ploc(i) != ploc(j))
      {

        pfighting(i) = pfighting(j) = -1;
      }
      else
      {
        hit_player(i, j, pwpn(i));

        /* "Wimpy" code, make victim flee if his
         * * strength is less then his "wimpy" val.
   Mobile wimpy by Marty 1996  */
        if (pstr(j) >= 0 && pstr(j) < pwimpy(j))
        {
          if (j < max_players)
          {
            int x = mynum;
            setup_globals(j);
            stp = 0;
            strbuf[0] = '\0';
            fleecom();
            setup_globals(x);
          }
          else
          {
            fleemob(j);
          }
        }
      }
    }
  }

  for (i = max_players; i < numchars; i++)
  {
    chkfight(i);
  }

  if (ocarrf(OBJ_CASTLE_RUNESWORD) >= CARRIED_BY)
  {
    dorune(oloc(OBJ_CASTLE_RUNESWORD));
  }

  for (i = 0; i < max_players; i++)
  {
    if (is_in_game(i) && phelping(i) != -1)
    {
      helpchkr(i);
    }
  }
}

void chkfight(int mon)
{
  int plx;

  if (mon < max_players ||
      mon >= numchars ||
      (pagg(mon) == 0 && no_hate(mon)) ||
      ststflg(mon, SFL_OCCUPIED) ||
      EMPTY(pname(mon)) ||
      pstr(mon) < 0 ||
      testpeace(mon) ||
      pfighting(mon) >= 0)
  {
    return;
  }

  /* See if we can hit someone.... */

  for (plx = lfirst_mob(ploc(mon));
       plx != SET_END;
       plx = lnext_mob(ploc(mon)))
  {

    if (plx < max_players &&
        is_in_game(plx) &&
        !ptstflg(plx, PFL_NOHASSLE) &&
        pvis(plx) == 0 &&
        !ststflg(plx, SFL_GHOST))
    {

      if (mtstflg(mon, MFL_CROSS) &&
          carries_obj_type(plx, OBJ_CHURCH_CROSS) > -1)
      {
        continue;
      }

      if (hates_you(mon, plx))
      {
        send_hate_msg(mon, plx);
      }
      else if (randperc() >= pagg(mon) ||
               (!hates_you(mon, plx) &&
                (pagg(mon) <= 0)))
      {
        continue;
      }
      else if (randperc() >= pagg(mon))
      {
        continue;
      }

      if (mtstflg(mon, MFL_NOHEAT) && p_ohany(plx, (1 << OFL_LIT)))
      {
        sendf(plx,
              "%s seems disturbed by naked flame, and keeps its distance.\n",
              pname(mon));
        continue;
      }

      hit_player(mon, plx, pwpn(mon)); /* Start the fight */
    }
  }
}

void consid_move(int mon)
{
  int s, t;
  static char *weak[] = {
      "&+COuch!  That hurts!&*",
      "&+CI'm telling my mommy!&*",
      "&+COw!&*",
      "&+CLeave me alone!&*"};

  static char *medium[] = {
      "&+CHmm... I'm not so sure about this...",
      "&+CWow, you're good.",
      "&+CWhew, I need a break!",
      "&+CThis sucks."};

  static char *strong[] = {
      "&+CPuny mortal!&*",
      "&+CYou cannot defeat me!&*",
      "&+CYou shall die!&*",
      "&+CFlee, while you still can, weakling!&*",
      "&+CGee, you're funny!&*"};

  static char *m[] =
      {
          "&+CI love this mud. So many weird people! I get hit on all the time.&*",
          "&+CLook at the pretty colors!&*",
          "&+CTo iterate is human.  To recurse, divine.&*",
          "&+CHow'd those fish get up there?&*",
          "&+CAnd NO you CANNOT use my scales to weigh that...&*",
          "&+CBarney must die! But then again... We could enslave him...&*",
          "&+CFree donuts at Warm Haven! What! No glazed!?&*",
          "&+CWhat? Did you say TORCH!?&*",
          "&+CI'm walking here!&*",
          "&+CI'm not only the president of scales club for women, I'm also a client.&*",
          "&+CLife is like a box of chocolates, you never know whtat youre gonna get.&*",
          "&+CYou think I'm pointless? Go ahead. Try to light that torch with your breath.&*",
          "&+CElvis has left the building!&*",
          "&+CHey, can I have some of that? Im HUNGRY!&*",
          "&+CHey! Keep those comments to your self!&*",
          "&+CNice weather we are having, isnt it.&*",
          "&+CHi!&*",
          "&+CYou are ugly. And you smell.&*"};
  static char *puffshouts[] =
      {
          "Where'd all my pals go?",
          "Hey, did you hear the one about Guxx and the treehouse?",
          "Who stole my towel?",
          "Why is everyone so quiet?",
          "I'm hungry!",
          "Anyone need a... LIGHT?",
          "Elvis lives!",
          "I love you!",
          "Anyone have a breath mint?",
          "Hey, who forgot to feed the dragon?",
          "Who turned out the lights?",
          "Swing your partner do-see-do, and be sure not to crush his toe!"};
  static char *puffactions[] =
      {
          "%s blinks, then wanders off aimlessly.\n",
          "%s coughs noisily.\n",
          "%s laughs at some private joke.\n",
          "%s dances around in circles and trips over her own tail.\n",
          "%s knocks you over and gives you a big hug.\n",
          "%s's stomach growls alarmingly.\n",
          "%s giggles insanely.\n",
          "%s looks at you and grins evilly.\n",
          "%s hiccups and blushes as a gout of flame spurts out her nostrils."};
  int puffrand;
  char blob[MAX_COM_LEN];
  int plx;

  if (EMPTY(pname(mon)))
    return;

  for (plx = 0; plx < lnumchars(ploc(mon)); plx++)
  {
    if (pfighting(lmob_nr(plx, ploc(mon))) == mon)
    {
      if (randperc() > 8)
        return;
      s = pstr_reset(mon) - pstr(mon);
      t = (int)(pstr_reset(mon) / 3);
      if (s < (2 * t))
      {
        sendf(ploc(mon), "&+W\001p%s\003&* says '&+W%s&*'\n", pname(mon),
              strong[my_random() % arraysize(strong)]);
        return;
      }
      else if (s < t)
      {
        sendf(ploc(mon), "&+W\001p%s\003&* says '&+W%s&*'\n", pname(mon),
              medium[my_random() % arraysize(medium)]);
        return;
      }
      else
        sendf(ploc(mon), "&+W\001p%s\003&* says '&+W%s&*'\n", pname(mon),
              weak[my_random() % arraysize(weak)]);
    }
  }

  if (randperc() * 6 / (10 * TIMER_INTERRUPT) < pspeed(mon))
    movemob(mon);
  if (pnum(mon) == MOB_VALLEY_CHICKEN && randperc() < 8)
    sendf(ploc(mon), "\001p%s\003 says 'The sky is about to fall in.'\n",
          pname(mon));
  if (pnum(mon) == MOB_CATACOMB_GHOST && randperc() < 12)
    sendf(ploc(mon), "The Ghost moans, sending chills down your spine.\n");
  if (pnum(mon) == MOB_START_PUFF && randperc() < 1 && randperc() < 25)
  {
    puffrand = randperc();

    if ((puffrand <= 45) && !ststflg(mon, SFL_DUMB))
      sendf(ploc(mon), "\001p%s\003 says '%s'\n",
            pname(mon), m[my_random() % arraysize(m)]);

    if ((puffrand > 45 && puffrand <= 70) && (!ststflg(mon, SFL_NOSHOUT)))
    {
      sprintf(blob, "%s",
              puffshouts[my_random() % arraysize(puffshouts)]);
      send_g_msg(DEST_ALL, shout_test, mon, blob);
    }

    if (puffrand > 70 && puffrand <= 100)
      sendf(ploc(mon), puffactions[my_random() % arraysize(puffactions)],
            pname(mon));
  }
  if (((mtstflg(mon, MFL_THIEF) && randperc() < 20 && stealstuff(mon))) || ((mtstflg(mon, MFL_PICK) && randperc() < 40 && shiftstuff(mon))))
    return;
}

/* Handle Runesword */
void dorune(int plx)
{
  int ply;

  if (pfighting(plx) >= 0 || testpeace(plx))
    return;

  for (ply = lfirst_mob(ploc(plx)); ply != SET_END; ply = lnext_mob(ploc(plx)))
  {

    if (ply != plx && !EMPTY(pname(ply)) &&
        plev(ply) < LVL_APPREN && fpbns(pname(ply)) >= 0 &&
        randperc() >= 9 * plev(plx))
    {
      sendf(plx, "The Runesword twists in your hands, lashing out savagely!\n");
      hit_player(plx, ply, OBJ_CASTLE_RUNESWORD);
      return;
    }
  }
}

Boolean
dragget(void)
{
  int l;

  if (plev(mynum) >= LVL_APPREN)
    return False;
  return False;
}

void helpchkr(int plx)
{
  int x = phelping(plx);

  if (!is_in_game(x))
  {
    sendf(plx, "You can no longer help.\n");
    setphelping(plx, -1);
  }
  else if (ploc(x) != ploc(plx))
  {
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
  r = randperc() % 6; /* change this.... here chance is less if few exits */
  if ((n = getexit(ploc(x), r)) >= EX_SPECIAL)
    return;
  if (n >= DOOR)
  {
    if (state(n - DOOR) > 0)
      return;
    n = /*oloc((n - DOOR) ^ 1); */ obj_loc(olinked(n - DOOR));
  }
  if (n >= 0 || ltstflg(n, LFL_NO_MOBILES) || ltstflg(n, LFL_DEATH))
    return;

  if (lnumchars(ploc(x)) > 1)
    send_msg(ploc(x), 0, pvis(x), LVL_MAX, x, NOBODY,
             "%s has gone %s.\n", pname(x), exits[r]);
  setploc(x, n);
  if (lnumchars(ploc(x)) > 1)
    send_msg(ploc(x), 0, pvis(x), LVL_MAX, x, NOBODY,
             "%s has arrived.\n", pname(x));
}

/*
 * Steal random object from random player in the same room
 * -Nicknack  Sep. 1990
 * improved by Alf Oct, 1991
 * improved by Nicknack May, 1993
 */
Boolean
stealstuff(int m)
{
  int p[50]; /* Room for mortals and objects */
  int i, j, k;
  int nr = 0;

  /* Count the number of mortals in the same room:
   */
  for (j = 0; j < lnumchars(ploc(m)) && nr < 50; j++)
  {
    i = lmob_nr(j, ploc(m));

    if (is_in_game(i) && i < max_players && plev(i) < LVL_APPREN)
    {
      p[nr++] = i;
    }
  }
  if (nr == 0)
    return False;

  /* select a random one of those in the same room:
   */
  i = p[randperc() % nr];

  /* Count the number of objects he carries that we can take:
   */
  for (nr = k = 0; k < pnumobs(i) && nr < 50; k++)
  {
    j = pobj_nr(k, i);

    if (iscarrby(j, i))
    {
      if (((ocarrf(j) == WORN_BY || ocarrf(j) == BOTH_BY) && !mtstflg(m, MFL_SWORN)) ||
          ((ocarrf(j) == WIELDED_BY || ocarrf(j) == BOTH_BY) && !mtstflg(m, MFL_SWPN)))
        continue;
      else
        p[nr++] = j;
    }
  }

  if (nr == 0)
    return False;

  /* Select random object from those he carries
   */
  j = p[randperc() % nr];

  sendf(i, "\001p%s\003 steals the %s from you!\n", pname(m), oname(j));

  send_msg(ploc(i), 0, LVL_MIN, LVL_MAX, i, NOBODY,
           "\001p%s\003 steals the %s from \001p%s\003!\n",
           pname(m), oname(j), pname(i));

  if (otstbit(j, OFL_WEARABLE))
  {
    setoloc(j, m, WORN_BY);
  }
  else
  {
    setoloc(j, m, CARRIED_BY);
  }

  if (otstbit(j, OFL_WEAPON) &&
      (pwpn(m) == -1 || odamage(pwpn(m)) < odamage(j)))
  {
    set_weapon(m, j);
  }
  return True;
}

Boolean
shiftstuff(int m)
{
  Boolean took = False;
  int a, b;
  int maxdam = 0, w = -1;
  int maxarm = 0, ww = -1;

  for (b = 0; b < lnumobs(ploc(m)); b++)
  {

    a = lobj_nr(b, ploc(m));

    if (!oflannel(a))
    {
      took = True;

      sendf(ploc(m), "\001p%s\003 takes the %s.\n",
            pname(m), oname(a));

      /* Wield the best weapon, wear the best armor:
       */
      if (otstbit(a, OFL_WEAPON) && odamage(a) > maxdam)
      {
        w = a;
        maxdam = odamage(a);
      }
      if (otstbit(a, OFL_ARMOR) && oarmor(a) > maxarm)
      {
        ww = a;
        maxarm = oarmor(a);
      }
      setoloc(a, m, CARRIED_BY);
    }

    if (w >= 0)
      set_weapon(m, w);
    if (ww >= 0)
    {
      setoloc(ww, m, (w == ww) ? BOTH_BY : WORN_BY);
    }
  }
  return took;
}

char *
xname(char *n)
{
  char *t;

  if (n != NULL && (t = strrchr(n, ' ')) != NULL)
  {
    return t + 1;
  }
  return n;
}

void setname(int plx)
{
  register PLAYER_REC *p = cur_player;

  if (psex(plx))
    p->wd_them = strcpy(p->wd_her, pname(plx));
  else
    p->wd_them = strcpy(p->wd_him, pname(plx));
}

Boolean
see_player(int pla, int plb)
{
  if (pla < 0 || pla >= numchars)
    return False;
  if (plb == pla || plb < 0 || plb >= numchars)
    return True;
  if (pvis(plb) > 0 && plev(pla) < pvis(plb))
    return False;
  if (ststflg(pla, SFL_BLIND))
    return False;
  if (ploc(pla) == ploc(plb) && r_isdark(ploc(pla), pla))
    return False;
  return True;
}

char *
see_name(int pla, int plb)
{
  return see_player(pla, plb) ? pname(plb) : "Someone";
}

Boolean
seeplayer(int plx)
{
  if (plx == mynum || plx < 0 || plx >= numchars)
    return True;
  if (!see_player(mynum, plx))
    return False;
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
  int nn;

  if (name == NULL || strlen(name) > MNAME_LEN)
    return -1;

  name = xname(name); /* Skip the "The " if there. */

  while (*name != '\0' && isalpha(*name))
    *p++ = *name++;
  *p = '\0';

  if (isdigit(*name))
  {
    n = atoi(name);

    while (isdigit(*++name))
      ;
    if (*name != '\0')
      return -1;
  }
  else if (*name != '\0')
  {
    return -1;
  }
  else
    n = 1;

  if (*b == '\0')
  {

    if (n >= GLOBAL_MAX_OBJS && n < GLOBAL_MAX_OBJS + numchars && is_in_game(n - GLOBAL_MAX_OBJS))
      return n - GLOBAL_MAX_OBJS;
    else
      return -1;
  }
  else
  {
    /* Try the mobiles in the players location first:
     */
    if (is_in_game(mynum))
    {
      int m = n;
      int loc = ploc(mynum);

      for (i = 0; i < lnumchars(loc); i++)
      {

        if (EQ(b, xname(pname(lmob_nr(i, loc)))) && is_in_game(lmob_nr(i, loc)) && --m == 0)

          return lmob_nr(i, loc);
      }

      if (m < n)
        return -1;
    }

    /* Now try anyone.
     */

    nn = n;
    /* got an exact match? */
    for (i = 0; i < numchars; i++)
    {
      if (EQ(b, xname(pname(i))) && is_in_game(i) && --n == 0)
        return i;
    }

    n = nn;
    if (cur_player)
      if (cur_player->iamon)
        for (i = 0; i < numchars; i++)
        {
          if (!strncasecmp(b, xname(pname(i)), strlen(b)) &&
              is_in_game(i) && --n == 0)
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

  if (id >= max_players && id < num_const_chars)
    return id;

  return (x = lookup_entry(id, &id_table)) == NOT_IN_TABLE || x < 0 || x >= numchars ? -1 : x;
}

/* Find player or mobile, return number if in game.
 * Set f to true if in file, False if in game. If exists, put
 * the data in the object pointed to by p.
 */
int find_player(char *name, PERSONA *p, Boolean *f)
{
  int plr;

  *f = False;
  if ((plr = fpbns(name)) >= 0)
  {
    if (!seeplayer(plr))
      return -1;
    player2pers(p, NULL, plr);
    return plr;
  }
  else if (ptstflg(mynum, PFL_UAF) != 0 && getuaf(name, p))
  {
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
  if (lev < LVL_TOAD)
    return LEV_NEG; /* Negative level */
  if (lev < LVL_APPREN)
    return LEV_MORTAL; /* Mortal */
  if (lev == LVL_APPREN)
    return LEV_APPRENTICE; /* Apprentice */
  if (lev < LVL_WIZARD)
    return LEV_EMERITUS; /* Emeriti */
  if (lev < LVL_SENIOR)
    return LEV_WIZARD; /* Wizard */
  if (lev < LVL_ARCHWIZARD)
    return LEV_SENIOR; /* Senior */
  if (lev < LVL_HIGHARCH)
    return LEV_ARCHWIZARD; /* Arch wizard */
  if (lev < LVL_DEMI)
    return LEV_HIGHARCH; /* High Arch */
  if (lev < LVL_HIGHDEMI)
    return LEV_DEMI; /* Demi */
  if (lev < LVL_GOD)
    return LEV_HIGHDEMI; /* High Demi */
  if (lev < LVL_MASTER)
    return LEV_GOD;  /* God */
  return LEV_MASTER; /* Master */
}

Boolean
do_okay_l(int p, int v, Boolean c)
{
  int lev_p = wlevel(p);
  int lev_v = wlevel(v);

  if (lev_v > lev_p && lev_p > LEV_NEG)
  {
    return False;
  }

  return (c || (lev_v < lev_p && lev_p > LEV_WIZARD) || lev_p >= LEV_MASTER);
}

/* Can p(layer) do XX to v(ictim) ?
 * ** prot_flag protects against it.
 */

Boolean
do_okay(int p, int v, int prot_flag)
{
  return do_okay_l(plev(p), plev(v),
                   (prot_flag < PFL_MAX && !ptstflg(v, prot_flag)));
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

  /* First remove plr from his current location:
   */
  if (exists(ploc(plr)))
    remove_int(plr, lmobs(ploc(plr)));

  /* Then add him to the new room:
   */
  if (exists(room))
    add_int(plr, lmobs(room));

  ploc(plr) = room;
}

int ptothlp(int pl)
{
  int ct;

  for (ct = 0; ct < numchars; ct++)
  {
    if (ploc(ct) == ploc(pl) && phelping(ct) == pl)
      return ct;
  }
  return -1;
}

int maxstrength(int p)
{
  return pmaxstrength(plev(p));
}

char *
make_here(int num)
{
  static char buff[256];
  static char tbuff[512];
  register int t;
  int save_mynum;

  if (num >= max_players)
  { /* mobile */
    sprintf(buff, "%s", pftxt(num));
  }

  else
  {
    save_mynum = real_mynum;
    setup_globals(num);

    if (!psitting(mynum))
    {
      build_setin(buff, cur_player->setstand, pname(mynum), NULL);
      sprintf(tbuff, "%s", buff);
    }
    else
    {
      build_setin(buff, cur_player->setsit, pname(mynum), NULL);
      sprintf(tbuff, "%s", buff);
    }
    if (ststflg(mynum, SFL_GLOWING))
      strcat(tbuff, " (providing light)");
    for (t = 0; t != PNAME_LEN + TITLE_LEN + 20; t++)
      if (tbuff[t] == '%' && tbuff[t + 1] == 'n')
      {
        tbuff[t] = '%';
        tbuff[t + 1] = 's';
      }

    sprintf(buff, tbuff, pname(num));
    setup_globals(save_mynum);
  }

  return buff;
}

char *
make_title(char *title, char *name)
{
  static char buff[PNAME_LEN + TITLE_LEN + 20];

  sprintf(buff, (EMPTY(title) ? "%s the Unknown" : title), name);
  return buff;
}

char *
std_title(int level, Boolean sex)
{
  extern char *MLevels[];
  extern char *FLevels[];

  static char buff[TITLE_LEN + 10];
  int wl = wlevel(level);

  strcpy(buff, "%s the ");

  switch (wl)
  {
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
Boolean
reset_mobile(int m)
{
  int loc;

  setpstr(m, pstr_reset(m));
  setpvis(m, pvis_reset(m));
  setpflags(m, pflags_reset(m));
  setmflags(m, mflags_reset(m));
  setsflags(m, sflags_reset(m));
  setpmsk1(m, 0);
  setpmsk2(m, 0);
  setpmsk3(m, 0);
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
  clear_hate(m);

  if (EMPTY(pname(m)))
    setpname(m, pname_reset(m));

  if ((loc = find_loc_by_id(ploc_reset(m))) == 0)
  {
    setpstr(m, -1);
    setploc(m, LOC_DEAD_DEAD);
    return False;
  }
  else
  {
    setploc(m, loc);
    return True;
  }
}

void p_crapup(int player, char *str, int flags)
{
  int m = real_mynum;

  setup_globals(player);
  crapup(str, flags | CRAP_RETURN);
  setup_globals(m);
}

void crapup(char *str, int flags)
{
  if ((flags & CRAP_UNALIAS) != 0)
  {
    unalias(real_mynum);
    unpolymorph(real_mynum);
  }
  if (in_group(real_mynum))
    pleave();
  xcrapup(str, (flags & CRAP_SAVE) != 0);
  if ((flags & CRAP_RETURN) == 0)
  {
    longjmp(to_main_loop, JMP_QUITTING);
  }
}

void xcrapup(char *str, Boolean save_flag)
{
  int i, temp;
  static char *dashes =
      "--------------------------------------------------------------------------";

  if (cur_player->aliased)
  {
    bprintf("%s\n", str);
    unalias(real_mynum);
    return;
  }
  else if (cur_player->polymorphed >= 0)
  {
    bprintf("%s\n", str);
    unpolymorph(real_mynum);
    return;
  }
  cur_player->tr.trace_item = -1;
  temp = mynum;
  for (i = 0; i < max_players; i++)
  {
    setup_globals(i);
    if (cur_player->converse.active == True)
      if (cur_player->converse.talking_to == temp)
      {
        bprintf("%s has left, you can no longer converse with them.\n",
                pname(temp));
        cur_player->converse.talking_to = -1;
        cur_player->converse.active = False;
        strcpy(cur_player->prompt, cur_player->converse.old_prompt);
      }
  }
  setup_globals(temp);
  quit_player(); /* So we don't get a prompt after exit */

  loseme(save_flag);
  if (str != NULL)
  {
    bprintf("\n%s\n\n%s\n\n%s\n", dashes, str, dashes);
    bflush();
  }
  setpname(mynum, "");
  setploc(mynum, 0);

  remove_entry(mob_id(mynum), &id_table);

  cur_player->iamon = False;
}

/* Remove me from the game. Dump objects, send messages, save etc..
 * May only be used after has been called.
 */
void loseme(Boolean save_flag)
{
  char b[80];
  int x, y, pocket;
  Boolean emp = EMPTY(pname(mynum));

  if (cur_player->aliased || cur_player->polymorphed >= 0)
    return;

  if (cur_player->iamon)
  {
    if (!emp)
    {

      if (save_flag)
        saveme();

      if ((x = the_world->w_lock) > 0 && (y = plev(mynum)) >= x && y >= LVL_APPREN)
        bprintf("\nDon't forget the game is locked....\n");

      send_msg(DEST_ALL, MODE_QUIET, max(pvis(mynum), LVL_APPREN),
               LVL_MAX, mynum, NOBODY,
               "&+Y[%s has departed from MUD in %s (%s)]&*\n",
               pname(mynum), sdesc(ploc(mynum)), xshowname(b, ploc(mynum)));
    }

    dumpitems();
    if (!emp)
    {
      mudlog("EXIT:  %s [lev %d, scr %d, str %d/%d]",
             pname(mynum), plev(mynum), pscore(mynum), pstr(mynum), maxstrength(mynum));

      setpname(mynum, "");
      setploc(mynum, 0);
    }

    cur_player->iamon = False;

    snoop_off(mynum);
  }
}

char *
lev2s(char *b, int lvl, Boolean x)
{
  switch (lvl)
  {
  case 0:
    strcpy(b, "Toad ");
    return b;
  case LVL_SIXTEEN:
    strcpy(b, "Priest ");
    return b;
  case LVL_FIFTEEN:
    strcpy(b, "Holy ");
    return b;
  case LVL_FOURTEEN:
    strcpy(b, "Honorable ");
    return b;
  case LVL_THIRTEEN:
    strcpy(b, "Evoker ");
    return b;
  case LVL_TWELVE:
    strcpy(b, "Magician ");
    return b;
  case LVL_ELEVEN:
    strcpy(b, "Conqueror ");
    return b;
  case LVL_TEN:
    strcpy(b, "Ruler ");
    return b;
  case LVL_NINE:
    strcpy(b, "Prince ");
    return b;
  case LVL_EIGHT:
    strcpy(b, "Knight ");
    return b;
  case LVL_SEVEN:
    strcpy(b, "Squire ");
    return b;
  case LVL_SIX:
    strcpy(b, "Champion ");
    return b;
  case LVL_FIVE:
    strcpy(b, "Warrior ");
    return b;
  case LVL_FOUR:
    strcpy(b, "Nomad ");
    return b;
  case LVL_THREE:
    strcpy(b, "Sightseer ");
    return b;
  case LVL_TWO:
    strcpy(b, "Wanderer ");
    return b;
  case LVL_ONE:
    strcpy(b, "Novice ");
    return b;
  default:
    switch (wlevel(lvl))
    {
    case LEV_APPRENTICE:
      sprintf(b, "Apprentice ");
      return b;
    case LEV_EMERITUS:
      sprintf(b, "Emeritus ");
      return b;
    case LEV_WIZARD:
      sprintf(b, "%s ", x ? "Witch" : "Wizard");
      return b;
    case LEV_SENIOR:
      sprintf(b, "Senior %s ", x ? "Witch" : "Wizard");
      return b;
    case LEV_ARCHWIZARD:
      sprintf(b, "Arch%s ", x ? "Witch" : "Wizard");
      return b;
    case LEV_HIGHARCH:
      sprintf(b, "High Arch ");
      return b;
    case LEV_DEMI:
      sprintf(b, "Demi%s ", x ? "Goddess" : "God");
      return b;
    case LEV_HIGHDEMI:
      sprintf(b, "High Demi%s ", x ? "Goddess" : "God");
      return b;
    case LEV_GOD:
      sprintf(b, "%s ", x ? "Goddess" : "God");
      return b;
    case LEV_MASTER:
      sprintf(b, "Master ");
      return b;
    }
    break;
  }
  sprintf(b, "Level %d ", lvl);
  return b;
}

int tscale(void)
{
  int a = 0;
  int b;

  for (b = 0; b < max_players; b++)
    if (is_in_game(b) && plev(b) < LVL_APPREN)
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
  int rmynum = mynum;
  int oldlevel;
  static char partybuff[MAX_COM_LEN];
  int plx, leader;

  if (pl >= max_players || !players[pl].iamon || players[pl].aliased ||
      players[pl].polymorphed >= 0)
    return;

  oldlevel = plev(pl);
  if ((b = levelof(pscore(pl), plev(pl))) != plev(pl) &&
      plev(pl) > 0)
  {

    if (b == LVL_APPREN && (!tstbits(qflags(pl), Q_ALL)))
    {
      return;
    }

    setplev(pl, b);
    setptitle(pl, std_title(b, psex(pl)));
    sendf(pl, "You are now %s\n", make_title(ptitle(pl), pname(pl)));
    if (oldlevel < plev(pl) && plev(pl) >= LVL_FOUR && plev(pl) < LVL_APPRENTICE)
      sendf(DEST_ALL, "&+YCongratulations, %s!\n", make_title(ptitle(pl), pname(pl)));
    mudlog("%s to level %d", pname(pl), b);

    send_msg(DEST_ALL, MODE_QUIET | MODE_COLOR, max(pvis(pl), LVL_APPREN),
             LVL_MAX, NOBODY, NOBODY,
             "[%s is now level %d]\n", pname(pl), plev(pl));

    setup_globals(pl);
    saveallcom();
    setup_globals(rmynum);
    setpstr(pl, maxstrength(pl));
    switch (b)
    {
    case LVL_SEVEN:
      sendf(pl, "\nWelcome, %s.  You may now use the POSE command.\n",
            (psex(pl) ? FLevels : MLevels)[LVL_SEVEN]);
      break;
    case LVL_TEN:
      sendf(pl, "\nNice work, %s.  You may now use the BANG command.\n",
            (psex(pl) ? FLevels : MLevels)[LVL_TEN]);
      break;
    case LVL_FOUR:
      set_doublebit(&pflags(pl), PFL_EMOTE);
      set_doublebit(&pflags(pl), PFL_TITLES);
      sendf(pl, "\nCongratulations %s!\n You may now emote everywhere and change title.\n", (psex(pl) ? FLevels : MLevels)[LVL_FOUR]);
      break;
    case LVL_FIFTEEN:
      set_xpflags(LVL_FIFTEEN, &pflags(pl), &pmask(pl));
      sendf(pl, "\nCongratulations %s!  You may now use EMOTE anywhere.\n",
            (psex(pl) ? FLevels : MLevels)[LVL_FIFTEEN]);
      break;
    case LVL_APPREN:
      set_xpflags(LVL_APPREN, &pflags(pl), &pmask(pl));
      if (in_group(mynum))
      {
        if (p_group_leader(mynum) == mynum)
        { /* Drop the whole group.   */
          sprintf(partybuff, "Party is disbanded");
          partyannounce(partybuff);
          for (plx = 0; plx < max_players; plx++)
          {
            if (is_in_game(plx) && (p_group(plx) == p_group(mynum)) && plx != mynum)
            {
              leaveplayer(plx);
            }
          } /* Now only the leader is in the group */
          p_group_shares(mynum) -= p_share(mynum);
          p_share(mynum) = 0;
          free(p_group(mynum)); /* Loose allocated mem */
          p_group(mynum) = NULL;
        }
        else
        {
          /* Lose one player, not the party, We dont have to check if the Player is the
          last player, because the last player is always the leader, which is handled in
          the bit of code above */
          sprintf(partybuff, "%s has left the party", pname(mynum));
          partyannounce(partybuff);
          leaveplayer(mynum);
        }
      }
      sendf(DEST_ALL, "Trumpets sound to praise %s, the new Wizard.\n",
            pname(pl));

      sendf(pl, "\001f%s\003", GWIZ);

      update_wizlist(pname(pl), LEV_APPRENTICE);

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

int levelof(int score, int lev)
{
  int i;

  if (lev > LVL_APPREN || lev < LVL_ONE)
    return lev;
  for (i = LVL_APPREN; i > LVL_TOAD; i--)
    if (score >= levels[i])
      return i;
  return 0;
}

Boolean
check_busy(int plx)
{
  if (ststflg(plx, SFL_BUSY))
  {
    bprintf("%s is busy, try later!\n", pname(plx));
    return True;
  }
  return False;
}

char *
build_prompt(int plx)
{
  static char b[PROMPT_LEN + 30];

  *b = 0;
  strcat(b, players[plx].prompt);
  return b;
}

int vicf2(int fl, int i)
{
  int plr;

  if (ltstflg(ploc(mynum), LFL_NO_MAGIC) != 0 && plev(mynum) < LVL_APPREN)
  {
    bprintf("Something about this location has drained your mana.\n");
    return -1;
  }
  if (fl >= SPELL_VIOLENT && plev(mynum) < LVL_APPREN && testpeace(mynum))
  {
    bprintf("No, that's violent!\n");
    return -1;
  }
  if ((plr = vicbase()) < 0)
    return -1;

  if (pstr(mynum) < 10)
  {
    bprintf("You are too weak to cast magic.\n");
    return -1;
  }
  if (plev(mynum) < LVL_APPREN)
    setpstr(mynum, pstr(mynum) - 1);

  if (carries_obj_type(mynum, OBJ_WINDOW_POWERSTONE) > -1)
    i++;
  if (carries_obj_type(mynum, OBJ_WINDOW_POWERSTONE1) > -1)
    i++;
  if (carries_obj_type(mynum, OBJ_WINDOW_POWERSTONE2) > -1)
    i++;

  if (plev(mynum) < LVL_APPREN && randperc() > i * plev(mynum))
  {
    bprintf("You fumble the magic.\n");
    if (fl == SPELL_REFLECTS)
    {
      bprintf("The spell reflects back.\n");
      return mynum;
    }
    return -1;
  }
  if (plev(mynum) < LVL_APPREN)
  {
    if ((fl >= SPELL_VIOLENT) && wears_obj_type(plr, OBJ_CATACOMB_SHIELD) > -1)
    {
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

  if ((a = vicf2(SPELL_VIOLENT, cth)) < 0)
    return a;
  if (ploc(a) != ploc(mynum))
  {
    bprintf("%s isnt here.\n", psex(a) ? "She" : "He");
    return -1;
  }
  return a;
}

int vichere(void)
{
  int a;
  if ((a = vicbase()) == -1)
    return -1;
  if (ploc(a) != ploc(mynum))
  {
    bprintf("They aren't here.\n");
    return -1;
  }
  return a;
}

int vicbase(void)
{
  int a;

  do
  {
    if (brkword() == -1)
    {
      bprintf("Who?\n");
      return -1;
    }
  } while (EQ(wordbuf, "at"));
  if ((a = fpbn(wordbuf)) < 0)
  {
    bprintf("That person isn't playing now.\n");
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
  static int jumtb[] =
      {
          /* FROM,                    TO */
          LOC_CASTLE_LEAP, LOC_VALLEY_NPATH,
          LOC_TOWER_LEDGE, LOC_CASTLE_WALLS,
          LOC_TREEHOUSE_1, LOC_VALLEY_ESIDE,
          LOC_MOOR_3, LOC_LEDGE_8,
          LOC_WINDOW_WINDOW, LOC_WINDOW_CHAMBER,
          LOC_JBS_BRIDGE, LOC_JBS_MOUNTAINS,
          LOC_WINDOW_CHAMBER, LOC_WINDOW_WINDOW,
          LOC_CATACOMB_CHASM1, LOC_CATACOMB_CAVERN,
          LOC_CATACOMB_CHASM2, LOC_CATACOMB_CAVERN,
          LOC_CATACOMB_SLEDGE2, LOC_CATACOMB_CAVERN,
          LOC_CATACOMB_NLEDGE2, LOC_CATACOMB_CAVERN,
          LOC_CATACOMB_SLEDGE1, LOC_CATACOMB_CHASM1,
          LOC_CATACOMB_NLEDGE1, LOC_CATACOMB_CHASM2,
          0, 0};

  if (psitting(mynum))
  {
    bprintf("You have to stand up first.\n");
    return;
  }

#ifdef LOCMIN_KASTLE
  if (ploc(mynum) == LOC_KASTLE_16)
  {
    sprintf(ms, "You hear a loud flush come from somewhere.\n");
    broad(ms);
    bprintf("You leap upwards and grab ahold of the chain.  Your weight pulls the chain.\n");
    bprintf("........There is a great flushing sound as the pit empties.\n");
    teletrap(LOC_KASTLE_22);
    return;
  }
#endif

#ifdef LOCMIN_EASTCOAST
  if (ploc(mynum) == LOC_EASTCOAST_3 || ploc(mynum) == LOC_EASTCOAST_4)
  {
    bprintf("Ignoring the dangers of your position, you jump and hurtle\n");
    bprintf("over the edge of the cliff, and down to your fate below...\n");

    teletrap(LOC_EASTCOAST_6);
    return;
  }

  if (ploc(mynum) == LOC_EASTCOAST_37 || ploc(mynum) == LOC_EASTCOAST_38)
  {
    if (iscarrby(OBJ_START_UMBRELLA, mynum) && state(OBJ_START_UMBRELLA))
    {
      bprintf("You float down to safety...\n");

      switch (ploc(mynum))
      {
      case LOC_EASTCOAST_37:
        teletrap(LOC_EASTCOAST_12);
        break;
      case LOC_EASTCOAST_38:
        teletrap(LOC_EASTCOAST_13);
        break;
      }
    }
    else
    {
      bprintf("You ignore the warnings and clumsily sail over the fence...\n");
      teletrap(LOC_EASTCOAST_6); /* As good as any, I guess... */
    }
    return;
  }
#endif

#ifdef LOCMIN_ABYSS
  if (ploc(mynum) == LOC_ABYSS_CLOSET)
  {
    bprintf("You leap upward and grab ahold of the shelf, pulling yourself "
            "up.\n");
    teletrap(LOC_ABYSS_SHELF);
    return;
  }
  if (ploc(mynum) == LOC_ABYSS_HEAD || ploc(mynum) == LOC_ABYSS_WICKET)
    if (!iswornby(OBJ_ABYSS_RING, mynum))
    {
      crapup("You rapidly fall to your death.....S P L A T !!\n", 1);
      return;
    }
    else
    {
      bprintf("You float slowly downward.  Falling.. falling..\n");
      teletrap(LOC_ABYSS_BANK1);
      return;
    }
#endif

#ifdef LOCMIN_FROBOZZ
  if (ploc(mynum) == LOC_FROBOZZ_ENDING)
  {
    sprintf(ms, "%s jumps into the hole and disappears!\n", pname(mynum));
    sillycom(ms);
    setploc(mynum, LOC_WINDOW_GLITTERING);
    bprintf("You jump into the hole...\n\n\n\nAnd fall....."
            "Until the tunnel stops abruptly\n"
            "and ends as a hole in a ceiling, "
            "through which you fall and land\n");
    sprintf(ms, "%s comes falling through a hole in the ceiling and lands\n",
            pname(mynum));
    sillycom(ms);
    if (ploc(mynum) == ploc(max_players + MOB_WINDOW_GIANT))
    {
      bprintf("on the stomach of a snoring giant!\n");
      sillycom("on the stomach of the the snoring giant!\n");
      hit_player(max_players + MOB_WINDOW_GIANT, mynum, -1);
    }
    else
    {
      bprintf("on the ground with a thud, hurting yourself badly!\n");
      sillycom("on the ground with a loud thud, being badly hurt!\n");
      if (plev(mynum) < LVL_APPREN)
        setpstr(mynum, pstr(mynum) / 2);
    }
    return;
  }
#endif

  /* Search the jump-table for special locations..
   */
  for (a = 0, b = 0; jumtb[a]; a += 2)
  {
    if (jumtb[a] == ploc(mynum))
    {
      b = jumtb[a + 1];
      break;
    }
  }

#ifdef LOCMIN_CATACOMB
  if (ploc(mynum) == LOC_CATACOMB_SLEDGE2 || ploc(mynum) == LOC_CATACOMB_NLEDGE2)
  {
    if (oloc(OBJ_START_UMBRELLA) == mynum &&
        ocarrf(OBJ_START_UMBRELLA) == CARRIED_BY)
    {
      bprintf(
          "You grab hold of the umbrella and step off the ledge.\n"
          "\n"
          "You are drifting slowly through the darkness of the chasm, falling towards an\n"
          "uncertain fate.  The rocky walls slide past you, as does another ledge, just\n"
          "beyond your reach.  After several minutes, your descent comes to an end and\n"
          "you alight in ...\n");
      trapch(b);
      return;
    }
    else
    {
      bprintf(
          "You step off the ledge, but instead of plummeting into the darkness below,\n"
          "your motion is stopped and you find yourself ...\n");
      trapch((ploc(mynum) == LOC_CATACOMB_SLEDGE2) ? LOC_CATACOMB_CHASM1 : LOC_CATACOMB_CHASM2);
      return;
    }
  }

  if (ploc(mynum) == LOC_CATACOMB_SLEDGE1 || ploc(mynum) == LOC_CATACOMB_NLEDGE1)
  {
    bprintf(
        "You make a leap of Faith, throwing yourself off the ledge.  You are hurtling\n"
        "rapidly through the darkness of the chasm, falling to almost certain doom.\n"
        "The rocky walls rush past you ...\n");
    trapch(b);
    return;
  }
#endif

  /* Are we by a pit ? If so we'll jump into it.
   */
  for (i = 0; (j = pits[i++]) != -1 && oloc(j) != ploc(mynum);)
    ;

  if (j >= OBJ_CATACOMB_PIT_NORTH && j <= OBJ_CATACOMB_PIT_WEST && state(i) == 0)
  {
    b = LOC_CATACOMB_RIPS;
  }
  else if ((j >= OBJ_START_PIT && j <= OBJ_START_CHURCH_PIT) || oloc(OBJ_ORCHOLD_HOLEORCS) == ploc(mynum))
  {
    b = LOC_PIT_PIT;
  }

  if (b == 0)
  {
    bprintf("&+MWheeeeee....&*\n");
    return;
  }

  if ((x = carries_obj_type(mynum, OBJ_START_UMBRELLA)) > -1 && state(x) != 0)
  {
    sprintf(ms, "%s jumps off the ledge.\n", pname(mynum));
    bprintf("You grab hold of the %s and fly down like "
            "Mary Poppins.\n",
            oname(x));
  }
  else if (plev(mynum) < LVL_APPREN)
  {
    sprintf(ms, "%s makes a perfect swan dive off the ledge.\n",
            pname(mynum));
    if (b != LOC_PIT_PIT)
    {
      send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY, ms);
      setploc(mynum, b);
      bprintf("&+MWheeeeeeeeeeeeeeeee&*       <<<<SPLAT>>>>\n");
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
  int stop = (player == SP_ALL) ? max_players - 1 : player; /*max_chars ? */
  int i;
  int store_mynum;

  /* additional variables used with trace routine */
  TRACE *tr;
  char b[512];  /* for the object trace message */
  char b1[100]; /* for the object trace message parts */
  int x, k, y;

  int obj;
  int curbestwpn;
  int curbestarm;

  /* Puff's Assorted Yammerings */
  static char *pufftells[] =
      {
          "I'm Puff the Fractal Dragon. Who are you?",
          "Did you steal my towel?",
          "Why does everyone ignore me?",
          "Don't forget to feed me!",
          "Don't mind me, I'm just imagining you on a platter with a cream sauce.",
          "Never annoy a dragon, for thou art crunchy and good tasting.",
          "Can i have your autograph?",
          "Hmm. I think you'd be a bit too salty for my tastes."};

  static char *puffpersonshouts[] =
      {
          "Why won't %s talk with me?",
          "I think %s stole my towel!",
          "Hey %s! Where is my food?",
          "I'm sorry %s, was that *your* foot!?",
          "%s, wanna dance?",
          "Why does %s think I'm annoying? I'm not annoying, am I?",
          "%s, can i have your autograph?"};
  char blob[MAX_COM_LEN];

  if (player >= max_players)
    return;

  for (i = start; i <= stop; ++i)
    if (is_in_game(i))
    {
      calib_player(i);

      /* Puff Enhanced - dragorn
         Yes, you may lynch me now. */
      if ((randperc() < 1 && randperc() < 10) && (is_in_game(MOB_START_PUFF + max_players) &&
                                                  alive((MOB_START_PUFF + max_players)) != -1))
      {
        if ((randperc() < 25) && (pvis(i) == 0) &&
            (!ststflg(MOB_START_PUFF + max_players, SFL_NOSHOUT)))
        {
          sprintf(blob, puffpersonshouts[my_random() % arraysize(puffpersonshouts)],
                  pname(i));
          send_g_msg(DEST_ALL, shout_test, MOB_START_PUFF + max_players, blob);
        }
        else if ((!ststflg(i, SFL_DEAF)) && (!ltstflg(ploc(i), LFL_SOUNDPROOF) && ploc(MOB_START_PUFF + max_players) != ploc(i)) &&
                 (!ststflg(MOB_START_PUFF + max_players, SFL_DUMB)))
        {
          sendf(i, "Puff tells you '&+C%s&*'\n",
                pufftells[my_random() % arraysize(pufftells)]);
        }
      }

      /* ghost code. -Dragorn */
      if (ststflg(i, SFL_GHOST))
      {
        store_mynum = mynum;
        setup_globals(i);
        cur_player->ghostcounter++;
        if ((cur_player->ghostcounter == 50) || (cur_player->ghostcounter == 100) || (cur_player->ghostcounter == 150))
        {
          bprintf("Your ghostly form begins to fade further.\n");
        }
        if (cur_player->ghostcounter >= 150)
        {
          cur_player->ghostcounter = 0;
          sclrflg(mynum, SFL_GHOST);
          strcpy(cur_player->setin, cur_player->ghostsetin);
          strcpy(cur_player->setout, cur_player->ghostsetout);
          /* Again, in newer dyrt versions, need to use setstand and setsit */
          /*   strcpy (cur_player->sethere, cur_player->ghostsethere); */
          strcpy(cur_player->setstand, cur_player->ghostsetstand);
          strcpy(cur_player->setsit, cur_player->ghostsetsit);

          p_crapup(i, "\t\tYou fade to nothing, and vanish.", CRAP_SAVE);
          mynum = store_mynum;
          setup_globals(mynum);
          return;
        }
      }

      /*
       * show trace of person/item if needed
       * [code by Twizzly July 1995]
       */
      tr = &players[i].tr;
      if ((x = tr->trace_item) != -1)
      {
        switch (tr->trace_class)
        {
        case 2: /* player */
          if (is_in_game(x))
          {
            if (ploc(x) != tr->trace_loc)
            {
              sendf(i, "&+CTRACE:&* %s is now at %s\n",
                    pname(x), showname(ploc(x)));
              tr->trace_loc = ploc(x);
            }
          }
          else /* player is not in the game */
          {
            tr->trace_item = -1;
          }
          break;
        case 1: /* an object */
          if (oloc(x) != tr->trace_loc || ocarrf(x) != tr->trace_carrf || tr->trace_oroom != roomobjin(x))
          {
            y = roomobjin(x);
            sprintf(b, "&+CTRACE:&* %s is", oname(x));
            tr->trace_loc = oloc(x);
            tr->trace_carrf = ocarrf(x);
            tr->trace_oroom = y;
            k = oloc(x);
            while (ocarrf(x) == IN_CONTAINER)
            {
              sprintf(b1, " in the %s", oname(k));
              strcat(b, b1);
              x = k;
              k = oloc(x);
            }
            if (ocarrf(x) == CARRIED_BY)
            {
              sprintf(b1, " carried by %s", pname(oloc(x)));
              strcat(b, b1);
            }
            else if (ocarrf(x) == WORN_BY)
            {
              sprintf(b1, " worn by %s", pname(oloc(x)));
              strcat(b, b1);
            }
            else if (ocarrf(x) == WIELDED_BY)
            {
              sprintf(b1, " wielded by %s", pname(oloc(x)));
              strcat(b, b1);
            }
            else if (ocarrf(x) == BOTH_BY)
            {
              sprintf(b1, "worn and wielded by %s", pname(oloc(x)));
              strcat(b, b1);
            }
            sprintf(b1, " in %s\n", showname(y));
            strcat(b, b1);
            sendf(i, b);
          }
          break;
        }
      }
      if (ploc(i) == LOC_CASTLE_MAIDEN && state(OBJ_CASTLE_IN_MAIDEN) == 1)
      {
        broad("\001dThere is a long drawn out scream in the distance\n\003");
        p_crapup(i, "\t\tThe iron maiden closes....... S Q U I S H !",
                 CRAP_SAVE);
        continue;
      }
      if (plev(i) < LVL_APPREN && ltstflg(ploc(i), LFL_ON_WATER) && (!ststflg(i, SFL_GHOST)))
      {
        if (!carries_boat(i))
        {
          p_crapup(i, "\t\tYou plunge beneath the waves....", CRAP_SAVE);
          continue;
        }
      }
      if (ltstflg(ploc(i), LFL_IN_WATER))
      {
        if (
            (!iswornby(OBJ_BLIZZARD_TALISMAN, i)) &&
            (!iswornby(OBJ_TREEHOUSE_AMULET, i)) &&
            players[i].iamon &&
            plev(i) < LVL_WIZARD)
        {
          p_crapup(i, "\t\tWater rushes in to fill your lungs....", CRAP_SAVE);
          broad("In this distance you hear some splashing and a gurgle.\n");
          continue;
        }
        if (randperc() % 100 <= 5)
          bprintf("You notice air bubbles floating up as you exhale.\n");
      }

      /*
       * If you are underwater and have an extinguishable light, KILL IT!
       */
      if (ltstflg(ploc(i), LFL_IN_WATER))
      {
        int_set *inv = pinv(i);
        int x;

        for (x = first_obj(inv); x != SET_END; x = next_obj(inv))
          if (iscarrby(x, i) && otstbit(x, OFL_LIT) && otstbit(x, OFL_EXTINGUISH))
          {
            sendf(i, "The water promptly extingishues your %s!\n", oname(x));
            oclrbit(x, OFL_LIT);
          }
      }

      /* 10% chance of the tree swallowing you
       */
      if (ploc(i) == LOC_FOREST_INTREE && plev(i) < LVL_APPREN && randperc() < 10)
      {
        ++odamage(OBJ_FOREST_TREEEATING);
        sendf(DEST_ALL, "The Tree shouts 'Hahaha!  Score: Me - %d Mortals -%d!'\n",
              odamage(OBJ_FOREST_TREEEATING), oarmor(OBJ_FOREST_TREEEATING));
        p_crapup(i, "You have been absorbed, and crushed to death by "
                    "the tree....",
                 CRAP_SAVE);
        broad("There is a hideous scream and a grinding of bone...\n");
        continue;
      }

      /* 40% chance that tree will suck you in
       */
      if (ploc(i) == oloc(OBJ_FOREST_TREEEATING) && randperc() < 40)
      {
        int me = real_mynum;
        sendf(i, "You are suddenly grabbed and taken into the tree!\n");
        setup_globals(i);
        teletrap(LOC_FOREST_INTREE);
        setup_globals(me);
      }
      /* 10% chance that the floorboards will squeak */
      if (ploc(i) == oloc(OBJ_VILLAGE_TOP_BOARDS) && randperc() < 10)
      {
        sendf(i, "The floorboards creak alarmingly as you move around.\n");
      }
      /* 10% chance of the fox barking */
      if (ploc(i) == oloc(OBJ_OAKTREE_TOPFOXHOLE) && randperc() < 10)
      {
        sendf(i, "The sharp bark of a fox reverberates off the oaks.\n");
      }
      if (ploc(mynum) == oloc(OBJ_MOOR_ALTAR) && plev(mynum) < LVL_APPREN)
      {
        if (oloc(OBJ_CHURCH_CROSS) == mynum)
        {
          bprintf("The altar and cross suddenly start sparking and flashing, as if in some kind of\n"
                  "conflict.  With a massive bang, the altar cracks.\n");
          destroy(OBJ_MOOR_ALTAR);
        }
      }
      /* If the snoop-victim has disappeard or the snooper is (only) a wizard
       * * and the target has gone into a private room, then stop the snoop.
       */
      if (players[i].snooptarget != -1)
      {

        if (!is_in_game(players[i].snooptarget))
        {
          sendf(i, "You can no longer snoop.\n");
          snoop_off(i);
        }
        else if (ltstflg(ploc(players[i].snooptarget), LFL_PRIVATE) && plev(i) < LVL_ARCHWIZARD)
        {

          sendf(i, "%s went into a PRIVATE room, you can no longer snoop.\n",
                pname(players[i].snooptarget));

          snoop_off(i);
        }
      }

      /* count down if polymorphed
       */
      if (players[i].polymorphed == 0)
        unpolymorph(i);
      if (players[i].polymorphed > -1)
        players[i].polymorphed--;

      /* if invis, count down
       */
      if (players[i].me_ivct > 0 && --players[i].me_ivct == 0)
      {
        setpvis(i, 0);
      }

      if (pstr(i) < 0 && (players[i].aliased || players[i].polymorphed != -1))
      {
        sendf(i, "You've just died.\n");
        if (players[i].polymorphed != -1)
          unpolymorph(i);
        else
          unalias(i);
      }

    } /* end for each player */
}

void regenerate(void)
{
  /* chance of heal is now 15 and 30 instead of 20 and 40 since
   * the interrupt now occurs every 2 secs. instead of 3
   */
  int i;

  for (i = 0; i < max_players; ++i)
    if (is_in_game(i) && pfighting(i) < 0 && pstr(i) < maxstrength(i) && (!ststflg(i, SFL_GHOST)))
    {
      Boolean has_ring = (wears_obj_type(i, OBJ_TOWER_RING) > -1);

      if ((randperc() < 15) && ltstflg(ploc(i), LFL_NEG_REGEN) &&
          (!has_ring && pstr(i) > 1) && plev(i) < LVL_APPREN)
      {
        sendf(i, "You feel your energy draining away!\n");
        setpstr(i, pstr(i) - 1);
        return;
      }

      if (((!has_ring && (randperc() < 15)) || ((psitting(i) && randperc() < 30))) || ((has_ring && randperc() < 67)))
      {

        if (maxstrength(i) == pstr(i) + 1)
        {
          sendf(i, "You feel fully healed.\n");
        }

        setpstr(i, pstr(i) + 1);

        if (players[i].iamon)
          calib_player(i);
      }
    }
}

static void
_userlist(char mode)
{
  char line[256];
  register int t;
  int old_mynum = 0;
  int a[256], a_len = 0;
  int idle = 0;
  struct tm *tm;
  int j;
  char buff[10];
  char uname[20];
  int usercount = 0;
  Boolean show_hostname = False;
  Boolean show_location = False;
  Boolean notHere = False;
  Boolean a_new_player = False;
  if (cur_player->aliased || cur_player->polymorphed != -1)
  {
    bprintf("Not while aliased.\n");
    return;
  }
  if (the_world->w_tournament || plev(mynum) >= LVL_APPREN)
    show_location = True;
  if (ptstflg(mynum, PFL_SHUSER) || the_world->w_tournament)
    show_hostname = True;

  if (mode)
  {
    bprintf("Level       Player\n");
    bprintf("&+R----------- %s\n", "------&*");
  }
  else
  {
    bprintf("&+R%-7s %-12.12s %-16.16s %-8.8s %-10.10s %-21.21s&*\n",
            "Level", "Player", show_hostname ? "Hostname" : " ",
            "Idle",
            show_location ? "Zone" : " ",
            show_location ? "Room" : " ");
    bprintf("&+R-----------------------------------------------------------------------------&*\n");
  }

  for (t = 0; t < max_players; t++)
    if (is_in_game(t) && ((pvis(t) <= plev(mynum)) || (t == mynum)))
    {
      a[a_len++] = t;
    }
  qsort(a, a_len, sizeof(int), cmp_player);

  for (j = 0; t = a[j], j < a_len; ++j)
  {
    old_mynum = real_mynum;

    setup_globals(t);
    idle = time(0) - cur_player->last_command;
#ifdef RS6000
    tm = (struct tm *)gmtime((const time_t *)&idle);
#else
    tm = (struct tm *)gmtime(&idle);
#endif

    if ((plev(mynum) < LVL_APPREN && (idle > 180)) || (plev(mynum) >= LVL_APPREN && (idle > 300)))
      notHere = True;

    lev2s(uname, plev(mynum), psex(mynum));
    uname[strlen(uname) - 1] = '\0';
    if (mode)
    {
      sprintf(line, "&+B[%-9.9s]&* %s&+W%s%s%s%s%s%s%s%s%s%s%s&*\n",
              uname,
              /*
                     make_title (ptitle (mynum), pname (mynum)),
              */
              mynum < max_players ? make_title(ptitle(mynum), pname(mynum)) : "&+W[Possessed&*]",
              pnewplayer(mynum) ? " [New Player]" : "",
              whopartytst(),
              ststflg(mynum, SFL_NOSHOUT) ? " [NoShout]" : "",
              ststflg(mynum, SFL_NO_GOSSIP) ? " [NoGossip]" : "",
              ststflg(mynum, SFL_BUSY) ? " [Busy]" : "",
              ststflg(mynum, SFL_AWAY) ? " [Away]" : "",
              ststflg(mynum, SFL_CODING) ? " [Coding]" : "",
              ststflg(mynum, SFL_NO_WIZ) ? " [NoWiz]" : "",
              ststflg(mynum, SFL_POSTING) ? " [Posting]" : "",
              notHere ? " [Inactive]" : "",
              cur_player->inmailer ? " [Mail]" : "");
      notHere = False;
    }
    else
    {
      idle = time(0) - cur_player->last_command;
#ifdef RS6000
      tm = (struct tm *)gmtime((const time_t *)&idle);
#else
      tm = (struct tm *)gmtime(&idle);
#endif

      strftime(buff, 10, "%H:%M:%S", tm);
      sprintf(line,
              "&+g[%5d]&* &+R%s%-12.12s&* &+B%-16.16s&* &+Y%-8.8s&* &+M%-10.10s&* %-21.21s\n",
              plev(mynum),
              "",
              pname(mynum),
              show_hostname ? cur_player->hostname : " ",
              buff,
              show_location ? showname(ploc(mynum)) : " ",
              show_location ? sdesc(ploc(mynum)) : " ");
    }

    setup_globals(old_mynum);
    if (((pvis(t) <= plev(mynum)) || t == mynum) && strlen(pname(t)))
    {
      usercount++;
      bprintf(line);
    }
  }
  bprintf("\n&+RA total of %d visible users.&*\n", usercount);
  return;
}

void setpscore(int pl, int newsc)
{
  if (pl >= max_players) /* Otherwise the mud will crash on kill */
  {
    pscore(pl) = newsc;
  }
  else
  {
    if (!in_group(pl))
    {
      pscore(pl) = newsc;
    }
    else
    {
      int oldsc = pscore(pl);
      int wonsc = newsc - oldsc;
      int plx, share = 1;

      if (wonsc > 0)
      {
        wonsc *= 1.5; /* Give some bonus for teamwork *
/
share = wonsc / p_group_shares(pl);
if (share == 0)                    /* Nah, must be a calc error */
        share = 1;
        /* Split points over group */
        for (plx = 0; plx < max_players; plx++)
        {
          if (is_in_game(plx) && p_group(plx) == p_group(pl))
            pscore(plx) += (share * p_share(pl));
        }
        p_group_xp(pl) += p_group_shares(pl) * share; /* add the xp to coun
t */
      }
      else
        pscore(pl) = newsc;
    }
  }
}
