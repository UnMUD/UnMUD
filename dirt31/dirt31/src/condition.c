/*
**  Condition evaluator for the system
*/
#include "kernel.h"
#include "levels.h"
#include "condact.h"
#include "oflags.h"
#include "sflags.h"
#include "pflags.h"
#include "cflags.h"
#include "condition.h"
#include "mobile.h"

CONDITION *conds;

int pptr;
int ct;

short int parameters[NUM_CONDS];

Boolean exec_match(int i,int ob, int pl)
{
  /* Check if item i matches the ob or pl specified */

  if (i < 0) {
    switch(i) {
    case PL: return (pl != -1);
    case OB: return (ob != -1);
    case NONE: return (pl == -1 && ob == -1);
    }
  } else if (IS_PL(i)) return ((i + max_players) == P(pl));
  else if (IS_OB(i)) return (i == O(ob));
  return True;
}

void set_msg(char *b,Boolean dir_ok)
{
  char k[MAX_COM_LEN];

  getreinput(k);
  if (check_setin(k,dir_ok)) strcpy(b,k);
  else bprintf( "Not changed, wrong format.\n");
}

Boolean check_conditions(CONDITION cond_arr[])
{
  conds = cond_arr;
  for (ct = 0, pptr = 0; ct < NUM_CONDS && condeval(cond_arr[ct]); ++ct);
  return (ct >= NUM_CONDS);
}

void do_actions(ACTION act_arr[])
{
  for (ct = 0, pptr = 0; ct < NUM_ACTS && stdaction(act_arr[ct]); ++ct);
}

int getnext(void)
{
  return ct >= NUM_CONDS ? 0 : prmmod(conds[++ct].cnd_data);
}

int getpar(void)
{
  return prmmod(parameters[pptr++]);
}

Boolean condeval(CONDITION cond)
{
  int     p   = prmmod(cond.cnd_data);
  Boolean neg = (cond.cnd_code & NOT) != 0;
  Boolean r;
  int     b;

  switch (cond.cnd_code & COND_MASK) {
  case PAR:        parameters[pptr++] = cond.cnd_data; r = True; break;
  case LEVEQ:      r = plev(mynum) == p; break;
  case LEVLT:      r = plev(mynum) < p; break;
  case IN:         r = ploc(mynum) == p; break;
  case GOT:        r = iscarrby(p,mynum); break;
  case AVL:        r = isavl(p); break;
  case IFDEAF:     r = ststflg(mynum, SFL_DEAF) != 0; break;
  case IFDUMB:     r = ststflg(mynum, SFL_DUMB) != 0; break;
  case IFBLIND:    r = ststflg(mynum, SFL_BLIND) != 0; break;
  case IFCRIPPLED: r = ststflg(mynum, SFL_CRIPPLED) != 0; break;
  case IFFIGHTING: r = pfighting(mynum) >= 0; break;
  case IFSITTING:  r = psitting(mynum); break;
  case PFLAG:      r = ptstflg(p, getnext()) != 0; break;
  case STATE:      r = state(p) == getnext(); break;
#if 0
  case EQc:        r = globaldata[p] == getnext(); break;
  case LT:         r = globaldata[p] < getnext(); break;
  case GT:         r = globaldata[p] > getnext(); break;
#endif
  case VISLT:      r = pvis(p) < getnext(); break;
  case VISEQ:      r = pvis(p) == getnext(); break;
  case CANSEE:     r = pvis(p) <= plev(mynum); break;
  case STRLT:      r = pstr(mynum) < p; break;
  case STREQ:      r = pstr(mynum) == p; break;
  case ISIN:       r = iscontin(p, getnext()); break;
  case HERE:       r = ishere(p); break;
  case PLGOT:      r = iscarrby(p,getnext()); break;
  case DESTROYED:  r = otstbit(p, OFL_DESTROYED) != 0; break;
  case CHANCE:     r = randperc() < p; break;
  case OBIT:       r = otstbit(p, getnext()); break;
  case INZONE:     r = loc2zone(ploc(mynum)) == loc2zone(p); break;
  case ISME:       r = p == mynum; break;
  case DARK:       r = isdark(); break;
  case OBJAT:      r = ocarrf(p) == IN_ROOM && oloc(p) == getnext(); break;
  case WIZ:        r = plev(mynum) >= LVL_WIZARD; break;
  case AWIZ:       r = plev(mynum) >= LVL_ARCHWIZARD; break;
  case PHERE:      r = ploc(p) == ploc(mynum); break;
  case PLWORN:     r = iswornby(p,getnext()); break;
  case WPN:        r = pwpn(mynum) == p; break;
  case ENEMY:      r = pfighting(mynum) == p; break;
#if 0
  case CANGO:      r = cango(p); break;
#endif
  case PREP:       r = prep == p; break;
  case ALONE:      r = tscale() == 100; break;
  case WILLFITIN:  r = willhold(p, getnext()); break;
  case CANCARRY:   r = cancarry(p); break;
  case HASANY:     r = ohany((int)(1 << p)) != 0; break;
  case ISMONSTER:  r = p >= max_players; break;
  case HELPED:     r = ptothlp(mynum) > 0; break;
  case OP:         r = OPERATOR(pname(mynum)); break;
  case OBYTEEQ:
    b = getnext();
    if (b == 0) r = odamage(p) == getnext();
    else r = oarmor(p) == getnext();
    break;
  case OBYTEGT:
    b = getnext();
    if (b == 0) r = odamage(p) > getnext();
    else r = oarmor(p) > getnext();
    break;
  case OBYTEZ:
    b = getnext();
    if (b == 0) r = odamage(p) == 0;
    else r = oarmor(p) == 0;
    break;
  default:
    mudlog("bad condition - code %d", cond.cnd_code & COND_MASK);
    exit(1);
  }
  return neg ? !r : r;
}

