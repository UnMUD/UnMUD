#include <stdlib.h>
#include "kernel.h"
#include "utils.h"
#include "stdinc.h"

void init_rand (void)
{
#ifndef SYS_NO_RANDOM
  srandom (9876);
#else
  srand (9876);
#endif
}


int 
randperc (void)
{
  return my_random () % 100;
}

char *x_strcpy (char *d, char *s)
{
  /* Like strcpy except it returns the pointer just past last char in d */
  while ((*d++ = *s++) != '\0');
  return d - 1;
}

Boolean tst_bit (LongInt * f, int b)
{
  if (b >= 32)
    {
      return xtstbit ((f->h), (b - 32));
    }
  return xtstbit (f->l, b);
}

Boolean tst_doublebit (DoubleLongInt * f, int b)
{
  if (b >= 64)
    {
      return xtstbit((f->b3), (b - 64));
    }
  else if (b >= 32)
    {
      return xtstbit((f->b2), (b - 32));
    }
  else 
    {
      return xtstbit(f->b1, b);
    }
}

void set_bit (LongInt * f, int b)
{
  if (b >= 32)
    {
      xsetbit ((f->h), (b - 32));
    }
  else
    {
      xsetbit (f->l, b);
    }
}

void set_doublebit (DoubleLongInt *f, int b)
{
  if (b >= 64)
    {
      xsetbit ((f->b3), (b-64));
    }
  if (b >= 32)
    {
      xsetbit ((f->b2), (b-32));
    }
  else
    {
      xsetbit (f->b1, b);
    }
}

void clr_bit (LongInt * f, int b)
{
  if (b >= 32)
    {
      xclrbit ((f->h), (b - 32));
    }
  else
    {
      xclrbit (f->l, b);
    }
}

void clr_doublebit (DoubleLongInt * f, int b)
{
  if (b >= 64)
    {
      xclrbit ((f->b3), (b-64));
    }
  if (b >= 32)
    {
      xclrbit ((f->b2), (b-32));
    }
  else
    {
      xclrbit (f->b1, b);
    }
}

Boolean match (char *p, char *q)
{
  register char *s = p;
  register char *t = q;
  register char c;
  register char d;
  
  for (;;)
    {
      if ((c = *s++) == 0)
	{
	  return (*t == 0);
	}
      if ((d = *t++) == 0)
 	{
	  break;
	}
      if (isupper (c))
	{
	  c = tolower (c);
	}
      if (isupper (d))
	{
	  d = tolower (d);
	}
      if (c == d)
	{
	  continue;
	}
      if (d != '*')
	{
	  break;
	}
      if ((d = *t++) == 0)
 	{
	  return True;
	}
      while (*s++ != d)
	{
	  if (*(s - 1) == 0)
	    {
	      return False;
	    }
	}
    }
  return False;
}

Boolean infile (char *file, char *line)
{
  register char *p;
  register char *q;
  FILE *fl;
  Boolean invert;
  char a[80];
  char b[80];
  
  if ((fl = fopen (file, "r")) == 0)
    {
      return False;
    }
  invert = False;
  if (!fgets (a, sizeof a, fl))
    {
      fclose (fl);
      return False;
    }
  if (*a == '!')
    {
      invert = True;
      if (!fgets (a, sizeof a, fl))
	{
	  fclose (fl);
	  return False;
	}
    }
  for (p = line; isspace (*p); ++p);
  for (q = b; !isspace (*p) && *p != 0; ++p)
    *q++ = *p;
  *q = 0;
  do
    {
      for (p = a; isspace (*p); ++p);
      for (q = p; !isspace (*q) && *q != 0; ++q);
      *q = 0;
      if (match (b, p))
	{
	  fclose (fl);
	  return !invert;
	}
    }
  while (fgets (a, sizeof a, fl));
  fclose (fl);
  return invert;
}

#ifdef SYS_EQBUG
/*
 * Copyright (c) 1987 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that this notice is preserved and that due credit is given
 * to the University of California at Berkeley. The name of the University
 * may not be used to endorse or promote products derived from this
 * software without specific written prior permission. This software
 * is provided ``as is'' without express or implied warranty.
 */

