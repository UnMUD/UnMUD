#include "kernel.h"
#include "stdinc.h"

static void change_visibility(void);
static void change_speed(void);
static void change_sex(void);
static void change_level(void);
static void change_score(void);
static void change_strength(void);
static void change_damage(void);
static void change_armor(void);
static void change_aggression(void);
static void change_passwd(void);
static void change_name(void);
void change_wimpy(void);
static void change_email(void);

static char *ChTable[] =
    {
        "sex", "speed", "score", "strength", "level",
        "visibility", "damage", "pflags", "mask", "lflags",
        "mflags", "aggression", "title", "password", "name",
        "description", "armor", "wimpy", "email", TABLE_END};

void (*ChFunTable[])(void) =
    {
        change_sex, change_speed, change_score, change_strength, change_level,
        change_visibility, change_damage, pflagscom, maskcom, lflagscom,
        mflagscom, change_aggression, change_title, change_passwd, change_name,
        change_desc, change_armor, change_wimpy, change_email};

#define SEX 0
#define SPEED 1
#define SCORE 2
#define STRENGTH 3
#define LEVEL 4
#define VIS 5
#define DAMAGE 6
#define PFL 7
#define MASK 8
#define LFL 9
#define MFL 10
#define AGG 11
#define TITLE 12
#define PASSWD 13
#define NAME 14
#define DESC 15
#define ARMOR 16
#define EMAIL 17

static void ask_old_passwd(char *pass);
static void ask_new_passwd(char *pass);
static void ask_confirm_passwd(char *pass);

void changecom(void)
{
  int x;

  if (brkword() == -1)
  {
    bprintf("Change what?\n");
    return;
  }

  if (cur_player->aliased || cur_player->polymorphed >= 0)
  {
    bprintf("Not while aliased.\n");
    return;
  }

  if ((x = tlookup(wordbuf, ChTable)) >= 0)
  {
    ChFunTable[x]();
  }
  else
  {
    bprintf("I don't know how to change that.\n");
  }
}

/* Change sex on a player or mobile.
 */
static void
change_sex(void)
{
  int a, w;
  PERSONA p;
  Boolean f, is_me;

  if (!ptstflg(mynum, PFL_UAF) || plev(mynum) < LVL_APPREN)
  {
    if ((a = vicf2(SPELL_VIOLENT, 7)) < 0)
      return;
    player2pers(&p, NULL, a);
    f = False;
  }
  else if (brkword() == -1 || (a = find_player(wordbuf, &p, &f)) == -1)
  {
    bprintf("Who?\n");
    return;
  }

  is_me = !f && a == mynum;

  if ((w = wlevel(plev(mynum)) - wlevel(p.p_level)) < 0 || (w == 0 && plev(mynum) >= LVL_APPREN && plev(mynum) < LVL_GOD && !is_me))
  {
    bprintf("You can't do that to %s.\n", p.p_name);
    return;
  }

  if (f)
  {
    p.p_sflags ^= (1 << SFL_FEMALE);

    putuaf(&p);
  }
  else
  {
    setpsex(a, !psex(a));

    sendf(a, "Your sex has been magically changed!\nYou are now %s.\n",
          psex(a) ? "female" : "male");
    /* SHOULD HAVE A CALIBME(A) HERE */
  }
}

/* Change the speed of mobiles.
 */
static void
change_speed(void)
{
  int a, new_speed;

  if (plev(mynum) < LVL_APPREN)
  {
    bprintf("I don't know how to change that.\n");
    return;
  }

  if (brkword() == -1)
  {
    bprintf("For whom ?\n");
    return;
  }

  if ((a = fpbn(wordbuf)) < 0 || a < max_players)
  {
    bprintf("There is no mobile by that name.\n");
    return;
  }

  if (brkword() == -1 || !isdigit(*wordbuf))
  {
    bprintf("Set speed to what ?");
    return;
  }

  if ((new_speed = atoi(wordbuf)) > 100)
  {
    bprintf("The highest value is 100.\n");
    new_speed = 100;
  }
  bprintf("Setting speed for %s to %d.\n", pname(a), new_speed);
  setpspeed(a, new_speed);
}

