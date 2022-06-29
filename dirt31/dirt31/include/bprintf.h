void pbfr(void);
void bflush(void);
void snoopcom(void);
void snoop_off(int plr);

void print_buf(char *b,Boolean notself);

#ifdef VARGS
void bprintf(char *format, ...);
#else
void bprintf();
#endif