/*
 * This array is designed for mapping upper and lower case letter
 * together for a case independent comparison.  The mappings are
 * based upon ascii character sequences.
 */
static u_char charmap[] =
{
  '\000', '\001', '\002', '\003', '\004', '\005', '\006', '\007',
  '\010', '\011', '\012', '\013', '\014', '\015', '\016', '\017',
  '\020', '\021', '\022', '\023', '\024', '\025', '\026', '\027',
  '\030', '\031', '\032', '\033', '\034', '\035', '\036', '\037',
  '\040', '\041', '\042', '\043', '\044', '\045', '\046', '\047',
  '\050', '\051', '\052', '\053', '\054', '\055', '\056', '\057',
  '\060', '\061', '\062', '\063', '\064', '\065', '\066', '\067',
  '\070', '\071', '\072', '\073', '\074', '\075', '\076', '\077',
  '\100', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
  '\150', '\151', '\152', '\153', '\154', '\155', '\156', '\157',
  '\160', '\161', '\162', '\163', '\164', '\165', '\166', '\167',
  '\170', '\171', '\172', '\133', '\134', '\135', '\136', '\137',
  '\140', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
  '\150', '\151', '\152', '\153', '\154', '\155', '\156', '\157',
  '\160', '\161', '\162', '\163', '\164', '\165', '\166', '\167',
  '\170', '\171', '\172', '\173', '\174', '\175', '\176', '\177',
  '\200', '\201', '\202', '\203', '\204', '\205', '\206', '\207',
  '\210', '\211', '\212', '\213', '\214', '\215', '\216', '\217',
  '\220', '\221', '\222', '\223', '\224', '\225', '\226', '\227',
  '\230', '\231', '\232', '\233', '\234', '\235', '\236', '\237',
  '\240', '\241', '\242', '\243', '\244', '\245', '\246', '\247',
  '\250', '\251', '\252', '\253', '\254', '\255', '\256', '\257',
  '\260', '\261', '\262', '\263', '\264', '\265', '\266', '\267',
  '\270', '\271', '\272', '\273', '\274', '\275', '\276', '\277',
  '\300', '\341', '\342', '\343', '\344', '\345', '\346', '\347',
  '\350', '\351', '\352', '\353', '\354', '\355', '\356', '\357',
  '\360', '\361', '\362', '\363', '\364', '\365', '\366', '\367',
  '\370', '\371', '\372', '\333', '\334', '\335', '\336', '\337',
  '\340', '\341', '\342', '\343', '\344', '\345', '\346', '\347',
  '\350', '\351', '\352', '\353', '\354', '\355', '\356', '\357',
  '\360', '\361', '\362', '\363', '\364', '\365', '\366', '\367',
  '\370', '\371', '\372', '\373', '\374', '\375', '\376', '\377',
};

int strcasecmp (char *s1, char *s2)
{
  register u_char *cm = charmap, *us1 = (u_char *) s1, *us2 = (u_char *) s2;
  
  while (cm[*us1] == cm[*us2++])
    if (*us1++ == '\0')
      {
        return (0);
      }
    return (cm[*us1] - cm[*--us2]);
}

int strncasecmp (char *s1, char *s2, register int n)
{
  register u_char *cm = charmap, *us1 = (u_char *) s1, *us2 = (u_char *) s2;
  
  while (--n >= 0 && cm[*us1] == cm[*us2++])
    if (*us1++ == '\0')
      {
        return (0);
      }
  return (n < 0 ? 0 : cm[*us1] - cm[*--us2]);
}
/*
 * **  END OF BERKELEY INCLUDE
 */
#endif


int glookup (char *elem, int n, char **table,
	 int (*strcmpfun) (char *s1, char *s2, int n))
{
  char **t;
  int x;
  
  for (t = table, x = 0; *t != TABLE_END; ++t, ++x)
    {
      if (*t == NULL)
	{
	  continue;
	}
      if (strcmpfun (elem, *t, n) == 0)
	{
	  return x;
	}
    }
  return (-1);
}

int tlookup (char *elem, char **table)
{
  return glookup (elem, strlen (elem), table, strncasecmp);
}

