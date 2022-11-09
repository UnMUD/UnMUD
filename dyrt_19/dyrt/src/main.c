#ifdef DEBUG
#define LAZY
#endif
#define ERIC_DB
#include <sys/file.h>
#include "kernel.h"
#include <errno.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/socket.h>
#ifdef RS6000
#include <sys/select.h>
#endif
#include <arpa/inet.h>
#include <netdb.h>
#include "config.h"
#include <stdarg.h>
#include "locations.h"
#include "stdinc.h"
#include "machines.h"
#ifdef IDENT
# include "ident.h"
#endif
#ifdef INTERMUD
#include "intermud.h"
#endif


/* Char-by-char mode fixed by ErIC */


static int xmain (int fd);
static int xmain_reboot (int fd);
static int go_background (int pid_fd);
static int check_pid (void);
static void get_options (int argc, char **argv);
static void usage (void);
static void main_loop (int m_socket);
void sysreboot(int saveLoc);
void rm_pid_file (void);
static void end_connection (void);
static void new_connection (int fd);
static void handle_packet (int fd);
static void dump_info (void);

void sig_handler (int sig);

/* Some local variables */
fd_set sockets_fds;
fd_set buffer_fds;
int mud_port = -1;
int main_socket;
int width;
#ifdef REBOOT
int old_proc_num = 0;
int port_number;
#endif
int fildes[2];
char pidFileName[256];

Boolean kill_other_mud = False;
Boolean clear_syslog_file = False;
Boolean stay_foreground = False;
Boolean sig_term_happened = False;
Boolean sig_timer_happened = False;
Boolean Debug = False;
Boolean kill_and_exit = False;

static int inp_buf_c = 0;
static struct timeval zerotime =
{0, 0};

int 
main (int argc, char **argv, char **ep)
{
  int fd, x;
  char path[1024];
  char binary[1024];
  
  sigsetmask(0);
  envp = ep;
  progname = argv[0];
  width = getdtablesize ();
  
  /************/
  getwd (path);
  sprintf (binary, "%s/%s", path, argv[0]);
  
#ifdef HANDLER			/* see debug.h */
  install_handler (binary, (char *) 0);
#endif
  
  /************/
  
  if (data_dir != NULL)
    chdir (data_dir);		/* Do chdir if specified */
  
  get_options (argc, argv);	/* Parse command line */
  
  printf ("\ndata_dir = \"%s\".\nmax_players = %d.\nport = %d.\n",
	  (data_dir == NULL ? "<null>" : data_dir), max_players, mud_port);
  
  printf ("%s " LOG_FILE " file.\n",
	  clear_syslog_file ? "Clear" : "Do not clear");
  
#ifdef DEBUG
  Debug = True;
#endif
  
  printf ("Debugging is %s.\n", Debug ? "on" : "off");
  
#ifdef REBOOT
  if (old_proc_num)
    kill_other_mud = True;
#endif
  if (kill_other_mud)
    {
      printf ("Kill other mud.\n");
    }
  else
    {
      printf ("Ask if other mud should be killed.\n");
    }
  
  if (data_dir != NULL)
    chdir (data_dir);		/* Do chdir if specified */
  
  /* Check if PID_FILE is there...and what it contains */
  fd = check_pid ();
  
  /* We arrive here only if we are to continue and now we are alone. */
  /* Also, the PID_FILE is opened */
  
  if (kill_and_exit)
    exit (0);
#ifdef REBOOT
  if (!old_proc_num)
#endif
    x = xmain (fd);
#ifdef REBOOT
  else
    x = xmain_reboot (fd);
#endif
  unlink (pidFileName);
  if (x < 0)
    {
      mudlog ("Abnormal termination of mud");
      exit (1);
    }
  mudlog ("Normal termination of mud");
  return True;
}

static void 
connect_ok (char *h, int port)
{
  char b[80];
  
  sprintf (b, "Connected to port %d on %s.\n\001\n", port, h);
  if (stay_foreground)
    {
      if (write (1, b, strlen (b) - 2) == -1)
	progerror ("write(1,b,strlen(b)).1 failed");
      
    }
  else
    {
      if (write (fildes[1], b, strlen (b)) == -1)
	progerror ("write(1,b,strlen(b)).2 failed");
      while (getppid () > 1);
      close (fildes[1]);
    }
}