static void
change_score(void)
{
  Boolean f, is_me, is_mobile;
  int a;
  int new_score;
  PERSONA p;

  if (brkword() == -1 || (a = find_player(wordbuf, &p, &f)) == -1)
  {
    bprintf("Who?\n");
    return;
  }

  is_me = !f && a == mynum;
  is_mobile = !f && a >= max_players;

  if (plev(mynum) < LVL_APPREN || !ptstflg(mynum, PFL_CH_SCORE))
  {
    erreval();
    return;
  }

  if (!is_mobile && !is_me && !ptstflg(mynum, PFL_FROB))
  {
    bprintf("You can only change your own score.\n");
    return;
  }

  if (wlevel(plev(mynum)) <= wlevel(p.p_level) && !is_me)
  {
    bprintf("That is beyond your powers.\n");
    return;
  }

  new_score = (brkword() < 0) ? p.p_score : atoi(wordbuf);

  if (!is_me && !is_mobile)
  {
    mudlog("%s changed score on %s from %d to %d",
           pname(mynum), p.p_name, p.p_score, new_score);
  }

  bprintf("Setting score for %s to %d\n", p.p_name, new_score);

  if (f)
  {
    p.p_score = new_score;
    putuaf(&p);
  }
  else
  {
    setpscore(a, new_score);
  }
}

static void
change_strength(void)
{
  Boolean f, is_mobile, is_me;
  int a, new_str;
  PERSONA p;

  if (brkword() == -1 || (a = find_player(wordbuf, &p, &f)) == -1)
  {
    bprintf("Who?\n");
    return;
  }

  is_me = !f && a == mynum;
  is_mobile = !f && a >= max_players;

  if (((is_mobile && !ptstflg(mynum, PFL_CH_MDATA)) || (!is_mobile && !ptstflg(mynum, PFL_HEAL))))
  {
    erreval();
    return;
  }

  if (wlevel(plev(mynum)) <= wlevel(p.p_level) && !is_me)
  {
    bprintf("That is beyond your powers.\n");
    return;
  }

  new_str = (brkword() < 0) ? p.p_strength : max(0, atoi(wordbuf));

  if (p.p_strength > new_str && !is_mobile && !is_me)
  {
    if (!ptstflg(mynum, PFL_FROB))
    {
      bprintf("How rude of you! Trying to make other players weaker!\n");
      return;
    }
    else
    {
      mudlog("%s lowered %s's strength from %d to %d",
             pname(mynum), p.p_name, p.p_strength, new_str);
    }
  }

  bprintf("Setting strength of %s to %d\n", p.p_name, new_str);

  if (f)
  {
    p.p_strength = new_str;
    putuaf(&p);
  }
  else
    setpstr(a, new_str);
}

static void
change_level(void)
{
  Boolean f, is_mobile, is_me;
  int a, new_lvl;
  PERSONA p;

  if (plev(mynum) < LVL_APPREN || !ptstflg(mynum, PFL_CH_LEVEL))
  {
    bprintf("I don't know how to change that.\n");
    return;
  }

  if (brkword() == -1 || (a = find_player(wordbuf, &p, &f)) == -1)
  {
    bprintf("Who is that?\n");
    return;
  }

  is_me = !f && a == mynum;
  is_mobile = !f && a >= max_players;

  if (is_mobile)
  {
    bprintf("Changing level on mobiles doesn't make sense now!\n");
    return;
  }

  if (((!is_me && (!ptstflg(mynum, PFL_FROB))) || ((wlevel(p.p_level) >= wlevel(plev(mynum)) && plev(mynum) < LVL_GOD))))
  {
    bprintf("You can't change level on %s.\n", p.p_name);
    return;
  }

  if (brkword() < 0)
  {
    bprintf("What level?\n");
    return;
  }

  if ((new_lvl = atoi(wordbuf)) >= LVL_MAX || new_lvl < LVL_MIN)
  {
    bprintf("Level must be between %d and %d.\n", LVL_MIN, LVL_MAX - 1);
    return;
  }

  if ((plev(mynum) >= LVL_GOD) || (wlevel(new_lvl) < wlevel(plev(mynum))) || (is_me && wlevel(new_lvl) == wlevel(plev(mynum))))
  {

    if (wlevel(p.p_level) != wlevel(new_lvl))
    {

      update_wizlist(p.p_name, wlevel(new_lvl));

      set_xpflags(new_lvl,
                  f ? &p.p_pflags : &pflags(a),
                  f ? &p.p_mask : &pmask(a));
    }

    if (f)
    {
      p.p_level = new_lvl;
      putuaf(&p);
    }
    else
    {
      setplev(a, new_lvl);
    }
  }
  else
    bprintf("You can't change %s to level %d!\n", p.p_name, new_lvl);
}

