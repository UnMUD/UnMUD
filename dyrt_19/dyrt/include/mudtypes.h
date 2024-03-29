#ifndef __MUDTYPES_H__
#define __MUDTYPES_H__

#include "types.h"
#include <time.h>
#include <sys/time.h>

typedef long int LFLAGS;
typedef long int OFLAGS;
typedef DoubleLongInt PFLAGS;
typedef long int SFLAGS;
typedef LongInt MFLAGS;
typedef long int QFLAGS; /* Quest flags */

#define TITLE_LEN 55    /* Max. length of a players title */
#define PROMPT_LEN 20   /* Max. length of the prompt */
#define PNAME_LEN 12    /* Max. length of a player-name */
#define MNAME_LEN 16    /* Max length, mobile-name */
#define ONAME_LEN 32    /* Max length, object-name */
#define SETIN_MAX 90    /* Max length, travel-message */
#define TTY_LEN 64      /* Max length, name of players host */
#define MSG_LEN 240     /* Max length of a message */
#define PASSWD_LEN 16   /* Max chars of passwd + 1 */
#define MAX_COM_LEN 300 /* Max chars in a command line */
#define NUM_CONDS 6
#define NUM_ACTS 4
#define MAX_EMAIL_LEN 128 /* max chars in email address */

#define YN_INIT (0)
#define ASSUME_YES (-1)
#define ASSUME_NO (1)
#define ASSUME_NOTHING (0)
#define YN_YES (-1)
#define YN_NO (1)

typedef struct _yn_struct
{
  void (*yn_func)(int yn);
  int assume;
  char prompt[MAX_COM_LEN];
  char old_prompt[PROMPT_LEN + 20];
  void (*old_handler)(char *str);
} YN_STRUCT;

/* Trace struct */
typedef struct
{
  int trace_item;  /* number for item/person */
  int trace_class; /* 1 = obj, 2 = person */
  int trace_loc;   /* location of the item/person */
  int trace_carrf; /* carry flag */
  int trace_oroom; /* the room an object is in */
} TRACE;

/*
 * Setin struct
 */
typedef struct _SETIN_STRUCT
{
  char prompt[PROMPT_LEN + 20];
  char setin[SETIN_MAX + 32];
  char setout[SETIN_MAX + 32];
  char setmin[SETIN_MAX + 32];
  char setmout[SETIN_MAX + 32];
  char setvin[SETIN_MAX + 32];
  char setvout[SETIN_MAX + 32];
  char setqin[SETIN_MAX + 32];
  char setqout[SETIN_MAX + 32];
  char setsit[SETIN_MAX + 32];
  char setstand[SETIN_MAX + 32];
  char settrenter[SETIN_MAX + 32];
  char settrvict[SETIN_MAX + 32];
  char settrroom[SETIN_MAX + 32];
} SETIN_STRUCT;

/* Party code */
typedef struct _party
{
  int leader;           /* The leader of the group */
  char name[PNAME_LEN]; /* Group name, will show up in who */
  int shares;           /* Total amount of shares */
  int xp;               /* xp gained by the group */
} PARTY;

/* A setin-file entry.
 */
typedef struct _SETIN_REC
{
  char name[PNAME_LEN + 1];
  char prompt[PROMPT_LEN + 32];
  char setin[SETIN_MAX + 32];
  char setout[SETIN_MAX + 32];
  char setmin[SETIN_MAX + 32];
  char setmout[SETIN_MAX + 32];
  char setvin[SETIN_MAX + 32];
  char setvout[SETIN_MAX + 32];
  char setqin[SETIN_MAX + 32];
  char setqout[SETIN_MAX + 32];
  char setsit[SETIN_MAX + 32];
  char setstand[SETIN_MAX + 32];
  char settrenter[SETIN_MAX + 32];
  char settrvict[SETIN_MAX + 32];
  char settrroom[SETIN_MAX + 32];
} SETIN_REC;

