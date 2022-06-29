#ifndef __SFLAGS_H__ 
#define __SFLAGS_H__ 


/*
**  Sflag defines
*/
#define	SFL_FEMALE	0	/* If you don't know, you're too young */
#define SFL_GLOWING	1	/* Player (wizard+) is lit */
#define SFL_COLOR	2	/* ANSI color support */
#define SFL_NOSHOUT	3	/* Don't receive shouts */
#define	SFL_BRIEF	4	/* Brief mode, no long descriptions */
#define SFL_QUIET	5	/* Doesn't receive []'d messages */
#define	SFL_OCCUPIED	6	/* This mobile is aliased */
#define SFL_ALOOF	7	/* May not be bothered by mortals */
#define SFL_BLIND	8	/* Player is blind */
#define SFL_DEAF	9	/* Player is deaf */
#define SFL_DUMB	10	/* Player can't speak */
#define SFL_CRIPPLED	11	/* Player can't move */
#define SFL_BUSY        12      /* Don't use tell, emoteto et.c. */
#define SFL_NO_WIZ      13      /* Wizard can't hear 'wiz' messages. */
#define SFL_NO_GOSSIP	14	/* ignore gossip messages */
#define SFL_NO_AWIZ	15	/* ignore awiz channel */
#define SFL_NO_DGOD	16	/* ignore dgod channel */
#define SFL_NO_GOD	17	/* ignore god channel */
#define SFL_AWAY	18	/* tells others that you are afk */
#define SFL_AUTOEXIT 	19	/* show exits automatically */
#define SFL_NOSLAIN	20	/* will not see death messages */
#define SFL_SAYBACK	21	/* echos of what you say and do */
#define SFL_NOBATTLE	22	/* will not see battle occuring in the room */
#define SFL_NOINVEN	23	/* will not see the inventories of players */
#define SFL_CODING	24	/* Will be seen as coding on the who list */
#define SFL_COMPACT     25	/* Newstyle output (no prompt on every line) */
#define SFL_NOBEEP	26	/* doesn't hear beeps */
#define SFL_POSTING	27	/* player is posting on board */
#define SFL_GHOST	28	/* Kinda self-explanatory. */
#define SFL_INTERVIS	29	/* Invis to the intermud */
#define SFL_NO_IGOSSIP	30	/* Does not receive intermud gossip channel */
#define SFL_DRUNK	31	/* Player is drunk */
#endif /* Add nothing past this line... */
