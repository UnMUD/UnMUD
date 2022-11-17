#include <malloc.h>
#include <sys/param.h>
#include "kernel.h"
#include "stdinc.h"
#include "mobiles.h"
struct _ext_msg
{
  char code;  /* Codes EXT_TNAM..EXT_ONAM uses this form */
  char *next; /* Pointer to next part of string. */
};

struct _ext_msg1
{
  struct _ext_msg e;
  char *str; /* Code EXT_STRING uses this form */
};

struct _ext_msg2
{
  struct _ext_msg e; /* Codes EXT_TARGET and EXT_ACTOR uses this form */
  char *male_txt;
  char *female_txt;
};

/* code is one of the following: */
#define EXT_STRING 1 /* insert string here */
#define EXT_TARGET 2 /* Select string on psex(target) */
#define EXT_ACTOR 3  /* Select string on psex(actor) */
#define EXT_TNAM 4   /* insert pname(target) here */
#define EXT_ANAM 5   /* insert pname(actor) here */
#define EXT_THIM 6   /* Insert psex(target) ? "her" : "him" here */
#define EXT_AHIM 7   /* Insert psex(actor)  ? "her" : "him" here */
#define EXT_THIS 8   /* Insert psex(target) ? "her" : "his" here */
#define EXT_AHIS 9   /* Insert psex(actor)  ? "her" : "his" here */
#define EXT_ONAM 10  /* Insert oname(ob2) here if EXTF_OBJ */
#define EXT_MTXT 11  /* Insert user text via %s here */

#define EXT_ERROR ((char *)(-2)) /* Error return from various functions. */

/* struct for the external commands */
typedef struct _ext
{
  struct _ext *next;
  int flags;
  char *verb;
  char *msg_to_all;
  char *msg_to_me;
} EXTERN_CMD_REC;

/* These are in addition if the command accept a target */
struct _ext2
{
  EXTERN_CMD_REC e;
  char *msg_to_target;
  char *msg_to_sender;
  char *msg_to_others;
};

#define EXTF_ALL 0x01
#define EXTF_SINGLE 0x02
#define EXTF_HOSTILE 0x04
#define EXTF_ALOOF 0x08
#define EXTF_OBJ 0x10  /* Command takes an object as arg */
#define EXTF_TEXT 0x20 /* Command accepts a string at the end */
#define EXTF_FAR 0x40  /* Target may be in other room */

static void ext_free_msg(char *s);
static int ext_get_tok(void);
static char *ext_get_itm(void);
static char *ext_get_string(void);
static char *ext_get_simple(int code);
static char *ext_get_mf(int code, int middle, int end,
                        int fla_m, int fla_c);
static struct _ext_msg *ext_mk_struct(char *s);
static char *ext_get_anything(void);
static char *ext_get_msg(int *w, FILE *f, int bsiz, int l, int h,
                         int *mem_used);
static char *build_action(char *a);

#define EXT_MSG_ALL 0
#define EXT_MSG_ME 1
#define EXT_MSG_TARGET 2
#define EXT_MSG_SENDER 3
#define EXT_MSG_OTHERS 4

#define EXT_ANGLE_OPEN 0x100   /* %< ok? */
#define EXT_ANGLE_MIDDLE 0x200 /* %: ok? */
#define EXT_ANGLE_CLOSE 0x400  /* %> ok? */
#define EXT_SQ_MIDDLE 0x800    /* %/ ok? */
#define EXT_SQ_CLOSE 0x1000    /* %] ok? */
#define EXT_HAT 0x2000         /* %^ or %~ ok? */
#define EXT_T 0x2000           /* %t ok? */
#define EXT_O 0x4000           /* %o ok? */
#define EXT_M 0x8000           /* %m ok? */

static char *ext_msg_names[] =
    {"all", "me", "target", "sender", "others",
     TABLE_END};

EXTERN_CMD_REC *externs;
static char *buffer;
static char *bufp;
static int cur_tok;
static int ext_who;
static int mused;
static int ext_lino;
static int ext_flags;

static char *s_ext[] =
    {"Flowers", "Pet", "Pray", "Tickle",
     "Wave", TABLE_END};

/*
 * Function to free a msg.
 */

