#ifndef __EXITS_H__ 
#define __EXITS_H__ 


/* Exit numbers DOOR...EDOOR - 1 are doors
 */
#define DOOR	10000
#define EDOOR	20000

/* Special exits
 */
#define EX_SPECIAL    20000
#define EX_DEFENDER   20000
#define EX_GET_STUFF  20001
#define EX_DOWN_SLIME 20200
#define EX_MAX        30000

#define EX_OPEN   0
#define EX_CLOSED 1
#define EX_LOCKED 2

#define NEXITS	 10   /* number of exits there can be from a room */

#define EX_NORTH 0
#define EX_EAST  1
#define EX_SOUTH 2
#define EX_WEST  3
#define EX_UP    4
#define EX_DOWN  5
#define EX_NORTHEAST 6
#define EX_SOUTHEAST 7
#define EX_SOUTHWEST 8
#define EX_NORTHWEST 9



#endif /* Add nothing past this line... */
