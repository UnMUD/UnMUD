#ifndef __PARSE_H__
#define __PARSE_H__

int gamecom(char *str, Boolean savecom);
char *getreinput(char *b);
int brkword(void);
int chkverb(void);
int chklist(char *word, char *lista[], int listb[]);
int Match(char *x, char *y);
int prmmod(int p);
void doaction(int n);
void quit_game(int yn);
void erreval(void);
Boolean parse_2(int vb);
int findprep(char *t);

#endif /* Add nothing past this line... */
