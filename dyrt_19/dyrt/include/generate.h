#define DIR_SIZE 1024
#define ZONE_COUNT 500
#define MOBS_COUNT 1500
#define OBJS_COUNT 5000
#define LOCS_COUNT 5000
#define TEXT_SIZE 1000000

/* cpp is defined in the particular machine file */

#ifndef NullF
#define NullF (FILE *)0
#endif
#ifndef ZCAT
#define ZCAT "/usr/ucb/zcat"
#endif

#ifdef GOOD
#undef GOOD
#endif

#ifdef BAD
#undef BAD
#endif

#ifdef VARGS
#include <stdarg.h>
#endif

#define get_oref(z, F) ((XOBJ *)get_ref('O', z, F))
#define get_lref(z, F) get_ref('L', z, F)
#define get_mref(z, F) ((XMOB *)get_ref('M', z, F))

#define UNKNOWN ((XOBJ *)(-1))
#define BAD ((XOBJ *)(-2))
#define GOOD ((XOBJ *)(-3))
#define REFERRED ((XOBJ *)(-4))
#define DEFINED ((XOBJ *)(-5))

#define IS_LINK 0
#define IS_CONTAINED 1
#define IS_CHECK 2

typedef struct _XVERBS
{ /* move this inside make verbs ? */
  struct _XVERBS *next;
  int code;
  char name[30];
} XVERBS;

typedef struct _XZON
{
  char *name;
  char *fname;
  int zone;  /* This zone's number */
  int loc;   /* The loc number of room ZONE0 */
  int n_loc; /* Number of locations in this zone */
  int mob;   /* The mob number of first mobile in zone */
  int n_mob; /* Number of mobiles in this zone */
  struct _XLOC *locs;
  struct _XOBJ *objs;
  struct _XMOB *mobs;
  struct _XLOC *rlocs;
  struct _XMOB *rmobs;
} XZON;

typedef struct _XMOB
{
  struct _XOBJ *aux;
  struct _XZON *zone;
  struct _XMOB *next;
  char *name;
  char *pname;
  int mob;
  struct _XLOC *loc;
  int str;
  int armor;
  int damage;
  int agg;
  int speed;
  int vis;
  int wimpy;
  PFLAGS pflags;
  SFLAGS sflags;
  MFLAGS mflags;
  int attitude;
  int att_param;
  char *desc;
  char *exam;
} XMOB;

typedef struct _XOBJ
{
  struct _XOBJ *aux;
  struct _XZON *zone;
  struct _XOBJ *next;
  char *name;
  struct _XOBJ *the_next;
  struct _XOBJ *linked;
  char *pname;
  char *aname;
  int obj;
  int cflag;
  struct _XLOC *loc;
  unsigned char damage;
  unsigned char armor;
  OFLAGS oflags;
  int state;
  int mstate;
  int bvalue;
  int osize;
  int oweight;
  char *desc[4];
  char *examine;
  int vis;
} XOBJ;

typedef struct _XLOC
{
  struct _XOBJ *aux;
  struct _XZON *zone;
  struct _XLOC *next;
  char *name;
  char *pname;
  int loc;
  LFLAGS lflags;
  char exit_types[NEXITS];
  struct _XLOC *exits[NEXITS];
  char *description;
} XLOC;

#define T_NAME 0
#define T_PNAME 1
#define T_LOC 2
#define T_DESC 3
#define T_END 4

#define TMOB_STR 5
#define TMOB_DAM 6
#define TMOB_ARMOR 7
#define TMOB_AGG 8
#define TMOB_SFLAGS 9
#define TMOB_PFLAGS 10
#define TMOB_MFLAGS 11
#define TMOB_SPEED 12
#define TMOB_EXAM 13
#define TMOB_VIS 14
#define TMOB_WIMPY 15

#define TOBJ_ANAME 5
#define TOBJ_OFLAGS 6
#define TOBJ_ARMOR 7
#define TOBJ_DAMAGE 8
#define TOBJ_MAX_STATE 9
#define TOBJ_STATE 10
#define TOBJ_BVALUE 11
#define TOBJ_SIZE 12
#define TOBJ_WEIGHT 13
#define TOBJ_EXAM 14
#define TOBJ_LINKED 15
#define TOBJ_VIS 16

char *Mob_tab[] =
    {
        "Name", "Pname", "Location", "Description", "End",
        "Strength", "Damage", "Armor", "Aggression", "SFlags",
        "PFlags", "MFlags", "Speed", "Examine", "Visibility",
        "Wimpy", TABLE_END};

char *Obj_tab[] =
    {
        "Name", "Pname", "Location", "Description", "End",
        "AltName", "Oflags", "Armor", "Damage",
        "MaxState", "State", "BValue", "Size",
        "Weight", "Examine", "Linked", "Visibility",
        TABLE_END};

char *Cflags[] =
    {
        "In room", "In container", "Carried by", "Worn by",
        "Wielded by", "Both worn and wielded by"};

/* generate.c */
int main(int argc, char *argv[]);
char *alloc_text(int s);
char *save_text(char *t);
int read_main(char *mf);
int lookup(char *s, char **t);
XMOB *find_mob(XZON *z, char *name);
XOBJ *find_obj(XZON *z, char *name);
XLOC *find_loc(XZON *z, char *name);
void mkdef_mob(XZON *z, XMOB *m);
void mkdef_loc(XZON *z, XLOC *l);
void read_zones(XZON *zo, int nz);
void clean_up(XZON *zon, int nz, int *mm, int *oo, int *lo, int *ll);
void write_zone(XZON *ZON, int numz);
void write_mob(XZON *ZON, int numz, int m);
void write_obj(XZON *ZON, int numz, int o, int l);
void write_loc(XZON *ZON, int numz, int l);
void write_files(XZON *ZON, int numz, int l, int o, int lo, int m);
void init_rand(void);
int randperc(void);
char *x_strcpy(char *d, char *s);
int glookup(char *elem, int n, char **table, int (*strcmpfun)(char *s1, char *s2, int n));
int tlookup(char *elem, char **table);
int xlookup(char *elem, char **table);
char *lowercase(char *str);
char *uppercase(char *str);
void *xmalloc(int nelem, int elem_size);
char *my_crypt(char *buf, char *pw, int len);
char *mk_string(char *b, char *str, int k, int stopch);
void *resize_array(void *start, int elem_size, int oldlen, int newlen);
char *safe_strcpy(char *s1, char *s2);
