#ifndef __FIGHT_H__ 
#define __FIGHT_H__ 


int dambyitem(int pl, int it);
void wieldcom(void);
void hit_player(int attacker,int victim,int weapon);
void killcom(void);
void bloodrcv(int *array, int isme);
void breakitem(int x);
Boolean testpeace(int plr);
Boolean wound_player(int attacker, int victim, int damage, int hit_type);
void woundmn(int mon, int am);
void fleecom(void);
void mhitplayer(int mon);
void combatmessage(int victim, int attacker, int wpn, int ddn);
void setpfighting(int x, int y);
void spellcom(int spell_verb);

#endif /* Add nothing past this line... */