static void
ext_free_msg(char *s)
{
  if (s == NULL || s == EXT_ERROR)
    return;
  if (*s < ' ' || *s > '~')
  {
    switch (((struct _ext_msg *)s)->code)
    {
    case EXT_STRING:
      ext_free_msg(((struct _ext_msg1 *)s)->str);
      break;
    case EXT_TARGET:
    case EXT_ACTOR:
      ext_free_msg(((struct _ext_msg2 *)s)->male_txt);
      ext_free_msg(((struct _ext_msg2 *)s)->female_txt);
      break;
    case EXT_TNAM:
    case EXT_ANAM:
    case EXT_THIM:
    case EXT_AHIM:
    case EXT_THIS:
    case EXT_AHIS:
    case EXT_ONAM:
      break;
    default:
      mudlog("Illegal extern code %d\n", ((struct _ext_msg *)s)->code);
      return; /* Don't free anything */
    }
    ext_free_msg(((struct _ext_msg *)s)->next); /* Free the next part */
  }
  free(s);
}

static int
ext_get_tok(void)
{
  int c;
  int k;

  if ((c = *bufp++) == '%')
  {
    switch (c = *bufp++)
    {
    case 'A':
      c = 'a';
    case 'a':
    case '[':
      return cur_tok = 0400 + c;
    case '%':
      return cur_tok = c;
    case '<':
      k = EXT_ANGLE_OPEN;
      break;
    case ':':
      k = EXT_ANGLE_MIDDLE;
      break;
    case '>':
      k = EXT_ANGLE_CLOSE;
      break;
    case '~':
    case '^':
      k = EXT_HAT;
      break;
    case 'T':
      c = 't';
    case 't':
      k = EXT_T;
      break;
    case '/':
      k = EXT_SQ_MIDDLE;
      break;
    case ']':
      k = EXT_SQ_CLOSE;
      break;
    case 'O':
      c = 'o';
    case 'o':
      k = EXT_O;
      break;
    case 'M':
      c = 'm';
    case 'm':
      k = EXT_M;
      break;
    default:
      k = 0;
    }
    if (k != 0 && (ext_flags & k) != 0)
      return cur_tok = c + 0400;
    c = '%';
    --bufp;
  }
  if (c < ' ' || c > '~')
    c = 0;
  return cur_tok = c;
}

static char *
ext_get_itm(void)
{
  switch (cur_tok)
  {
  case 0400 + '<':
    return ext_get_mf(EXT_TARGET, ':', '>', EXT_ANGLE_MIDDLE, EXT_ANGLE_CLOSE);
  case 0400 + '[':
    return ext_get_mf(EXT_ACTOR, '/', ']', EXT_SQ_MIDDLE, EXT_SQ_CLOSE);
  case 0400 + '/':
  case 0400 + ':':
  case 0400 + '>':
  case 0400 + ']':
  case 0:
    return NULL;
  case 0400 + '^':
    return ext_get_simple(ext_who == EXT_MSG_SENDER ? EXT_THIS : EXT_AHIS);
  case 0400 + '~':
    return ext_get_simple(ext_who == EXT_MSG_SENDER ? EXT_THIM : EXT_AHIM);
  case 0400 + 'a':
    return ext_get_simple(EXT_ANAM);
  case 0400 + 't':
    return ext_get_simple(EXT_TNAM);
  case 0400 + 'o':
    return ext_get_simple(EXT_ONAM);
  case 0400 + 'm':
    ext_flags &= ~EXT_M;
    return ext_get_simple(EXT_MTXT);
  }
  return ext_get_string();
}

static char *
ext_get_string(void)
{
  char b[256];
  int c = cur_tok;
  int ct = 0;

  while (c >= ' ' && c <= '~')
  {
    b[ct++] = c;
    c = ext_get_tok();
  }
  if (ct == 0)
    return NULL;
  mused += ct + 1;
  b[ct] = '\0';
  return COPY(b);
}

static char *
ext_get_simple(int code)
{
  struct _ext_msg *m;

  (void)ext_get_tok();
  m = NEW(struct _ext_msg, 1);
  m->code = code;
  m->next = NULL;
  mused += sizeof(struct _ext_msg);
  return (char *)m;
}