#ifdef REBOOT
static int xmain_reboot (int fd)
{
  int f, nzones_to_reload;
  FILE *fp;
  char newname[256];
  int p;
  int s;
  /* We arrive here only if we are to continue and now we are alone. */
  /* Also, the PID_FILE is opened */
  
  if (open_logfile(LOG_FILE,clear_syslog_file) < 0) {
    close(fd);
    return -1;
  }
  /* Open up the file that contains the information about the reboot. */
  sprintf(newname,"reboot_file%d",old_proc_num);
  mudlog("REBOOT: Opening file %s.",newname);
  if(!(fp = fopen(newname,"r")))
    {
      mudlog("Error on reboot, %s does not exist.",newname);
      return -1;
    }
  /* Now that the reboot info file is opened, read in some of
   * the info, namely, how much to loop, and the main socket desc. */
  fscanf(fp,"%d ",&p);
  port_number = p;
  fscanf(fp,"%d ",&s);
  fscanf(fp,"%d ",&f);
  mudlog("REBOOT: Port: %d  Masterport: %d  Number of Entries: %d",p,s,f);
  mudlog("REBOOT: Bootstrapping again.");
  if (bootstrap() < 0) { /* Initialize data structures */
    mudlog("REBOOT: Bootstrap failed.");
    close(fd);
    return -1;
  }
  init_userfile();
  reorg_userfile();
  fscanf(fp,"%d ",&nzones_to_reload);
  while (nzones_to_reload) {
    char zone_name[256];
    
    fgets(zone_name,255,fp);
    zone_name[strlen(zone_name)-1] = '\0';
    load_zone (zone_name, NULL, NULL, NULL, NULL, NULL, NULL);
    nzones_to_reload--;
  }
  
  main_socket = s;
  
  FD_ZERO(&sockets_fds);
  FD_ZERO(&buffer_fds);
  FD_SET(s,&sockets_fds);
  
  mudlog("REBOOT: Done with bootstrap - reading player info.");
  /* Now, read in reboot information from the file. */
  while(f)
    {
      int fd;
      int new_loc;
      int plx;
      char hostname[256], char_name[256];
      char *s;
      PLAYER_REC *pl;
      Boolean priv = False;
      Boolean ok = False;
      
      fscanf(fp,"%d ",&fd);
      fscanf(fp,"%d ",&new_loc);
      fgets(hostname,255,fp);
      fgets(char_name,255,fp);
      hostname[strlen(hostname)-1] = '\0';
      char_name[strlen(char_name)-1] = '\0';
      plx = find_free_player_slot();
      printf("Put %s into %d.\n",char_name,plx);
      setup_globals(plx);
      pl = cur_player;
      FD_SET(fd,&sockets_fds);
      if (OPERATOR (char_name))
	ok = priv = True;
      else if (privileged_user (char_name))
	ok = priv = True;
      cur_player->isawiz = priv;
      pl->sock_buffer_p = pl->sock_buffer_end = pl->sock_buffer;
      pl->fil_des = fd;
      pl->stream = fdopen(fd,"w");
      fprintf(pl->stream,"Puff decides to prank the players and reboots the mud!\n");
      pl->isforce= False;
      s = pl->hostname;
      cur_player->last_command = time(0);
      cur_player->last_cmd = time(0);
      cur_player->tr.trace_item = -1;
      cur_player->logged_on = time(0);
      strcpy(s, hostname);
      setpname(mynum,char_name);
      getuafinfo(char_name);
      if(new_loc==-1)
          new_loc= (phome(mynum)<0) ? phome(mynum) :
                   ( (randperc() > 66) ? LOC_START_TEMPLE :
                     (  (randperc() > 33) ? LOC_START_CHURCH :
                        LOC_HAVEN_HAVEN ));
      setpwpn(mynum, -1);
      setphelping(mynum, -1);
      setpfighting(mynum, -1);
      setpsitting(mynum, 0);
      cur_player->iamon = True;
      cur_player->iamon = True;
      fetchprmpt(mynum);
      push_input_handler(get_command);
      get_command(NULL);
      if (exists (new_loc))
	setploc(mynum,new_loc);
      else if (exists (new_loc = find_loc_by_id(phome(mynum))))
	setploc(mynum,phome(mynum));
      else
	setploc(mynum, randperc() > 50 ? LOC_START_TEMPLE : LOC_START_CHURCH);
      fprintf(pl->stream,"The Gods emphasize to Puff that her joke was NOT funny.\n");
      f--;
    }
  
  /* Main program loop */
  fclose(fp);
  unlink (newname);
  init_calendar ();
  update_calendar ();
  boot_reset ();
  send_msg(DEST_ALL,0,LVL_MIN,LVL_MAX,NOBODY,NOBODY,
           "Puff grins sheepishly and apologizes for her behavior.  Unfortunately, the game has still been reset.\n");
  /* write a new pid file */
  {
    /* ugly programming, don't try this at home.. declare your
       variables at the top! */
    FILE *rbfd;
    rbfd = fopen(pidFileName,"w+");
    if (!rbfd) {
      progerror("Cannot open new pid file.\n");
    }
    else {
      fprintf(rbfd,"%d",getpid());
      fclose(rbfd);
    }
  }
  
  main_loop(s);
  mudlog( "Closing listening socket");
#ifdef INTERMUD
  imShutdown(0);
#endif
  close(s);
  return 0;
}
#endif

