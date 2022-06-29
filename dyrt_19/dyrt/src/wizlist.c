#include "kernel.h"
#include "stdinc.h"

extern char *WizLevels[];

struct _w
{
  struct _w *next;
  char name[PNAME_LEN + 1];
  char wlev;
};

struct _w *wizlist;
char wizfile[256];

void set_wizfile (char *f)
{
  strcpy(wizfile, f);
}

int boot_wizlist (FILE *f, char *fname)
{
  int mused = 0;
  int plev;
  struct _w *first = NULL;
  struct _w *p, *q, *r;
  char buff[256];
  char name[100];
  char lev[100];
  
  if (f == NULL)
    {
      wizlist = NULL;
      return 0;
    }
  while (fgets (buff, sizeof buff, f) != NULL)
    {
      char *pp= buff; 
      sscanf(buff, "%s ",name);
      pp += (strlen(name)) +1;
      pp[strlen(pp)-1] = '\0';
      strcpy(lev,pp);
      if ((plev = tlookup(lev, WizLevels)) < 0)
	{
	  printf("Error in wizlist, invalid wizlevel %s.\n", lev);
	  continue;
	}
      if (strlen(name) > PNAME_LEN)
	{
	  printf("Error in wizlist, name %s is too long.\n", name);
	  continue;
	}
      r = NEW(struct _w, 1);
      mused += sizeof(struct _w);
      strcpy (r->name, name);
      r->wlev = plev;
      q = NULL;
      for (p = first; p != NULL && p->wlev >= plev;
	   q = p, p = p-> next);
      r->next = p;
      if (q == NULL)
	{
	  first = r;
	}
      else
	{
	  q->next = r;
	}
    }
  if (!feof(f))
    {
      printf("Error while reading wizlist.");
      ferror(f);
    }
  wizlist = first;
  return mused;
}

void dump_wizlist(void)
{
  struct _w *p;
  FILE *f;
  
  if ((f = fopen(wizfile, "w")) == NULL)
    {
      mudlog("Error when dumping wizfile \"%s\".", wizfile);
      progerror(wizfile);
      return;
    }
  for (p = wizlist; p != NULL; p = p->next)
    {
      if (p->wlev > LEV_MORTAL && p->wlev <= LEV_MASTER)
	{
	  fprintf(f, "%s %s\n", p->name, WizLevels[(int)p->wlev]);
	}
    }
  fclose(f);
}

void update_wizlist (char *name, int new_wlevel)
{
  struct _w *p, *q, *r;
  Boolean m = False;
  
  for (q = NULL, p = wizlist; p != NULL && !EQ (p->name, name);
       q = p, p = p->next);
  if (p != NULL)
    {
      if (new_wlevel == p->wlev && new_wlevel != LEV_MORTAL)
	return;
      if (q == NULL)
	{
	  wizlist = p->next;
	}
      else
	{
	  q->next = p->next;
	}
      m = True;
    }
  if (new_wlevel > LEV_MORTAL && new_wlevel <= LEV_MASTER)
    {
      if (p == NULL)
	{
	  p = NEW(struct _w, 1);
	  strcpy(p->name, name);
	}
      p->wlev = new_wlevel;
      
      for (q = NULL, r = wizlist; r != NULL && r->wlev >= p->wlev;
	   q = r, r= r->next);
      p->next = r;
      if (q == NULL)
	{
	  wizlist = p;
	}
      else
	{
	  q->next = p;
	}
      m = True;
    }
  if (m)
    {
      dump_wizlist();
    }
}

/* Function to parse a string s assumed to be on one of the following forms:
 *
 * +wizlevel      return low = wizlevel, high = god, return '+';
 * -wizlevel      return low = apprentice, high = wizlevel, return '-';
 * =wizlevel      return low = high = wizlevel, return '=';
 * wizlevel1:wizlevel2 return low = wizlevel1, high = wizlevel2, return ':';
 * nickname       return low = pointer to node in wizlist describing
 *                nickname. high = 0, return -2
 * wizlevel       return low = high = wizlevel, return 0;
 *
 * if invalid wizlevel, return -3; */

