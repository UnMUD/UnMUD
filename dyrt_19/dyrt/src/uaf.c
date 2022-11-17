#include <sys/file.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "kernel.h"
#include <errno.h>
#include "stdinc.h"
#include <gdbm.h>
#include "machines.h"

GDBM_FILE dbf;
extern char *WizLevels[];

void get_gender(char *gen);
time_t time(time_t *v);

void pers2player(PERSONA *d, int plx)
{
  setpstr(plx, d->p_strength);
  setpdam(plx, d->p_damage);
  setphome(plx, d->p_home);
  setpscore(plx, d->p_score);
  setparmor(plx, d->p_armor);
  setsflags(plx, d->p_sflags);
  setpflags(plx, d->p_pflags);
  setpmask(plx, d->p_mask);
  setqflags(plx, d->p_quests);
  setpvis(plx, ptstflg(plx, PFL_STARTINVIS) ? d->p_vlevel : 0);
  setplev(plx, d->p_level);
  setpwimpy(plx, d->p_wimpy);
  mob_id(plx) = d->p_id;
  if (plx < max_players)
  {
    setptitle(plx, d->p_title);
    (void)strcpy(players[plx].email, d->p_email);
    (void)strcpy(players[plx].passwd, d->p_passwd);
  }
}

void player2pers(PERSONA *d, time_t *last_on, int plx)
{
  d->p_strength = pstr(plx);
  d->p_damage = pdam(plx);
  d->p_home = phome(plx);
  d->p_score = pscore(plx);
  d->p_armor = parmor(plx);
  d->p_sflags = sflags(plx);
  d->p_pflags = pflags(plx);
  d->p_mask = pmask(plx);
  d->p_quests = qflags(plx);
  d->p_vlevel = pvis(plx);
  d->p_level = plev(plx);
  d->p_damage = pdam(plx);
  d->p_armor = parmor(plx);
  d->p_wimpy = pwimpy(plx);
  d->p_id = mob_id(plx);
  if (plx < max_players)
  {
    (void)strcpy(d->p_title, ptitle(plx));
    (void)strcpy(d->p_passwd, players[plx].passwd);
    (void)strcpy(d->p_email, pemail(plx));
  }
  (void)strcpy(d->p_name, pname(plx));
  if (last_on != NULL)
    d->p_last_on = *last_on;
}

void get_gender(char *gen)
{
  Boolean ok = False;
  Boolean female = False;
  PERSONA d;

  if (gen == NULL)
  {
    replace_input_handler(get_gender);
  }
  else if (*gen == 'M' || *gen == 'm')
  {
    sclrflg(mynum, SFL_FEMALE);
    ok = True;
  }
  else if (*gen == 'F' || *gen == 'f')
  {
    ssetflg(mynum, SFL_FEMALE);
    ok = female = True;
  }
  else
  {
    bprintf("M or F");
  }
  if (ok)
  {
    /* initialize a very new user */
    if (OPERATOR(pname(mynum)))
    {
      /* We make him a god */
      sprintf(ptitle(mynum), "%%s the %s", WizLevels[LEV_GOD]);
      setplev(mynum, LVL_GOD);
      setpstr(mynum, pmaxstrength(LVL_GOD));
      setpscore(mynum, levels[LVL_APPREN]);
      update_wizlist(pname(mynum), LEV_GOD);
    }
    else
    {
      sprintf(ptitle(mynum), "%%s the %s",
              (female ? FLevels : MLevels)[LVL_ONE]);
      setplev(mynum, LVL_ONE);
      setpstr(mynum, 40);
      setpscore(mynum, 0);
    }
    setpwimpy(mynum, 0);
    mob_id(mynum) = id_counter++;
    setphome(mynum, 0);
    setpdam(mynum, 8);
    setparmor(mynum, 0);
    setpvis(mynum, 0);
    setsflags(mynum, 0);
    if (female)
      ssetflg(mynum, SFL_FEMALE);
    setqflags(mynum, 0);
    pflags(mynum).b1 = pflags(mynum).b2 = pflags(mynum).b3 = 0;
    set_xpflags(plev(mynum), &(pflags(mynum)), &(pmask(mynum)));
    if (female)
      xsetbit(d.p_sflags, SFL_FEMALE);
    player2pers(&d, &global_clock, mynum);

    putuaf(&d);
    save_id_counter();
    do_motd(NULL);
  }
  else
  {
    bprintf("\n");
    bprintf(strcpy(cur_player->cprompt, "Sex (M/F) : >"));
  }
}

