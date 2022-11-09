#ifndef __MACROS_H__ 
#define __MACROS_H__ 


/* $Id: macros.h,v 1.1 1993/12/18 14:37:14 vpopesc Exp $ */

/* $Log: macros.h,v $
 * Revision 1.1  1993/12/18  14:37:14  vpopesc
 * Initial revision
 * */




#define min(x,y)                ((x)>(y)?(y):(x))
#define max(x,y)                ((x)<(y)?(y):(x))
#define arraysize(a)            (sizeof(a)/sizeof(a[0]))
#define EVEN(N)                 (((N)&1) == 0)
#define ODD(N)                  (((N)&1) != 0)

#define setbits(W,F)		((W) |= (F))
#define clrbits(W,F)		((W) &= ~(F))

/* Extracts the bits in mask M from W */
#define bits(W,M)		((W) & (M))

#define xsetbit(W,V)		setbits((W),(1 << V))
#define xclrbit(W,V)		clrbits((W),(1 << V))
#define xtstbit(W,V)	       	bits((W),(1 << (V)))

#ifndef SYS_NO_RANDOM
#define my_random()             ((unsigned int)random())
#else
#define my_random()             ((unsigned int)(rand() >> 8))
#endif

#endif /* Add nothing past this line... */