static int 
xmain (int fd)
{
  int s;
  int k;
  struct sockaddr_in sin;
  
  /* We arrive here only if we are to continue and now we are alone. */
  /* Also, the PID_FILE is opened */
  
  if (open_logfile (LOG_FILE, clear_syslog_file) < 0)
    {
      close (fd);
      return -1;
    }
  
  if (bootstrap () < 0)
    {				/* Initialize data structures */
      close (fd);
      return -1;
    }
  init_userfile();
  reorg_userfile();
  
  if (mud_port == -1)
    mud_port = PORT;
  
  /* Now we go background */
  if (go_background (fd) < 0)
    {
      return -1;
    }
  
  k = 10;
  bzero (&sin, sizeof (struct sockaddr_in));
  /***/
  
  while ((s = main_socket = make_service (mud_port, my_hostname,
					  sizeof (my_hostname),
					  &my_hostent, &sin)) == -4
	 && errno == EADDRINUSE && --k >= 0)
    {
      sleep (2);
    }
  
  if (s < 0)
    {
      mudlog ("Error code %d from make_service", s);
      progerror ("make_service");
      return -1;
    }
  
#ifndef REBOOT
  if (s > 0)
    {				/* We want the main socket at fd 0 */
      dup2 (s, 0);
      close (s);
      s = main_socket = 0;
    }
#endif
  
  connect_ok (my_hostname, mud_port);
  
  FD_ZERO (&sockets_fds);
  FD_ZERO (&buffer_fds);
  FD_SET (s, &sockets_fds);
  
  /* Main program loop */
  
  init_calendar ();
  update_calendar ();
  boot_reset ();
  main_loop (s);
  
#ifdef DYRTWHO
  setup_udp();
#endif
#ifdef INTERMUD
  imShutdown(0);
#endif
  close (s);
  return 0;
}

static int 
go_background (int fd)
{
  char b[80];
  int tty, x, y;
  char *s;
  
  /* Go background */
  signal (SIGHUP, SIG_IGN);
  signal (SIGINT, SIG_IGN);
  signal (SIGQUIT, SIG_IGN);
  signal (SIGTSTP, SIG_DFL);
  signal (SIGTTOU, SIG_DFL);
  signal (SIGTTIN, SIG_DFL);
  if (!stay_foreground)
    {
      if (pipe (fildes) < 0)
	{
	  progerror ("go_background/pipe");
	  exit (1);
	}
      switch (pid = fork ())
	{
	case -1:
	  progerror ("fork");
	  close (fd);
	  return -1;
	case 0:
	  break;
	default:
	  printf ("PID = %d\n", pid);
	  close (fd);
	  close (fildes[1]);
	  fflush (stdout);	/* Flush stdout */
	  while ((x = read (fildes[0], b, sizeof (b))) > 0)
	    {
	      for (s = b, y = 0; y < x && s[y] != '\001'; ++y)
		{
		  if (s[y] != '\n')
		    continue;
		  if (*s == '\001')
		    break;
		  if (write (1, s, y + 1) == -1)
		    progerror ("write(1, s, y + 1)  failed");
		  s += y + 1;
		  x -= y + 1;
		  y = -1;
		}
	      if (*s == '\001')
		break;
	      if (x > 0)
		{
		  if (write (1, s, x) == -1)
		    progerror ("write(1, s, x)  failed");
		}
	    }
	  exit (0);		/* Let our parent process die */
	}
      if (stdin)
	fclose (stdin);
      if (stdout)
	fclose (stdout);
      close (fildes[0]);
    }
  
  pid = getpid ();		/* Get our process id */
  sprintf (b, "%d\n", pid);
  if (write (fd, b, strlen (b)) == -1)
    progerror ("write(fd, b, strlen(b)) failed");
  close (fd);
  
  
  if (!stay_foreground)
    {
      
      if ((tty = open ("/dev/tty", O_RDWR, S_IRUSR | S_IWUSR)) >= 0)
	{
	  
	  if (ioctl (tty, TIOCNOTTY, 0) < 0)
	    {
	      progerror ("ioctl,TIOCNOTTY");
	      return -1;
	    }
	  close (tty);
#ifdef LINUX
       	  setpgrp();
#else
	  setpgrp (pid, pid);	/* Make our own process group */
#endif
	}
      else if (errno != ENXIO)
	{
	  progerror ("open,tty");
	  return -1;
	}
      
      signal (SIGINT, SIG_IGN);
      signal (SIGQUIT, SIG_IGN);
    }
  else
    {
      signal (SIGINT, SIG_DFL);
      signal (SIGQUIT, SIG_DFL);
    }
  
  signal (SIGTERM, sig_handler);
  signal (SIGTSTP, SIG_DFL);
  signal (SIGCONT, SIG_DFL);
  signal (SIGTTOU, SIG_DFL);
  signal (SIGTTIN, SIG_DFL);
#ifndef DEBUG
# ifndef HANDLER /* these may be handled by the custom handler */
  signal (SIGSEGV, sig_handler); /* Segmentation fault */
  signal (SIGBUS, sig_handler);	 /* Bus error */
# endif
  signal (SIGSYS, sig_handler);	/* Bad argument to system call */
#endif
  signal (SIGPIPE, SIG_IGN);	/* Broken pipe */
  signal (SIGHUP, SIG_IGN);     /* hang up */
  return 1;
}

