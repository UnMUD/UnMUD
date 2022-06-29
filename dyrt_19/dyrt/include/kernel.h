#ifndef __KERNEL_H__ 
#define __KERNEL_H__ 
#include "_VERSION_"

#include <sys/types.h>
#include <sys/param.h>
#include <setjmp.h>
#include <netinet/in.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "machines.h"


/* Include user-specified configuration constants and defines. */
#include "config.h"

#include "levels.h"

#include "exits.h"
#include "types.h"
#include "utils.h"
#include "mudtypes.h"
#include "mudmacros.h"
#include "extern.h"
#include "files.h"


#define NEW(t, c)		((t *)xmalloc((c), sizeof (t)))
#define BCOPY(s,l)              memcpy(xmalloc(1, l),s,l)
#define COPY(s)			strcpy(NEW(char, strlen(s) + 1), s)
#define EMPTY(p)		(*(p) == '\0')
#define EQ(a, b)		(strcasecmp((a), (b)) == 0)

#define OPERATOR(n)	(EQ(n,MASTERUSER))

#define WIZZONE_EXIST_H  72L      /* How many hours will a wizard's zone be
                                     kept in the game without him being on ? */


#define DO_EMOTE 0x01
#define DO_EMOTETO 0x02
#define DO_EMOTEALL 0x03
#define DO_ECHO 0x04
#define DO_ECHOTO 0x05
#define DO_ECHOALL 0x06

#endif /* Add nothing past this line... */