static char *
ext_get_mf(int code, int middle, int end, int fla_m, int fla_c)
{
  struct _ext_msg2 *s;
  char *m, *f;
  int flags = ext_flags;

  (void)ext_get_tok();
  ext_flags |= fla_m | fla_c;
  if ((m = ext_get_anything()) == EXT_ERROR)
  {
    ext_flags = flags;
    return EXT_ERROR;
  }
  f = NULL;
  ext_flags = flags;
  if (cur_tok == 0400 + middle)
  {
    ext_flags |= fla_c;
    (void)ext_get_tok();
    if ((f = ext_get_anything()) == EXT_ERROR)
    {
      ext_free_msg(m);
      ext_flags = flags;
      return EXT_ERROR;
    }
    ext_flags = flags;
  }
  if (cur_tok == 0400 + end)
    (void)ext_get_tok();
  s = NEW(struct _ext_msg2, 1);
  ((struct _ext_msg *)s)->next = NULL;
  ((struct _ext_msg *)s)->code = code;
  s->male_txt = m;
  s->female_txt = f;
  mused += sizeof(struct _ext_msg2);
  return (char *)s;
}

static struct _ext_msg *
ext_mk_struct(char *s)
{
  struct _ext_msg1 *m;
  int c;

  if (s == NULL || s == EXT_ERROR)
    return (struct _ext_msg *)s;
  if (*s >= ' ' && *s <= '~')
  {
    m = NEW(struct _ext_msg1, 1);
    m->str = s;
    ((struct _ext_msg *)m)->code = EXT_STRING;
    ((struct _ext_msg *)m)->next = NULL;
    mused += sizeof(struct _ext_msg1);
    return (struct _ext_msg *)m;
  }
  c = ((struct _ext_msg *)s)->code;
  if (c >= EXT_STRING && c <= EXT_ONAM)
    return (struct _ext_msg *)s;
  mudlog("ext_mk_struct: Illegal ext code %d", c);
  return (struct _ext_msg *)EXT_ERROR;
}

static char *
ext_get_anything(void)
{
  struct _ext_msg *first, *second_last;
  char *s, *t;

  if ((s = ext_get_itm()) == NULL || s == EXT_ERROR)
    return s;
  if ((t = ext_get_itm()) == EXT_ERROR)
  {
    ext_free_msg(s);
    return EXT_ERROR;
  }
  if (t == NULL)
    return s;

  /* build a list with s as first and t as last */
  second_last = first = ext_mk_struct(s); /* make sure s is a struct */
  first->next = t;
  while ((s = ext_get_itm()) != NULL && s != EXT_ERROR)
  {
    /* Make sure last is a struct */
    second_last->next = (char *)ext_mk_struct(second_last->next);

    /* move second_last to point to last */
    second_last = (struct _ext_msg *)second_last->next;

    second_last->next = s; /* append s to the list */
  }
  if (s == EXT_ERROR)
  {
    /* Clear the list */
    ext_free_msg((char *)first);
    return EXT_ERROR;
  }
  return (char *)first;
}

static char *
ext_get_msg(int *w, FILE *f, int bsiz, int l, int h, int *mem_used)
{
  char *s;
  int x = *w;
  int fl = ext_flags;

  if (bufp == EXT_ERROR)
    return EXT_ERROR;
  if (feof(f) || fgets(buffer, bsiz, f) == NULL)
    return NULL;
  ++ext_lino;
  bufp = buffer;
  if ((s = strchr(bufp, ':')) != NULL && s > bufp)
  {
    *s = 0;
    if ((x = tlookup(bufp, ext_msg_names)) < l || x > h)
    {
      x = *w;
    }
    else
    {
      *w = x;
      bufp = s + 1;
    }
    *s = ':';
  }

  if ((ext_who = x) >= EXT_MSG_TARGET)
  {
    ext_flags |= EXT_T | EXT_ANGLE_OPEN;
  }
  if (x < EXT_MSG_OTHERS)
  {
    ext_flags |= EXT_HAT;
  }

  mused = 0;
  (void)ext_get_tok();
  if ((s = ext_get_anything()) == EXT_ERROR)
  {
    ext_flags = fl;
    return bufp = EXT_ERROR;
  }
  if (cur_tok != 0)
  {
    ext_free_msg(s);
    printf("Illegal syntax in actions file\n");
    ext_flags = fl;
    return bufp = EXT_ERROR;
  }
  *mem_used += mused;
  ext_flags = fl;
  if (s != NULL)
    ext_flags |= (1 << x);
  return s;
}

