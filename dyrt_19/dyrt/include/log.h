#ifndef __LOG_H__
#define __LOG_H__

int open_logfile(char *logfile, Boolean clear_flag);
void close_logfile(void);
void progerror(char *name);

#ifdef VARGS
/*void vmudlog (char *format, va_list pvar); */
void mudlog(char *format, ...);
#else
/* VARARGS2 */
void mudlog(format, a1, a2, a3, a4, a5, a6)
#endif
#endif /* Add nothing past this line... */
