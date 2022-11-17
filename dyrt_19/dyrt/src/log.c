#include "kernel.h"
#include <sys/file.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#ifdef VARGS
#include <stdarg.h>
#endif
#include <errno.h>
#include "stdinc.h"

/*
extern char *sys_errlist[];
*/

int open_logfile(char *logfile, Boolean clear_flag)
{
  int fd;
  int x = O_WRONLY | O_CREAT;

  if (clear_flag)
    x |= O_TRUNC;
  else
    x |= O_APPEND;
  if ((fd = open(logfile, x, S_IRUSR | S_IWUSR)) < 0)
  {
    fprintf(stderr, "\n%s: Cannot open logfile %s, program failed.\n",
            progname, logfile);
    perror("open");
    return -1;
  }
  dup2(fd, fileno(stderr));
  close(fd);
  return 0;
}

void close_logfile()
{
  fclose(stderr);
}

void progerror(char *name)
{
  mudlog("PERROR %s: [%d] %s", name, errno, strerror(errno));
}

#ifdef VARGS
void vmudlog(char *format, va_list pvar)
{
  char *z;
  z = ctime(&global_clock);
  z[19] = '\0'; /* remove year specification */
  fprintf(stderr, "%s: ", z);
  vfprintf(stderr, format, pvar);
  putc('\n', stderr);
}

void mudlog(char *format, ...)
{
  va_list pvar;
  va_start(pvar, format);
  vmudlog(format, pvar);
  va_end(pvar);
}
#else
/* VARARGS2 */
void mudlog(format, a1, a2, a3, a4, a5, a6) char *format, *a1, *a2, *a3, *a4, *a5, *a6;
{
  char *z;

  z = ctime(&global_clock);
  z[19] = '\0'; /* remove year specification */
  fprintf(stderr, "%s: ", z);
  fprintf(stderr, format, a1, a2, a3, a4, a5, a6);
  putc('\n', stderr);
}
#endif
