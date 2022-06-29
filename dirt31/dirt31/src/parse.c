#include "kernel.h"
#include "condact.h"
#include "verbs.h"
#include "sflags.h"
#include "sendsys.h"
#include "locations.h"
#include "objects.h"
#include "parse.h"
#include "mobile.h"
#include "exec.h"
#include "move.h"
#include "commands.h"
#include "weather.h"
#include "wizard.h"
#include "change.h"
#include "clone.h"

#define NOISECHAR(c)	((c) == ' ' || (c) == '.' || (c) == ',' || (c) == '%')

static void parse_1(char *itemb,int *pl,int *ob,int *pre);
static char *markpos(void);

static char *preptable[] = { NULL, "at", "on", "with", "to", "in", "into",
			       "from", "out", TABLE_END};

static int xgamecom(char *str,Boolean savecom)
{
  int a;

  if (str == NULL || *str == 0) {
    return 0;
  } else if (EQ(str,"!")) {
    return gamecom(cur_player->prev_com,False);
  } else {
    if (savecom) (void)strcpy(cur_player->prev_com,str);

    /* Translate : and ; to "emote", and ' and " to "say".
     */
    a = -1;

    if (str[0] == '\"' || str[0] == '\'') {
      strcpy(strbuf,"say ");
      strcat(strbuf,str+1);
    } else if (str[0] == ':' || str[0] == ';') {
      strcpy(strbuf,"emote ");
      strcat(strbuf,str+1);
    } else {
      strcpy(strbuf,str);
    }

    stp = 0;

    if (brkword() == -1) {	/* if line contains nothing but  */
      erreval();		/* pn's or articles (a, an, the) */
      return -1;
    }
    if ((a = chkverb()) != -1) {
      doaction(a);
      return 0;
    }
    if (fextern(wordbuf) >= 0) {	/* handle external commands */
      stp = 0;
      return 0;
    }
    stp = 0;
    (void)brkword();
    if ((a = fpbn(wordbuf)) >= 0) { /* translate NAME to TELL NAME */
      stp = 0;
      a = VERB_TELL;
      doaction(a);
      return 0;
    }
    bprintf("Pardon?\n");
    return -1;
  }
}

int gamecom(char *str,Boolean savecom)
{
  int x;

  plast_cmd(real_mynum) = global_clock;
  x = xgamecom(str,savecom);
  exectable(status,0);
  special_events(mynum);
  return x;
}

char *getreinput(char *b)
{
  while (strbuf[stp] == ' ')
    stp++;
  return b != NULL ? strcpy(b, &strbuf[stp]) : &strbuf[stp];
}

int brkword(void)
{
  static int neword = 0;
  int worp;

  if (stp == 0)
    neword = 0;
  if (neword) {
    neword = 0;
    return 0;
  }
  for ( ; ; ) {
    while (NOISECHAR(strbuf[stp]))
      stp++;
    for (worp = 0; strbuf[stp] != 0 && !NOISECHAR(strbuf[stp]); )
      wordbuf[worp++] = strbuf[stp++];
    wordbuf[worp] = 0;
    lowercase(wordbuf);
    if (EQ(wordbuf, "the") || EQ(wordbuf, "a") || EQ(wordbuf, "an")
	|| EQ(wordbuf, "of") || EQ(wordbuf, "with"))
      continue;

    if (EQ(wordbuf, "it")) {
      if (cur_player->wd_it == NULL) {
	bprintf( "What exactly do you mean by \'it\'?\n");
	return -1;
      }
      strcpy(wordbuf, cur_player->wd_it);
    }

    if (EQ(wordbuf, "him")) {
/*      if (*cur_player->wd_him == '\0') {
	bprintf( "What exactly do you mean by \'him\'?\n");
	return -1;
      }
*/
      strcpy(wordbuf, cur_player->wd_him);
    }

    if (EQ(wordbuf, "her")) {
/*      if (*cur_player->wd_her == '\0') {
	bprintf( "What exactly do you mean by \'her\'?\n");
	return -1;
      }
*/
      strcpy(wordbuf, cur_player->wd_her);
    }

    if (EQ(wordbuf, "them")) {
/*      if (cur_player->wd_them == NULL) {
	bprintf( "What exactly do you mean by \'them\'?\n");
	return -1;
      }
*/
      strcpy(wordbuf, cur_player->wd_them);
    }

    if (EQ(wordbuf, "me")) {
      strcpy(wordbuf, pname(mynum));
    }

    break;
  }

  return (worp ? 0 : -1);
}



