#ifndef __CONFIG_H__ 
#define __CONFIG_H__ 

/*
 *
 *  === C O N F I G U R A T I O N    S E C T I O N ===
 *
 */

#define MASTERUSER      "Rex"    /* Name of the MASTER character */
#define UNVEIL_PASS     "unveil"         /* The UNVEIL-password. */

#define MUD_NAME	"Milliways"
#define ADMIN_EMAIL	"kmowry@hollyfeld.org"

#define PORT            6715          /* port to use if -p option is missing*/

/* The absolute maximum number of locations/objects/mobiles (permanent +
 * all in-game wizard-made) you want in the game at any one time:
 */
#define GLOBAL_MAX_MOBS 1500
#define GLOBAL_MAX_LOCS 3000
#define GLOBAL_MAX_OBJS 1500



/* Some system dependent defines:
 */



#define VARGS  /* by default, it is on */
#ifdef NO_VARARGS
# undef VARGS
#endif

#ifdef NEED_RANDOM
# define SYS_NO_RANDOM
#endif



/* what to log */
#define LOG_RESURRECT
#define LOG_STORE
#define LOG_LOAD
#define LOG_ALIAS
#define LOG_TOUT
#define LOG_STEAL
#define LOG_SNOOP
#define LOG_HEAL
#define LOG_ZAP
#define LOG_SLAIN
#define LOG_EXORCISE  
#define LOG_CLONE     /* log destroy and clone */
#define LOG_RESET     /* logs reset (zone or total) */
#undef LOG_WAR        /* logs war and peace */
#define LOG_SET       /* not implemented */

#define P_LAST_LOGIN  /* print last login on login */
#define TCP_ANNOUNCE  /* announce tcp connections */


#define LINES 20


#endif /* Add nothing past this line... */
