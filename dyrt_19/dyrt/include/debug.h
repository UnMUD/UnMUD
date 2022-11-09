#ifndef __DEBUG_H__ 
#define __DEBUG_H__ 



#ifndef debug_h
#define debug_h

#define PRINT mudlog  /* function used to print debugging info */
#define USERINFO 1    /* show user/host info on crash */


void install_handler (char *prog, char *srcdir);
void remove_handler(void);


/* supported machines.. currently the handler is only installed
   on sparcs, since it hasn't been tested on other machines */

#ifndef sparc
# ifdef HANLDER
#  undef HANDLER
# endif
#endif

#endif

#endif /* Add nothing past this line... */
