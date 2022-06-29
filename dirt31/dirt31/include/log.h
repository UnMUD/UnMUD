int  open_logfile(char *logfile,Boolean clear_flag);
void close_logfile(void);
void progerror(char *name);

#ifdef VARGS

void vmudlog( char *format, va_list pvar);
void mudlog( char *format, ...);

#else

/* VARARGS2 */
void mudlog();

#endif