static void
change_visibility(void)
{
  int a, new_vis, max_vis;
  Boolean f, is_me;
  PERSONA p;

  if (plev(mynum) < LVL_APPREN)
  {
    bprintf("I don't know how to change that.\n");
    return;
  }

  if (brkword() == -1 || (a = find_player(wordbuf, &p, &f)) == -1)
  {
    bprintf("Who is that?\n");
    return;
  }

  is_me = !f && a == mynum;

  if (!is_me && wlevel(p.p_level) >= wlevel(plev(mynum)) && plev(mynum) < LVL_GOD)
  {

    bprintf("You can't change visibility on %s!\n", p.p_name);
    return;
  }

  new_vis = (brkword() == -1) ? 0 : atoi(wordbuf);
  max_vis = LVL_GOD;

  if (is_me)
    switch (wlevel(plev(mynum)))
    {
    case LEV_MASTER:
      max_vis = LVL_GOD;
      break;
    case LEV_GOD:
      max_vis = LVL_GOD;
      break;
    case LEV_HIGHDEMI:
      max_vis = LVL_GOD;
      break;
    case LEV_DEMI:
      max_vis = LVL_HIGHDEMI;
      break;
    case LEV_HIGHARCH:
      max_vis = LVL_DEMI;
      break;
    case LEV_ARCHWIZARD:
      max_vis = LVL_HIGHARCH;
      break;
    case LEV_SENIOR:
      max_vis = LVL_ARCHWIZARD;
      break;
    case LEV_WIZARD:
      max_vis = LVL_SENIOR;
      break;
    case LEV_EMERITUS:
      max_vis = LVL_WIZARD;
      break;
    case LEV_APPRENTICE:
      max_vis = LVL_EMERITUS;
      break;
    case LEV_MORTAL:
    case LEV_NEG:
      max_vis = LVL_WIZARD;
      break;
    }
  if (new_vis > max_vis)
  {
    bprintf("The maximum is %d.\n", max_vis);
    new_vis = max_vis;
  }

  bprintf("Setting visibility for %s to %d.\n", p.p_name, new_vis);

  if (f)
  {
    p.p_vlevel = new_vis;
    putuaf(&p);
  }
  else
  {
    setpvis(a, new_vis);
  }
}

static void
change_damage(void)
{
  Boolean f, is_me, is_mobile;
  int a, new_damage;
  PERSONA p;

  if (plev(mynum) < LVL_APPREN)
  {
    bprintf("I don't know how to change that.\n");
    return;
  }

  if (brkword() == -1 || (a = find_player(wordbuf, &p, &f)) == -1)
  {
    bprintf("Who?\n");
    return;
  }

  is_me = (!f && (a == mynum));
  is_mobile = (!f && (a >= max_players));

  if (is_mobile && !ptstflg(mynum, PFL_CH_MDATA))
  {
    bprintf("That is beyond your powers.\n");
    return;
  }
  if (!is_mobile && !is_me)
    if ((plev(mynum) < LVL_ARCHWIZARD) ||
        (wlevel(plev(mynum)) <= wlevel(p.p_level)))
    {
      bprintf("That is beyond your powers.\n");
      return;
    }
  if (brkword() == -1)
  {
    bprintf("Change damage on %s to what ?\n", p.p_name);
    return;
  }

  new_damage = max(0, atoi(wordbuf));

  if (!is_me)
  {
    mudlog("%s changed damage on %s from %d to %d",
           pname(mynum), p.p_name, p.p_damage, new_damage);
  }

  bprintf("Setting damage on %s %s to %d\n",
          p.p_name, is_mobile ? " " : "permanently", new_damage);

  if (f)
  {
    p.p_damage = new_damage;
    putuaf(&p);
  }
  else
    setpdam(a, new_damage);
}

