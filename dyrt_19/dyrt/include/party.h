#ifndef _PARTY_H_
#define _PARTY_H_

void pstatus(void);
void pcreate(void);
void pleave(void);
void padd(void);
void partyname(void);
void pfollow(void);
void pshare(void);
void pleader(void);
void leaveplayer(int plx);
void addplayer(int plx, PARTY *p);
char *whopartytst();
void psay();

#endif
