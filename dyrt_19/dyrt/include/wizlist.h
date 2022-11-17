#ifndef __WIZLIST_H__
#define __WIZLIST_H__

void set_wizfile(char *f);
int boot_wizlist(FILE *f, char *fname);
void dump_wizlist(void);
void update_wizlist(char *name, int new_wlevel);
int parse_wizlevel(char *s, int *low, int *high);
void wizlistcom(void);
void powerscom(void);

#endif /* Add nothing past this line... */