static void
dump_action(EXTERN_CMD_REC *v)
{
  if (v == NULL)
    return;
  bprintf("Data about %s:\n", v->verb);
  bprintf("Next verb: %s\n", (v->next == NULL) ? "<none>" : v->next->verb);
  bprintf("Flags: ");
  if ((v->flags & EXTF_ALL) != 0)
    bprintf("a");
  if ((v->flags & EXTF_SINGLE) != 0)
    bprintf("s");
  if ((v->flags & EXTF_HOSTILE) != 0)
    bprintf("h");
  if ((v->flags & EXTF_ALOOF) != 0)
    bprintf("i");
  if ((v->flags & EXTF_FAR) != 0)
    bprintf("f");
  if ((v->flags & EXTF_OBJ) != 0)
    bprintf("o");
  if ((v->flags & EXTF_TEXT) != 0)
    bprintf("t");
  bprintf("\n");
  if ((v->flags & EXTF_ALL) != 0)
  {
    bprintf("Msg to all: %s\n", build_action(v->msg_to_all));
    bprintf("Msg to me: %s\n", build_action(v->msg_to_me));
  }
  if ((v->flags & EXTF_SINGLE) != 0)
  {
    bprintf("Msg to target: %s\n",
            build_action(((struct _ext2 *)v)->msg_to_target));
    bprintf("Msg to sender: %s\n",
            build_action(((struct _ext2 *)v)->msg_to_sender));
    if (((struct _ext2 *)v)->msg_to_others != NULL)
    {
      bprintf("Msg to others: %s\n",
              build_action(((struct _ext2 *)v)->msg_to_others));
    }
  }
  bprintf("\n");
}

static void
dump_act_vb(char *s)
{
  EXTERN_CMD_REC *v;
  int i;

  for (v = externs; v != NULL && !EQ(v->verb, wordbuf); v = v->next)
    ;
  if (v == NULL)
  {
    if ((i = tlookup(wordbuf, s_ext)) < 0)
    {
      bprintf("I don't know about any %s action.\n", wordbuf);
    }
    else
    {
      bprintf("%s is a special action.\n", s_ext[i]);
    }
  }
  else
  {
    dump_action(v);
  }
}

