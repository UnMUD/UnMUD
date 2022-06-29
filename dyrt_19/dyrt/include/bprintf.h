#ifndef __BPRINTF_H__ 
#define __BPRINTF_H__ 


void pbfr(void);
void bflush(void);
void snoopcom(void);
void snoop_off(int plr);

void print_buf(char *b,Boolean notself);

void fix_color(unsigned char *dest, unsigned char *src);

#ifdef VARGS
void bprintf(char *format, ...);
#else
void bprintf();
#endif





#endif /* Add nothing past this line... */