static void
change_armor(void)
{
  Boolean f, is_me, is_mobile;
  int a, new_armor;
  PERSONA p;

  if (plev(mynum) < LVL_APPREN)
  {
    bprintf("I don't know how to change that.\n");
    return;
  }

  if (brkword() == -1 || (a = find_player(wordbuf, &p, &f)) == -1)
  {
    bprintf("Who?\n");
    return;
  }

  is_me = !f && a == mynum;
  is_mobile = !f && a >= max_players;

  if (is_mobile && !ptstflg(mynum, PFL_CH_MDATA))
  {
    bprintf("That is beyond your powers.\n");
    return;
  }
  if (!is_mobile && !is_me)
    if ((plev(mynum) < LVL_ARCHWIZARD) ||
        (wlevel(plev(mynum)) <= wlevel(p.p_level)))
    {
      bprintf("That is beyond your powers.\n");
      return;
    }
  if (brkword() == -1)
  {
    bprintf("Change armor on %s to what ?\n", p.p_name);
    return;
  }

  new_armor = max(0, atoi(wordbuf));

  if (!is_me)
  {
    mudlog("%s changed armor on %s from %d to %d",
           pname(mynum), p.p_name, p.p_armor, new_armor);
  }

  bprintf("Setting armor on %s%sto %d\n",
          p.p_name, is_mobile ? " " : " permanently ", new_armor);

  if (f)
  {
    p.p_armor = new_armor;
    putuaf(&p);
  }
  else
    setparmor(a, new_armor);
}

static void
change_aggression(void)
{
  int a, new_agg;

  if (plev(mynum) < LVL_APPREN || !ptstflg(mynum, PFL_CH_MDATA))
  {
    bprintf("I don't know how to change that.\n");
    return;
  }

  if (brkword() == -1)
  {
    bprintf("For whom ?\n");
    return;
  }

  if ((a = fpbn(wordbuf)) < 0 || a < max_players)
  {
    bprintf("There is no mobile by that name.\n");
    return;
  }

  if (brkword() == -1 || !isdigit(*wordbuf))
  {
    bprintf("Set aggression to what ?");
    return;
  }

  if ((new_agg = atoi(wordbuf)) > 100)
  {
    bprintf("The highest value is 100.\n");
    new_agg = 100;
  }

  setpagg(a, new_agg);

  mudlog("Change Agg: %s set %s to %d", pname(mynum), pname(a), new_agg);
}

/* Check that a title has a valid format. Currently this means that it must
 * contain one (and only one) occurence of the string %s, that that occurence
 * is not preceded by a % (%%s) and that it does not contain other
 * occurences of %.
 */
static Boolean
check_title(char *title, Boolean strict)
{
  char *p;

  return ((p = strchr(title, '%')) == strrchr(title, '%') && p != NULL && p[1] == 's') || ((p == NULL) && !strict);
}

static void change_email(void)
{
  char a[200];

  getreinput(a);
  if (strlen(a) == 0)
  {
    bprintf("Change your email address to what?\n");
    return;
  }
  strcpy(pemail(mynum), a);
}

void change_title(void)
{
  PERSONA p;
  int a, b;
  char buff[MAX_COM_LEN];
  Boolean f, is_me, is_mobile, disp_title = False;
  Boolean strict;

  if (!ptstflg(mynum, PFL_TITLES))
  {
    erreval();
    return;
  }

  if (brkword() == -1 || (a = find_player(wordbuf, &p, &f)) == -1)
  {
    bprintf("Who?\n");
    return;
  }

  if (plev(mynum) < LVL_APPREN)
    strict = True;
  else
    strict = False;

  is_me = !f && a == mynum;
  is_mobile = !f && a >= max_players;

  getreinput(buff);
  /* strcpy (buff, rawbuf); */

  if (EMPTY(buff))
  {
    disp_title = True;
  }
  else if (is_mobile)
  {
    int lev;
    char *owner = powner(a);

    if (ppermanent(a))
    {
      bprintf("That's a permanent mobile.\n");
      return;
    }

    lev = (b = fpbns(owner)) >= 0 ? plev(b) : getuaf(owner, &p) ? p.p_level
                                                                : -1;

    if (!EQ(pname(mynum), owner) && !do_okay_l(plev(mynum), lev, False))
    {

      bprintf("You can't change title on %s's mobiles.\n", owner);
      return;
    }

    if (strchr(buff, '^') != NULL)
    {
      bprintf("Illegal character(s): '^' in title.\n");
      return;
    }
  }
  else if (!is_me && !do_okay_l(plev(mynum), p.p_level,
                                wlevel(p.p_level) < wlevel(plev(mynum))))
  {
    bprintf("That is beyond your powers.\n");
    return;
  }
  else if (strlen(buff) > TITLE_LEN)
  {
    bprintf("Maximum title-length is %d characters.\n", TITLE_LEN);
    return;
  }

  else if (!check_title(buff, strict))
  {
    bprintf("Invalid format. See HELP TITLE\n");
    return;
  }

  if (!f)
  {
    if (disp_title)
      bprintf("Title: %s\n",
              is_mobile ? pftxt(a) : make_title(ptitle(a), "<name>"));
    else if (is_mobile)
    {
      if (pftxt(a) != NULL)
        free(pftxt(a));

      pftxt(a) = COPY(buff);
    }
    else
    {
      setptitle(a, buff);
    }
  }
  else
  {
    if (disp_title)
      bprintf("Title: %s\n", make_title(p.p_title, "<name>"));
    else
    {
      strcpy(p.p_title, buff);
      putuaf(&p);
    }
  }

  if (!disp_title)
    bprintf("Title changed on %s.\n", is_mobile ? pname(a) : p.p_name);
}