/* the function used for xlookup */
static int xstrcasecmp (char *s1, char *s2, int n)
{
  /* n is ignored */
  return strcasecmp (s1, s2);
}

/* Like tlookup but uses strcasecmp (actually xstrcasecmp) instead of 
   strncasecmp */
int xlookup (char *elem, char **table)
{
  return glookup (elem, 0, table, xstrcasecmp);
}

/* Convert a string to lowercase */
char *lowercase (char *str)
{
  char *p;
  
  for (p = str; *p; p++)
    if (isupper (*p))
      {
        *p = tolower (*p);
      }
  return str;
}

/* Convert a string to uppercase. */
char *uppercase (char *str)
{
  char *p;
  
  for (p = str; *p; p++)
    if (islower (*p))
      {
        *p = toupper (*p);
      }
  return str;
}

/* Get memory */
void *xmalloc (int nelem, int elem_size)
{
  void *p;
  
  if ((p = calloc (nelem, elem_size)) == NULL)
    {
      fprintf (stderr, "No room to allocate bytes. [%d]\n", (int)p);
      abort ();
    }
  return p;
}

/* Test if all of the bits set in M is also set in W */
Boolean tstbits (int w, int m)
{
  return (bits (w, m) == m);
}

char *my_crypt (char *buf, char *pw, int len)
{
  char *s = crypt (pw, "Mu");
  s[len - 1] = 0;
  return strcpy (buf, s);
}

char *mk_string (char *b, char *str, int k, int stopch)
{
  unsigned char *s = (unsigned char *) str;
  char *t = b;
  int c;
  
  while ((c = 0377 & *s++) != stopch && --k >= 0)
    {
      switch (c)
	{
	case '\\':
	  *t++ = '\\';
	  *t++ = '\\';
	  break;
	case '\n':
	  *t++ = '\\';
	  *t++ = 'n';
	  break;
	case '\t':
	  *t++ = '\\';
	  *t++ = 't';
	  break;
	case '\b':
	  *t++ = '\\';
	  *t++ = 'b';
	  break;
	case '\f':
	  *t++ = '\\';
	  *t++ = 'f';
	  break;
	case '\r':
	  *t++ = '\\';
	  *t++ = 'r';
	  break;
	case '\"':
	  *t++ = '\\';
	  *t++ = '\"';
	  break;
	default:
	  if (isprint (c))
	    *t++ = c;
	  else
	    {
	      *t++ = '\\';
	      *t++ = '0' + ((c >> 6) & 3);
	      *t++ = '0' + ((c >> 3) & 7);
	      *t++ = '0' + (c & 7);
	    }
	  break;
	}
    }
  *t = 0;
  return b;
}



void *resize_array (void *start, int elem_size, int oldlen, int newlen)
{
  void *p = NULL;
  
  if (newlen != 0)
    {
      p = xmalloc (newlen, elem_size);
    }
  if (start != NULL)
    {
      if (newlen != 0)
	{
	  memcpy (p, start, min (oldlen, newlen) * elem_size);
	}
      free (start);
    }
  return p;
}

/* A package for handling sets of integers. Nicknack, April '93 */
static Boolean check_for_possible_resize (int_set * p);

void init_intset (int_set * p, int len)
{
  p->len = p->current = 0;
  p->list = resize_array (NULL, sizeof (int), 0, p->maxlen = len);
}

void free_intset (int_set * p)
{
  if (p->list != NULL)
    {
      free (p->list);
    }
}

Boolean add_int (int n, int_set * p)
{
  int i;
  
  check_for_possible_resize (p);
  for (i = 0; i < p->len; i++)
    {
      if (p->list[i] == n)
	{
	  return False;
	}
    }
  p->list[p->len++] = n;
  return True;
}

Boolean remove_int (int n, int_set * p)
{
  int i;
  
  if (p->list == NULL)
    {
      return False;
    }
  for (i = 0; i < p->len && p->list[i] != n; i++);
  if (i == p->len)
    {
      return False;
    }
  if (i < p->current)
    {
      if (i < p->current - 1)
	{
	  p->list[i] = p->list[p->current - 1];
 	}
      i = --p->current;
    }
  p->list[i] = p->list[--p->len];
  check_for_possible_resize (p);
  return True;
}

