#ifndef __PFLAGS_H__ 
#define __PFLAGS_H__ 


/*
**  Pflag defines
*/

/* First protection bits */
#define	PFL_NOEXOR     0	/* Can not be kicked off */
#define	PFL_NOSNOOP    1	/* Can not be snooped on */
#define	PFL_NOHASSLE   2	/* Can not be attacked */
#define	PFL_NOALIAS    3	/* Can not be aliased into */
#define PFL_NOZAP      4        /* Can not be zapped */
#define PFL_NOSUMMON   5        /* Can not be summoned */
#define PFL_NOTRACE    6        /* Can not be traced */
#define PFL_NOSTEAL    7        /* You cannot steal from him */
#define PFL_NOMAGIC    8        /* Cripple, blind et.c doesn't work */
#define PFL_NOFORCE    9        /* Can not be forced */

/* Priveliges */
#define PFL_ASMORTAL   15       /* may execute commands as a mortal */
#define PFL_FROB       16       /* 17-18 work on others & May use FROB cmd. */
#define PFL_CH_SCORE   17       /* May change score */
#define PFL_CH_LEVEL   18       /* May change level */
#define PFL_CH_MDATA   19       /* May change mobile data */

#define PFL_CLONE      20       /* May use the CLONE command */
#define PFL_LD_STORE   21       /* May use the LOAD and STORE commands */
#define PFL_TIMEOUT    22       /* can use the timeout command */
#define	PFL_PFLAGS     23	/* May set privileges on others */

#define PFL_MASK       24       /* May set priveliges in MASK */
#define PFL_ROOM       25       /* May set location bits and exits */
#define PFL_MFLAGS     26       /* May set mobile bits */
#define PFL_OBJECT     27       /* May set object properties with SET*/

#define PFL_TITLES     28       /* May set titles/setins permanently */
#define PFL_UAF        29       /* May get/set data on players not in game*/
#define PFL_EXOR       30       /* May exorcise players */
#define PFL_RESET      31       /* May reset the game */

#define PFL_SHUTDOWN   32       /* May use the SHUTDOWN and OPENGAME commands*/
#define PFL_SNOOP      33       /* May snoop or trace other players */
#define PFL_HEAL       34       /* May use HEAL */
#define PFL_ALIAS      35       /* May alias mobiles */

#define PFL_ALIASP     36       /* May alias other players */
#define PFL_RAW        37       /* May send RAW messages */
#define PFL_EMOTE      38       /* May use EMOTE */
#define PFL_ECHO       39       /* May use ECHO */

#define PFL_CRASH      40	/* May CRASH the game */
#define PFL_ZAP        41       /* May use zap */
#define PFL_RES        42       /* May use resurect */
#define PFL_SHUSER     43       /* User-id show up on USERS command and entry*/

#define PFL_STATS      44       /* May use STAT and SHOW and PRIVS */
#define PFL_GOTO       45       /* May use GOTO and IN/AT */
#define PFL_OPENG      46	/* May open the game */	
#define PFL_SUMOBJ     47       /* May summon objects */

#define PFL_WEATHER    48       /* May control the weather */
#define PFL_LOCK       49       /* May lock the game */
#define PFL_DELETE     50	/* May delete other players */	
#define PFL_WRECK      51       /* May wreck doors (don't need key!) */

#define PFL_PEACE      52       /* May declare peace or war */
#define PFL_SYSLOG     53       /* May look at the system log-file */
#define PFL_STARTINVIS 54	/* May start as invisible */
#define PFL_TRACE      55       /* May trace players/objects */

#define PFL_HEALALL    56	/* May heal all non-fighting players at once */
#define PFL_ECHOALL    57	/* May echo to the whole game */
#define PFL_NAMERAW    58	/* May raw WITH their name in it */
#define PFL_MAYCODE    59	/* Give this to coders to use "coding" pflag */
#define PFL_DISCONNECT 60	/* May disconnect a player */
#define PFL_SOCKET     61	/* May use Socinfo and Killsoc */
#define PFL_GATEMAIL   62	/* Gateway mail privs */
#define PFL_INTERMUD	63	/* Has access to the intermud */
#define PFL_BAN		64	/* May ban players/hosts online */

#define PFL_MAX        96       /* One more than max p-flag */

#endif /* Add nothing past this line... */
