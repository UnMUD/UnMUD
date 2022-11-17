#include "kernel.h"
#include "stdinc.h"
#ifdef VARGS
#include <stdarg.h>
#endif

/* Simple interfaces: */
void broad(char *mesg)
{
  sendf(DEST_ALL, "%s", mesg);
}

void sillycom(char *txt)
{
  send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, NOBODY,
           txt, pname(mynum), pname(mynum));
}

void sillytp(int per, char *msg)
{
  sendf(per, "\001p%s\003 %s\n", pname(mynum), msg);
}

static void ___send_msg(int to,
                        char *text)
{
  /* send a message to 'to', supposedly from 'from'. 'from' doesn't have
   * to be an actual player or mobile in the game though.
   */

  /* Note that mynum, cur_player, etc is modified in this function. */
  /* This will cause pbfr() to be called. */

  char str[32];
  setup_globals(to);
  if (pstr(mynum) != 0)
    sprintf(str, "&+W[&+%s%d&+W]", (maxstrength(mynum) / pstr(mynum) >= 3) ? "R" : (maxstrength(mynum) / pstr(mynum) >= 2) ? "Y"
                                                                                                                           : "W",
            pstr(mynum));
  else
    sprintf(str, "&+W[&+R0&+W]");

  if (ststflg(mynum, SFL_COMPACT))
    bprintf("\r%s\r&+W%s%s&*%s%s&+W%s&*", text, pvis(mynum) ? "(" : "",
            pfighting(mynum) >= 0 ? str : "",
            cur_player->aliased ? "@" : "", cur_player->prompt,
            pvis(mynum) ? ")" : "");
  else
    bprintf("\n%s\r&+W%s%s&*%s%s&+W%s&*", text, pvis(mynum) ? "(" : "",
            pfighting(mynum) >= 0 ? str : "",
            cur_player->aliased ? "@" : "", cur_player->prompt,
            pvis(mynum) ? ")" : "");
}

/*
 * Values for destination:
 * negative values:            room number.
 * 0..max_players - 1          player with specified index.
 * max_players..numchars - 1   mobile with specified index.
 * DEST_ALL                    all players and mobiles.
 */

static Boolean test_rcv(int player, /* Who to send to */
                        int mode,   /* Flags to control sending */
                        int min,    /* Minimum level of recipient */
                        int max,    /* Maximum level of recipient */
                        int x1,     /* Do not send to him */
                        int x2)     /* Nor to him */
{
  Boolean b;
  if (player == x1 || player == x2)
    return False;
  b = (plev(player) >= min && plev(player) < max);
  if (mode & MODE_NODEAF)
    b = b && !ststflg(player, SFL_DEAF);
  if (mode & MODE_NODUMB)
    b = b && !ststflg(player, SFL_DUMB);
  if (mode & MODE_NOCRIP)
    b = b && !ststflg(player, SFL_CRIPPLED);
  if (mode & MODE_QUIET)
    b = b && !ststflg(player, SFL_QUIET);
  if (mode & MODE_NOBATTLE)
    b = b && !ststflg(player, SFL_NOBATTLE);
  if (mode & MODE_NOSLAIN)
    b = b && !ststflg(player, SFL_NOSLAIN);
  if (mode & MODE_OUTDOORS)
    b = b && ltstflg(ploc(player), LFL_OUTDOORS);

  switch (mode & (MODE_PFLAG | MODE_NPFLAG))
  {
  case MODE_PFLAG:
    b = b && ptstflg(player, mode & MODE_FLAGS);
    break;
  case MODE_NPFLAG:
    b = b && !ptstflg(player, mode & MODE_FLAGS);
    break;
  }

  switch (mode & (MODE_SFLAG | MODE_NSFLAG))
  {
  case MODE_SFLAG:
    b = b && ststflg(player, (mode >> MODE_S) & MODE_FLAGS);
    break;
  case MODE_NSFLAG:
    b = b && !ststflg(player, (mode >> MODE_S) & MODE_FLAGS);
    break;
  }
  if (MODE_NEG & mode)
    return !b;
  return b;
}

