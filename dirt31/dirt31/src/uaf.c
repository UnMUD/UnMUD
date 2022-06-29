#include <sys/file.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include "pflags.h"
#include "kernel.h"

#include "sflags.h"

#include "uaf.h"

#include "mobile.h"
#include "flags.h"

#include <errno.h>



void get_gender(char *gen);

extern int errno;

extern char *WizLevels[];

time_t time(time_t *v);


void pers2player(PERSONA *d,int plx)
{
  setpstr(plx,d->p_strength);
  setpdam(plx,d->p_damage);
  setphome(plx,d->p_home);
  setpscore(plx,d->p_score);
  setparmor(plx,d->p_armor);
  setsflags(plx,d->p_sflags);
  setpflags(plx,d->p_pflags);
  setpmask(plx,d->p_mask);
  setqflags(plx,d->p_quests);
  setpvis(plx, ptstflg(plx, PFL_STARTINVIS) ? d->p_vlevel : 0);
  setplev(plx,d->p_level);
  setpwimpy(plx, d->p_wimpy);
  mob_id(plx) = d->p_id;
  if (plx < max_players) {
    setptitle(plx,d->p_title);
    (void)strcpy(players[plx].passwd,d->p_passwd);
  }
}

void player2pers(PERSONA *d,time_t *last_on,int plx)
{
  d->p_strength = pstr(plx);
  d->p_damage   = pdam(plx);
  d->p_home     = phome(plx);
  d->p_score    = pscore(plx);
  d->p_armor    = parmor(plx);
  d->p_sflags   = sflags(plx);
  d->p_pflags   = pflags(plx);
  d->p_mask     = pmask(plx);
  d->p_quests   = qflags(plx);
  d->p_vlevel   = pvis(plx);
  d->p_level    = plev(plx);
  d->p_damage   = pdam(plx);
  d->p_armor    = parmor(plx);
  d->p_wimpy    = pwimpy(plx);
  d->p_id       = mob_id(plx);
  if (plx < max_players) {
    (void)strcpy(d->p_title,ptitle(plx));
/*  bcopy(players[plx].passwd,d->p_passwd,PASSWD_LEN); */
    (void)strcpy(d->p_passwd,players[plx].passwd);
  }
  (void)strcpy(d->p_name,pname(plx));
  if (last_on != NULL) d->p_last_on = *last_on;
}

void get_gender(char *gen)
{
  Boolean ok = False;
  Boolean female = False;
  PERSONA d;

  if (gen == NULL) {
    replace_input_handler(get_gender);
  } else if (*gen == 'M' || *gen == 'm') {
    sclrflg(mynum,SFL_FEMALE);
    ok = True;
  } else if (*gen == 'F' || *gen == 'f') {
    ssetflg(mynum,SFL_FEMALE);
    ok = female = True;
  } else {
    bprintf( "M or F" );
  }
  if (ok) {
    /* initialize a very new user */
    if (OPERATOR(pname(mynum))) {
      /* We make him a god */
      sprintf(ptitle(mynum), "%%s the %s", WizLevels[LEV_GOD]);
      setplev(mynum,LVL_GOD);
      setpstr(mynum,pmaxstrength(LVL_GOD));
      setpscore(mynum,levels[LVL_WIZARD]);
      update_wizlist(pname(mynum),LEV_GOD);
    } else {
      sprintf(ptitle(mynum), "%%s the %s",
	      (female ? FLevels : MLevels)[LVL_NOVICE]);
      setplev(mynum,LVL_NOVICE);
      setpstr(mynum,40);
      setpscore(mynum,0);
    }
    setpwimpy(mynum, 0);
    mob_id(mynum) = id_counter++;
    setphome(mynum,0);
    setpdam(mynum,8);
    setparmor(mynum,0);
    setpvis(mynum,0);
    setsflags(mynum,0);
    if (female) ssetflg(mynum,SFL_FEMALE);
    setqflags(mynum,0);
    setpflgh(mynum,0);
    setpflgl(mynum,0);
    setpmskh(mynum,0);
    setpmskl(mynum,0);
    set_xpflags(plev(mynum),&(pflags(mynum)), &(pmask(mynum)));

    if (female)
      xsetbit(d.p_sflags,SFL_FEMALE);

    player2pers(&d, &global_clock, mynum);
    putuaf(&d);
    save_id_counter();
    do_motd(NULL);
  } else {
    bprintf( "\n");
    bprintf( strcpy( cur_player->cprompt, "Sex (M/F) : >"));
  }
}

