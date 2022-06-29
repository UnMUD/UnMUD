/*  This module handles all the Input/Output of the system */

#include "kernel.h"
#include "sflags.h"
#include "pflags.h"
#include "lflags.h"
#include "bprintf.h"
#include "mud.h"
#include "mobile.h"

#ifdef VARGS
#include <stdarg.h>
#endif

static void kiputs (char *string, FILE *file);
static void dcprnt (char *ct, FILE *file);

/* 
** Output Functions
**
*/

#define SYSBUFSIZE 4096
#define MAX_ARGS   2

#define prnt(x, f) if ((x).nest) dcprnt((x).str, f); else kiputs((x).str, f);

static char *bufptr, *sysbuf;
static int buflen;
static int snooplev = 0;
static Boolean new_line = True; /* True when next char is first in a line */

struct arg
{
  char *str;
  int nest;
};

static void pansi (struct arg [], FILE *);
static void pcls (struct arg [], FILE *);
static void ppnblind (struct arg [], FILE *);
static void pfilter (struct arg [], FILE *);
static void pryou (struct arg [], FILE *);
static void pryour (struct arg [], FILE *);
static void ppndeaf (struct arg [], FILE *);
static void pndark (struct arg [], FILE *);
static void pndeaf (struct arg [], FILE *);
static void pfile (struct arg [], FILE *);
static void prname (struct arg [], FILE *);
static void pcansee (struct arg [], FILE *);

struct _code
{
  char name;
  int args;
  void (*func)(struct arg [], FILE *);
};

static struct _code codes[] =
  {{'A', 1, pansi   },
   {'C', 0, pcls    },
   {'D', 1, ppnblind},
   {'F', 1, pfilter },
   {'N', 1, pryou   },
   {'P', 1, ppndeaf },
   {'c', 1, pndark  },
   {'d', 1, pndeaf  },
   {'f', 1, pfile   },
   {'n', 1, pryour  },
   {'p', 1, prname  },
   {'s', 2, pcansee }};

#define NUM_CODES (sizeof codes / sizeof(struct _code))

static void start_line(FILE *file)
{
  int i;

  if (new_line && snooplev > 0) {
    for (i = 0; i < snooplev; i++) {
      putc('|',file);
    }
  }
}

static void kiputs(char *s,FILE *file)
{
  char *t;

  start_line(file);
  while ((t = strchr(s, '\n')) != NULL) {
    *t = '\0';
    fputs(s,file);
    putc('\n', file);
    *t = '\n';
    new_line = True;
    start_line(file);
    s = t + 1;
  }
  if (*s != '\0') {
    fputs(s,file);
    new_line = False;
  }
}


static void makebfr()
{
  if ((bufptr = sysbuf = NEW(char, SYSBUFSIZE+BUFSIZ)) == NULL) {
    mudlog("ERROR: Out of Memory!");
    _exit(1);
  }
  sysbuf[0] = 0;
  buflen = 0;
}


static int _code_cmp (const void *c, const void *code)
{
  return (*((const char *)c) - ((const struct _code *)code)->name);
}

static int tocontinue(char **ct)
{
  register char *s = *ct;
  register int n = 1;
  register int nest = 0;
  struct _code *code;

  for (; n; s++) {
    switch (*s) {
      case '\002':
      case '\003':
        --n;
        break;
      case '\001':
	if (*++s != '\001') {
	  code = (struct _code *) bsearch(s, (char *)codes, NUM_CODES,
					  sizeof(struct _code), _code_cmp);
	  if (code == NULL) {
	    mudlog("ERROR: tocontinue(): Unknown control code %3o",
		   0377 & *s);
	    _exit(2);
	  }
	  n += code->args;
	  nest = 1;
	}
	break;
      case '\0':
        mudlog("ERROR: tocontinue(): Buffer overrun\n");
	mudlog("txt:%s", *ct);
        _exit(2);
        break;
    }
  }

  *ct = s;
  return(nest);
}


