#ifndef __WIZARD_H__ 
#define __WIZARD_H__ 


void rawcom(void);
void deletecom(void);
void opengamecom(void);

void tournamentcom(void);
void shutdowncom(char crash);
void syslogcom(void);
void echocom(void);
void echoallcom(void);
void echotocom(void);
void tracecom();
void emotecom(void);
void emotetocom(void);
void cfrcom();
void talklinecom(int vb);
void newstylecom();
void namerawcom();
void warnzapcom();
void puntzapcom ();
void notalklinecom(int vb);
void setcom(void);
void saveset(void);
void exorcom(void);
void acctcom();
void setstart(void);
void noshoutcom(void);
void _showitem(int x);
void showlocation(int o);
void showitem(void);

#if 0
void wizlock(void);
#endif

void warcom(void);
void peacecom(void);

#if 0
void tracecom(void);
#endif

void zapcom(void);
void wizcom(void);
void nowizcom(void);
void awizcom(void);
void demigodcom(void);
void godcom(void);
void showtty(void);






#endif /* Add nothing past this line... */