void saveme(void)
{
  PERSONA d;

  if (cur_player->aliased || cur_player->polymorphed >= 0) {
    bprintf( "Not while aliased.\n");
    return;
  }

  player2pers(&d, &global_clock, mynum);
  bprintf( "\nSaving %s\n", pname(mynum));
  bflush();
  putuaf(&d);
}

/*
** This procedure tries to locate an uaf record in the uaf file.
** If we find the record, we return and the file position is just
** past the record we found.
*/
Boolean finduaf(char *name,PERSONA *d,int fd)
{
  unsigned long int x = 0;

  while (read(fd, (char *)d, sizeof(PERSONA)) == sizeof(PERSONA)) {
    if (x == 0 && EMPTY(d->p_name)) x = lseek(fd,0L,SEEK_CUR);
    if (EQ(d->p_name,name)) {
      lseek(fd,(long) -sizeof(PERSONA),SEEK_CUR);
      return True;
    }
  }
  if (x != 0) {
    lseek(fd,(long)(x - sizeof(PERSONA)),SEEK_SET);
  }
  return False;
}

/*
** This procedure gets one uaf record
** from the system
** return 'True' if a record with the specified name is found and
** d is filled with the specified info.
** return 'False' if record could not be found and d do not contain.
** any meaningful data.
**
*/
Boolean getuaf(char *name,PERSONA *d)
{
  int fd;
  Boolean b = False;

  if ((fd = open(UAF_RAND, O_RDONLY,0)) >= 0) {
    b = finduaf(name,d,fd);
    close(fd);
  } else if (errno != ENOENT) { /* UAF_RAND file doesn't exist */
    mudlog("Error in getuaf for open");
    progerror("getuaf/open");
  }
  return b;
}

void putuaf(PERSONA *d)
{
  int fd;
  PERSONA x;

  /* Find if he is there already */

  if ((fd = open(UAF_RAND, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR)) < 0) {
    mudlog("putuaf: Error in open");
    progerror("putuaf/open");
    exit(1);
  }
  lseek(fd,0L,SEEK_SET);
  (void)finduaf(d->p_name,&x,fd);
  if (write(fd,(char *)d, sizeof(PERSONA)) != sizeof(PERSONA)) {
    mudlog("Error in putuaf for write, UAF_RAND = " UAF_RAND);
    progerror("putuaf/write");
    exit(1);
  }
  lseek(fd,0L,SEEK_END);
  close(fd);
}


#if 0
/* Construct the name of the file where a players data will be stored.
 */
static char *player_filename(char *buff, char *player)
{
	sprintf(buff, "%s/%s", UAF_RAND, player);
	return buff;
}

static char *uaf_tbl[] = {   "Name", "Title", "Passwd", "Level", "Score",
			     "Str",  "Vis",   "Damage", "Armor", "Wimpy",
			     "Home", "LastOn","ID",     "Sflags","Pflags",
			     "Mask", "Quests", TABLE_END };
#define UAF_NAME     0
#define UAF_TITLE    1
#define UAF_PASSWD   2
#define UAF_LEVEL    3
#define UAF_SCORE    4
#define UAF_STR      5
#define UAF_VIS      6
#define UAF_DAMAGE   7
#define UAF_ARMOR    8
#define UAF_WIMPY    9
#define UAF_HOME    10
#define UAF_LAST_ON 11
#define UAF_ID      12
#define UAF_SFLAGS  13
#define UAF_PFLAGS  14
#define UAF_MASK    15
#define UAF_QUESTS  16


