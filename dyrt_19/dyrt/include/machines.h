#ifndef __MACHINES_H__ 
#define __MACHINES_H__ 


#ifdef LINUX 
# include "machine/linux.h"
#endif

#ifdef CONCENTRIX
# include "machine/concentrix.h"
#endif

#ifdef MIPS
# include "machine/mips.h"
#endif

#ifdef RS6000
# include "machine/rs6000.h"
#endif

#ifdef SUN
# include "machine/sun.h"
#endif

#endif /* Add nothing past this line... */