/* A condition as defined in database.c
 */
typedef struct _CONDITION
{
  short cnd_code;
  short cnd_data;
} CONDITION;

typedef short ACTION;

/* A line as defined in database.c
 */
typedef struct _LINE
{
  short verb;
  short item1;
  short item2;
  CONDITION conditions[NUM_CONDS];
  ACTION actions[NUM_ACTS];
} LINE;

/* A PERSONA as described in the uaf_rand file
 */
typedef struct _PERSONA
{
  char p_name[PNAME_LEN + 1];
  char p_title[TITLE_LEN + 1];
  char p_email[MAX_EMAIL_LEN + 1];
  char p_passwd[PASSWD_LEN];
  long int p_home;
  int p_score;
  int p_strength;
  int p_damage;
  int p_armor;
  SFLAGS p_sflags;
  PFLAGS p_pflags;
  PFLAGS p_mask;
  QFLAGS p_quests;
  int p_vlevel;
  int p_level;
  time_t p_last_on;
  int p_wimpy;
  long int p_id;
} PERSONA;

/* zone table entry
 */
typedef struct _ZONE
{
  char *z_name; /* Zone name */

  int maxlocs;
  int maxmobs;
  int maxobjs;

  int_set locations;
  int_set mobiles;
  int_set objects;

  Boolean temporary; /* Real zone, or one to destruct on reset ? */
} ZONE;

/* Location data in the rooms arrray as read from locations file
 */
typedef struct
{
  int r_exit[NEXITS];
  LFLAGS r_flags;
  char *r_short;
  char *r_long;
  long int id;
  int_set objects;     /* Set of objects in this loc. */
  int_set mobiles;     /* Set of players/mobiles in this loc. */
  int_set exits_to_me; /* Set of locations that have exits to this */
  Boolean temporary;   /* Can it be destructed on reset ? */
  Boolean touched;     /* Has any exit been changed since last reset?*/
  int zone;            /* To which zone do we belong ? */
                       /* Reset data: */
  long int r_exit_reset[NEXITS];
  LFLAGS r_flags_reset;
} Location;

/* An object
 */
typedef struct
{

  /* Unalterable, no need to reset on a reset: */
  char *oname;       /* objects name */
  char *oaltname;    /* objects alternate name */
  char *odesc[4];    /* descriptions for each state  */
  int omaxstate;     /* max state a wiz can SET an obj to */
  long oexamine;     /* ptr in file where examine text is found */
  char *oexam_text;  /* ptr to exam-txt for in-game created objs */
  long int id;       /* unique ID, needed for new saved objects */
  Boolean temporary; /* Part of a wizards permenent zone ? */
  int linked;        /* Which object, if any, is this linked to ? */
  int zone;          /* To which zone do we belong ? */
  int onum;          /* number for the code to test so that cloned
objects can behave like the originals. */
                     /* Alterable, needs saved reset values too: */
  int ovalue;        /* base value */
  int osize;
  int oloc;
  int ovis; /* the objects visibility level */
  int odamage;
  int oarmor;
  int ocarrf;
  int ostate;
  OFLAGS oflags;

  long int oloc_reset;
  int osize_reset;
  int ovalue_reset;
  int ovis_reset;
  int odamage_reset;
  int oarmor_reset;
  int ocarrf_reset;
  int ostate_reset;
  OFLAGS oflags_reset;

  int_set objects; /* set of objects inside this object */
} Object;

/* A record describing a player or mobile in the world.
 */