static void
change_name(void)
{
  PERSONA p, q;
  char old_name[MNAME_LEN + 1];
  char buff[256];
  char buff2[256];
  char *r;
  int num;
  int max_len;
  Boolean in_uaf;
  Boolean is_mobile;
  Boolean is_me;

  if (plev(mynum) < LVL_APPREN)
  {
    erreval();
    return;
  }

  if (brkword() == -1)
  {
    bprintf("Change name on who ?\n");
    return;
  }

  if ((num = find_player(wordbuf, &p, &in_uaf)) == -1)
  {
    bprintf("No such player or mobile: %s.\n", wordbuf);
    return;
  }

  strcpy(old_name, p.p_name);

  is_mobile = num >= max_players;
  is_me = num == mynum;
  max_len = is_mobile ? MNAME_LEN : PNAME_LEN;

  if (EMPTY(getreinput(wordbuf)))
  {
    bprintf("Change name on %s to what ?\n", old_name);
    return;
  }

  if (strlen(wordbuf) > max_len)
  {
    bprintf("Name too long, max = %d.\n", max_len);
    return;
  }

  for (r = wordbuf; isalpha(*r) || (is_mobile && *r == ' '); r++)
    ;

  if (*r != '\0')
  {
    bprintf("Name contains illegal character: %c\n", *r);
    return;
  }

  if (isalpha(*wordbuf))
    *wordbuf = toupper(*wordbuf);

  if (is_mobile)
  {

    Boolean denied;
    char *owner = powner(num);

    if (ppermanent(num))
    {
      bprintf("That's a permanent mobile.\n");
      return;
    }

    if ((denied = !EQ(pname(mynum), owner)))
    {
      int x, lev;

      lev = (x = fpbns(owner)) >= 0 ? plev(x) : getuaf(owner, &p) ? p.p_level
                                                                  : 0;

      if (do_okay_l(plev(mynum), lev, False))
      {
        denied = False;
      }
    }

    if (denied)
    {
      bprintf("You're not powerful enough to change "
              "%s's mobiles.\n",
              owner);
      return;
    }

    setpname(num, wordbuf);

    free(pname_reset(num));
    pname_reset(num) = COPY(wordbuf);
  }
  else
  {
    char *new_name = wordbuf;

    if (plev(mynum) < LVL_DEMI)
    {
      bprintf("You can only change name on mobiles.\n");
      return;
    }

    if (getuaf(new_name, &q))
    {
      bprintf("%s already exists.\n", new_name);
      return;
    }

    deluaf(old_name);
    strcpy(p.p_name, new_name);
    putuaf(&p);

    if (!in_uaf)
    {
      setpname(num, new_name);
    }

    if ((num = get_zone_by_name(old_name)) >= num_const_zon)
    {
      free(zname(num));
      zname(num) = COPY(new_name);
    }

    wiz_loc_filename(buff, old_name);
    wiz_loc_filename(buff2, new_name);
    rename(buff, buff2);

    wiz_mob_filename(buff, old_name);
    wiz_mob_filename(buff2, new_name);
    rename(buff, buff2);

    wiz_obj_filename(buff, old_name);
    wiz_obj_filename(buff2, new_name);
    rename(buff, buff2);

    update_wizlist(old_name, LEV_MORTAL);
    update_wizlist(new_name, wlevel(p.p_level));

    bprintf("Ok.\n");
    mudlog("%s changed name on %s to %s",
           pname(mynum), old_name, new_name);
  }
}

static void room_desc_handler(void *w, void *ad, int adlen);