/* Return False if last action, true if next action should also be performed.
*/
Boolean stdaction(ACTION a)
{
  int x, y, i, j;

  switch (a) {
  case NUL: break;                                         /* Do nothing */
  case GAIN:  pscore(mynum) += getpar(); calibme(); break; /* Gain points */
  case LOSE:  pscore(mynum) -= getpar(); calibme(); break; /* Lose points */
  case WOUND: pstr(mynum)   -= getpar(); calibme(); break; /* take damage */
  case HEAL:  pstr(mynum)   += getpar(); calibme(); break;
  case INC:   x = getpar(); setobjstate(x, state(x) + 1); break;
  case DEC:   x = getpar(); setobjstate(x, state(x) - 1); break;
  case PUT:   x = getpar(); setoloc(x, getpar(), IN_ROOM); break;
  case PUTIN: x = getpar(); setoloc(x, getpar(), IN_CONTAINER); break;
  case CARRY: x = getpar(); setoloc(x, getpar(), CARRIED_BY); break;
  case WEAR:  x = getpar(); setoloc(x, getpar(), WORN_BY); break;
  case PUTWITH:
    x = getpar();
    y = getpar();
    setoloc(x, oloc(y), ocarrf(y));
    break;
  case OK:    bprintf("Ok\n"); return False;
  case DONE:  return False;
  case DESTROY: destroy(getpar()); break;
  case QUIT:  crapup(messages[getpar()], SAVE_ME); break;
  case DIE:   crapup(messages[getpar()], NO_SAVE); break;
  case GOBY:  teletrap(oloc(getpar())); break;
  case GOTO:  teletrap(getpar()); break;
  case SWAP:
    x = getpar();
    y = getpar();
    i = oloc(x);
    j = ocarrf(x);
    setoloc(x, oloc(y), ocarrf(y));
    setoloc(y, i, j);
    break;
  case POBJ:    bprintf( "%s", oname(getpar())); break;
  case PPLAYER: bprintf( "%s", pname(getpar())); break;
  case SETLEVEL: setplev(mynum,getpar()); calibme(); break;
  case SETVIS:   setpvis(mynum,getpar()); break;
/*  case LOBJAT:   listobject(getpar(), IN_ROOM); break; */
  case LOBJIN:   listobject(getpar(), IN_CONTAINER); break;
  case LOBJCARR: listobject(getpar(), CARRIED_BY); break;
  case CREATE:   oclrbit(getpar(), OFL_DESTROYED); break;
  case SETSTATE: x = getpar(); setobjstate(x, getpar()); break;
  case COM_EAT:       eatcom(); break;
  case COM_INVEN:     inventory(); break;
  case COM_EXITS:     exitcom(); break;
  case COM_MOVE:      dodirn(getpar()); break;
  case COM_SETIN:     set_msg(cur_player->setin,False); break;
  case COM_SETOUT:    set_msg(cur_player->setout,True); break;
  case COM_SETMIN:    set_msg(cur_player->setmin,False); break;
  case COM_SETMOUT:   set_msg(cur_player->setmout,False); break;
  case COM_SETVIN:    set_msg(cur_player->setvin,False); break;
  case COM_SETVOUT:   set_msg(cur_player->setvout,False); break;
  case COM_SETQIN:    set_msg(cur_player->setqin,False); break;
  case COM_SETQOUT:   set_msg(cur_player->setqout,False); break;
  case COM_TELL:      tellcom(); break;
  case COM_SAY:       saycom(); break;
  case COM_SHOUT:     shoutcom(); break;
  case COM_WIZMSG:    wizcom(); break;
  case COM_AWIZMSG:   awizcom(); break;
  case COM_BUG:       bugcom(); break;
  case COM_TYPO:      typocom(); break;
  case COM_BLIND:     ssetflg(mynum, SFL_BLIND); break;
  case COM_DEAF:      ssetflg(mynum, SFL_DEAF); break;
  case COM_DUMB:      ssetflg(mynum, SFL_DUMB); break;
  case COM_CRIPPLE:   ssetflg(mynum, SFL_CRIPPLED); break;
  case COM_CURE:
    sclrflg(mynum, SFL_BLIND);
    sclrflg(mynum, SFL_DEAF);
    sclrflg(mynum, SFL_CRIPPLED);
    sclrflg(mynum, SFL_DUMB);
    break;
  case COM_LISTFILE:  bprintf("\001f%s\003", messages[getpar()]); break;
  case COM_USERS:     usercom(); break;
  case COM_SETSEX:    x = getpar(); setpsex(a, getpar()); break;
  case COM_PRONOUNS:  pncom(); break;
  case COM_WHO:       whocom(); break;
  case COM_GET:       getcom(); break;
  case COM_DROP:      dropobj(); break;
  case COM_WEAR:      wearcom(); break;
  case COM_REMOVE:    removecom(); break;
  case COM_GIVE:      givecom(); break;
  case COM_STEAL:     stealcom(); break;
  case COM_VALUE:     valuecom(); break;
  case COM_SCORE:     scorecom(); break;
  case COM_WEAPON:    wieldcom(); break;
  case COM_PUT:       putcom(); break;

  case COM_TIME:      timecom(); break;

  case COM_EXAMINE:   examcom(); break;

  case COM_RESET:     resetcom(False); break;
  case COM_IN:        incom(False); break;
  case COM_GOTO:      gotocom(False); break;
  case COM_PFLAGS:    pflagscom(); break;
  case COM_PLAYERS:   usercom(); break;
  case COM_MOBILES:   mobilecom(); break;
  case COM_WHERE:     wherecom(); break;
  case DECOBYTE:
    x = getpar();
    y = getpar();
    if (y == 0) osetdamage(x, odamage(x) - 1);
    else osetarmor(x, oarmor(x) - 1);
    break;
  case INCOBYTE:
    x = getpar();
    y = getpar();
    if (y == 0) osetdamage(x, odamage(x) + 1);
    else osetarmor(x, oarmor(x) - 1);
    break;
  case SETOBYTE:
    x = getpar();
    y = getpar();
    i = getpar();
    if (y == 0) osetdamage(x, i);
    else osetarmor(x, i);
    break;
  case PROVOKE:    hit_player(getpar(),mynum,-1); break;
  case COM_KILL:   killcom(); break;
  case COM_LOOK:   lookcom(); break;
  case BROAD:      broad(messages[getpar()]); break;
  case WOUNDMON:
    x = getpar();
    wound_player(mynum,x,getpar(),-1);
    break;
  default:
    if (a >= 2000) bprintf( "%s", messages[a-2000]);
    else if (a >= 500) {
      bprintf("\n%s\n%s", messages[a-500], cur_player->cprompt);
      bflush();
    } else if (a >= 200)  bprintf("%s\n", messages[a-200]);
    else {
      mudlog( "Invalid action %d", a);
      bprintf( "**Invalid Action %d\n", a);
    }
    break;
  }
  return True;
}






