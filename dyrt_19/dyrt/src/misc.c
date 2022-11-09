#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "kernel.h"
#ifdef MIPS
# include "machine/mips.h"
#endif

/*
extern char *sys_errlist[];
*/

/* This file should only contain generate (non-game related) functions */
/* Copy, padding the possible dangerous codes */

char *safe_strcpy (char *s1, char *s2)
{
  register int from;
  register int to;
  register int len;
  len = strlen (s2);
  for (from = 0, to = 0;
       from != len;
       from++, to++)
    {
      s1[to] = s2[from];
    }
  return (s1);
}

/* open/close files, and pipe them if they are compressed.  Read-only for
   now, easily modified (but not needed for my purposes) - val */
#ifndef NullF
#define NullF (FILE *)0
#endif
#ifndef ZCAT
#define ZCAT "/usr/ucb/zcat"
#endif

/* PROTOTYPES */
FILE *bopen (char *file);
void bclose (FILE * fp);

/* OPEN FILE, OR PIPE IF COMPRESSED */
FILE * bopen (char *file)
{
  FILE *fp;
  char magic[10], *retcode;
  char command[1200];
  char line[1200];
/*
  extern char *sys_errlist[];
*/
  int x;
  
  if (!file)
    return (NullF);
  fp = fopen (file, "r");
  if (!fp)
    {
      x = errno;
      /* check if we maybe have a .Z */
      sprintf (line, "%s.Z", file);
      fp = fopen (line, "r");
      if (fp)
	goto bopen_cont;
      errno = x;
      fprintf (stderr, "cannot open file %s (%s)", file, strerror(errno));
      return (NullF);
    }
 bopen_cont:
  /* NOW read the magic number if any */
  fgets (magic, 5, fp);
  rewind (fp);
  if (!retcode || strlen(magic) < (unsigned int)2 || magic[0] != '\037' || magic[1] != '\235')
    return(fp);
  /*   we have a compressed file.. whoopee    */
  (void) fclose (fp);
  sprintf (command, "%s %s", ZCAT, file);
  fp = popen (command, "r");
  if (!fp)
    {
      fprintf (stderr, "Pipe failed while opening %s (%s)", command, strerror(errno));
      return (NullF);
    }
  return (fp);
}

/* FCLOSE OR PCLOSE FILE, ACCORDINGLY */
void bclose (FILE * fp)
{
  int n;
  struct stat s;
  
  n = fileno (fp);
  if (fstat (n, &s) < 0)
    {
      fprintf (stderr, "Fstat failed in zclose (%s)", strerror(errno));
      return;
    }
  if (S_ISFIFO (s.st_mode))
    (void) pclose (fp);
  else
    (void) fclose (fp);
  return;
}

Boolean concat_files (char *to, char *from)
{
  FILE *fin;
  FILE *fout;
  char x[1024];
  
  fin = fopen (from, "r");
  if (!fin)
    {
      fprintf (stderr, "error on opening source file for concat_files.\n");
      return (False);
    }
  fout = fopen (to, "a+");
  if (!fout)
    {
      fprintf (stderr, "error on opening destination file for concat_files.\n");
      fclose (fin);
      return (False);
    }
  while (!feof (fin))
    {
      fgets (x, 1024, fin);
      fprintf (fout, "%s",  x);
      x[0] = '\0';
    }
  fclose (fin);
  fclose (fout);
  return (True);
}

/* pattern matching, based on /rich $alz's code.
   it doesn't contain [] matching, which was unreliable */
check_match (char *line, char *pattern, Boolean pad_with_star)
{
  char patbuff[MAX_COM_LEN + 3];
  register char *pat;
  
  pat = patbuff;
  if (pattern[0] == '*' && pattern[1] == '\0')
    return True;
  if (*pattern == '\0')
    return True;
  if (pad_with_star)
    sprintf (pat, "*%s*", pattern);
  else
    strcpy (pat, pattern);
  for (; *pat; line++, pat++)
    {
      if (*line == '\0' && *pat != '*')
	return True;
      switch (*pat)
	{
	case '\\':
	  pat++;
	default:
	  if (*line != *pat && *line != tolower(*pat) && *line != toupper(*pat))
	    return (False);
	  continue;
	case '?':
	  continue;
	case '*':
	  while (*++pat == '*')
	    continue;
	  if (*pat == '\0')
	    return (True);
	  while (*line)
	    if (check_match (line++, pat, False) != False)
	      return True;
	  return False;
	}
    }
  return *line == '\0';
}