static int 
check_pid (void)
{
  int fd;
  int pid = -1;
  int c;
  FILE *f;
  char b[80];
  
  sprintf(pidFileName,"%s.%d",PID_FILE,mud_port);
  if ((fd = open (pidFileName, O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR)) < 0)
    {
      /* The file exist already */
      if (errno == EEXIST)
	{
	  if ((f = fopen (pidFileName, "r")) != NULL)
	    {
	      /******************8
		fgets (b, sizeof b, f);
		pid = atoi (b);
		**********************/
	      if (fgets (b, sizeof b, f))
		pid = atoi(b);
	      else
		pid = 0;
	      if (f)
		fclose (f);
	    }
	  else
	    {
	      perror ("fopen," PID_FILE);
	      exit (1);
	    }
	  if (pid > 0)
	    {
	      if (!kill_other_mud)
		{
		  printf ("There is another mud running, want me to kill it? ");
		  while ((c = getchar ()) != 'N' && c != 'n' && c != 'Y' &&
			 c != 'y' && c != '\n' && c != '\r');
		  if (c == 'Y' || c == 'y')
		    kill_other_mud = True;
		}
	      if (!kill_other_mud)
		{
		  printf ("Ok, then I'll die.\n");
		  exit (0);
		}
	      printf ("Ok, Will kill other mud (PID = %d)\n", pid);
	      if (kill (pid, SIGTERM) < 0)
		{
		  if (errno != ESRCH)
		    {
		      perror ("kill");
		      exit (1);
		    }
		  else if (unlink (pidFileName) < 0)
		    {		/* PID_FILE without process */
		      perror ("unlink");
		      exit (1);
		    }
		}
	    }
	  else
	    {
	      if (unlink (pidFileName) < 0)
		{		/* PID_FILE without process */
		  perror ("unlink");
		  exit (1);
		}
	    }
	  c = 6;
	  while (True)
	    {
	      sleep (1);
	      if ((fd = open (pidFileName, O_WRONLY | O_CREAT | O_EXCL,
			      S_IRUSR | S_IWUSR)) >= 0)
		break;
	      if (--c < 0)
		{
		  printf ("Timeout, kill it yourself, I give up.\n");
		  exit (0);
		}
	    }
	}
      else
	{
	  perror ("open");
	  exit (1);
	}
    }
  return fd;
}

