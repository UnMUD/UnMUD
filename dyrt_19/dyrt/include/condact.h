#ifndef __CONDACT_H__ 
#define __CONDACT_H__ 


/* #define A(a,b,c,d)	((a) + ((b) << 8) + ((c) << 16) + ((d) << 24)) */
#define PL_CODE 10000
#define IS_PL(n) ((n) >= PL_CODE)
#define IS_OB(n) ((n) >= 0 && !IS_PL(n))

#define P(n)	 (PL_CODE + (n))
#define O(n)     (n)

/*
**  Conditions
*/
#define PAR		0
#define LEVEQ		1
#define LEVLT		2
#define IN		3
#define GOT		4
#define AVL		5
#define IFDEAF		6
#define IFDUMB		7
#define IFBLIND		8
#define IFCRIPPLED	9
#define IFFIGHTING	10
#define IFSITTING       11
#define PFLAG		12
#define STATE		13
#define EQc		14
#define LT		15
#define GT		16
#define VISLT		18
#define VISEQ		19
#define CANSEE		20
#define STRLT		21
#define STREQ		22
#define ISIN		23
#define HERE		24
#define PLGOT		25
#define DESTROYED	26
#define CHANCE		27
#define OBIT		28
#define INZONE          29
#define ISME		31
#define DARK		32
#define OBJAT		33
#define WIZ		35
#define AWIZ		36
#define PHERE           37
#define PLWORN		39
#define WPN		41
#define ENEMY		42
#define CANGO		44
#define PREP		45
#define ALONE		46
#define WILLFITIN	47
#define CANCARRY	48
#define HASANY          50
#define ISMONSTER	51
#define HELPED          52
#define OP              53
#define OBYTEEQ         54
#define OBYTEGT         55
#define OBYTEZ          56
#define COND_MASK	127
#define NOT		128

/* Actions */

#define NUL		0
#define GAIN		1
#define LOSE		2
#define WOUND		3
#define HEAL		4
#define INC		5
#define DEC		6

#define ADD		7
#define SUB		8
#define MUL		9
#define DIV		10
#define MOD		11
#define LET		12
#define CLR		13
#define SET		14

#define PUT		15
#define PUTIN		16
#define CARRY		17
#define WEAR            18
#define PUTWITH		19

#define OK		20
#define DONE		21
#define DESTROY		22
#define QUIT		23
#define DIE		24

#define GOBY		25
#define DISPROOM	26
#define GOTO		27
#define SWAP		28
#define POBJ		29
#define PPLAYER		30
#define SETLEVEL	32
#define SETVIS		33

/*#define LOBJAT		39*/
#define LOBJIN		40
#define LOBJCARR	41

#define CREATE		42
#define SETSTATE	46

/*
**    These kernel actions will be gradually replaced with database
**    stuff with no checking
*/
#define PL		(-32767) /* The player */

#define COM_INVEN	50
#define COM_EXITS	51
#define COM_MOVE	52
#define COM_SETIN	53
#define COM_SETOUT	54
#define COM_SETMIN	55
#define COM_SETMOUT	56
#define COM_SETVIN	57
#define COM_SETVOUT	58
#define COM_SETQIN	59
#define COM_SETQOUT	60
#define COM_TELL	61
#define COM_SAY		62
#define COM_SHOUT	63
#define COM_WIZMSG	64
#define COM_AWIZMSG	65
#define COM_BUG		66
#define COM_TYPO	67
#define COM_BLIND       68
#define COM_DEAF        69
#define COM_DUMB	70
#define COM_CRIPPLE	71
#define COM_CURE        72
#define COM_LISTFILE    73
#define COM_USERS       74
#define COM_SETSEX      75
#define COM_PRONOUNS    76
#define COM_WHO		77
#define COM_GET		78
#define COM_DROP	79
#define COM_WEAR	80
#define COM_REMOVE	81
#define COM_GIVE	82
#define COM_STEAL	83
#define COM_VALUE	84
#define COM_SCORE	85
#define COM_WEAPON	86
#define COM_PUT		87
#define COM_FORCE	88
#define COM_EXORCISE	89
#define COM_TIME	90
#define COM_SET		91
#define COM_EXAMINE	92
#define COM_LOC		93
#define COM_DIR		94
#define COM_RESET	95
#define COM_IN		96
#define COM_GOTO	97
#define COM_PFLAGS	98
#define COM_PLAYERS	99
#define COM_MOBILES	100
#define COM_WHERE	101
#define DECOBYTE	102
#define INCOBYTE	103
#define SETOBYTE	104
#define PROVOKE		105
#define COM_KILL	106
#define COM_LOOK	107
#define BROAD		108
#if 0
#define CRASH		109
#define UNVEIL		110	/* unimplemented */
#define SAVE		111

#define FLEE		112

#define FIGHT		113
#define FROB		114
#define ZAP		115
#define FOLLOW		116
#define BECOME		117
#define DEBUGMODE	118
#define SUMMON		119
#define WRITE		120	/* unimp */
#define STATS		121
#define EMPTYc		122	/* unimp */
#define ROOMS		123
#define SNOOP		124	/* unimp */
#define LOG		125	/* unimp */
#define EXEC		126	/* unimp */
#define BRIEF		127
#define UPDATE		128
#define TRACE		129
#define EMOTE		130
#define PATCH		131	/* unimp */
#define RMEDIT		132	/* unimp */
#define SHOW		133	/* unimp */
#define LOCNODES	134
#define DROPIN		135
#define GETOUT		136	/* unimp */
#define NORMAL		137	/* unimplemented - hardwired version */
#define BEG		138
#define ROOMNAME	139	/* unimp */
#define GETOLOC		140
#define GETPLOC		141
#define	FOB		142
#define DECOBYTE        143
#define INCOBYTE        144
#define SETOBYTE        145
#define PROVOKE         146
#define MOVEMON         147
#endif

#define WOUNDMON        148

#if 0
#define KILL            149
#endif

#define COM_EAT		150
#define COM_SETSTAND	151
#define COM_SETSIT     160
#define COM_SETTRVICT  161
#define COM_SETTRENTER  162
#define COM_SETTRROOM   163
#define AMASK		0xff

/* Special values. */
#define PL		(-32767) /* The player */
#define OB		(-32766) /* The object */
#define ANY		(-32765) /* Anything */
#define NONE		(-32764) /* Nothing */

#define FL_PL1		20000
#define FL_PL2		20001
#define FL_OB1		20002
#define FL_OB2		20003
#define FL_CURCH	20004
#define FL_PREP		20005
#define FL_LEV		20006
#define FL_STR		20007
#define FL_SEX		20008
#define FL_SCORE	20009
#define FL_SENDER	20010
#define FL_MYNUM	20011


#endif /* Add nothing past this line... */
