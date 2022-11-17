#ifndef __MOBILE_H__
#define __MOBILE_H__

void whocom(void);
void mwhocom(void);
void mobilecom(void);

int player_damage(int player);
int player_armor(int player);

Boolean dragget(void);
void list_people(void);
void move_mobiles(void);
void onlook(void);
void setpscore(int pl, int newsc);
void chkfight(int mon);
void lastoncom(void);
void consid_move(int mon);
void dorune(int plx);
void showemail(void);
void helpchkr(int plx);
void movemob(int x);
void stopcom(void);
void startcom(void);
Boolean stealstuff(int m);
Boolean shiftstuff(int m);
char *xname(char *n);
void setname(int plx);
Boolean see_player(int pla, int plb);
Boolean seeplayer(int plx);
char *see_name(int pla, int plb);
int fpbn(char *name);
int find_player_by_name(char *name);
int find_player(char *name, PERSONA *p, Boolean *f);
int find_mobile_by_id(long int id);
int alive(int i);
int wlevel(int lev);
Boolean do_okay_l(int p, int v, Boolean c);
Boolean do_okay(int p, int v, int prot_flag);
void setpsex(int chr, Boolean v);
void setploc(int plr, int room);
int ptothlp(int pl);
int maxstrength(int p);
void destroy_mobile(int mob);
char *make_title(char *title, char *name);
char *std_title(int level, Boolean sex);
Boolean reset_mobile(int mobile);
void p_crapup(int player, char *str, int flags);
void crapup(char *str, int flags);
void xcrapup(char *str, Boolean saveflag);
void loseme(Boolean saveflag);
char *lev2s(char *b, int lvl, Boolean x);
int tscale(void);
Boolean chkdumb(void);
Boolean chkcrip(void);
Boolean chksitting(void);
void calib_player(int pl);
void calibme(void);
int levelof(int score, int lev);
Boolean check_setin(char *s, Boolean d);
Boolean check_busy(int plx);
char *build_prompt(int plx);
int vicf2(int fl, int chance);
int vichere(void);
int vicbase(void);
void jumpcom(void);
void special_events(int player);
void regenerate(void);

/* Values for crapup */
#define CRAP_SAVE 0x01
#define CRAP_RETURN 0x02
#define CRAP_UNALIAS 0x04

#define SAVE_ME CRAP_SAVE
#define NO_SAVE 0

/* values for vicf2 */
#define SPELL_PEACEFUL 0
#define SPELL_VIOLENT 1
#define SPELL_REFLECTS 2

/* values for special_events() */
#define SP_ALL (-1)

#define fpbns(N) find_player_by_name(N)

#endif /* Add nothing past this line... */