int boot_extern(FILE *f, char *fname)
{
  char buff[256];
  char *verb;
  EXTERN_CMD_REC *first = NULL, *last = NULL, *this;
  int flags;
  int i, j, low, high;

  char *s, *t;
  int mem_used = 0;
  int m_used = 0;
  int lino = 0;
  char *msgs[5];

  buffer = buff;
  externs = NULL;
  while (!feof(f) && fgets(buff, sizeof buff, f) != NULL)
  {
    ++ext_lino;
    if (*buff != ':')
      continue;
    s = buff;
    while (*++s == ' ' || *s == '\t')
      ;
    t = s;
    while (isalpha(*t))
      ++t;
    *t = 0;
    if (t == s)
    {
      /* No verb given */
      printf("Error in actions file line %d, no verb given.", lino);
      continue;
    }
    verb = COPY(s);
    lowercase(verb);
    m_used = strlen(s) + 1;
    flags = 0;
    do
    {
      ++t;
    } while (*t != '\0' && !isalpha(*t));
    while (isalpha(*t))
    {
      switch (*t++)
      {
      case 'h':
      case 'H':
        flags |= EXTF_HOSTILE;
        break;
      case 'i':
      case 'I':
        flags |= EXTF_ALOOF;
        break;
      case 'a':
      case 'A':
        flags |= EXTF_ALL;
        break;
      case 's':
      case 'S':
        flags |= EXTF_SINGLE;
        break;
      case 'o':
      case 'O':
        flags |= EXTF_OBJ;
        break;
      case 't':
      case 'T':
        flags |= EXTF_TEXT;
        break;
      case 'f':
      case 'F':
        flags |= EXTF_FAR;
        break;
      }
    }

    if ((flags & EXTF_SINGLE) == 0)
      flags |= EXTF_ALL;

    low = ((flags & EXTF_ALL) != 0) ? EXT_MSG_ALL : EXT_MSG_TARGET;
    high = ((flags & EXTF_SINGLE) != 0) ? EXT_MSG_OTHERS : EXT_MSG_ME;
    ext_flags = 0;
    if ((flags & EXTF_OBJ) != 0)
      ext_flags |= EXT_O;

    for (i = EXT_MSG_ALL; i <= EXT_MSG_OTHERS;)
      msgs[i++] = NULL;
    for (i = low; i <= high; i++)
    {
      j = i;
      if ((flags & EXTF_TEXT) != 0)
        ext_flags |= EXT_M;
      s = ext_get_msg(&j, f, sizeof buff, low, high, &m_used);
      if (s == EXT_ERROR)
        break;
      if ((t = msgs[j]) != NULL)
      {
        printf("Double definition for msg for verb %s.\n", verb);
        ext_free_msg(t);
      }
      msgs[j] = s;
    }

    if (s == EXT_ERROR)
    {
      printf("Error in actions file for verb %s, error in messages.\n",
             verb);
    }
    else
    {
      s = NULL;
      if (low == EXT_MSG_ALL && (ext_flags & 0x03) != 0x03)
      {
        printf("Error in actions file for verb %s, missing ALL messages.\n",
               verb);
        s = EXT_ERROR;
      }
      else if (high == EXT_MSG_OTHERS && (ext_flags & 0x0c) != 0x0c)
      {
        printf("Error in actions file for verb %s, missing TARGET messages.\n",
               verb);
        s = EXT_ERROR;
      }
    }
    if (s == EXT_ERROR)
    {
      for (i = EXT_MSG_ALL; i <= EXT_MSG_OTHERS; i++)
      {
        if ((s = msgs[i]) != NULL)
          ext_free_msg(s);
      }
      free(verb);
      continue;
    }
    if ((ext_flags & 0x0c) == 0x0c)
    {
      this = (EXTERN_CMD_REC *)NEW(struct _ext2, 1);
      m_used += sizeof(struct _ext2);
      ((struct _ext2 *)this)->msg_to_target = msgs[EXT_MSG_TARGET];
      ((struct _ext2 *)this)->msg_to_sender = msgs[EXT_MSG_SENDER];
      ((struct _ext2 *)this)->msg_to_others = msgs[EXT_MSG_OTHERS];
    }
    else
    {
      this = NEW(EXTERN_CMD_REC, 1);
      m_used += sizeof(EXTERN_CMD_REC);
    }
    this->next = NULL;
    this->flags = flags;
    this->verb = verb;
    this->msg_to_all = msgs[EXT_MSG_ALL];
    this->msg_to_me = msgs[EXT_MSG_ME];

    /*    dump_action(this); */
    if (first == NULL)
    {
      first = last = this;
    }
    else
    {
      last->next = this;
      last = this;
    }
    mem_used += m_used;
  }
  externs = first;
  return mem_used;
}

static char *
ext_copy(char *b, char *m)
{
  char *p;
  char v[50];

  if (m == NULL || m == EXT_ERROR)
  {
    *b = 0;
    return b;
  }
  if (*m >= ' ' && *m <= '~')
    return x_strcpy(b, m);
  p = v;
  switch (((struct _ext_msg *)m)->code)
  {
  case EXT_STRING:
    p = ((struct _ext_msg1 *)m)->str;
    break;
  case EXT_TARGET:
    p = psex(pl1) ? ((struct _ext_msg2 *)m)->female_txt : ((struct _ext_msg2 *)m)->male_txt;
    break;
  case EXT_ACTOR:
    p = (psex(mynum) ? ((struct _ext_msg2 *)m)->female_txt : ((struct _ext_msg2 *)m)->male_txt);
    break;
  case EXT_TNAM:
    sprintf(p, "\001p%s\003", pname(pl1));
    break;
  case EXT_ANAM:
    sprintf(p, "\001p%s\003", pname(mynum));
    break;
  case EXT_THIM:
    p = psex(pl1) ? "her" : "him";
    break;
  case EXT_AHIM:
    p = psex(mynum) ? "her" : "him";
    break;
  case EXT_THIS:
    p = psex(pl1) ? "her" : "his";
    break;
  case EXT_AHIS:
    p = psex(mynum) ? "her" : "his";
    break;
  case EXT_ONAM:
    p = oname(ob2);
    break;
  case EXT_MTXT:
    p = "%s";
  default:
    return b;
  }
  b = x_strcpy(b, p);
  return ext_copy(b, ((struct _ext_msg *)m)->next);
}