static void 
get_options (int argc, char **argv)
{
  // static void usage (void);
  
  char *s;
  int x;
  
  
  if (argc == 1)
    {
#ifdef LAZY
      stay_foreground = True;
      kill_other_mud = True;
      mud_port = PORT;
      max_players = 40;
#else
      stay_foreground = False;
      clear_syslog_file = False;
      kill_other_mud = False;
      mud_port = PORT;
      max_players = 40;
#endif
      return;
    }
  else
    {
      /*
       * *since we make odd defaults with no args, we have to add
       * * an option to say 'use real defaults', I select option a for that.
       * * mudd -a is what usually should be just 'mudd'. -Alf
       */
    }
  
  while (--argc > 0)
    {
      s = *++argv;
      if (*s++ != '-')
	{
	  usage ();
	  exit (1);
	}
      x = *s++;
      switch (x)
	{
	case 'a':
	  break;
#ifdef REBOOT
        case 'r':
 	  if ((*s != '\0') || ((--argc > 0) && (*(s = *++argv) != '\0')))
	    old_proc_num = atoi(s);
	  break;
#endif
	case 'p':
	  if ((*s != '\0') || ((--argc > 0) && (*(s = *++argv) != '\0')))
	    {
	      if ((mud_port = atoi (s)) < 1024 || mud_port > 65535)
		{
		  mud_port = PORT;
		}
	    }
	  break;
	case 'f':
	  stay_foreground = True;
	  break;
	case 'k':
	  kill_other_mud = True;
	  break;
	case 'c':
	  clear_syslog_file = True;
	  break;
	case 'z':
#ifdef HANDLER
	  remove_handler ();
#else
	  printf ("Handler is not compiled in, option -z not operational.\n");
	  exit (0);
#endif
	  break;
	case 'x':
	  kill_other_mud = True;
	  kill_and_exit = True;
	  break;
	case 'i':
	  dump_info ();
	  exit (0);
	  break;
	case 'n':
	  if ((*s != '\0') || ((--argc > 0) && (*(s = *++argv) != '\0')))
	    {
	      if ((max_players = atoi (s)) < 1 || max_players > 1000)
		{
		  max_players = 40;
		}
	    }
	  break;
	case 'd':
	  if ((*s != '\0') || ((--argc > 0) && (*(s = *++argv) != '\0')))
	    {
	      data_dir = s;
	    }
	  break;
	default:
	  usage ();
	  exit (1);
	}
    }
  if (argc > 0)
    {
      usage ();
      exit (1);
    }
}

static void 
usage (void)
{
  fprintf (stderr, "Usage: %s [-p port] [-d dir] [-k] [-c] [-f] [-n count] [-x]\
\n\
Where:\n\
\tp port               port to attach to.\n\
\td directory          data directory.\n\
\tk                    kill other mud.\n\
\tc                    clear log file.\n\
\tn count              maximum users.\n\
\tf                    stay in foregreound.\n\
\ti                    some interesting information.\n\
\tx                    just kill the running mud, don't run.\n\
", progname);
  
  exit (1);
}

static void 
main_loop (int m_socket)
{
  // static void new_connection (int fd);
  
  int w = width;
  int v;
  int fd;
  int plx = 0;
  struct timeval *tv;
  fd_set r_fds, e_fds;
  
  /*  reset(); */
  cur_player = NULL;
  quit_list = -1;
  signal (SIGALRM, sig_handler);
  for(v=0;v<32;v++) signal(v,sig_handler);        
  signal(SIGCHLD, SIG_DFL);
  signal(SIGTRAP, SIG_DFL);
  signal(SIGTERM, sig_handler);
  signal(SIGTSTP, SIG_DFL);
  signal(SIGCONT, SIG_DFL);
  signal(SIGTTOU, SIG_DFL);
  signal(SIGTTIN, SIG_DFL);
  signal(SIGSEGV, sig_handler); /* Segmentation fault */
  signal(SIGPIPE, SIG_IGN);     /* Broken pipe */
  set_timer ();
#ifdef INTERMUD
  mudlog("INTERMUD: Attempting to connect to %s:%d",raddr,rport);
  if((imfd = __iConn(raddr,rport)) < 0 ) {
    mudlog("INTERMUD: Error connecting to %s:%d",raddr,rport);
  }
  else FD_SET(imfd,&sockets_fds);
#endif
  while (!sig_term_happened)
    {
      switch (v = setjmp (to_main_loop))
	{
	case JMP_QUITTING:
	  bflush ();
	  end_connection ();
	  setup_globals (-1);
	  ++fd;
	  break;
	}
      if (sig_timer_happened)
	{
	  sig_timer_happened = False;
	  signal (SIGALRM, sig_handler);
	  setup_globals (-1);	/* bflush() and disable mynum */
	  set_timer ();
	  on_timer ();
	}
      else
	{
	  r_fds = e_fds = sockets_fds;
	  
	  if (inp_buf_c > 0)
	    tv = &zerotime;
	  else
	    tv = NULL;
	  
	  if ((v = select (w, &r_fds, NULL, &e_fds, tv)) < 0)
	    {
	      if (errno == EINTR)
		continue;
	      progerror ("select");
	      exit (1);
	    }
	  fd = 0;
	  while (fd < width)
	    {
	      if (FD_ISSET (fd, &e_fds))
		{
#ifdef INTERMUD
                  if( fd == imfd )
                    {
                      mudlog( "INTERMUD: Exception.  Shutting down connection." );
                      FD_CLR( fd, &sockets_fds );
                      close( fd );
                      imfd = -1;
                    }
#endif
		  --v;
		}
	      if (FD_ISSET (fd, &r_fds) || FD_ISSET (fd, &buffer_fds))
		{
		  --v;
		  if (fd == m_socket)
		    {
		      new_connection (fd);
#ifdef INTERMUD
          } else if (fd ==imfd) {
            imPacket(fd);
#endif
		    }
		  else
		    {
		      handle_packet (fd);
		    }
		  bflush ();
		}
	      while (quit_list >= 0)
		end_connection ();
	      ++fd;
	    }
	}
    }
  mudlog ("SIGNAL: SIGTERM Handled");
  for (plx = 0; plx < max_players; plx++)
    {
      if (!EMPTY (pname (plx)))
	{
	  setup_globals (plx);
	  crapup ("\tSomething very unpleasant seems to have happened...",
		  CRAP_UNALIAS | CRAP_SAVE | CRAP_RETURN);
	}
    }
  rm_pid_file ();
}

