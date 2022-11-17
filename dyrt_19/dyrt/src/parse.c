#include "kernel.h"
#include "verbs.h"
#include "locations.h"
#include "objects.h"
#include "stdinc.h"
#ifdef INTERMUD
#include "intermud.h"
#endif
#include "colors.h"

#define NOISECHAR(c) ((c) == ' ' || (c) == '.' || (c) == ',' || (c) == '%')

static void parse_1(char *itemb, int *pl, int *ob, int *pre);
static char *markpos(void);

static char *preptable[] =
    {NULL, "at", "on", "with", "to", "in", "into",
     "from", "out", TABLE_END};

int xgamecom(char *str, Boolean savecom)
{
  int a, workcount, strcount;
  char dumpchar;

  if (str == NULL || *str == 0)
  {
    return 0;
  }
  else if (EQ(str, "!"))
  {
    return gamecom(cur_player->prev_com, False);
  }
  else
  {
    /* Percent-sign fix. - Dragorn */
    /*
    strcount = 0;
    workcount = 0;

    for (strcount = 0; strcount < strlen(str); strcount++)
    {
     dumpchar = str[strcount];
     if (dumpchar != '%' && ((str[strcount+1] != 'n') || (str[strcount+1] != 'd')))
     {
      str[workcount] = dumpchar;
      workcount++;
     }
    }
    if (workcount < strcount)
     str[workcount] = '\0';
    */
    if (savecom)
      (void)strcpy(cur_player->prev_com, str);
    if (mynum < max_players)
      cur_player->last_command = time(0);

    /* Translate : and ; to "emote", and ' and " to "say".
 [ to gossip */
    a = -1;
    if ((mynum < max_players) && cur_player->converse.active)
    {
      if (cur_player->converse.talking_to != -1)
        if (!is_in_game(cur_player->converse.talking_to))
        {
          bprintf("I'm sorry, the person you were conversing with is"
                  "no longer in the game.\n");
          cur_player->converse.active = False;
          strcpy(cur_player->prompt, cur_player->converse.old_prompt);
          return 0;
        }
      if (str[0] == '*')
        if (str[1] == '*')
        {
          cur_player->converse.active = False;
          strcpy(cur_player->prompt, cur_player->converse.old_prompt);
          return 0;
        }
        else
          strcpy(strbuf, str + 1);
      else if (cur_player->converse.talking_to == -1)
        sprintf(strbuf, "say %s", str);
      else
        sprintf(strbuf, "tell %s %s", pname(cur_player->converse.talking_to), str);
      strcpy(str, strbuf);
    }
    if (str[0] == '\"' || str[0] == '\'')
    {
      strcpy(strbuf, "say ");
      strcat(strbuf, str + 1);
    }
    else if (str[0] == '[')
    {
      strcpy(strbuf, "gossip ");
      strcat(strbuf, str + 1);
    }
    else if (str[0] == ':' || str[0] == ';' || str[0] == '/')
    {
      strcpy(strbuf, "emote ");
      strcat(strbuf, str + 1);
    }
    else if (str[0] == '0')
    {
      strcpy(strbuf, "gossip ");
      strcat(strbuf, str + 1);
    }
    else if (str[0] == '1')
    {
      strcpy(strbuf, "wiz ");
      strcat(strbuf, str + 1);
    }
    else if (str[0] == '2')
    {
      strcpy(strbuf, "awiz ");
      strcat(strbuf, str + 1);
    }
    else if (str[0] == '3')
    {
      strcpy(strbuf, "dgod ");
      strcat(strbuf, str + 1);
    }
    else if (str[0] == '4')
    {
      strcpy(strbuf, "god ");
      strcat(strbuf, str + 1);
    }
    else
    {
      strcpy(strbuf, str);
    }

    stp = 0;

    if (brkword() == -1)
    {            /* if line contains nothing but  */
      erreval(); /* pn's or articles (a, an, the) */
      return -1;
    }
    if ((a = chkverb()) != -1)
    {
      doaction(a);
      return 0;
    }
    if (fextern(wordbuf) >= 0)
    { /* handle external commands */
      stp = 0;
      return 0;
    }
    stp = 0;
    (void)brkword();
    if ((a = fpbn(wordbuf)) >= 0)
    { /* translate NAME to TELL NAME */
      stp = 0;
      a = VERB_TELL;
      doaction(a);
      return 0;
    }
#ifdef INTERMUD
    if (strchr(wordbuf, '@'))
    {
      stp = 0;
      a = VERB_ITELL;
      doaction(a);
      return 0;
    }
#endif
    bprintf("What drug are you on?\n");
    return -1;
  }
}

