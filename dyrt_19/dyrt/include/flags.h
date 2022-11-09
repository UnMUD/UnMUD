#ifndef __FLAGS_H__ 
#define __FLAGS_H__ 


void show_bits(int *bits, int n, char **table);
Boolean dump_pflags(void);
void xshow_lflags(int loc);
void pflagscom(void);
void maskcom(void);
void mflagscom(void);
void lflagscom(void);
void set_xpflags(int y,PFLAGS *p, PFLAGS *m);


#endif /* Add nothing past this line... */