void 
rm_pid_file (void)
{
  if (unlink (pidFileName) < 0)
    {
      progerror ("rm_pid_file");
    }
}

static void 
end_connection (void)
{
  INP_HANDLER *i, *j;
  int fd;
  int me = real_mynum;
  int x;
  
  if ((x = quit_list) < 0)
    return;
  if (x == real_mynum)
    me = -1;
  setup_globals (x);
  quit_list = cur_player->quit_next;
  cur_player->quit_next = -2;
  
  fd = cur_player->fil_des;
  bflush ();
  i = cur_player->inp_handler;
  while (i != NULL)
    {
      j = i;
      i = i->next;
      free ((char *) j);
    }
  cur_player->inp_handler = NULL;
  if (FD_ISSET (fd, &buffer_fds))
    inp_buf_c--;
  FD_CLR (fd, &sockets_fds);
  FD_CLR (fd, &buffer_fds);
  close (fd);
  if (cur_player->stream)
    fclose (cur_player->stream);
  cur_player->stream = NULL;
  cur_player->inp_buffer_p = cur_player->inp_buffer_end = cur_player->inp_buffer;
  cur_player->sock_buffer_p = cur_player->sock_buffer_end = cur_player->sock_buffer;
  setup_globals (me);
}

static void 
new_connection (int m_socket)
{
  PLAYER_REC *pl;
  FILE *f;
  struct hostent *h;
  int plx;
  int fd;
  int sin_len;
  Boolean host_banned = False, host_b2 = False;
  struct sockaddr_in sin;
  char *host, *s;
  char hostnum[MAXHOSTNAMELEN + 20];
  char full_hostname[MAXHOSTNAMELEN + 20];
  
  bzero ((char *) &sin, sizeof (struct sockaddr_in));
  sin_len = sizeof (struct sockaddr_in);
  if ((fd = accept (m_socket, (struct sockaddr *)&sin, &sin_len)) < 0)
    {
      progerror ("accept");
    }
  else if ((f = fdopen (fd, "w")) == NULL)
    {
      progerror ("fdopen");
      exit (1);
    }
  else
    {
      
      
#ifdef BROKEN_GCC
      strncpy (hostnum,inet_ntoa (&sin.sin_addr), MAXHOSTNAMELEN);
#else
      strncpy (hostnum, inet_ntoa (sin.sin_addr), MAXHOSTNAMELEN);
#endif
      
      host_b2 = is_host_banned (hostnum);
      host = hostnum;
      
      if ((h = gethostbyaddr ((char *) &sin.sin_addr, sizeof (sin.sin_addr),
			      AF_INET)) == NULL)
	{
	  mudlog ("gethostbyaddr: Couldn't find hostentry for %s", hostnum);
	}
      else
	{
	  host_banned = is_host_banned (h->h_name);
	  host = h->h_name;
	}
      
#ifdef IDENT
      ident_username = (char *)GetIdent(h,m_socket,fd);
      
      if (ident_username) { 
	sprintf(full_hostname,"%s@%s",ident_username,host);
      }
      else 
#endif
	strcpy(full_hostname,host);
      
      
      if ((plx = find_free_player_slot ()) < 0)
	{
	  fprintf (f, "\nSorry, this mud is full. Please come back later.\n");
	  if (f)
	    fflush (f);
	  if (f)
	    fclose (f);
	  return;
	}
      
      
      setup_globals (plx);
      pl = cur_player;
      
      pl->sock_buffer_p = pl->sock_buffer_end = pl->sock_buffer;
      pl->sin_len = sin_len;
      pl->sin = sin;
      pl->fil_des = fd;
      pl->stream = f;
      pl->isforce = False;
      s = pl->hostname;
      if (host_banned || host_b2)
	*s++ = '*';
      
#ifdef IDENT
      strncpy(s,full_hostname,MAXHOSTNAMELEN);
#else
      strncpy (s, host,MAXHOSTNAMELEN);
#endif
      
      
      /* Include this socket as a socket to listen to */
      FD_SET (fd, &sockets_fds);
      new_player ();
    }
}