void saveme(void)
{
  PERSONA d;

  if (cur_player->aliased || cur_player->polymorphed >= 0)
  {
    bprintf("Not while aliased.\n");
    return;
  }
  player2pers(&d, &global_clock, mynum);
  bprintf("\n&+CSaving %s&*\n", pname(mynum));
  bflush();
  putuaf(&d);
}

/* Initialize userfile and keep it open */
void init_userfile()
{
  void (*mudlog)();

  if (dbf)
    return; /* already open */
#ifdef SAFE_USERFILE
  if ((dbf = gdbm_open(UAF_RAND, sizeof(PERSONA), GDBM_WRCREAT, S_IRUSR | S_IWUSR, mudlog)) == NULL)
  {
#else
  if ((dbf = gdbm_open(UAF_RAND, sizeof(PERSONA), GDBM_WRCREAT | GDBM_FAST, S_IRUSR | S_IWUSR, mudlog)) == NULL)
  {
#endif
    mudlog("GDBM failed to access user file (%s)", UAF_RAND);
    exit(1);
  }
  return;
}

/* sync userfile (safety for GDBM_FAST writes) */

void sync_userfile()
{
#ifndef SAFE_USERFILE /* no need if SAFE is on */
  if (dbf)
    gdbm_sync(dbf);
#endif
  return;
}

/* reorganize userfile (at startup usually) */

int reorg_userfile()
{
  return (gdbm_reorganize(dbf));
}

void close_userfile()
{
  if (dbf)
    gdbm_close(dbf);
  {
    dbf = NULL;
  }
}

Boolean getuaf(char *name, PERSONA *d)
{
  datum in;
  datum key;
  datum data;

  key.dptr = name;
  lowercase(key.dptr);
  key.dptr[0] = toupper(key.dptr[0]);
  key.dsize = strlen(name) + 1;
  data.dptr = (char *)d;
  data.dsize = sizeof(PERSONA) + 1;

  if (!dbf)
  {
    init_userfile();
  }
  /* locate a user */
  in = gdbm_fetch(dbf, key);
  if (in.dptr == NULL)
  {
    return (False); /* no such entry */
  }
  /* copy it to our given pointer */
  bcopy(in.dptr, d, sizeof(PERSONA));
  /* free the pointer */
  if (in.dptr)
  {
    free(in.dptr);
  }
  return (True);
}

/* opens the userfile and writes a record to it; if the record exists,
   it will replace it */
void putuaf(PERSONA *d)
{
  int ret;
  PERSONA x;
  datum key;
  datum data;

  key.dptr = d->p_name;
  lowercase(key.dptr);
  key.dptr[0] = toupper(key.dptr[0]);
  key.dsize = strlen(d->p_name) + 1;
  data.dptr = (char *)d;
  data.dsize = sizeof(PERSONA) + 1;

  /* store the user */
  ret = gdbm_store(dbf, key, data, GDBM_REPLACE);
  switch (ret)
  {
  case 1:
    mudlog("gdbm_store() - caller not official writer, or key or datum is null");
    exit(1);
    break;
  case -1:
    mudlog("gdbm_store() - key exists and not allowed to replace.");
    exit(1);
    break;
  default:;
  }
  return;
}

void deluaf(char *name)
{
  int ret;
  datum key;

  key.dptr = name;
  lowercase(key.dptr);
  key.dptr[0] = toupper(key.dptr[0]);
  key.dsize = strlen(name) + 1;

  ret = gdbm_delete(dbf, key);
}

/* Load the user's name as it appears in the database */
Boolean getuafinfo(char *name)
{
  PERSONA d;
  Boolean b;

  b = getuaf(name, &d);
  if (b)
  {
    pers2player(&d, mynum);
    setpname(mynum, d.p_name);
  }
  return b;
}

Boolean findsetins(char *name, SETIN_REC *s, int fd)
{
  while (read(fd, s, sizeof(SETIN_REC)) == sizeof(SETIN_REC))
    if (EQ(s->name, name))
    {
      return True;
    }
  return False;
}

Boolean getsetins(char *name, SETIN_REC *s)
{
  int fd;
  Boolean b;

  if ((fd = open(SETIN_FILE, O_RDONLY, 0)) < 0)
  {
    return False;
  }
  b = findsetins(name, s, fd);
  close(fd);
  return b;
}

void putsetins(char *name, SETIN_REC *s)
{
  SETIN_REC v;
  int fd;
  Boolean b;

  if ((fd = open(SETIN_FILE, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) < 0)
  {
    mudlog("putsetins: Error in open for " SETIN_FILE);
    progerror("open");
    exit(1);
  }
  if ((b = findsetins(name, &v, fd)))
  {
    lseek(fd, (long)-sizeof(SETIN_REC), SEEK_CUR);
  }
  else
  {
    lseek(fd, 0L, SEEK_END);
  }
  if (write(fd, s, sizeof(SETIN_REC)) != sizeof(SETIN_REC))
  {
    mudlog("putsetins: Error in write for " SETIN_FILE);
    progerror("write");
  }
  lseek(fd, 0L, SEEK_END);
  close(fd);
}

void fetchprmpt(int plr)
{
  SETIN_REC s;

  if (plr >= max_players || plr < 0)
  {
    return;
  }
  if (plev(plr) >= LVL_APPREN && getsetins(pname(plr), &s))
  {
    strcpy(players[plr].prompt, s.prompt);
    strcpy(players[plr].setin, s.setin);
    strcpy(players[plr].setout, s.setout);
    strcpy(players[plr].setmin, s.setmin);
    strcpy(players[plr].setmout, s.setmout);
    strcpy(players[plr].setvin, s.setvin);
    strcpy(players[plr].setvout, s.setvout);
    strcpy(players[plr].setqin, s.setqin);
    strcpy(players[plr].setqout, s.setqout);
    strcpy(players[plr].setsit, s.setsit);
    strcpy(players[plr].setstand, s.setstand);
    strcpy(players[plr].settrenter, s.settrenter);
    strcpy(players[plr].settrvict, s.settrvict);
    strcpy(players[plr].settrroom, s.settrroom);
  }
  else
  {
    strcpy(players[plr].prompt, "&+CDyrt>&*");
    strcpy(players[plr].setin, "%n has arrived.");
    strcpy(players[plr].setout, "%n has gone %d.");
    strcpy(players[plr].setmin, "%n appears with an ear-splitting bang.");
    strcpy(players[plr].setmout, "%n vanishes in a puff of smoke.");
    strcpy(players[plr].setvin, "%n suddenly appears!");
    strcpy(players[plr].setvout, "%n has vanished!");
    strcpy(players[plr].setqin, "%n has entered the game.");
    strcpy(players[plr].setqout, "%n has left the game.");
    strcpy(players[plr].setsit, "%n is sitting here.");
    strcpy(players[plr].setstand, "%n is standing here.");
    strcpy(players[plr].settrenter, "%n appears, looking bewildered.");
    strcpy(players[plr].settrvict, "You are summoned by %n.");
    strcpy(players[plr].settrroom, "%n evocates the summoning spell.");
  }
}

char *build_setin(char *b, char *s, char *n, char *d)
{
  char *p, *q, *r;

  for (p = b, q = s; *q != 0;)
  {
    if (*q != '%')
    {
      *p++ = *q++;
    }
    else
    {
      switch (*++q)
      {
      case 'n':
        for (r = n; *r != 0;)
          *p++ = *r++;
        break;
      case 'd':
        if (d == NULL)
          return NULL;
        for (r = d; *r != 0;)
          *p++ = *r++;
        break;
      case 'N':
        for (r = xname(n); *r != 0;)
          *p++ = *r++;
        break;
      case 'f':
        for (r = (psex(mynum) ? "her" : "his"); *r != 0;)
          *p++ = *r++;
        break;
      case 'F':
        for (r = (psex(mynum) ? "her" : "him"); *r != 0;)
          *p++ = *r++;
        break;
      case 0:
        --q;
        break;
      default:
        *p++ = *q;
      }
      ++q;
    }
  }
  if (p[-1] == '\n')
  {
    --p;
  }
  *p = 0;
  return b;
}