void putuaf(PERSONA *d)
{
	char buff[256];

	char *filename = player_filename(buff, d->p_name);
	FILE *f = fopen(filename, "w");

	if (f == NULL) {
		progerror("putuaf/fopen");
		exit(1);
	}

	if ( fprintf(f, "%s: %s\n"  "%s: %s\n"  "%s: %s\n"  "%s: %d\n"
		        "%s: %d\n"  "%s: %d\n"  "%s: %d\n"  "%s: %d\n"
		        "%s: %d\n"  "%s: %d\n"  "%s: %ld\n" "%s: %ld\n"
		        "%s: %ld\n"

		        "%s: %ld\n"
		        "%s: %ld:%ld\n" "%s: %ld:%ld\n" "%s: %ld\n",

		        uaf_tbl[UAF_NAME],   d->p_name,
		        uaf_tbl[UAF_TITLE],  d->p_title,
		        uaf_tbl[UAF_PASSWD], d->p_passwd,
		        uaf_tbl[UAF_LEVEL],  d->p_level,
		        uaf_tbl[UAF_SCORE],  d->p_score,
		        uaf_tbl[UAF_STR],    d->p_strength,
		        uaf_tbl[UAF_VIS],    d->p_vlevel,
		        uaf_tbl[UAF_DAMAGE], d->p_damage,
		        uaf_tbl[UAF_ARMOR],  d->p_armor,
		        uaf_tbl[UAF_WIMPY],  d->p_wimpy,
		        uaf_tbl[UAF_HOME],   d->p_home,
		        uaf_tbl[UAF_LAST_ON],d->p_last_on,
		        uaf_tbl[UAF_ID],     d->p_id,

		        uaf_tbl[UAF_SFLAGS], d->p_sflags,
		        uaf_tbl[UAF_PFLAGS], d->p_pflags.h, d->p_pflags.l,
		        uaf_tbl[UAF_MASK],   d->p_mask.h, d->p_mask.l,
		        uaf_tbl[UAF_QUESTS], d->p_quests) == EOF) {

		progerror("putuaf/fprintf");
		unlink(filename);
		exit(1);
	}

	fclose(f);
}

