#ifndef _HATE_H_
#define _HATE_H_

#define  no_hate(P)   ((hates_who(P) == -1) ? True : False)

Boolean  hates_you(int p, int i);
int  hates_who(int p);
void clear_all_hate(void);
void clear_hate(int p);
void hatecom(void);
void send_hate_msg(int m, int p);
void set_hate(int p, int i);
void set_hate_str(int p,char *str);
void should_hate(int m, int p, int d);

#endif /* DON'T ENTER ANYTHING AFTER THIS LINE */
