#ifndef b_mips_
#define b_mips_

#define MBANNER "mips-ep/ix"
#define SAFE_USERFILE

#undef HANDLER
#undef EQ_BUG
#define NEED_STRFTIME /* for the life of me I couldn't find strftime on this \
                         machine.. tho the man page exists... */
#undef NEED_RANDOM
#define USE_XPP
#undef NO_VARARGS
#define S_ISFIFO(m) (((m)&S_IFMT) == S_IFIFO)

#endif