int find_int (int n, int_set * p)
{
  int i;
  
  if (p->list == NULL)
    {
      return -1;
    }
  for (i = 0; i < p->len && p->list[i] != n; i++);
  return i < p->len ? i + 1 : 0;
}

int find_int_number (int n, int_set * p)
{
  return (n < 0 || n >= p->len) ? SET_END : p->list[n];
}

int foreach_int (int_set * p, int (*func) (int))
{
  int i;
  int n = 0;
  
  for (i = 0; i < p->len; i++)
    {
      if (func (p->list[i]))
	n++;
    }
  return n;
}

void remove_current (int_set * p)
{
  p->list[--p->current] = p->list[--p->len];
  check_for_possible_resize (p);
}

/* set_size() and is_empty() are macros */
int get_set_mem_usage (int_set * p)
{
  return p->len * sizeof (int);
}

static Boolean check_for_possible_resize (int_set * p)
{
  int oldlen = p->maxlen;
  
  if (p->len == p->maxlen)
    {
      p->maxlen = p->len < 20 ? 2 * (p->len + 1) : p->len + 25;
    }
  else if (p->maxlen > 0 && p->len < p->maxlen / 5)
    {
      p->maxlen /= 2;
    }
  else
    {
      return False;
    }
  p->list = resize_array (p->list, sizeof (int), oldlen, p->maxlen);
  return True;
}

static int hash (long int value, int tbl_size)
{
  return value % tbl_size;
}

void init_inttable (int_table * p, int size)
{
  p->table = NEW (table_entry *, p->len = size);
  
  while (--size >= 0)
    p->table[size] = NULL;
}

void free_inttable (int_table * p)
{
  free (p->table);
}

static table_entry *
new_entry (long int key, long int value, table_entry * next)
{
  table_entry *p = NEW (table_entry, 1);
  p->key = key;
  p->value = value;
  p->next = next;
  return p;
}

static void unlink_entry (table_entry ** entry)
{
  table_entry *p = *entry;
  *entry = (*entry)->next;
  free (p);
}

/* Return a pointer to the pointer to the entry that represents 'key',
 * starting the search at the entry to which *p points.
 * If it does not exist, return a pointer to the pointer to the entry that
 * would succede 'key', had it existed.
 */
static table_entry **find_position (long int key, table_entry ** p)
{
  while (*p != NULL && (*p)->key > key)
    p = &(*p)->next;
  return p;
}

static table_entry *find_entry (long int key, int_table * p)
{
  table_entry **r;
  if (key < 0) return NULL;
  r = find_position(key, &p->table[hash(key, p->len)]);
  return (*r != NULL && (*r)->key == key) ? *r : NULL;
}

Boolean insert_entry (long int key, long int value, int_table * p)
{
  table_entry **q;
  if (key < 0) return False;
  q = find_position(key, &p->table[hash(key, p->len)]);
  if ((*q) == NULL)
    {
      *q = new_entry (key, value, NULL);
    }
  else if ((*q)->key < key)
    {
      *q = new_entry (key, value, *q);
    }
  else
    return False;
  return True;
}

Boolean remove_entry (long int key, int_table * p)
{
  table_entry **q;

  if(key < 0) return False;
  q = find_position (key, &p->table[hash (key, p->len)]);
  if ((*q) != NULL && (*q)->key == key)
    {
      unlink_entry (q);
      return True;
    }
  else
    {
      return False;
    }
}

long int lookup_entry (long int key, int_table * p)
{
  table_entry *q = find_entry (key, p);
  return (q != NULL) ? (q)->value : NOT_IN_TABLE;
}

long int change_entry (long int key, long int new_value, int_table * p)
{
  table_entry *q = find_entry (key, p);
  if (q != NULL)
    {
      long int v = q->value;
      q->value = new_value;
      return v;
    }
  else
    {
      return NOT_IN_TABLE;
    }
}

int get_table_mem_usage (int_table * p)
{
  return p->len * sizeof (table_entry *);
}