static void
change_room_desc(int loc, char *room_name)
{
  long int loc_ident = loc_id(loc);
  char *owner = lowner(loc);
  Boolean denied;

  if (lpermanent(loc))
  {
    bprintf("That's a permanent location.\n");
    return;
  }

  if ((denied = !EQ(pname(mynum), owner)))
  {
    int x, lev;
    PERSONA p;

    lev = (x = fpbns(owner)) >= 0 ? plev(x) : getuaf(owner, &p) ? p.p_level
                                                                : 0;

    if (do_okay_l(plev(mynum), lev, False))
    {
      denied = False;
    }
  }

  if (denied)
  {
    bprintf("You're not powerful enough to change %s's rooms.",
            owner);
    return;
  }

  start_writer("End your input with ** on the beginning of a new line.\n"
               "The room description cannot contain the ^ character.",

               "DESC>",
               &loc_ident,
               sizeof(loc_ident),
               room_desc_handler,
               WR_CMD | '*',
               50);
}

void room_desc_handler(void *w, void *ad, int adlen)
{
  int loc = find_loc_by_id(*(long int *)ad);

  char *s;
  char *t;
  int k;
  int error = 0;
  int num_bytes;

  if (loc == 0)
  {
    bprintf("Sorry, but the room doesn't exist any more!\n");
    terminate_writer(w);
    return;
  }

  num_bytes = wnum_lines(w) + wnum_chars(w);

  t = s = NEW(char, num_bytes + 1);

  while ((k = wgetc(w)) != EOF)
  {
    if (k != '^')
      *t++ = k;
    else
      error++;
  }
  *t = 0;

  if (error)
  {
    bprintf("Warning: The description cannot contain '^'.\n");
    bprintf("The %d ^'s was ignored.\n", error);
  }

  if (llong(loc) != NULL)
    free(llong(loc));
  llong(loc) = s;
  bprintf("Room description changed.\n");
}

static void mob_desc_handler(void *w, void *ad, int adlen);

static void
change_mob_desc(int mob, char *mob_name)
{
  long int mob_ident = mob_id(mob);
  char *owner = powner(mob);
  Boolean denied;

  if (ppermanent(mob))
  {
    bprintf("That's a permanent mobile.\n");
    return;
  }

  if ((denied = !EQ(pname(mynum), owner)))
  {
    int x, lev;
    PERSONA p;

    lev = (x = fpbns(owner)) >= 0 ? plev(x) : getuaf(owner, &p) ? p.p_level
                                                                : 0;

    if (do_okay_l(plev(mynum), lev, False))
    {
      denied = False;
    }
  }

  if (denied)
  {
    bprintf("You're not powerful enough to change %s's mobiles.",
            owner);
    return;
  }

  start_writer("End your input with ** on the beginning of a new line.\n"
               "The mobile description cannot contain the ^ character.",

               "DESC>",
               &mob_ident,
               sizeof(mob_ident),
               mob_desc_handler,
               WR_CMD | '*',
               50);
}

void mob_desc_handler(void *w, void *ad, int adlen)
{
  int mob = find_mobile_by_id(*(long int *)ad);

  char *s;
  char *t;
  int k;
  int error = 0;
  int num_bytes;

  if (mob < 0)
  {
    bprintf("Sorry, but the mobile doesn't exist any more!\n");
    terminate_writer(w);
    return;
  }

  num_bytes = wnum_lines(w) + wnum_chars(w);

  t = s = NEW(char, num_bytes + 1);

  while ((k = wgetc(w)) != EOF)
  {
    if (k != '^')
      *t++ = k;
    else
      error++;
  }
  *t = 0;

  if (error)
  {
    bprintf("Warning: The description cannot contain '^'.\n");
    bprintf("The %d ^'s was ignored.\n", error);
  }

  if (pexam(mob) != NULL)
    free(pexam(mob));
  pexam(mob) = s;
  bprintf("Description on %s changed.\n", pname(mob));
}

static void obj_desc_handler(void *w, void *ad, int adlen);

void change_obj_desc(int obj, char *obj_name)
{
  long int obj_ident = obj_id(obj);
  char *owner = oowner(obj);
  Boolean denied;

  if (opermanent(obj))
  {
    bprintf("That's a permanent object.\n");
    return;
  }

  if ((denied = !EQ(pname(mynum), owner)))
  {
    int x, lev;
    PERSONA p;

    lev = (x = fpbns(owner)) >= 0 ? plev(x) : getuaf(owner, &p) ? p.p_level
                                                                : 0;

    if (do_okay_l(plev(mynum), lev, False))
    {
      denied = False;
    }
  }

  if (denied)
  {
    bprintf("You're not powerful enough to change %s's objects.",
            owner);
    return;
  }

  start_writer("End your input with ** on the beginning of a new line.\n"
               "The object description cannot contain the ^ character.",

               "DESC>",
               &obj_ident,
               sizeof(obj_ident),
               obj_desc_handler,
               WR_CMD | '*',
               50);
}