static void dcprnt (char *ct, FILE *file)
{
  char *str;
  char bk[100];
  struct arg args[MAX_ARGS+1];
  struct _code *code;
  int n;

  while (*(str = ct)) {
    if ((ct = strchr(str, '\001')) == NULL) {
      kiputs(str, file);
      break;
    }
    *ct = '\0';
    kiputs(str, file);
    *ct++ = '\001';

    if (*ct == '\001') {
      kiputs("\001", file);
      ct++;
    } else {
      code = (struct _code *)bsearch((char *)ct, (char *)codes, NUM_CODES,
				     sizeof(struct _code), _code_cmp);
      if (code == NULL) {
	mudlog("ERROR: dcprnt(): Unknown control code %c (%3o) \n",
	       *ct, 0377 & *ct);
	mudlog("ct[0..19] = \"%s\".", mk_string(bk,ct,20,-1));
	_exit(2);
      } else {
	args[0].str = ++ct;

	for (n = 1; n <= code->args; n++) {
	  args[n-1].nest = tocontinue(&ct);
	  ct[-1] = '\0';
	  args[n].str = ct;
	}

	code->func(args, file);
	for (n = 1; n <= code->args; n++)
	  args[n].str[-1] = '\003';
      }
    }
  }
}

static void pfilter(struct arg *args, FILE *file)
{
  FILE *a;
  char x[BUFSIZ];

  if ((a = popen(args[0].str, "r")) == NULL)
    fprintf(stderr, "[Cannot find filter ->%s]\n", args[0].str);
  else {
    while (fgets(x, sizeof x, a))
      kiputs(x, file);
    pclose(a);
  }
}


static void pfile(struct arg *args, FILE *file)
{
  FILE *a;
  char x[BUFSIZ];

  if ((a = fopen(args[0].str, "r")) == NULL)
    fprintf(stderr, "[Cannot find file ->%s]\n", args[0].str);
  else {
    while (fgets(x, sizeof(x), a))
      kiputs(x, file);
    fclose(a);
  }
}


static void pndeaf(struct arg *args, FILE *file)
{
  if (!ststflg(mynum, SFL_DEAF))
    prnt(args[0], file);
}

static void pcls(struct arg *args, FILE *file)
{
#if 0
  if (ststflg(mynum, SFL_CLS))
    prnt("\033[2J", file); /* Ansi clear screen sequence */
#endif
}


static void pcansee(struct arg *args, FILE *file)
{
  int a;

  a = fpbns(args[0].str);
  if (seeplayer(a) && (a != -1))
    prnt(args[1], file);
}


static void prname(struct arg *args, FILE *file)
{
  kiputs(seeplayer(fpbns(args[0].str)) ? args[0].str : "Someone", file);
}

static void pryou (struct arg *args, FILE *file)
{
  kiputs((fpbns(args[0].str) == mynum) ? "you" : args[0].str, file);
}

/*
  Prints "your" if player is the receiver, otherwise prints the
  possessive form of the receiver's name
*/

static void pryour (struct arg *args, FILE *file)
{
  if (fpbns(args[0].str) == mynum)
    kiputs("your", file);
  else
    {
      kiputs(args[0].str, file);
      kiputs("'s", file);
    }
}


static void pndark(struct arg *args, FILE *file)
{
  if ((!isdark()) && (!ststflg(mynum, SFL_BLIND)))
    prnt(args[0], file);
}

static void pansi(struct arg *args, FILE *file)
{
  if (cur_player->iamon && ststflg(mynum, SFL_COLOR))
    prnt(args[0], file);
}

static void ppndeaf(struct arg *args, FILE *file)
{
  if (!ststflg(mynum, SFL_DEAF)) prname(args,file);
}

static void ppnblind(struct arg *args, FILE *file)
{
  if (!ststflg(mynum, SFL_BLIND)) prname(args,file);
}