int chkverb()
{
  return chklist(wordbuf, verbtxt, verbnum);
}

int chklist(char *word, char *lista[], int listb[])
{
  int a, b, c;
  int d = -1;

  b = c = 0;
  for (a = 0; lista[a]; a++)
    if ((b = Match(word, lista[a])) > c) {
      c = b;
      d = listb[a];
    }
  return (c < 5 ? -1 : d);
}

int Match(char *x, char *y)
{
  if (strncasecmp(x, y, strlen(x)) == 0)
    return 10000;
  return 0;
}

void doaction(int vb)
{
  char xx[128];
  char  yy[20];

  if (do_tables(vb) == 2)
    return;
  if (vb > 1 && vb < 8) {
    if (cur_player->i_follow >= 0) {
      bprintf("You stopped following.\n");
      cur_player->i_follow = -1;
    }
    dodirn(vb);
    return;
  }
  if (cur_player->isforce) {

    switch (vb) {
    case VERB_QUIT:
    case VERB_BECOME:
    case VERB_BUG:
    case VERB_TYPO:
    case VERB_FORCE:
    case VERB_MAIL:
      bprintf("You can't be forced to do that.\n");
      return;
    }

  }
  switch (vb) {
  case VERB_GO:
    if (cur_player->i_follow >= 0) {
      bprintf("You stopped following %s.\n", pname(cur_player->i_follow));
      cur_player->i_follow = -1;
    }
    dogocom();
    break;
  case VERB_FLOWERS:
    flowercom();
    break;
  case VERB_DESCRIPTION:
    change_desc();
    break;
  case VERB_CLONE:
    clonecom(True);
    break;
  case VERB_DESTRUCT:
    destructcom(NULL);
    break;
  case VERB_LOAD:
    loadcom();
    break;
  case VERB_STORE:
    storecom();
    break;
  case VERB_DEBUG:
    debugcom();
    break;
  case VERB_TICKLE:
    ticklecom();
    break;
  case VERB_PET:
    petcom();
    break;
  case VERB_QUIT:
    quit_game();
    break;
  case VERB_SIT:
    sitcom();
    break;
  case VERB_STAND:
    standcom(mynum);
    break;
  case VERB_GET:
    getcom();
    break;
  case VERB_ASMORTAL:
    asmortalcom();
    break;
  case VERB_DROP:
    dropobj();
    break;
  case VERB_WHO:
    whocom();
    break;
  case VERB_MWHO:
    mwhocom();
    break;
  case VERB_RESET:
    resetcom(RES_TEST);
    break;
  case VERB_ZAP:
    zapcom();
    break;
  case VERB_EAT:
    eatcom();
    break;
  case VERB_SAVE:
    saveme();
    break;
  case VERB_SAVESET:
    saveset();
    break;
  case VERB_GLOBAL:
    globalcom();
    break;
  case VERB_STEAL:
    stealcom();
    break;
  case VERB_REVIEW:
    reviewcom();
    break;
  case VERB_LEVELS:
    levelscom();
    break;
  case VERB_COUPLES:
    bprintf("\001f%s\003", COUPLES);
    break;
  case VERB_WIZLIST:
    wizlistcom();
    break;
  case VERB_BULLETIN:
    bulletincom();
    break;
  case VERB_MAIL:
    mailcom();
    break;
  case VERB_PROMPT:
    promptcom();
    break;
#if 0
  case VERB_TIMEOUT: /* Not needed in dirt3 */
    timeoutcom();
    break;
#endif
  case VERB_INFO:
    infocom();
    break;
  case VERB_QUESTS:
    questcom();
    break;
  case VERB_TOURNAMENT:
    tournamentcom();
    break;
  case VERB_SYSLOG:
    syslogcom();
    break;
  case VERB_DELETE:
    deletecom();
    break;
  case VERB_OPENGAME:
    opengamecom();
    break;
  case VERB_HELP:
    helpcom();
    break;
  case VERB_STATS:
    showplayer();
    break;
  case VERB_EXAMINE:
    examcom();
    break;
  case VERB_EXORCISE:
    exorcom();
    break;
  case VERB_SUMMON:
    sumcom();
    break;
  case VERB_WIELD:
    wieldcom();
    break;
  case VERB_KILL:
    killcom();
    break;
  case VERB_POSE:
    posecom();
    break;
  case VERB_SET:
    setcom();
    break;
  case VERB_PRAY:
    praycom();
    break;
  case VERB_STORM:
    set_weather(stormy);
    break;
  case VERB_RAIN:
    set_weather(rainy);
    break;
  case VERB_SUN:
    set_weather(sunny);
    break;
  case VERB_SNOW:
    set_weather(snowing);
    break;
  case VERB_HAIL:
    set_weather(hailing);
    break;
  case VERB_TIPTOE:
    gotocom(True);
    break;
  case VERB_GOTO:
    gotocom(False);
    break;
  case VERB_WEAR:
    wearcom();
    break;
  case VERB_REMOVE:
    removecom();
    break;
  case VERB_PUT:
    putcom();
    break;
  case VERB_WAVE:
    wavecom();
    break;
  case VERB_BLIZZARD:
    set_weather(blizzard);
    break;
  case VERB_FORCE:
    forcecom();
    break;
  case VERB_LIGHT:
    lightcom();
    break;
  case VERB_EXTINGUISH:
    extinguishcom();
    break;
  case VERB_CRIPPLE:
    cripplecom();
    break;
  case VERB_CURE:
    curecom();
    break;
  case VERB_HEAL:
    healcom();
    break;
  case VERB_MUTE:
    dumbcom();
    break;
  case VERB_CHANGE:
    changecom();
    break;
  case VERB_MISSILE:
    spellcom(VERB_MISSILE);
    break;
  case VERB_SHOCK:
    spellcom(VERB_SHOCK);
    break;
  case VERB_FIREBALL:
    spellcom(VERB_FIREBALL);
    break;
  case VERB_FROST:
    spellcom(VERB_FROST);
    break;
  case VERB_BLOW:
    blowcom();
    break;
  case VERB_WIZ:
    wizcom();
    break;
  case VERB_NOWIZ:
    nowizcom();
    break;
  case VERB_GOD:
    godcom();
    break;
  case VERB_EXITS:
    exitcom();
    break;
  case VERB_OBJECTS:
    dircom();
    break;
  case VERB_PUSH:
    pushcom();
    break;
  case VERB_IN:
    incom(True);
    break;
  case VERB_AT:
    incom(False);
    break;
  case VERB_INVISIBLE:
    inviscom();
    break;
  case VERB_VISIBLE:
    viscom();
    break;
  case VERB_DEAFEN:
    deafcom();
    break;
  case VERB_RESURRECT:
    resurcom();
    break;
  case VERB_TITLE:
    change_title();
    break;
  case VERB_SETSTART:
    setstart();
    break;
  case VERB_LOCATIONS:
  case VERB_ZONES:
    zonescom();
    break;
  case VERB_USERS:
    usercom();
    break;
  case VERB_BECOME:
    becom(NULL);
    break;
  case VERB_SNOOP:
    snoopcom();
    break;
  case VERB_RAW:
    rawcom();
    break;
  case VERB_ROLL:
    rollcom();
    break;
  case VERB_CREDITS:
    bprintf("\001f%s\003", CREDITS);
    break;
  case VERB_BRIEF:
    bprintf("Brief mode ");
    if (ststflg(mynum, SFL_BRIEF)) {
      bprintf("off.\n");
      sclrflg(mynum, SFL_BRIEF);
    }
    else {
      bprintf("on.\n");
      ssetflg(mynum, SFL_BRIEF);
    }
    break;
  case VERB_JUMP:
    jumpcom();
    break;
  case VERB_WHERE:
    wherecom();
    break;
  case VERB_FLEE:
    fleecom();
    break;
  case VERB_BUG:
    bugcom();
    break;
  case VERB_TYPO:
    typocom();
    break;
  case VERB_ACTIONS:
    lisextern();
    break;
  case VERB_PN:
    pncom();
    break;
  case VERB_BLIND:
    blindcom();
    break;
  case VERB_MASK:
    maskcom();
    break;
  case VERB_PFLAGS:
    pflagscom();
    break;
  case VERB_MFLAGS:
    mflagscom();
    break;
  case VERB_FROB:
    frobcom(NULL);
    break;
  case VERB_SHUTDOWN:
    shutdowncom(False);
    break;
  case VERB_CRASH:
    shutdowncom(True);
    break;
  case VERB_EMOTE:
    emotecom();
    break;
  case VERB_EMOTETO:
    emotetocom();
    break;
  case VERB_EMPTY:
    emptycom();
    break;
  case VERB_TIME:
    timecom();
    break;
  case VERB_TREASURES:
    treasurecom();
    break;
  case VERB_WAR:
    warcom();
    break;
  case VERB_PEACE:
    peacecom();
    break;
  case VERB_QUIET:
    if (plev(mynum) < LVL_WIZARD) {
      erreval();
      break;
    }
    bprintf("Quiet mode ");
    if (ststflg(mynum, SFL_QUIET)) {
      bprintf("off.\n");
      sclrflg(mynum, SFL_QUIET);
    }
    else {
      bprintf("on.\n");
      ssetflg(mynum, SFL_QUIET);
    }
    break;
  case VERB_NOSHOUT:
    noshoutcom();
    break;
  case VERB_COLOR:
    if (ststflg(mynum, SFL_COLOR)) {
      bprintf("Back to black and white.\n");
      sclrflg(mynum, SFL_COLOR);
    }
    else {
      bprintf("Welcome to the wonderful world of \033[1;31mC\033[32mO\033[33mL\033[34mO\033[35mR\033[36m!\033[0m\n");
      ssetflg(mynum, SFL_COLOR);
    }
    break;
  case VERB_ALOOF:
    if (plev(mynum) < LVL_WIZARD) {
      erreval();
      break;
    }
    if (ststflg(mynum, SFL_ALOOF)) {
      bprintf("You feel less distant.\n");
      sclrflg(mynum, SFL_ALOOF);
    }
    else {
      bprintf("From now on you'll ignore some mortals' actions.\n");
      ssetflg(mynum, SFL_ALOOF);
    }
    break;
  case VERB_SHOW:
    showitem();
    break;
  case VERB_WIZLOCK:
    wizlock();
    break;
  case VERB_AWIZ:
    awizcom();
    break;
  case VERB_FOLLOW:
    followcom();
    break;
  case VERB_LOSE:
    losecom();
    break;
#if 0
  case VERB_CLS:
    cls();
    break;
#endif
  case VERB_ECHO:
    echocom();
    break;
  case VERB_ECHOALL:
    echoallcom();
    break;
  case VERB_ECHOTO:
    echotocom();
    break;
  case VERB_WISH:
    wishcom();
    break;
#if 0
  case VERB_TRACE:
    tracecom();
    break;
#endif
  case VERB_START:
    startcom();
    break;
  case VERB_STOP:
    stopcom();
    break;
  case VERB_MOBILES:
    mobilecom();
    break;
  case VERB_UNVEIL:
    unveilcom(NULL);
    break;
  case VERB_BANG:
    if (plev(mynum) < LVL_SORCERER)
      erreval();
    else
      broad("\001dA huge crash of thunder echoes through the land.\n\003");
    break;
  case VERB_TRANSLOCATE:
    sumcom();
    break;
  case VERB_ALIAS:
    aliascom();
    return;
  case VERB_LFLAGS:
    lflagscom();
    break;
  case VERB_PRIVS:
    pflagscom();
    break;
  case VERB_VERSION:
    bprintf(VERSION "\n");
    break;
  case VERB_MEDITATE:

#ifdef LOCMIN_ANCIENT
    if (ploc(mynum) == LOC_ANCIENT_ANC59) {
        if (oarmor(OBJ_ANCIENT_LBOOK) == 0) {
            bprintf("You meditate, but nothing seems to happen.\n");
        } else {
            bprintf("You are teleported!\n\n\n");
            trapch(LOC_ANCIENT_ANC35);
	}
        break;
    }
#endif
    bprintf("You meditate quietly in the corner.\n");
    break;

  default:
    mudlog( "No match for verb = %d", vb);
    bprintf("You can't do that now.\n");
    break;
  }
}