void 
handle_packet (int fd)
{
  int plx = find_pl_index (fd);
  register int x, y, g;
  char *b, *c, *k;
  char *p;
/*
  extern char *sys_errlist[];
*/
  
  
  setup_globals (plx);
  
  g = 0;
  if (cur_player->inp_buffer_p >= cur_player->inp_buffer_end)
    {
      if ((x = read (fd, cur_player->inp_buffer, MAX_COM_LEN - 5)) < 0)
	{
	  if (errno == ECONNRESET ||	/* Connection reset by peer */
	      errno == EHOSTUNREACH ||	/* No route to host */
	      errno == ETIMEDOUT ||	/* Connection timed out */
	      errno == ENETUNREACH ||	/* Network is unreachable */
	      errno == ENETRESET ||	/* Net dropped connection on reset */
	      errno == ENETDOWN)
	    {			/* Network is down */
	      mudlog ("Connection failure: %s [%s]", strerror(errno), pname (mynum));
	      crapup (NULL, CRAP_SAVE | CRAP_UNALIAS | CRAP_RETURN);
	      
	      send_msg (DEST_ALL, MODE_QUIET | MODE_COLOR, max (pvis (mynum), LVL_APPREN),
			LVL_MAX, mynum, NOBODY,
			"[%s (%s) Link error: %s]\n", pname (mynum), cur_player->hostname, strerror(errno));
	      
	      errno = 0;
	      longjmp (to_main_loop, JMP_QUITTING);
	      
	      return;
	    }
/*
	  mudlog ("error when reading data from %s\n", pname (mynum));
*/
	  progerror (pname (mynum));
	  return;
	}
      if (x == 0)
	{
	  crapup (NULL, CRAP_SAVE | CRAP_UNALIAS | CRAP_RETURN);
	  errno = 0;
	  quit_player ();
	  return;
	}
      if (*(cur_player->inp_buffer) == '\377')
	{
	  return;
	}
      inp_buf_c++;
      
      /*
       * for (t = 0; t != MAX_COM_LEN; t++) 
       * if (cur_player->inp_buffer[t] == '%') 
       * cur_player->inp_buffer[t] = '&';
       */
      
      FD_SET (fd, &buffer_fds);
      b = cur_player->inp_buffer_p = (char *) cur_player->inp_buffer;
      cur_player->inp_buffer_end = b + x;
    }
  else
    {
      b = cur_player->inp_buffer_p;
      x = cur_player->inp_buffer_end - b;
    }
  c = cur_player->sock_buffer_p;
  
  
  
  for (y = 0;
       (b != 0) &&
       (y < x) &&
       (c < (cur_player->sock_buffer + MAX_COM_LEN - 6)) &&
       b[y] != '\n' &&
       b[y] != '\r';
       ++y)
    {
      if ((b[y] == '\010') || (b[y] == '\177'))
	{
	  if (c > cur_player->sock_buffer)
	    c--;
	}
      else
	*c++ = b[y];
    }
  cur_player->sock_buffer_p = c;
  if (((y < x) && (c < (cur_player->sock_buffer + MAX_COM_LEN - 6))) && b != 0)
    {
      while (++y < x && (b[y] == '\n' || b[y] == '\r'));
      *c = 0;
      k = c;
      c = cur_player->sock_buffer;
      
      for (p = c; *p != '\0'; p++)
	{
	  if (iscntrl (*p))
	    *p = ' ';
	}
      
      if (cur_player->snooped > 0)
	{
	  *k++ = '\n';
	  *k-- = 0;
	  print_buf (c, True);
	  *k = 0;
	}
      g = 1;
    }
  cur_player->sock_buffer_p = c;
  cur_player->inp_buffer_p = b + y;
  
  if (y >= x)
    {
      FD_CLR (fd, &buffer_fds);
      --inp_buf_c;
    }
  
  if (g)
    {
      cur_player->inp_handler->inp_handler (c);
      cur_player->sock_buffer_p = cur_player->sock_buffer;
    };
}