static char *
build_act_msg(char *b, char *m)
{
  if (m == NULL || m == EXT_ERROR)
  {
    *b = 0;
    return b;
  }
  if (*m >= ' ' && *m <= '~')
    return x_strcpy(b, m);
  switch (((struct _ext_msg *)m)->code)
  {
  case EXT_STRING:
    b = x_strcpy(b, ((struct _ext_msg1 *)m)->str);
    break;
  case EXT_TARGET:
    *b++ = '%';
    *b++ = '<';
    b = build_act_msg(b, ((struct _ext_msg2 *)m)->male_txt);
    *b++ = '%';
    *b++ = ':';
    b = build_act_msg(b, ((struct _ext_msg2 *)m)->female_txt);
    *b++ = '%';
    *b++ = '>';
    break;
  case EXT_ACTOR:
    *b++ = '%';
    *b++ = '[';
    b = build_act_msg(b, ((struct _ext_msg2 *)m)->male_txt);
    *b++ = '%';
    *b++ = '/';
    b = build_act_msg(b, ((struct _ext_msg2 *)m)->female_txt);
    *b++ = '%';
    *b++ = ']';
    break;
  case EXT_TNAM:
    *b++ = '%';
    *b++ = 't';
    break;
  case EXT_ANAM:
    *b++ = '%';
    *b++ = 'a';
    break;
  case EXT_THIM:
  case EXT_AHIM:
    *b++ = '%';
    *b++ = '~';
    break;
  case EXT_THIS:
  case EXT_AHIS:
    *b++ = '%';
    *b++ = '^';
    break;
  case EXT_ONAM:
    *b++ = '%';
    *b++ = 'o';
    break;
  case EXT_MTXT:
    *b++ = '%';
    *b++ = 'm';
    break;
  default:
    return b;
  }
  *b = 0;
  return build_act_msg(b, ((struct _ext_msg *)m)->next);
}

static char *
build_action(char *a)
{
  static char b[256];
  char *t;

  t = build_act_msg(b, a);
  *t = 0;
  return b;
}

/*
 * **  (C) Brian Preble (rassilon@eddie.mit.edu)
 * **  Handles external commands such as Kiss, Hug, Tickle, etc.
 * **
 * **  Enhancements of Alf.
 */