void quit_game(void)
{
    char xx[128];

    if (cur_player->aliased) {
      unalias(real_mynum);
      return;
    }

    if (pfighting(mynum) >= 0) {
      bprintf("Not in the middle of a fight!\n");
      return;
    }

    if (cur_player->polymorphed >= 0) {
      bprintf("A mysterious force won't let you quit.\n");
      return;
    }

    bprintf("Ok");

    send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
	     "%s\n", build_setin(xx, cur_player->setqout,pname(mynum), NULL));

    send_msg(DEST_ALL, MODE_QUIET|MODE_COLOR, max(pvis(mynum),LVL_WIZARD),
	     LVL_MAX, mynum, NOBODY, "[Quitting Game: %s]\n", pname(mynum));
    crapup("\t\t\tGoodbye", CRAP_SAVE);
}

void erreval(void)
{
  bprintf("You can't do that now.\n");
}

Boolean parse_2(int vb)
{
  int savestp = stp;
  int ob, pl, pre;

  *item1 = *item2 = '\0';

  if (vb == -1) return;
  ob1 = pl1 = ob2 = pl2 = -1;
  prep = 0;
  txt1 = markpos();
  txt2 = NULL;
  parse_1(item1,&pl,&ob,&pre);
#if 0
  bprintf( "vb = %d, item1 = %s, pl = %d, ob = %d, pre = %d\n",
	  vb, item1, pl, ob, pre);
#endif
  if (pre == 0) {
    ob1 = ob;
    pl1 = pl;
    txt2 = markpos();
    parse_1(item2,&pl,&ob,&pre);
  }
  if (pre != 0) {
    prep = pre;
    parse_1(item2,&pl,&ob,&pre);
  }
  if (pre != 0) {
    bprintf("Huh?\n");
    return False;
  } else {
    pl2 = pl;
    ob2 = ob;
  }
  stp = savestp;
  return True;
}