typedef struct
{
  char pname[MNAME_LEN + 1];
  char hate[MNAME_LEN + 1];
  int ploc;
  long int phome; /* players start-loc. and home */
  int pdam;       /* Damage */
  int parmor;
  int pagg;   /* Agression */
  int pspeed; /* Speed */
  int pcarry; /* Carrying capacity */
  int pstr;
  int pvis;
  time_t p_last_on;
  SFLAGS psflags;
  PFLAGS pflags;
  PFLAGS pmask;
  MFLAGS pmflags;
  QFLAGS pquests;
  int plev;
  int pweapon;
  int psitting;
  int phelping;
  int pfighting;
  int pscore;
  char *pftxt;       /* Mobile's one-line description */
  char *p_exam;      /* exam-text for mobiles (may also be in DESC)*/
  int pnum;          /* player/mobile number */
  int_set objects;   /* set of objects carried by this character */
  long int id;       /* Unique ID */
  Boolean temporary; /* Mobile part of a wizards permanent stuff ? */
  int zone;          /* To which zone do we belong ? */
  int pwimpy;

  /* Reset data for mobiles: */

  char *pname_reset;
  long int ploc_reset;
  int pstr_reset;
  int pvis_reset;
  SFLAGS psflags_reset;
  PFLAGS pflags_reset;
  MFLAGS pmflags_reset;
  int plev_reset;
  int pagg_reset;   /* Agression */
  int pspeed_reset; /* Speed */
  int pdam_reset;
  int parmor_reset;
  int pwimpy_reset;
} UBLOCK_REC;

typedef UBLOCK_REC Mobile;
typedef UBLOCK_REC Player;

typedef struct _a_inp_h
{
  struct _a_inp_h *next;
  void (*inp_handler)(char *input_string);
} INP_HANDLER;

/* iDIRT Message System */
typedef struct _MAIL
{
  void (*old_handler)(char *str);
  FILE *read;
  FILE *write;
  Boolean abort;
  Boolean gate;
  int pos;     /* Position in list */
  int current; /* Current mail */
  char r_name[14];
  char w_name[14];
  char last[14];
  char subject[MAX_COM_LEN];
  char cc_list[MAX_COM_LEN];
  char data[100];
  Boolean level;
  char prompt[PROMPT_LEN + 20];
} MAIL;

/********************
typedef struct _MAIL {
  void              (*old_handler)(char *str);
  char othernames[2048];
  FILE *read;
  FILE *write;
  char r_name[14];
  char w_name[14];
  char last[14];
  char subject[MAX_COM_LEN];
  char data[100];
  Boolean level;
} MAIL;
*****************************/

typedef struct _PAGER
{
  FILE *file;
  INP_HANDLER *oldhandler;
  int size;
  int read;
  Boolean brief;
  char old_prompt[PROMPT_LEN + 20];
  char *pattern;
  char stopper[20];
} PAGER;

typedef struct _CONVERSE
{
  char old_prompt[PROMPT_LEN + 20];
  int talking_to;
  Boolean active;
} CONVERSE;

typedef struct _EDIT
{
  void (*old_handler)(char *str);
  char old_prompt[PROMPT_LEN + 20];
  int num;
  Object *o;
  Object *o2;
  UBLOCK_REC *mob;
  Location *room;
} EDIT;

/* Player data that are not shared by mobiles.
 */
