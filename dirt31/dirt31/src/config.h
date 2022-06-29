/*
 *                   DIRT 3.1.2 / AberMUD 4
 *
 *
 *  === C O N F I G U R A T I O N    S E C T I O N ===
 *
 */

#define MASTERUSER      "Nicknack"    /* Name of the MASTER character */
#define UNVEIL_PASS     "xyz"         /* The UNVEIL-password. */

#define PORT            6715          /* port to use if -p option is missing*/

/* The absolute maximum number of locations/objects/mobiles (permanent +
 * all in-game wizard-made) you want in the game at any one time:
 */
#define GLOBAL_MAX_MOBS 500
#define GLOBAL_MAX_LOCS 3000
#define GLOBAL_MAX_OBJS 1500





/* Some system dependent defines:
 */

/* If the two routines strcasecmp() and strncasecmp() is missing from your
 * system, define SYS_EQBUG.
 */
/* #define SYS_EQBUG /* */


/* If variable number of args is implemented, define VARGS.
 */
#define VARGS  /* */


/* If your system doesn't have the random() function, define SYS_NO_RANDOM
 */
/* #define SYS_NO_RANDOM /* */


/* If we are on a Hewlett Packard HP/UX machine, define SYS_HP_UX
 */
/* #define SYS_HP_UX /* */


/* If the prototypes for inet_ntoa in the systems include files
 * are erroneous, define SYS_INET_NTOA_BUG
 */
#define SYS_INET_NTOA_BUG /* */