static void parse_1(char *itemb,int *pl,int *ob,int *pre)
{
  int o, p;

  *pl = *ob = -1;
  *pre = 0;
  if (brkword() != -1) {
    strcpy(itemb,wordbuf);
    if ((p = findprep(itemb)) != -1) {
      *pre = p;
    } else {
      /* It's not a preposition. */
      if ((p = *pl = fpbn(itemb)) != -1) {
	if (psex(p)) {
	  cur_player->wd_them = strcpy(cur_player->wd_her,pname(p));
	} else {
	  cur_player->wd_them = strcpy(cur_player->wd_him,pname(p));
	}
      }

      if ((o = *ob = fobnc(itemb)) != -1 || (o = *ob = fobna(itemb)) != -1) {
	cur_player->wd_it = oname(o);
      }
    }
  }
}

static char *markpos(void)
{
  register int c;

  while ((c = strbuf[stp]) == ',' || isspace(c)) ++stp;
  return strbuf + stp;
}

int findprep(char *t)
{
  return xlookup(t,preptable);
}

int prmmod(int p)
{
  switch (p) {
  case FL_PL1:   return pl1;
  case FL_PL2:   return pl2;
  case FL_OB1:   return ob1;
  case FL_OB2:   return ob2;
  case FL_CURCH: return ploc(mynum);
  case FL_PREP:  return prep;
  case FL_LEV:   return plev(mynum);
  case FL_STR:   return pstr(mynum);
  case FL_SEX:   return psex(mynum);
  case FL_SCORE: return pscore(mynum);
  case FL_MYNUM: return mynum;
  }
#if 0
  if (p >= 10000) return globaldata[p - 10000];
#endif

  if (IS_PL(p)) return p - PL_CODE + max_players;
  return p;
}