void obj_desc_handler(void *w, void *ad, int adlen)
{
  int obj = find_object_by_id(*(long int *)ad);

  char *s;
  char *t;
  int k;
  int error = 0;
  int num_bytes;

  if (obj < 0)
  {
    bprintf("Sorry, but the object doesn't exist any more!\n");
    terminate_writer(w);
    return;
  }

  num_bytes = wnum_lines(w) + wnum_chars(w);

  t = s = NEW(char, num_bytes + 1);

  while ((k = wgetc(w)) != EOF)
  {
    if (k != '^')
      *t++ = k;
    else
      error++;
  }
  *t = 0;

  if (error)
  {
    bprintf("Warning: The description cannot contain '^'.\n");
    bprintf("The %d ^'s was ignored.\n", error);
  }

  if (oexam_text(obj) != NULL)
    free(oexam_text(obj));
  oexam_text(obj) = s;
  bprintf("Description on %s changed.\n", oname(obj));
}

static void player_desc_handler(void *w, void *ad, int adlen);

static void
change_player_desc(PERSONA *p)
{
  char buff[128];

  if (!EQ(pname(mynum), p->p_name) &&
      !do_okay_l(plev(mynum), p->p_level, False))
  {

    bprintf("You're not powerful enough to change "
            "%s's description.\n",
            p->p_name);
    return;
  }

  sprintf(buff, DESC_DIR "/%s", p->p_name);

  start_writer(
      "End the player description with ** on the beginning of a new line",
      "DESC>",
      buff,
      strlen(buff) + 1,
      player_desc_handler,
      WR_CMD | '*',
      200);
}

static void
player_desc_handler(void *w, void *ad, int adlen)
{
  FILE *f;
  char b[100];

  if ((f = fopen((char *)ad, "w")) == NULL)
  {

    sprintf(b, "desc_handler/%s", (char *)ad);
    progerror(b);
    terminate_writer(w);
    return;
  }
  else
  {

    while (wgets(b, sizeof(b), w) != NULL)
    {
      fputs(b, f);
    }
    fclose(f);
  }
}

void change_desc(void)
{
#define DESC_LOC 0
#define DESC_MOB 1
#define DESC_OBJ 2
#define DESC_PLR 3

  char *desc_type[] =
      {"Location", "Mobile", "Object", "Player", TABLE_END};

  PERSONA p;
  int loc;

  if (brkword() == -1)
  {
    if (cur_player->aliased || cur_player->polymorphed != -1)
    {
      bprintf("Not while aliased.\n");
      return;
    }

    player2pers(&p, NULL, mynum);
    change_player_desc(&p);
  }
  else
  {
    int type = tlookup(wordbuf, desc_type);

    if (brkword() == -1 || type == -1)
    {
      bprintf("Change description on what?\n");
      return;
    }

    if (type == DESC_LOC &&
        (loc = find_loc_by_name(wordbuf)) < 0)
    {

      change_room_desc(loc, wordbuf);
    }
    else if (type == DESC_MOB && (loc = fpbn(wordbuf)) >= max_players)
    {

      change_mob_desc(loc, pname(loc));
    }
    else if (type == DESC_OBJ && (loc = fobn(wordbuf)) >= 0)
    {

      change_obj_desc(loc, oname(loc));
    }
    else if (type == DESC_PLR && getuaf(wordbuf, &p))
    {

      change_player_desc(&p);
    }
    else
    {
      bprintf("Can't find %s: %s\n",
              desc_type[type], wordbuf);
    }
  }
}