void print_buf(char *b,Boolean notself)
{
  int plr;
  int ct = 0;
  int me = real_mynum;
  Boolean n1, n2;

  if (cur_player->in_pbfr) return;
  cur_player->in_pbfr = True;
  n1 = new_line;
  if (!notself)
    dcprnt(b, cur_player->stream);
  n2 = new_line;
  if (cur_player->iamon && ploc(mynum) < 0 && !EMPTY(pname(mynum)) &&
      cur_player->snooped > 0) {
    ++snooplev;
    for (plr = 0; plr < max_players; plr++) {
      if (!is_in_game(plr)) continue;
      if (players[plr].snooptarget == me) {
	++ct;
	xsetup_globals(plr);
	new_line = n1;
	print_buf(b, False);
	fflush(cur_player->stream);
      }
    }
    xsetup_globals(me);
    new_line = n2;
    --snooplev;
    if (ct != cur_player->snooped) {
      mudlog("ERROR: Internal error, snooped = %d, ct = %d, check print_buf",
	     cur_player->snooped, ct);
    }
  }
  cur_player->in_pbfr = False;
}
  
void pbfr(void)
{
  FILE *fln;

  if (sysbuf == NULL) {
    makebfr();
  }

  if (buflen > 0 && cur_player != NULL && cur_player->stream != NULL) {
    print_buf(sysbuf, False);
    sysbuf[0] = 0;            /* clear buffer */
    bufptr = sysbuf;
    buflen = 0;
    new_line = True;
  }
}
 
void bflush(void)
{ 
  if (cur_player != NULL && cur_player->stream != NULL) {
    pbfr();
    fflush(cur_player->stream);
  }
} 


void snoopcom(void)
{
  int plr;

  if (!ptstflg(mynum, PFL_SNOOP)) {
    erreval();
    return;
  }
  if (cur_player->snooptarget >= 0) {
    bprintf("Stopped snooping on %s.\n", pname(cur_player->snooptarget));

    snoop_off(mynum);
#if 0
    --(players[plr].snooped); /* One less to snoop him */
    cur_player->snooptarget = -1;
#endif
  }

  if ((plr = pl1) < 0) {
    return;
  } else if (plr >= max_players) {
    bprintf("You can't snoop %s!\n", pname(plr));
    return;
  } else if (plr == mynum) {
    bprintf("You can't snoop yourself!\n" );
    return;
  }

  if (! do_okay( mynum, plr, PFL_NOSNOOP)) {
    bprintf("Your magical vision is obscured.\n");
    return;
  }
  /* Is this a PRIVATE room?  If so don't let them snoop. */
  if (ltstflg(ploc(plr), LFL_PRIVATE) && plev(mynum) < LVL_ARCHWIZARD) {
    bprintf("I'm sorry, %s, but the room is private.\n",
	    psex(mynum) ? "Madam" : "Sir");
    return;
  }

  cur_player->snooptarget = plr;
  ++(players[plr].snooped); /* One more to snoop him */

  bprintf("Started to snoop on %s.\n", pname(plr));
}

/* Make player no longer snoop his target
 */
void snoop_off(int plr)
{
  int target;

  if ((target = players[plr].snooptarget) >= 0) {

    players[plr].snooptarget = -1;

    /* One less to snoop him: */
    if (--players[target].snooped < 0)  players[target].snooped = 0;
  }
}


#ifdef VARGS

void bprintf(char *format, ...)
{
  va_list pvar;
  register int len;

  if(!sysbuf)
    makebfr();

  if (cur_player == NULL) {
    return;
    va_start(pvar,format);
    vprintf(format, pvar);
    va_end(pvar);
  } else if (cur_player->stream == NULL) {
    return;
  } else {
    va_start(pvar,format);
    vsprintf(bufptr, format, pvar);
    len = strlen(bufptr);
    buflen += len;
    va_end(pvar);

    if (buflen >= SYSBUFSIZE)
      pbfr();
    else
      bufptr += len;
  }
}
#else

void
bprintf(char *format, char *a1, char *a2, char *a3, char *a4, char *a5,
        char *a6, char *a7, char *a8, char *a9)
{
  register int len;

  if (!sysbuf)
    makebfr();

  if (cur_player == NULL) {
    printf( format, a1, a2, a3, a4, a5, a6, a7, a8, a9);
  } else {
    sprintf(bufptr, format, a1, a2, a3, a4, a5, a6, a7, a8, a9);
    len = strlen(bufptr);
    buflen += len;
    if (buflen >= SYSBUFSIZE)
      pbfr();
    else
      bufptr += len;
  }
}
#endif




