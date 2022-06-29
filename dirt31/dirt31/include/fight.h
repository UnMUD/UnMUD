int dambyitem(int pl, int it);
void wieldcom(void);
/*void hitplayer(int victim, int wpn);*/
void hit_player(int attacker,int victim,int weapon);
void killcom(void);
void bloodrcv(int *array, int isme);
void breakitem(int x);
Boolean testpeace(int plr);
void woundmn(int mon, int am);
void fleecom(void);
void mhitplayer(int mon);
void combatmessage(int victim, int attacker, int wpn, int ddn);
void setpfighting(int x, int y);
void spellcom(int spell_verb);