int parse_wizlevel (char *s, int *low, int *high)
{
  int t, x;
  char *a;
  struct _w *p;
  
  a = NULL;
  
  if ((t = *s++) != '+' && t != '-' && t != '=')
    {
      if ((a = strchr(--s, ':')) == NULL)
	t = 0;
      else
	{
	  *a++ = '\0';
	  t = ':';
	}
    }
  if (t != 0)
    {
      if ((x = tlookup(s, WizLevels)) < 0)
	return -3;
      switch (t)
	{
	case '+':
	  *low = x;
	  *high = LEV_MASTER;
	  break;
	case '-':
	  *low = LEV_APPRENTICE;
	  *high = x;
	  break;
	case '=':
	  *low = *high = x;
	  break;
	case ':':
	  *low = x;
	  if ((*high = tlookup(a, WizLevels)) < 0)
	    return -3;
	  break;
	}
    }
  else 
    {
      for (p = wizlist; p != NULL && !EQ (s, p->name); p = p->next);
      if (p != NULL)
	{
	  *low = (int)p;
	  *high = 0;
	  return -2;
	}
      if ((x = tlookup(s, WizLevels)) < 0)
	return -3;
      *low = *high = x;
    }
  return t;
}

/* wizlist command
 *
 * WIZLIST [+-=]wizlevel         wizard level
 * WIZLIST wizlevel:wizlevel     range of wizard levels
 * WIZLIST nickname              nickname
 *
 * each of the arguments may be repeated. */

void wizlistcom (void)
{
  static char *w[] =
    {NULL, "Apprentice Wizards", "Emeriti", "Wizards", "Senior Wizards", 
     "ArchWizards", "High Archwizards", "DemiGods", "High DemiGods -- Honorary",
     "Gods", "Masters"};
  char *s;
  int t;
  int x = -1;
  int low, high;
  struct _w *p;
  Boolean h = False;
  int k = 10;
  Boolean full_title = False;
  PERSONA pers;
  low = 0;
  high = 0;
  s = NULL;
  
  if ((t = brkword()) == -1)
    {
      low = LEV_APPRENTICE;
      high = LEV_MASTER;
    }
  do
    {
      if (t != -1)
	t = parse_wizlevel (wordbuf, &low, &high);
      if (t == -3)
	{
	  bprintf("No such immortal or immortal level: \"%s\".\n", wordbuf);
	}
      else if (t == -2)
	{
	  p = (struct _w *) low;
	  low = p->wlev;
	  bprintf("%s is %s %s.\n", p->name,
		  (low == LEV_APPRENTICE || low == LEV_ARCHWIZARD) ?
		  "an" : "a",
		  WizLevels[low]);
	}
      else
	{
	  for (p = wizlist; p != NULL && p->wlev >= low; p = p->next)
	    {
	      if (p->wlev <= high)
		{
		  if (!h)
		    {
		      bprintf("%s", "&+BThe following players have risen to power in the lands of AberMUD:&*");
		      if (low > LEV_APPRENTICE || high < LEV_MASTER)
			{
			  bprintf("\n&+G(Only the levels chosen are shown)&*");
			}
		      h = True;
		    }
		  if (p->wlev != x)
		    {
		      bprintf("\n\n&+Y%s&*\n", w[x = p->wlev]);
		      for (t = 0; t != strlen (w[x = p->wlev]); t++)
			bprintf ("-");
		      if (x >= 5)
			full_title = True;
		      else
			full_title = False;
		      bprintf ("\n");
		      k = 5;
		    }
		  if (!full_title)
		    bprintf("%-14.14s ", p->name);
		  else
		    {
		      int lf;
		      if (find_player(p->name, &pers, (Boolean*)&lf) != -1)
			bprintf("%s\n", make_title (pers.p_title, pers.p_name));
		      else
			bprintf("%s\n", p->name);
		    }
		  if (--k == 0 && !full_title)
		    {
		      bprintf("\n");
		      k = 5;
		    }
		}
	    }
	  if (k < 6 && !full_title)
	    bprintf("\n");
	}
    }
  while ((t = brkword()) != -1);
}

void powerscom(void)
{
  static char *w[] = 
    {NULL, "Apprentice Wizards", "Emeriti", "Wizards", "Senior Wizards", 
     "ArchWizards", "High Archwizards", "DemiGods", "High DemiGods", "Gods", 
     "Masters"};
  
  int x = -1;
  int low, high;
  struct _w *p;
  Boolean h = False;
  int count = 0;
  low = LEV_ARCHWIZARD;
  high = LEV_MASTER;
  for (p = wizlist; p != NULL && p->wlev >= low; p = p->next)
    {
      if (p->wlev <= high)
	{
	  if (!h)
	    {
	      bprintf ("\n&+BThe following powers are here to help you.&*\n");
	      h = True;
	    }
	  if (p->wlev != x)
	    {
	      bprintf("\n&+Y%-15.15s&*: ", w[x = p->wlev]);
	      count = 18;
	    }
	  else
	    bprintf (", ");
	  count = count + strlen (p->name) + 2;
	  if (count >= 75)
	    {
	      bprintf("\n");
	      bprintf("%17.17s", " ");
	      count = 17;
	    }
	  bprintf("%s", p->name);
	}
    }
  bprintf("\n\n");
}
