#ifndef __OBJSYS_H__ 
#define __OBJSYS_H__ 


Boolean ispit(register int o);
void	givecom(void);
void	giveallcom(void);
void	dogive(int ob, int pl);
void	stealcom(void);
void	wearcom(void);
void	removecom(void);
void	valuecom(void);
void equipcom(void);
void    eatcom(void);
void	inventory(void);
void closecom(void);
void opencom (void);
void lockcom(void);
void unlockcom(void);
void	mlobjsat(int x, int m);
void	aobjsat(int loc, int mode, int marg);
void listobject(int loc, int mode);
Boolean iscontin(int o1, int o2);
int     obj_loc(int obj);
int	fobn(char *word);
int	fobna(char *word);
int	fobnin(char *word, int ct);
int	fobnc(char *word);
int	fobncb(char *word, int by);
int	fobnh(char *word);
int     find_object_by_id(long int id);
int     fobn_can_take(char *word);
int     getcom(void);

Boolean ishere(int item);
Boolean iscarrby(int item, int user);
Boolean in_inventory(int obj, int player);
int     dropobj(int obj);
void    dropinpit(int o);
void	list_objects(int n,Boolean f);
void    dumpitems(void);
void    dumpstuff(int n, int loc);
Boolean set_weapon(int plr, int wpn);
typedef struct 
{
  char *class_name;
  OFLAGS class_mask;
  short class_state;
} 
CLASS_DATA;
CLASS_DATA *findclass(char *n);
int value_class(CLASS_DATA *cl, int plx, Boolean silent);
void	oplong(int x);
int	gotanything(int x);
Boolean cancarry(int plyr);
int     iswornby( int ob, int plr);
Boolean isavl(int ob);
Boolean reset_object(int o);
void	setobjstate(int o, int v);
void	destroy(int ob);
void	eat(int ob);
void	create(int ob);
void	setoloc(int ob, int l, int c);
Boolean ohany(int mask);
Boolean p_ohany(int plr,int mask);
Boolean ishere(int item);
Boolean p_ishere(int plr,int item);
int	ovalue(int ob);
char	*xdesloc(char *b, int loc, int cf);
void	desloc(int loc, int cf);
Boolean	otstmask(int ob, int v);
Boolean willhold(int x, int y);
int	ohereandget(void);
void	drop_some_objects(int plx);
void    desrm(int loc, int cf);
char   *xdesrm(char *b, int loc, int cf);


Boolean is_shield(int obj);
Boolean wears_shield(int pl);
Boolean is_armor(int obj);
Boolean wears_armor(int pl);
Boolean is_mask(int obj);
Boolean wears_mask(int pl);
Boolean is_boat(int obj);
Boolean carries_boat(int pl);
int     carries_obj_type(int pl, int type);
int     wears_obj_type(int pl, int type);
void    print_inventory(int who);
void    wearall(void);
#define O_BVALUE_MAX 10000
#define O_SIZE_MAX 30000
#define O_VIS_MAX LVL_GOD
#define O_DAM_MAX 100
#define O_ARMOR_MAX 100

#endif /* Add nothing past this line... */