typedef struct _a_player
{
  time_t last_cmd;
  time_t logged_on;
  time_t p_last_comm; /* last gossip or say */
  int fil_des;
  FILE *stream;
  struct sockaddr_in sin;
  int sin_len;
  INP_HANDLER *inp_handler;
  MAIL mails;       /* Mail structure */
  Boolean inmailer; /* Is the user in the mailer? */
  TRACE tr;         /* trace item struct */
  PAGER pager;
  EDIT edits;
  CONVERSE converse;
  YN_STRUCT yn_handler; /* Miscellaneous crap for the yn handler (; */
  char *inp_buffer_p;
  char *inp_buffer_end;
  char *sock_buffer_p;
  char *sock_buffer_end;
  void *writer;  /* Current writer */
  void *board;   /* similar to writer, to help with board */
  int no_logins; /* No of failed passwd */
  int work;      /* General work area for use by code */
  char work2[64];
  Boolean announced;
  Boolean is_new;       /* a new player */
  Boolean no_echo;      /* True if telnet should turn off echo */
  Boolean isawiz;       /* Is this player a system's wizard? */
  Boolean ismonitored;  /* is this player being monitored? */
  Boolean iamon;        /* Am I on? Init to false. */
  Boolean in_pbfr;      /* Are we busy inside pbfr()? */
  Boolean aliased;      /* Are we aliased? */
  Boolean a_new_player; /* Are we a new player? */
  int quit_next;        /* Next on quit_list, -2 if not in list */
  int polymorphed;      /* Polymorphed for how many more turns? */
  int pretend;          /* Who we pretend to be if aliased or poly */
  int isforce;          /* Set to -1 if not force, else pl. indx. */
  int i_follow;         /* Set to -1 if not following anyone. */
  int me_ivct;          /* Invisible for how many cmds ? */

  int snooped;     /* How many are snooping us? */
  int snooptarget; /* Who if any are we snooping? */
  int asmortal;    /* What level are we pretending to have? */
  int drunk;       /* We are drunk :) */

  char passwd[PASSWD_LEN];
  char cprompt[PROMPT_LEN + 32];
  char prompt[PROMPT_LEN + 32];
  char setin[SETIN_MAX + 32];
  char setout[SETIN_MAX + 32];
  char setmin[SETIN_MAX + 32];
  char setmout[SETIN_MAX + 32];
  char setvin[SETIN_MAX + 32];
  char setvout[SETIN_MAX + 32];
  char setqin[SETIN_MAX + 32];
  char setqout[SETIN_MAX + 32];
  char setsit[SETIN_MAX + 32];
  char setstand[SETIN_MAX + 32];
  char settrenter[SETIN_MAX + 32];
  char settrvict[SETIN_MAX + 32];
  char settrroom[SETIN_MAX + 32];

  int ghostcounter;
  char ghostsetout[SETIN_MAX + 1];
  char ghostsetin[SETIN_MAX + 1];
  char ghostsetstand[SETIN_MAX + 1];
  char ghostsetsit[SETIN_MAX + 1];
  char o_setin[SETIN_MAX + 1]; /* dupl. sets for aliased pl. */
  char o_setout[SETIN_MAX + 1];
  char o_prompt[PROMPT_LEN + 20];

  char wd_her[MNAME_LEN + 1];
  char wd_him[MNAME_LEN + 1];
  char *wd_them;
  char *wd_it;

  char ptitle[TITLE_LEN + 1];
  char hostname[MAXHOSTNAMELEN];
  char prev_com[MAX_COM_LEN];
  char inp_buffer[MAX_COM_LEN * 16];
  char sock_buffer[MAX_COM_LEN * 16];

  char email[MAX_EMAIL_LEN + 1];

  FILE *syslog_fp;
  char syslog_match[10][80];
  int syslog_sameline;
  Boolean syslog_popened;

  int login_time;
  int last_command;
  time_t p_last_on;
  PARTY *party; /* Pointer to master party object */
  int share;    /* Amount of shares owned by the player */

} PLAYER_REC;

/* The world. Contains some global variables.
 */
typedef struct _a_world
{
  int w_msg_low;
  int w_msg_high;
  int w_weather;
  int w_lock;
  int w_mob_stop;
  int w_peace;
  int w_max_users;
  int w_tournament;
  int w_delayed;
  int w_logins;
  int w_record_num;
  PFLAGS w_pflags[10];
  PFLAGS w_mask[10];
} WORLD_REC;

/* ---------- */

typedef struct _calendar
{
  char season;            /* spring, summer, etc  */
  char modifier;          /* late, early, etc     */
  char daytime;           /* morning, noon, etc   */
  char daycount;          /* 30 days per modifier */
  char light;             /* is there light now?  */
  char temp;              /* what is the temp?    */
  unsigned int countdown; /* for timing           */

} CALENDAR;

#endif /* Add nothing past this line... */