Boolean getuaf(char *name, PERSONA *d)
{
	char filename[256];
	char buff[256];
	Boolean ret_val = True;
	int s;

	player_filename(filename, name);

	FILE *f = fopen(filename, "r");

	if (f == NULL) return False;


	while ( (s = fscanf("%s:", buff)) == 1 ) {

		switch( tlookup(buff, uaf_tbl) ) {
		      case UAF_NAME:   s = fscanf(f, "%s", d->p_name); break;
		      case UAF_TITLE:  s = fscanf(f, "%s, d->p_title); break;
		      case UAF_PASSWD: s = fscanf(f, "%s, d->p_passwd); break;
		      case UAF_LEVEL:  s = fscanf(f, "%d", d->p_level); break;
		      case UAF_SCORE:  s = fscanf(f, "%d", d->p_score); break;
		      case UAF_STR:  s = fscanf(f, "%d", d->p_strength); break;
		      case UAF_VIS:    s = fscanf(f, "%d", d->p_vlevel);break;
		      case UAF_DAMAGE: s = fscanf(f, "%d", d->p_damage);break;
		      case UAF_ARMOR:  s = fscanf(f, "%d", d->p_armor); break;
		      case UAF_WIMPY:  s = fscanf(f, "%d", d->p_wimpy); break;
		      case UAF_HOME:   s = fscanf(f, "%ld", d->p_home); break;
		      case UAF_LAST_ON:s =fscanf(f, "%ld",d->p_last_on);break;
		      case UAF_ID:     s = fscanf(f, "%ld", d->p_id); break; 
		      case UAF_SFLAGS: s = fscanf(f, "%ld", d->p_sflags);break;
		      case UAF_PFLAGS: s = fscanf(f, "%ld:%ld", d->p_pflags.h,
						  d->p_pflags.l); break;

		      case UAF_MASK:   s = fscanf(f, "%ld:%ld", d->p_mask.h,
						  d->p_mask.l); break;
		      case UAF_QUESTS: s = fscanf(f, "%ld", d-p_quests); break;
		      case -1:
			mudlog("Error: getuaf: string %s for %s", buff, name);
			ret_val = False;
		}

		if (s == 0 || s == EOF) {
			mudlog("Error: getuaf: s = %d", s);
			ret_val = False;
		}
	}

	if (s != EOF) {
		mudlog("Error: getuaf: fscanf() returned %d for %s", s, name);
		ret_val = False;
	}

	fclose(f);
	return ret_val;
}

void deluaf(char *name)
{
	char buff[256];

	unlink( player_filename(buff, name) );
}
#endif



void deluaf(char *name)
{
  int fd;
  PERSONA p;

  if ((fd = open(UAF_RAND, O_RDWR, 0)) < 0) {
    if (errno == ENOENT) return;
    mudlog("Error for fopen(\"" UAF_RAND "\",\"r+\")");
    progerror("deluaf/open");
    exit(1);
  }

  if (finduaf(name, &p, fd)) {
    p.p_name[0] = '\0';
    if (write(fd,(char *)&p, sizeof(PERSONA)) != sizeof(PERSONA)) {
      progerror("deluaf/write");
      exit(1);
    }
  }
  lseek(fd,0L,SEEK_END);
  close(fd);
}


Boolean getuafinfo(char *name)
{
  PERSONA d;
  Boolean b;

  b = getuaf(name,&d);
  if (b) {
    pers2player(&d,mynum);
    setpname(mynum, d.p_name);
  }
  return b;
}


Boolean findsetins( char *name, SETIN_REC *s, int fd)
{
  while (read(fd, s, sizeof(SETIN_REC)) == sizeof(SETIN_REC))
    if (EQ(s->name,name)) {
      return True;
    }
  return False;
}

Boolean getsetins( char *name, SETIN_REC *s )
{
  int     fd;
  Boolean b;

  if ((fd = open(SETIN_FILE, O_RDONLY, 0)) < 0)
    return False;

  b = findsetins(name,s,fd);
  close(fd);
  return b;
}

void putsetins( char *name, SETIN_REC *s)
{
  SETIN_REC v;
  int       fd;
  FILE      *f;
  Boolean   b;

  if ((fd = open(SETIN_FILE, O_RDWR|O_CREAT,S_IRUSR|S_IWUSR)) < 0) {
    mudlog("putsetins: Error in open for " SETIN_FILE);
    progerror("open");
    exit(1);
  }
  if (b = findsetins(name,&v,fd)) {
    lseek(fd,(long)-sizeof(SETIN_REC),SEEK_CUR);
  } else {
    lseek(fd,0L,SEEK_END);
  }
  if (write(fd, s,sizeof(SETIN_REC)) != sizeof(SETIN_REC)) {
    mudlog("putsetins: Error in write for " SETIN_FILE);
    progerror("write");
  }
  lseek(fd,0L,SEEK_END);
  close(fd);
}


void fetchprmpt(int plr)
{
  SETIN_REC s;

  if (plr >= max_players || plr < 0) return;
  if (plev(plr) >= LVL_WIZARD && getsetins(pname(plr), &s)) {
    strcpy(players[plr].prompt,  s.prompt);
    strcpy(players[plr].setin,   s.setin);
    strcpy(players[plr].setout,  s.setout);
    strcpy(players[plr].setmin,  s.setmin);
    strcpy(players[plr].setmout, s.setmout);
    strcpy(players[plr].setvin,  s.setvin);
    strcpy(players[plr].setvout, s.setvout);
    strcpy(players[plr].setqin,  s.setqin);
    strcpy(players[plr].setqout, s.setqout);
  } else {
    strcpy(players[plr].prompt,  ">");
    strcpy(players[plr].setin,   "%n has arrived.");
    strcpy(players[plr].setout,  "%n has gone %d.");
    strcpy(players[plr].setmin,  "%n appears with an ear-splitting bang.");
    strcpy(players[plr].setmout, "%n vanishes in a puff of smoke.");
    strcpy(players[plr].setvin,  "%n suddenly appears!");
    strcpy(players[plr].setvout, "%n has vanished!");
    strcpy(players[plr].setqin,  "%n has entered the game.");
    strcpy(players[plr].setqout, "%n has left the game.");
  }
}



char *build_setin(char *b, char *s, char *n, char *d)
{
  char *p, *q, *r;

  for (p = b, q = s; *q != 0;) {
    if (*q != '%')
      *p++ = *q++;
    else {
      switch (*++q) {
      case 'n': for (r = n; *r != 0;) *p++ = *r++; break;
      case 'd':
        if (d == NULL) return NULL;
        for (r = d; *r != 0;) *p++ = *r++;
        break;
      case 'N': for (r = xname(n); *r != 0;) *p++ = *r++; break;
      case 'f':
        for (r = (psex(mynum)?"her":"his"); *r != 0;) *p++ = *r++;
        break;
      case 'F':
        for (r = (psex(mynum)?"her":"him"); *r != 0;) *p++ = *r++;
        break;
      case 0: --q; break;
      default: *p++ = *q;
      }
      ++q;
    }
  }
  if (p[-1] == '\n') --p;
  *p = 0;
  return b;
}



