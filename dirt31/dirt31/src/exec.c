#include <setjmp.h>

#include "kernel.h"
#include "condact.h"
#include "exec.h"

Boolean doing_table = False;

jmp_buf exec_jmp;

/*
**  The executive to drive the tables
*/

int do_tables(int a)
{
  int x;

#if 0
  bprintf( "do_tables(%d); is called.\n", a);
#endif
  if (!parse_2(a)) return 2;
  x = exectable(event,a);
#if 0
  bprintf( "exectable(event,%d) == %d.\n", a, x);
#endif
  return x;
}

/*
** return value:
** 0: Didn't find any match.
** 1: Found a match, but conditions didn't match so we didn't do anything.
** 2: Found a match and performed the actions.
*/
int exectable(LINE *v,int vb)
{
  int m = 0;
  int ct;

  if (doing_table) return 0;

  if (setjmp(exec_jmp) == 0) {
    doing_table = True;
    for (;v->verb != -1; ++v) {
      if ( (v->verb == 0 || v->verb == vb) &&
	  exec_match(v->item1,ob1,pl1) &&
	  exec_match(v->item2,ob2,pl2)) {
	/* Found a match in vocab */
#if 0
	bprintf( "Found match for %d\n", vb);
#endif
	m = 1;
	if (check_conditions(v->conditions)) {
#if 0
	  bprintf( "Conditions ok for %d\n", vb);
#endif
	  pptr = 0;
	  m = 2;
	  do_actions(v->actions);
	  doing_table = False;
	  return 2;
	}
      }
    }
  }
  doing_table = False;
  return m;
}

void error(void)
{
  if (doing_table)
    longjmp(exec_jmp,1);
}