/* Send general message.  */
void send_g_msg(int destination,                       /* Where to send to */
                char *func(int plx, int arg, char *t), /* Test function */
                int arg,                               /* Argument to test */
                char *text)                            /* Text to send */
{
  char *t;
  int i, p, q, me = real_mynum;

  if (destination >= numchars)
    return; /* Illegal value */
  if (func == NULL && text == NULL)
    return; /* Nothing to send */
  if (destination >= max_players)
  {
    /* a mobile, let us see if anyuone is aliasing it */

    if ((p = find_pretender(destination)) < 0)
      return;
    /* Someone is aliased/polymorphed as the mobile we're supposed
     * * to send to, send to him
     */
    if ((t = (func == NULL ? text : func(destination, arg, text))) != NULL)
    {
      ___send_msg(p, t);
    }
  }
  else if (destination >= 0)
  {
    /* A player, send to him */
    if (is_in_game(destination) &&
        (t = (func == NULL ? text : func(destination, arg, text))) != NULL)
    {
      if (players[destination].pretend < 0)
      {
        ___send_msg(destination, t);
      }
      if ((p = find_pretender(destination)) >= 0)
      {
        ___send_msg(p, t);
      }
    }
  }
  else if (destination == DEST_ALL)
  {
    for (p = 0; p < max_players; ++p)
    {
      if (is_in_game(p) &&
          (t = (func == NULL ? text : func(p, arg, text))) != NULL)
      {
        if (players[p].pretend < 0)
        {
          ___send_msg(p, t);
        }
        if ((q = find_pretender(p)) >= 0)
        {
          ___send_msg(q, t);
        }
      }
    }
    for (; p < numchars; ++p)
    {
      if (is_in_game(p) && pstr(p) >= 0 &&
          (t = (func == NULL ? text : func(p, arg, text))) != NULL &&
          (q = find_pretender(p)) >= 0)
      {
        ___send_msg(q, t);
      }
    }
  }
  else /* must be a specific location */
    for (i = 0; i < lnumchars(destination); i++)
      if (is_in_game(p = lmob_nr(i, destination)) &&
          (t = (func == NULL ? text : func(p, arg, text))) != NULL)
      {
        if (p < max_players)
        {
          if (players[p].pretend < 0)
            ___send_msg(p, t);
          if ((q = find_pretender(p)) >= 0)
            ___send_msg(q, t);
        }
        else if (is_in_game(p) && pstr(p) >= 0 && (q = find_pretender(p)) >= 0)
          ___send_msg(q, t);
      }
  setup_globals(me);
}

struct _send_msg_box
{
  int mode;
  int min;
  int max;
  int x1;
  int x2;
};

char *check_send_msg(int plx, int a, char *t)
{
  struct _send_msg_box *b = (struct _send_msg_box *)a;

  if (test_rcv(plx, b->mode, b->min, b->max, b->x1, b->x2))
    return t;
  return NULL;
}

#ifdef VARGS

void send_msg(int destination,   /* Where to send to */
              int mode,          /* Flags to control sending */
              int min,           /* Minimum level of recipient */
              int max,           /* Maximum level of recipient */
              int x1,            /* Do not send to him */
              int x2,            /* Nor to him */
              char *format, ...) /* Format with args -> text to send */
{
  va_list pvar;
  char bf[2048];
  char bf2[2048];
  struct _send_msg_box b;
  char *bb;

  b.mode = mode;
  b.min = min;
  b.max = max;
  b.x1 = x1;
  b.x2 = x2;
  bb = format;
  if ((mode & MODE_COLOR) != 0)
  {
    sprintf(bf2, "\001A\033[1;33m\003%s\001A\033[0m\003", format);
    bb = bf2;
  }
  va_start(pvar, format);
  vsprintf(bf, bb, pvar);
  va_end(pvar);
  send_g_msg(destination, check_send_msg, (int)&b, bf);
}

void sendf(int destination, char *format, ...)
{
  char b[2048];
  va_list pvar;

  va_start(pvar, format);
  vsprintf(b, format, pvar);
  va_end(pvar);
  send_g_msg(destination, NULL, 0, b);
}

void gsendf(int destination,
            char *func(int plx, int arg, char *text),
            int arg,
            char *format, ...)
{
  char b[2048];
  va_list pvar;

  va_start(pvar, format);
  vsprintf(b, format, pvar);
  va_end(pvar);
  send_g_msg(destination, func, arg, b);
}

#else

void send_msg(int destination, /* Where to send to */
              int mode,        /* Flags to control sending */
              int min,         /* Minimum level of recipient */
              int max,         /* Maximum level of recipient */
              int x1,          /* Do not send to him */
              int x2,          /* Nor to him */
              char *format,    /* Format with args -> text to send */
              int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8,
              int a9)
{
  struct _send_msg_box b;
  char *bb;
  char bf[2048];
  char bf2[2048];

  b.mode = mode;
  b.min = min;
  b.max = max;
  b.x1 = x1;
  b.x2 = x2;
  bb = format;
  if ((mode & MODE_COLOR) != 0)
  {
    sprintf(bf2, "\001A\033[1;33m\003%s\001A\033[0m\003", format);
    bb = bf2;
  }
  sprintf(bf, bb, a1, a2, a3, a4, a5, a6, a7, a8, a9);
  send_g_msg(destination, check_send_msg, (int)&b, bf);
}

void sendf(int destination, char *format, int a1, int a2, int a3, int a4, int a5,
           int a6, int a7, int a8, int a9)
{
  char b[2048];

  sprintf(b, format, a1, a2, a3, a4, a5, a6, a7, a8, a9);
  send_g_msg(destination, NULL, 0, b);
}

void gsendf(int destination,
            char *func(int plx, int arg, char *text),
            int arg,
            char *format, int a1, int a2, int a3, int a4, int a5, int a6, int a7,
            int a8, int a9)
{
  char b[2048];

  sprintf(b, format, a1, a2, a3, a4, a5, a6, a7, a8, a9);
  send_g_msg(destination, func, arg, b);
}
#endif