void 
sig_handler (int sig)
{
  
  switch (sig)
    {
    case SIGTERM:
      mudlog ("SIGNAL: SIGTERM");
      sig_term_happened = True;
      return;
    case SIGALRM:
      sig_timer_happened = True;
      return;
    case SIGSEGV:
      mudlog ("SIGNAL: SIGSEGV[%d]", sig);
      break;
    case SIGBUS:
      mudlog ("SIGNAL: SIGBUS[%d]", sig);
      break;
    default:
      mudlog ("SIGNAL: %d", sig);
      break;
    }
  send_msg(DEST_ALL,0,LVL_MIN,LVL_MAX,NOBODY,NOBODY,
	"Puff giggles and dances around, ecstatic about her evil plan.\n");
#ifdef INTERMUD
  imShutdown(1);
#endif
  if(fork()==0)
    sysreboot(False);
  kill(getpid(),SIGTRAP);
}

void dump_info (void)
{
  time_t x;
  Boolean handler;
  
#ifdef HANDLER
  handler = True;
#else
  handler = False;
#endif
  
  x = time (0);
  printf ("Current time %s", ctime (&x));
  printf ("Version: %s (%s)\n", VERSION,MBANNER);
  printf ("Crash handler: %sabled\n", handler ? "en" : "dis");
  return;
}

#ifdef REBOOT
void rebootcom(void)
{
  if (plev(mynum) < LVL_DEMI) {
    bprintf ("You want me to do what?\n");
    return;
  }
  
  broad ("Puff grins from ear to ear.\n");
#ifdef INTERMUD
  imShutdown(1);
#endif
  sysreboot(True);
  
}

void delaycom()
{
  if (plev(mynum) < LVL_DEMI) {
    bprintf("You can't do that now.\n");
  } else {
  if (the_world->w_delayed == 0)
   the_world->w_delayed = 1;
  else
   the_world->w_delayed = 0;

   send_msg (DEST_ALL, MODE_QUIET, LVL_DEMI, LVL_MAX, NOBODY, NOBODY,
		  "[&+bDelayed Reboot now &+r%s]\n", the_world->w_delayed ? "Enabled" :
     "Disabled" );
    mudlog("&+bDelayed Reboot: &+r%s&+b by &+Y%s", the_world->w_delayed ? 
    "ENABLED" : "DISABLED", pname(mynum));
}
}

void sysreboot(int saveLoc)
{
  
  int in_bin, loop, nplayers, nzones_to_reload;
  char filename[256];
  char portinfo[256];
  
  FILE *fp;
  int port = port_number;
 
  FILE *lock;

lock = fopen(".lock", "r");
if (lock != NULL)
 {
  broad("&+rREBOOT CANCELLED: COMPILE IN PROGRESS\n");
  return;
 }
 
  sprintf(portinfo,"-p%d",port_number);
  if (access ("../bin/aberd", F_OK) != -1)
    in_bin = 1;
  else if (access ("../src/aberd", F_OK) != -1)
    in_bin = 0;
  else {
    bprintf ("Sorry, couldn't find aberd in ../bin/ or ../src/\n");
    return;
  }
  
  signal (SIGALRM, SIG_IGN);
  sprintf (filename, "reboot_file%d", getpid ());
  fp = fopen (filename, "w");
  
  fprintf (fp, "%d ", port);
  fprintf (fp, "%d ", main_socket);
  for (loop=0, nplayers=0; loop<max_players; loop++)
    if (players[loop].iamon)
      nplayers++;
  fprintf (fp, "%d ", nplayers);
  
  for (loop=num_const_zon, nzones_to_reload=0; loop<numzon; loop++)
    if (!ztemporary(loop))
      nzones_to_reload++;
  fprintf (fp, "%d ", nzones_to_reload);
  for (loop=num_const_zon; loop<numzon; loop++)
    if (!ztemporary(loop))
      fprintf (fp, "%s\n", zname(loop));
  
  for (loop=0; loop<max_players; loop++) {
    if (players[loop].iamon) {
      setup_globals (loop);
      saveme ();
      fprintf (fp, "%d ", cur_player->fil_des);
      fprintf (fp, "%d ", (saveLoc)?ploc(mynum):-1);
      fprintf (fp, "%s\n", cur_player->hostname);
      fprintf (fp, "%s\n", pname(mynum));
    }
  }
  close_userfile(); 
  fclose (fp);
  
  sprintf (filename, "%d", getpid ());
  mudlog ("REBOOT--------------------------------------%s", filename);
  unlink (pidFileName);
  if (in_bin)
    execl ("../bin/aberd", "../bin/aberd", "-r", filename, portinfo, NULL);
  else
    execl ("../src/aberd", "../src/aberd", "-r", filename, portinfo, NULL);
}
#endif





