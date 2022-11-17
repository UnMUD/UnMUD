#ifndef __LFLAGS_H__
#define __LFLAGS_H__

/* Location flags */

/* First two bits specify light */
#define LFL_L_SHIFT 0 /* Shift count for light */
#define LFL_LIGHT 00  /* No need for light src here (default) */
#define LFL_DARK 01   /* Always dark */
#define LFL_L_REAL 02 /* Dark in the night but light in the day */
#define LFL_L_MASK 03 /* Mask for light flags */

/* Next two bits are temparature */
#define LFL_T_SHIFT 2      /* Shift count for temparature */
#define LFL_T_ORDINARY 000 /* Normal (default) */
#define LFL_HOT 004        /* Need protection */
#define LFL_COLD 010       /* need protection */
#define LFL_T_REAL 014     /* Changing with day/night and season */
#define LFL_T_MASK 014     /* Mask for termparature flags */

#define LFL_B_START 4 /* The bits start here */

#define LFL_DEATH 4       /* Mortals get kicked out if they come here */
#define LFL_CANT_SUMMON 5 /* Can't summon people away from this room */
#define LFL_NO_SUMMON 6   /* Can't do summon in this room */
#define LFL_NO_QUIT 7     /* Can't quit in this room */
#define LFL_NO_SNOOP 8    /* Can't snoop people in this room */
#define LFL_NO_MOBILES 9  /* No mobiles here, safe for mortals */
#define LFL_NO_MAGIC 10   /* Spells are forbidden */
#define LFL_PEACEFUL 11   /* No spells, violent actions, etc. */
#define LFL_SOUNDPROOF 12 /* You can't hear them; they can't hear you */
#define LFL_ONE_PERSON 13 /* For when you want some time alone */
#define LFL_PARTY 14      /* All players may emote */
#define LFL_PRIVATE 15    /* For when you don't want to be disturbed ;) */
#define LFL_ON_WATER 16   /* You will need a boat to go here */
#define LFL_IN_WATER 17   /* You will need special items to breathe */
#define LFL_OUTDOORS 18   /* Weather Messages will be received */
#define LFL_T_EXTREME 19  /* HOT in the day, COLD in the night */
#define LFL_NEG_REGEN 20  /* Negative regeneration, eat a lot! */
#define LFL_REALTIME 21   /* Rooms go by the normal calendar and clock */
#define LFL_MAZE 22       /* Wizards do not receive exits */
#define LFL_WIZONLY 23    /* Only wizzes+ here without invitation */
#define LFL_AWIZONLY 24   /* Only awizzes+ here without invitation */
#define LFL_DGODONLY 25   /* Only dgods+ here without invitation */
#define LFL_GODONLY 26    /* Only gods+ here without invitation */
#define LFL_JAIL 27       /* room has many restrictions for bad people */
#endif                    /* Add nothing past this line... */
