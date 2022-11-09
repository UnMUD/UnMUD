#ifndef __UAF_H__ 
#define __UAF_H__ 


void    pers2player(PERSONA *d,int plr);
void    player2pers(PERSONA *d,time_t *last_on,int plr);
void    get_gender(char *gen);
void	saveme(void);
Boolean finduaf(char *name,PERSONA *d,int fd);
Boolean getuaf(char *name,PERSONA *d);
void    putuaf(PERSONA *d);
void    deluaf(char *name);
Boolean getuafinfo(char *name);
Boolean findsetins( char *name, SETIN_REC *s, int fd);
Boolean getsetins( char *name, SETIN_REC *s );
void    putsetins( char *name, SETIN_REC *s);
void fetchprmpt(int plr);
char *build_setin(char *b, char *s, char *n, char *d);


#endif /* Add nothing past this line... */
