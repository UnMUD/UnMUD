/*
**	Location flags
*/

/* First two bits specify light */
#define LFL_L_SHIFT	0	/* Shift count for light */
#define LFL_LIGHT       00	/* No need for light src here (default) */
#define LFL_DARK	01	/* Always dark */
#define LFL_L_REAL	02	/* Dark in the night but light in the day */
#define LFL_L_MASK      03	/* Mask for light flags */

/* Next two bits are temparature */
#define LFL_T_SHIFT	2	/* Shift count for temparature */
#define LFL_T_ORDINARY	000	/* Normal (default) */
#define LFL_HOT		004	/* Need protection */
#define LFL_COLD	010	/* need protection */
#define LFL_T_REAL	014	/* Changing with day/night and season */
#define LFL_T_MASK	014	/* Mask for termparature flags */

#define LFL_B_START	4	/* The bits start here */
/* the rest are bits */

#define LFL_DEATH	4	/* Mortals get kicked out if they come here */
#define LFL_CANT_SUMMON	5	/* Can't summon people away from this room */
#define LFL_NO_SUMMON	6	/* Can't do summon in this room */
#define LFL_NO_QUIT	7	/* Can't quit in this room */
#define LFL_NO_SNOOP	8
#define LFL_NO_MOBILES	9
#define LFL_NO_MAGIC	10
#define LFL_PEACEFUL	11
#define LFL_SOUNDPROOF	12
#define LFL_ONE_PERSON	13
#define LFL_PARTY	14
#define LFL_PRIVATE	15
#define LFL_ON_WATER	16
#define LFL_IN_WATER	17	/* not implemented */
#define LFL_OUTDOORS	18
#define LFL_T_EXTREME	19	/* HOT in the day, COLD in the night */
#define LFL_NEG_REGEN	20	/* Negative regeneration, eat a lot! */
