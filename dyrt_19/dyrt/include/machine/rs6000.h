#ifndef b_rs6000_
#define b_rs6000_

#define MBANNER "AIX"
#define SAFE_USERFILE

#undef HANDLER
#define SYS_EQBUG
#undef NEED_STRFTIME
#undef NEED_RANDOM
#undef USE_XPP
#undef NO_VARARGS
#define CPP "gcc -x c -E -P -traditional -I../include %s"

#endif