int fextern(char *verb)
{
  struct _ext *v;
  char *m;
  int p;
  int o;
  char b[MAX_COM_LEN];
  char b2[MAX_COM_LEN];
  char b3[MAX_COM_LEN];

  v = externs;
  verb = strcpy(b, verb);
  o = -1;
  /* Stupid, parse_2, doesn't use vb, but we must give one */
  if (!parse_2(1))
    return -1;

#if 0
  bprintf ("fextern(%s) is called, pl1 = %d, ob1 = %d, ob2 = %d.\n", verb,
	   pl1, ob1, ob2);
#endif
  for (; v != NULL; v = v->next)
  {
    if (!EQ(verb, v->verb))
      continue;
#if 0
      bprintf ("Found verb.\n");
      dump_action (v);
#endif
    stp = 0;
    (void)brkword();
    if ((v->flags & EXTF_ALL) == 0)
    {
      if (pl1 < 0)
        continue;
      p = pl1;
      brkword(); /* skip player */
      if ((v->flags & EXTF_OBJ) != 0)
      {
        if (ob2 < 0)
          continue;
        o = ob2;
        brkword();
      }
    }
    else if ((v->flags & EXTF_SINGLE) == 0)
    {
      p = -1;
      if ((v->flags & EXTF_OBJ) != 0)
      {
        if (ob1 < 0)
          continue;
        o = ob1;
        brkword(); /* Skip object */
      }
    }
    else if ((p = pl1) < 0)
    {
      if ((v->flags & EXTF_OBJ) != 0)
      {
        if (ob1 < 0)
          continue;
        o = ob1;
        brkword();
      }
    }
    else
    {
      brkword();
      if ((v->flags & EXTF_OBJ) != 0)
      {
        if (ob2 < 0)
          continue;
        o = ob2;
        brkword();
      }
    }
    if ((v->flags & EXTF_TEXT) != 0)
    {
      getreinput(b3);
      if (EMPTY(b3))
        continue;
    }
    else
    {
      *b3 = '\0';
    }
    ob2 = o;
    /* Now pl1 >= 0 if target, ob2 >= 0 if object and b3 is !EMPTY if txt */
    /* Verb found */
#if 0
      bprintf ("Verb satisfy the requirements.\n");
#endif

    if (pfighting(mynum) >= 0)
    {
      bprintf("Not in a fight!\n");
      return 0;
    }

    if ((v->flags & EXTF_HOSTILE) != 0 && pl1 >= 0)
    {
      if (testpeace(mynum))
      {
        bprintf("Nah, that's violent.\n");
        return 0;
      }
    }
    if (p < 0)
    {
      (void)ext_copy(b, v->msg_to_all);
      sprintf(b2, b, b3);
      sprintf(b, "\001s%%s\003%s\n\003", b2);
      sillycom(b);
      (void)ext_copy(b, v->msg_to_me);
      sprintf(b2, b, b3);
      bprintf("%s\n", b2);
      return 0;
    }

    if ((v->flags & EXTF_FAR) == 0 && ploc(p) != ploc(mynum))
    {
      bprintf("%s is not here.\n", psex(pl1) ? "She" : "He");
      return 0;
    }
    if (p == mynum)
    {
      bprintf("Good trick, that.\n");
      return 0;
    }
    if ((v->flags & EXTF_ALOOF) != 0 && ststflg(p, SFL_ALOOF) &&
        plev(mynum) < LVL_APPREN)
    {
      bprintf("%s thinks %s's too good for mere mortals.\n",
              pname(p), (psex(p) ? "she" : "he"));
      return 0;
    }

    ext_copy(b2, ((struct _ext2 *)v)->msg_to_target);
    sprintf(b, b2, b3);
    m = b + strlen(b);
    strcpy(m, "\n");
    send_msg(p, 0, LVL_MIN, LVL_MAX, NOBODY, NOBODY, b);
    if ((m = ((struct _ext2 *)v)->msg_to_others) != NULL)
    {
      ext_copy(b2, m);
      sprintf(b, b2, b3);
      send_msg(ploc(mynum), 0, pvis(mynum), LVL_MAX, mynum, p,
               "%s\n", b);
    }
    ext_copy(b2, ((struct _ext2 *)v)->msg_to_sender);
    sprintf(b, b2, b3);
    bprintf("%s\n", b);

    /* Hostile action? */
    if ((v->flags & EXTF_HOSTILE) != 0 && p >= max_players)
    {
      hit_player(p, mynum, pwpn(p));
    }
    return 0;
  }
  return -1; /* Command not found in extern list */
}

/*
 * **  Displays list of external commands such as Kiss, Hug, Tickle, etc.
 */
int lisextern(void)
{
  char line[80];
  int i = -1;
  char **t;
  EXTERN_CMD_REC *v;

  if (brkword() != -1)
  {
    dump_act_vb(wordbuf);
  }
  else
  {
    bprintf("The following actions are defined:\n");
    for (v = externs; v != NULL; v = v->next)
    {
      ++i;
      if (i % 9 == 0)
        bprintf("\n");
      strcpy(line, v->verb);
      *line = toupper(*line);
      bprintf("%s\t", line);
    }
    for (t = s_ext; *t != TABLE_END; ++t)
    {
      ++i;
      if (i % 9 == 0)
        bprintf("\n");
      bprintf("%s%c", *t, t[1] == TABLE_END ? '\n' : '\t');
    }
    bprintf("\n");
  }
  return 0;
}