static void
change_passwd(void)
{

  PERSONA *p;
  char b[40];

  if (brkword() != -1 && !EQ(wordbuf, pname(mynum)))
  {

    if (plev(mynum) < LVL_DEMI)
    {
      bprintf("You can only change your own password.\n");
      return;
    }

    p = NEW(PERSONA, 1);
    if (!getuaf(wordbuf, p))
    {
      bprintf("I cannot find anyone called \"%s\". ", wordbuf);
      free(p);
      return;
    }
    strcpy(cur_player->cprompt, "New Password: ");
    cur_player->work = (int)p;
    cur_player->no_echo = True;
    sprintf(b, "\n\377\373\001\001New Password for %s: ", wordbuf);
    bprintf(b);
    replace_input_handler(ask_new_passwd);
  }
  else
  {
    strcpy(cur_player->cprompt, "Old Password: ");
    bprintf("\n\377\373\001\001Old Password: ");
    replace_input_handler(ask_old_passwd);
  }
}

static void
ask_old_passwd(char *pass)
{
  PERSONA *p;
  char b[sizeof(cur_player->passwd)];

  my_crypt(b, pass, sizeof(b));
  if (strcmp(b, cur_player->passwd) != 0)
  {
    cur_player->no_echo = False;
    bprintf("\n\377\374\001\001Wrong password!\n");
    mudlog("CHANGE PASSWD: Wrong old password for %s", pname(mynum));
    get_command(NULL);
  }
  else
  {
    strcpy(cur_player->cprompt, "New Password: ");
    cur_player->work = (int)(p = NEW(PERSONA, 1));
    strcpy(p->p_name, pname(mynum));
    bprintf("\nNew password: ");
    replace_input_handler(ask_new_passwd);
  }
}

static void
ask_new_passwd(char *pass)
{
  PERSONA *p = (PERSONA *)(cur_player->work);

  strcpy(p->p_passwd, pass);
  strcpy(cur_player->cprompt, "Confirm Password: ");
  bprintf("\nPlease confirm the new password: ");
  replace_input_handler(ask_confirm_passwd);
}

static void
ask_confirm_passwd(char *pass)
{
  PERSONA *p = (PERSONA *)(cur_player->work);
  PERSONA q;
  int plx;

  bprintf("\377\374\001\001");
  cur_player->no_echo = False;
  if (strcmp(p->p_passwd, pass) != 0)
  {
    bprintf("\nNot same new password, password not changed.\n");
  }
  else if (!getuaf(p->p_name, &q))
  {
    bprintf("\nCouldn't find entry in UAF file.\n");
  }
  else
  { /* Change password... */

    if ((plx = fpbns(p->p_name)) >= 0 && plx < max_players)
    {
      /* Player is in game, modify his password in game, and save. */
      /* uaf record is invalid except for 'last_on' */

      player2pers(&q, &q.p_last_on, plx);
    }
    my_crypt(q.p_passwd, p->p_passwd, sizeof(q.p_passwd));
    putuaf(&q);
    if (plx >= 0 && plx < max_players)
    {
      strcpy(players[plx].passwd, q.p_passwd);
    }
    bprintf("\nPassword changed.\n");
    mudlog("%s changed password on %s.", pname(mynum), q.p_name);
  }
  free((PERSONA *)p);

  get_command(NULL);
}

void change_wimpy(void)
{
  Boolean f;
  int a, old_wimpy, new_wimpy;
  PERSONA p;

  if (brkword() == -1)
  {
    bprintf("Wimpy on who to what?\n");
    return;
  }

  if (isdigit(*wordbuf))
  {
    new_wimpy = atoi(wordbuf);
    old_wimpy = pwimpy(a = mynum);
    f = False;
  }
  else if ((a = find_player(wordbuf, &p, &f)) == -1)
  {
    bprintf("On who ?\n");
    return;
  }
  else
  {
    if (brkword() == -1)
    {
      if (plev(mynum) < LVL_APPREN)
      {
        bprintf("You can only see your own wimpy-level\n");
        return;
      }
      else
      {
        bprintf("Wimpy-level for %s is %d.\n",
                p.p_name, p.p_wimpy);
        return;
      }
    }
    else
    {
      old_wimpy = p.p_wimpy;
      new_wimpy = atoi(wordbuf);
    }
  }

  if (f)
  {
    p.p_wimpy = new_wimpy;
    putuaf(&p);
  }
  else
  {
    setpwimpy(a, new_wimpy);
  }

  if ((f || a != mynum) && (a < max_players) && (new_wimpy < old_wimpy))
  {
    mudlog("%s lowered wimpy on %s from %d to %d",
           pname(mynum), p.p_name, old_wimpy, new_wimpy);
  }

  bprintf("Setting Wimpy-level on %s to %d.\n", pname(mynum), new_wimpy);
}
