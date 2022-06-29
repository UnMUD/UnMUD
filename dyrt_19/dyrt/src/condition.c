/*
 * **  Condition evaluator for the system
 */
#include "kernel.h"
#include "stdinc.h"

void set_msg (char *b, Boolean dir_ok)
{
  char k[MAX_COM_LEN];
  
  getreinput (k);
  /* strcpy (k, rawbuf); */
  
  if (check_setin (k, dir_ok))
    strcpy (b, k);
  else
    bprintf ("Not changed, wrong format.\n");
}

Boolean 
check_setin (char *s, Boolean d)
{
  char *p;
  int nn = 0, dd = 0;
  
  for (p = s; (p = strchr (p, '%')) != NULL;)
    {
      switch (*++p)
	{
	case 'N':
	case 'n':
	  ++nn;
	  break;
	case 'd':
	  ++dd;
	  break;
	}
    }
  return (nn > 0 && (dd == 0 || d) && strlen (s) < SETIN_MAX);
}

void error(void) {}