int gamecom(char *str, Boolean savecom)
{
  int x;

  plast_cmd(real_mynum) = global_clock;
  x = xgamecom(str, savecom);
  special_events(mynum);
  return x;
}

char *
getreinput(char *b)
{
  char buffer[MAX_COM_LEN];

  while (strbuf[stp] == ' ')
    stp++;

  strcpy(buffer, &rawbuf[stp]);
  strcpy(rawbuf, buffer);

  return b != NULL ? strcpy(b, &strbuf[stp]) : &strbuf[stp];
}

int brkword(void)
{
  static int neword = 0;
  int worp;

  if (stp == 0)
    neword = 0;
  if (neword)
  {
    neword = 0;
    return 0;
  }
  for (;;)
  {
    while (NOISECHAR(strbuf[stp]))
      stp++;
    for (worp = 0; strbuf[stp] != 0 && !NOISECHAR(strbuf[stp]);)
      wordbuf[worp++] = strbuf[stp++];
    wordbuf[worp] = 0;
    lowercase(wordbuf);
    if (EQ(wordbuf, "the") || EQ(wordbuf, "a") || EQ(wordbuf, "an") || EQ(wordbuf, "of") || EQ(wordbuf, "with"))
      continue;
    if (EQ(wordbuf, "it"))
    {
      /* Make sure no NULL pointer in here */
      if (cur_player->wd_it)
        strcpy(wordbuf, cur_player->wd_it);
    }

    if (EQ(wordbuf, "him"))
    {
      /* Make sure no NULL pointer in here */
      if (cur_player->wd_him)
        strcpy(wordbuf, cur_player->wd_him);
    }

    if (EQ(wordbuf, "her"))
    {
      /* Make sure no NULL pointer in here */
      if (cur_player->wd_her)
        strcpy(wordbuf, cur_player->wd_her);
    }

    if (EQ(wordbuf, "them"))
    {
      /* Make sure no NULL pointer in here */
      if (cur_player->wd_them)
        strcpy(wordbuf, cur_player->wd_them);
    }

    if (EQ(wordbuf, "me"))
    {
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
    if ((b = Match(word, lista[a])) > c)
    {
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
  if (!parse_2(vb))
    return;
  if (vb > 1 && vb < 12)
  {
    if (cur_player->i_follow >= 0)
    {
      bprintf("You stopped following.\n");
      cur_player->i_follow = -1;
    }
    dodirn(vb);
    return;
  }
  if (cur_player->inmailer)
  {
    switch (vb)
    {
    case VERB_ALIAS:
    case VERB_DISCONNECT:
    case VERB_CONVERSE:
    case VERB_MAIL:
    case VERB_BECOME:
    case VERB_QUIT:
      bprintf("You can't do that while in the mailer.\n");
      return;
    }
  }
  if (cur_player->isforce)
  {

    switch (vb)
    {
    case VERB_QUIT:
    case VERB_BECOME:
    case VERB_BUG:
    case VERB_SUGGEST:
    case VERB_TYPO:
    case VERB_ZAP:
    case VERB_FORCE:
    case VERB_DISCONNECT:
    case VERB_MAIL:
#ifdef INTERMUD
    case VERB_ITELL:
    case VERB_IGOSSIP:
    case VERB_IMUDLIST:
    case VERB_IWHO:
    case VERB_IFINGER:
    case VERB_ILOCATE:
    case VERB_IBOOT:
    case VERB_ISHUTDOWN:
    case VERB_IMINVIS:
    case VERB_INOGOSSIP:
#endif
      bprintf("You can't be forced to do that.\n");
      return;
    }
  }

  cur_player->last_command = time(0);

  switch (vb)
  {
  case VERB_GO:
    if (cur_player->i_follow >= 0)
    {
      bprintf("You stopped following %s.\n", pname(cur_player->i_follow));
      cur_player->i_follow = -1;
    }
    dogocom();
    break;
  case VERB_ASMORTAL:
    asmortalcom();
    break;
  case VERB_FLOWERS:
    flowercom();
    break;
  case VERB_ROSE:
    rosecom();
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
  case VERB_POWERS:
    powerscom();
    break;
  case VERB_POCKET:
    pocketcom();
    break;
  case VERB_FREE:
    freecom();
    break;
  case VERB_LOAD:
    loadcom();
    break;
  case VERB_STORE:
    storecom();
    break;
  case VERB_TICKLE:
    ticklecom();
    break;
  case VERB_PET:
    petcom();
    break;
  case VERB_QUIT:
    quit_game(YN_INIT);
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
  case VERB_KOOSH:
    kooshcom();
    break;
  case VERB_MESSAGE:
    do_read();
    break;
  case VERB_ERASE:
    do_erase();
    break;
  case VERB_WRITE:
    write_board();
    break;
  case VERB_RING:
    ringcom();
    break;
  case VERB_SAY:
    saycom();
    break;
  case VERB_TELL:
    tellcom();
    break;
  case VERB_SHOUT:
    shoutcom();
    break;
  case VERB_DROP:
    dropobj(-1);
    break;
  case VERB_INVENTORY:
    inventory();
    break;
#ifdef INTERMUD
  case VERB_ITELL:
    imTell();
    break;
  case VERB_IGOSSIP:
    imGossip();
    break;
  case VERB_IMUDLIST:
    imMudlist();
    break;
  case VERB_IWHO:
    imWho();
    break;
  case VERB_IFINGER:
    imFinger();
    break;
  case VERB_ILOCATE:
    imLocate();
    break;
  case VERB_IBOOT:
    imBoot();
    break;
  case VERB_ISHUTDOWN:
    imShutdownWrap();
    break;
  case VERB_IMINVIS:
    imInvis();
    break;
  case VERB_INOGOSSIP:
    imNogos();
    break;
#endif
  case VERB_WHO:
    whocom();
    break;
  case VERB_MLEFT:
    mleftcom();
    break;
  case VERB_RESET:
    resetcom(RES_TEST);
    break;
  case VERB_ZAP:
    zapcom();
    break;
  case VERB_WARNZAP:
    warnzapcom();
    break;
  case VERB_PUNTZAP:
    puntzapcom();
    break;
  case VERB_EAT:
    eatcom();
    break;
  case VERB_SAVE:
    saveme();
    break;
  case VERB_SAVEALL:
    saveallcom();
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
    read_file(COUPLES, NULL, True, NULL);
    break;
  case VERB_WIZECHO:
    wizechocom();
    break;
  case VERB_WIZLIST:
    wizlistcom();
    break;
  case VERB_WEM:
    wemcom();
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
  case VERB_TIMEOUT:
    timeoutcom();
    break;
  case VERB_DISCONNECT:
    disconnectcom();
    break;
  case VERB_SOCINFO:
    socinfo();
    break;
  case VERB_KILLSOC:
    killsocket();
    break;
  case VERB_INFO:
    infocom();
    break;
  case VERB_QUESTS:
    questcom();
    break;
  case VERB_QDONE:
    qdonecom();
    break;
  case VERB_LOOK:
    lookcom();
    break;
  case VERB_SCORE:
    scorecom();
    break;
  case VERB_VALUE:
    valuecom();
    break;
  case VERB_TOURNAMENT:
    tournamentcom();
    break;
  case VERB_DISCHUNT:
    placediscs();
    break;
  case VERB_PLAY:
    playcom();
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
  case VERB_CLOSE:
    closecom();
    break;
  case VERB_OPEN:
    opencom();
    break;
  case VERB_LOCK:
    lockcom();
    break;
  case VERB_UNLOCK:
    unlockcom();
    break;
  case VERB_HELP:
    helpcom();
    break;
  case VERB_STATS:
    showplayer();
    break;
  case VERB_ACCT:
    acctcom();
    break;
  case VERB_COMPARE:
    comparecom();
    break;
  case VERB_DIAGNOSE:
    diagnosecom();
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
#ifdef REBOOT
  case VERB_REBOOT:
    rebootcom();
  case VERB_DELAY:
    delaycom();
#else
  case VERB_REBOOT:
    bprintf("Reboot is disabled this reboot. (Ha ha)\n");
#endif
    break;
  case VERB_BUSY:
    busycom();
    break;
  case VERB_AWAY:
    awaycom();
    break;
  case VERB_COMPACT:
    compactcom();
    break;
  case VERB_EMAIL:
    showemail();
    break;
  case VERB_NOINVENTORY:
    noinventorycom();
    break;
  case VERB_AUTOEXITS:
    autoexitcom();
    break;
  case VERB_SAYBACK:
    saybackcom();
    break;
  case VERB_PUNT:
    puntcom();
    break;
  case VERB_CONVERSE:
    conversecom();
    break;
  case VERB_KILL:
    killcom();
    break;
  case VERB_POSE:
    posecom();
    break;
  case VERB_BANG:
    bangcom();
    break;
  case VERB_SET:
    setcom();
    break;
  case VERB_PRAY:
    praycom();
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
  case VERB_WEARALL:
    wearall();
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
  case VERB_FORCE:
    forcecom();
    break;
  case VERB_LIGHT:
    lightcom();
    break;
  case VERB_LASTON:
    lastoncom();
    break;
  case VERB_CODE:
    codecom();
    break;
  case VERB_EXTINGUISH:
    extinguishcom();
    break;
  case VERB_CRIPPLE:
    cripplecom();
    break;
  case VERB_GIVE:
    givecom();
    break;
  case VERB_GIVEALL:
    giveallcom();
    break;
  case VERB_EQUIPMENT:
    equipcom();
    break;
  case VERB_CURE:
    curecom();
    break;
  case VERB_HEAL:
    healcom();
    break;
  case VERB_HEALALL:
    healallcom();
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
  case VERB_NOD:
    nodcom();
    break;
  case VERB_SPIT:
    spitcom();
    break;
  case VERB_NOGOSSIP:
  case VERB_NOWIZ:
  case VERB_NOAWIZ:
  case VERB_NODGOD:
  case VERB_NOGOD:
    notalklinecom(vb);
    break;
  case VERB_WIZ:
    wizcom();
    break;
  case VERB_GOSSIP:
    gossipcom();
    break;
  case VERB_DGOD:
    demigodcom();
    break;
  case VERB_GOD:
    godcom();
    break;
  case VERB_AWIZ:
    awizcom();
    break;
  case VERB_EXITS:
    exitcom(1);
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
  case VERB_WIMPY:
    change_wimpy();
    break;
  case VERB_SETIN:
    set_msg(cur_player->setin, False);
    break;
  case VERB_SETOUT:
    set_msg(cur_player->setout, True);
    break;
  case VERB_SETMIN:
    set_msg(cur_player->setmin, False);
    break;
  case VERB_SETMOUT:
    set_msg(cur_player->setmout, False);
    break;
  case VERB_SETVIN:
    set_msg(cur_player->setvin, False);
    break;
  case VERB_SETVOUT:
    set_msg(cur_player->setvout, False);
    break;
  case VERB_SETQIN:
    set_msg(cur_player->setqin, False);
    break;
  case VERB_SETQOUT:
    set_msg(cur_player->setqout, False);
    break;
  case VERB_SETSIT:
    set_msg(cur_player->setsit, False);
    break;
  case VERB_SETSTAND:
    set_msg(cur_player->setstand, False);
    break;
  case VERB_SETTRENTER:
    set_msg(cur_player->settrenter, False);
    break;
  case VERB_SETTRVICT:
    set_msg(cur_player->settrvict, False);
    break;
  case VERB_SETTRROOM:
    set_msg(cur_player->settrroom, False);
    break;
  case VERB_SETSTART:
    setstart();
    break;
  case VERB_CALENDAR:
    calendarcom();
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
  case VERB_NRAW:
    namerawcom();
    break;
  case VERB_CFR:
    cfrcom();
    break;
  case VERB_ROLL:
    rollcom();
    break;
  case VERB_THROW:
    throwcom();
    break;
  case VERB_CREDITS:
    read_file(CREDITS, NULL, True, NULL);
    break;
  case VERB_BRIEF:
    briefcom();
    break;
  case VERB_JUMP:
    jumpcom();
    break;
  case VERB_JAIL:
    jailcom();
    break;
  case VERB_DIG:
    digcom();
    break;
  case VERB_UNTIE:
    untiecom();
    break;
  case VERB_TIE:
    tiecom();
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
  case VERB_SUGGEST:
    suggestcom();
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
  case VERB_WIZFROB:
    wizfrobcom();
    break;
  case VERB_AWIZFROB:
    awizfrobcom();
    break;
  case VERB_DGODFROB:
    dgodfrobcom();
    break;
  case VERB_GODFROB:
    godfrobcom();
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
  case VERB_EMOTEALL:
    emoteallcom();
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
  case VERB_PCREATE:
    pcreate();
    break;
  case VERB_PSTATUS:
    pstatus();
    break;
  case VERB_PNAME:
    partyname();
    break;
  case VERB_PADD:
    padd();
    break;
  case VERB_PLEAVE:
    pleave();
    break;
  case VERB_PLEADER:
    pleader();
    break;
  case VERB_PSAY:
    psay();
    break;
  case VERB_PSHARE:
    pshare();
    break;
  case VERB_PFOLLOW:
    pfollow();
    break;
  case VERB_QUIET:
    quietcom();
    break;
  case VERB_NOSHOUT:
    noshoutcom();
    break;
  case VERB_NOSLAIN:
    noslaincom();
    break;
  case VERB_NOBEEP:
    nobeepcom();
    break;
  case VERB_NOBATTLE:
    nobattlecom();
    break;
  case VERB_COLOR:
    colorcom();
    break;
  case VERB_BEEP:
    beepcom();
    break;
  case VERB_HOME:
    homecom();
    break;
  case VERB_ALOOF:
    aloofcom();
    break;
  case VERB_SHOW:
    showitem();
    break;
  case VERB_WIZLOCK:
    wizlock();
    break;
  case VERB_FOLLOW:
    followcom();
    break;
  case VERB_LOSE:
    losecom();
    break;
  case VERB_CLS:
    pcls(NULL, NULL);
    break;
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
  case VERB_TRACE:
    tracecom();
    break;
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
  case VERB_BANHOST:
    bancom(True);
    break;
  case VERB_BANCHAR:
    bancom(False);
    break;
  case VERB_TRANSLOCATE:
    sumcom();
    break;
  case VERB_ALIAS:
    aliascom();
    break;
  case VERB_LFLAGS:
    lflagscom();
    break;
  case VERB_PRIVS:
    pflagscom();
    break;
  case VERB_VERSION:
    bprintf("Version: %s (%s)\n", VERSION, MBANNER);
    break;
  case VERB_MEDITATE:
    meditatecom();
    break;
  default:
    mudlog("No match for verb = %d", vb);
    bprintf("You can't do that now.\n");
    break;
  }
}

void quit_game(int yn)
{
  char xx[128];
  static char *goodbye = "\
&+B                   ___              _   ___ \n\
&+B                  / __|___  ___  __| | | _ )_  _ ___ \n\
&+B                 | (_ / _ \\/ _ \\/ _` | | _ \\ || / -_)\n\
&+B                  \\___\\___/\\___/\\__,_| |___/\\_, \\___|\n\
&+B                                            |__/\n";

  /******************
    if(yn == YN_NO) return;
    if(yn == YN_INIT) {
      get_yn(quit_game, "Are you sure you want to quit", ASSUME_NO);
      return;
    }
  ***************/
  if (cur_player->aliased)
  {
    unalias(real_mynum);
    return;
  }

  if (pfighting(mynum) >= 0)
  {
    bprintf("Not in the middle of a fight!\n");
    return;
  }

  if (ltstflg(ploc(mynum), LFL_NO_QUIT) && (plev(mynum) < LVL_APPREN))
  {
    bprintf("You are not allowed to quit from this room.\n");
    return;
  }
  if (cur_player->polymorphed >= 0)
  {
    bprintf("A mysterious force won't let you quit.\n");
    return;
  }

  bprintf("Ok");
  cur_player->ghostcounter = 0;
  sclrflg(mynum, SFL_GHOST);
  strcpy(cur_player->setin, cur_player->ghostsetin);
  strcpy(cur_player->setout, cur_player->ghostsetout);
  /* Again, in newer dyrt versions, need to use setstand and setsit */
  /*   strcpy (cur_player->sethere, cur_player->ghostsethere); */
  strcpy(cur_player->setstand, cur_player->ghostsetstand);
  strcpy(cur_player->setsit, cur_player->ghostsetsit);
  send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
           "%s\n", build_setin(xx, cur_player->setqout, pname(mynum), NULL));

  send_msg(DEST_ALL, MODE_QUIET | MODE_COLOR, max(pvis(mynum), LVL_APPREN),
           LVL_MAX, mynum, NOBODY, "[Quitting Game: %s]\n", pname(mynum));

  crapup(goodbye, CRAP_SAVE);
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

  if (vb == -1)
    return False;
  ob1 = pl1 = ob2 = pl2 = -1;
  prep = 0;
  txt1 = markpos();
  txt2 = NULL;
  parse_1(item1, &pl, &ob, &pre);
#if 0
  bprintf ("vb = %d, item1 = %s, pl = %d, ob = %d, pre = %d\n",
	   vb, item1, pl, ob, pre);
#endif
  if (pre == 0)
  {
    ob1 = ob;
    pl1 = pl;
    txt2 = markpos();
    parse_1(item2, &pl, &ob, &pre);
  }
  if (pre != 0)
  {
    prep = pre;
    parse_1(item2, &pl, &ob, &pre);
  }
#if 0
  if (pre != 0)
    {
      bprintf ("Huh?\n");
      return False;
    }
  else
    {
      pl2 = pl;
      ob2 = ob;
    }
#else
  pl2 = pl;
  ob2 = ob;
#endif
  stp = savestp;
  return True;
}

static void
parse_1(char *itemb, int *pl, int *ob, int *pre)
{
  int o, p;

  *pl = *ob = -1;
  *pre = 0;
  if (brkword() != -1)
  {
    strcpy(itemb, wordbuf);
    if ((p = findprep(itemb)) != -1)
    {
      *pre = p;
    }
    else
    {
      /* It's not a preposition. */
      if ((p = *pl = fpbn(itemb)) != -1)
      {
        if (psex(p))
        {
          cur_player->wd_them = strcpy(cur_player->wd_her, pname(p));
        }
        else
        {
          cur_player->wd_them = strcpy(cur_player->wd_him, pname(p));
        }
      }

      if ((o = *ob = fobnc(itemb)) != -1 || (o = *ob = fobna(itemb)) != -1)
      {
        cur_player->wd_it = oname(o);
      }
    }
  }
}

static char *
markpos(void)
{
  register int c;

  while ((c = strbuf[stp]) == ',' || isspace(c))
    ++stp;
  return strbuf + stp;
}

int findprep(char *t)
{
  return xlookup(t, preptable);
}

#if 0
int
prmmod (int p)
{
  switch (p)
    {
    case FL_PL1:
      return pl1;
    case FL_PL2:
      return pl2;
    case FL_OB1:
      return ob1;
    case FL_OB2:
      return ob2;
    case FL_CURCH:
      return ploc (mynum);
    case FL_PREP:
      return prep;
    case FL_LEV:
      return plev (mynum);
    case FL_STR:
      return pstr (mynum);
    case FL_SEX:
      return psex (mynum);
    case FL_SCORE:
      return pscore (mynum);
    case FL_MYNUM:
      return mynum;
    }
#if 0
  if (p >= 10000)
    return globaldata[p - 10000];
#endif
  
  if (IS_PL (p))
    return p - PL_CODE + max_players;
  return p;
}

#endif
