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
#define PFL_FROB       16       /* 17-18 work on others & May use FROB cmd. */
#define PFL_CH_SCORE   17       /* May change score */
#define PFL_CH_LEVEL   18       /* May change level */
#define PFL_CH_MDATA   19       /* May change mobile data */

#define PFL_CLONE      21       /* May use the CLONE command */
#define PFL_LD_STORE   22       /* May use the LOAD and STORE commands */

#define	PFL_PFLAGS     24	/* May set privileges on others */
#define PFL_MASK       25       /* May set priveliges in MASK */
#define PFL_ROOM       26       /* May set location bits and exits */
#define PFL_MFLAGS     27       /* May set mobile bits */
#define PFL_OBJECT     28       /* May set object properties with SET*/
#define PFL_TITLES     29       /* May set titles/setins permanently */

#define PFL_UAF        31       /* May get/set data on players not in game*/
#define PFL_EXOR       32       /* May exorcise players */
#define PFL_RESET      33       /* May reset the game */
#define PFL_SHUTDOWN   34       /* May use the SHUTDOWN and OPENGAME commands*/
#define PFL_SNOOP      35       /* May snoop or trace other players */
#define PFL_HEAL       36       /* May use HEAL */
#define PFL_ALIAS      37       /* May alias mobiles */
#define PFL_ALIASP     38       /* May alias other players */
#define PFL_RAW        39       /* May send RAW messages */
#define PFL_EMOTE      40       /* May use EMOTE */
#define PFL_ECHO       41       /* May use ECHO */

#define PFL_ZAP        43       /* May use zap */
#define PFL_RES        44       /* May use resurect */
#define PFL_SHUSER     45       /* User-id show up on USERS command and entry*/
#define PFL_STATS      46       /* May use STAT and SHOW and PRIVS */
#define PFL_GOTO       47       /* May use GOTO and IN/AT */

#define PFL_SUMOBJ     49       /* May summon objects */
#define PFL_WEATHER    50       /* May control the weather */
#define PFL_LOCK       51       /* May lock the game */

#define PFL_WRECK      53       /* May wreck doors (don't need key!) */

#define PFL_PEACE      55       /* May declare peace or war */
#define PFL_SYSLOG     56       /* May look at the system log-file */
#define PFL_STARTINVIS 57	/* May start as invisible */
#define PFL_TRACE      58       /* May trace players/objects */

#define PFL_MAX        64       /* One more than max p-flag */
