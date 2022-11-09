#ifndef __OFLAGS_H__ 
#define __OFLAGS_H__ 


/*
**	Object flags
*/
#define OFL_DESTROYED	0		/* destroyed */
#define OFL_NOGET	1		/* Flannel */
#define OFL_OPENABLE	2		/* can be opened (1=closed, 0=open */
#define OFL_LOCKABLE	3		/* can be locked (state 2 = locked) */
#define OFL_PUSHABLE	4		/* set state to 0 when pushed */
#define OFL_PUSHTOGGLE	5		/* toggle state when pushed */
#define OFL_FOOD	6		/* can be eaten */
#define OFL_ARMOR	7		/* is armor */
#define OFL_NOLOCATE	8		/* object cannot be located */
#define OFL_LIGHTABLE	9		/* can light */
#define OFL_EXTINGUISH	10		/* can extinguish */
#define OFL_KEY		11		/* is a key */
#define OFL_GETFLIPS	12		/* change to state 0 when taken */
#define OFL_LIT		13		/* is lit */
#define OFL_CONTAINER	14		/* is a container */
#define OFL_WEAPON	15		/* is a weapon */
#define OFL_BOAT	16		/* is a boat */
#define OFL_NEWBIE	17		/* is an object for newbies */
#define OFL_PITRESET	18		/* pitted object will be valued at 0
                                         * and set in the donations room */	
#define OFL_DRINK	19
#define OFL_WEARABLE		20
#define OFL_EXCLUDEWEAR		21
#define OFL_22			22
#define WEAR_OFFSET		23
#define OFL_WEARONFACE		23
#define OFL_WEARONHEAD		24
#define OFL_WEARONNECK		25
#define OFL_WEARONBODY		26
#define OFL_WEARONBACK		27
#define OFL_WEARONARMS		28
#define OFL_WEARONHANDS		29
#define OFL_WEARONLEGS		30
#define OFL_WEARONFEET		31
#define WEAR_LAST		31
#define OFL_ALCOHOL		32
#endif /* Add nothing past this line... */
