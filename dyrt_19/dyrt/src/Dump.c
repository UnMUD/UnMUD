#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
#include "kernel.h"
#include "macros.h"
#include "sflags.h"
#include "pflags.h"
#include "pflagnames.h"
#include <gdbm.h>

extern int getopt(int argc, char **argv, char *optstr);
extern int optind;
extern char *optarg;

void usage(char *progname);

int main(int argc, char *argv[])
{
  PERSONA p;
  GDBM_FILE dbf;
  datum in;
  datum key;
  datum newkey;
  int option;
  char *filename, ofilename[256];
  time_t earliest = 0;
  const int secs_in_a_day = 86400;
  int incl = 0, flags = 0, debug = 0, level = 1, days = 0;
  int playercounter = 0;

  while ((option = getopt(argc, argv, "ad:l:fh")) != -1)
  {
    switch (option)
    {
    case 'a':
      debug++;
      break;
    case 'h':
      incl++;
      break;
    case 'f':
      flags++;
      break;
    case 'l':
      level = atoi(optarg);
      break;
    case 'd':
      days = atoi(optarg);
      break;
    case '?':
      usage(*argv);
      exit(1);
    }
  }
  filename = optind < argc ? argv[optind] : DATA_DIR UAF_RAND;
  ofilename[0] = '\0';
  if ((dbf = gdbm_open(filename, sizeof(PERSONA), GDBM_READER, S_IRUSR | S_IWUSR, 0)) == NULL)
  {
    char cmd[1024];
    sprintf(ofilename, "%s.temp", filename);
    sprintf(cmd, "cp %s %s", filename, ofilename);
    system(cmd);
    if ((dbf = gdbm_open(ofilename, sizeof(PERSONA), GDBM_READER, S_IRUSR | S_IWUSR, 0)) == NULL)
    {
      perror(ofilename);
      exit(1);
    }
  }
  if (days > 0)
    earliest = time((time_t *)NULL) - days * secs_in_a_day;
  if (incl)
  {
    if (flags)
    {
      printf("Name        Sex  Level    Score   Strength   P-Flags");
      printf("\n------------------------");
    }
    else
    {
      printf("Name        Sex      Level        Score    Strength");
      printf("   E-mail\n------------------------");
    }
    printf("----------------------------------------------------\n\n");
  }
  key = gdbm_firstkey(dbf);
  while (key.dptr != NULL)
  {
    in = gdbm_fetch(dbf, key);
    bcopy(in.dptr, &p, sizeof(PERSONA));
    newkey = gdbm_nextkey(dbf, key);
    if (key.dptr)
      free(key.dptr);
    key = newkey;
    if ((!debug && (p.p_level < level || *p.p_name == '\0' ||
                    (days > 0 && p.p_last_on < earliest))) ||
        (p.p_level > 99999))
    {
      if (in.dptr)
        free(in.dptr);
      continue;
    }
    playercounter++;
    if (flags)
    {
      printf("%-13.12s%c %6d %10d %8d   0x%08x%08x%08x\n",
             p.p_name,
             xtstbit(p.p_sflags, SFL_FEMALE) ? 'f' : 'm',
             p.p_level,
             p.p_score,
             p.p_strength,
             (unsigned int)p.p_pflags.b3,
             (unsigned int)p.p_pflags.b2,
             (unsigned int)p.p_pflags.b1);
    }
    else
    {
      printf("%-13.12s%c   %9d   %10d   %9d   %-25.24s\n",
             p.p_name,
             xtstbit(p.p_sflags, SFL_FEMALE) ? 'f' : 'm',
             p.p_level,
             p.p_score,
             p.p_strength,
             p.p_email);
    }
    if (in.dptr)
      free(in.dptr);
  }
  if (key.dptr)
    free(key.dptr);
  gdbm_close(dbf);
  printf("%d users displayed.\n", playercounter);
  if (ofilename[0])
    unlink(ofilename);
  return (0);
}

void usage(char *progname)
{
  fprintf(stderr, "Usage: %s <options> <uaf-file>\n\n", progname);
  fprintf(stderr, "Options: -a        All entries (including empty ones)\n");
  fprintf(stderr, "         -d <#>    Only players on the last # days.\n");
  fprintf(stderr, "         -l <level>Only players > level. Default = 1.\n");
  fprintf(stderr, "         -h        Include Header explaning output\n");
  fprintf(stderr, "         -f        Display P-flags\n");
}
