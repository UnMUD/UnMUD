#include <sys/file.h>
#include "kernel.h"
#include <errno.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "jmp.h"

#ifdef VARGS
#include <stdarg.h>
#endif

#include "mud.h"
#include "log.h"
#include "bootstrap.h"
#include "sendsys.h"
#include "mobile.h"
#include "commands.h"
#include "timing.h"

#include "s_socket.h"




static int  xmain(int fd);
static int  go_background(int pid_fd);
static int  check_pid(void);
static void get_options(int argc,char **argv);
static void usage(void);
static void main_loop(int m_socket);
static void rm_pid_file(void);
static void end_connection(void);
static void new_connection(int fd);
static void handle_packet(int fd);

static void sig_handler(int sig);


/* Some local variables */
fd_set sockets_fds;
fd_set buffer_fds;
int  mud_port = PORT;
int  main_socket;
int  width;
int  fildes[2];
Boolean kill_other_mud = False;
Boolean clear_syslog_file = False;
Boolean stay_foreground   = False;
Boolean sig_term_happened = False;
Boolean sig_timer_happened = False;

static int inp_buf_c = 0;
static struct timeval zerotime = { 0, 0 };

void main(int argc, char **argv, char **ep)
{
  int fd, x;

  envp = ep;
  progname = argv[0];

  get_options(argc,argv); /* Parse command line */

  printf( "\ndata_dir = \"%s\".\nmax_players = %d.\nport = %d.\n",
	 (data_dir == NULL ? "<null>" : data_dir), max_players, mud_port);

  printf( "%s " LOG_FILE " file.\n",
	 clear_syslog_file ? "Clear" : "Do not clear");

  if (kill_other_mud) {
    printf( "Kill other mud.\n");
  } else {
    printf( "Ask if other mud should be killed.\n");
  }

  if (data_dir != NULL) chdir(data_dir); /* Do chdir if specified */

  /* Check if PID_FILE is there...and what it contains */
  fd = check_pid();

  /* We arrive here only if we are to continue and now we are alone. */
  /* Also, the PID_FILE is opened */
  x = xmain(fd);
  unlink(PID_FILE);
  if (x < 0) {
    mudlog("Abnormal termination of mud");
    exit(1);
  }
  mudlog("Normal termination of mud");
}

static void connect_ok(char *h, int port)
{
  char b[80];

  sprintf(b, "Connected to port %d on %s.\n", port, h);
  if (stay_foreground) {
    printf( "%s", b );
  } else {
    write(fildes[1], b, sizeof(b));

#ifdef TIOCNOTTY
    /* Waiting for parent to die */
    while (getppid() > 1);
#endif

    close(fildes[1]);
  }
}

static int xmain(int fd)
{
  int s;
  int k;

  /* We arrive here only if we are to continue and now we are alone. */
  /* Also, the PID_FILE is opened */

  if (open_logfile(LOG_FILE,clear_syslog_file) < 0) {
    close(fd);
    return -1;
  }

  if (bootstrap() < 0) { /* Initialize data structures */
    close(fd);
    return -1;
  }

  /* Now we go background */
  if (go_background(fd) < 0) {
    return -1;
  }

  k = 10;
  while ( (s = main_socket = make_service(mud_port,my_hostname,
					  sizeof(my_hostname),
					  &my_hostent, &s_in)) == -4
	 && errno == EADDRINUSE && --k >= 0) {
    sleep(2);
  }

  if (s < 0) {
    mudlog( "Error code %d from make_service", s);
    progerror("make_service");
    return -1;
  }

  if (s > 0) {   /* We want the main socket at fd 0 */
    dup2(s, 0);
    close(s);
    s = main_socket = 0;
  }

  width = 1;

  connect_ok(my_hostname, mud_port);

  FD_ZERO(&sockets_fds);
  FD_ZERO(&buffer_fds);
  FD_SET(s,&sockets_fds);

  /* Main program loop */
  main_loop(s);
  mudlog( "Closing listening socket");
  close(s);
  return 0;
}

#ifndef TIOCNOTTY

/* System V style daemonizing */

static int go_background(int fd)
{
  char b[80];
  int tty, x, y, z;
  char *s, *t;

  /* Go background */
  signal(SIGHUP,  SIG_IGN);
  signal(SIGINT,  SIG_IGN);
  signal(SIGQUIT, SIG_DFL);
  signal(SIGTSTP, SIG_DFL);
  signal(SIGTTOU, SIG_DFL);
  signal(SIGTTIN, SIG_DFL);

  if (!stay_foreground) {
    if (pipe(fildes) < 0) {
      progerror("go_background/pipe");
      exit(1);
    }
    setpgrp();
    switch (fork()) {
    case -1:
      progerror("fork");
      close(fd);
      return -1;
    case 0:
      break;
    default:
      close(fd);
      close(fildes[1]);
      read(fildes[0],b,sizeof(b));
      printf( "PID = %s\n", b );
      read(fildes[0],b,sizeof(b));
      printf( "%s", b );
      exit(0); /* Let our parent process die */
    }
    close(fildes[0]);
    fclose(stdin);
    fclose(stdout);

    setpgrp(); /* Detach from terminal. */
    switch ( pid = fork() ) {
    case -1:
      progerror("second fork");
      close(fd);
      close(fildes[1]);
      return -1;
    case 0:
      break;
    default:
      close(fd);
      close(fildes[1]);
      exit(0); /* Child process dies, grand-child lives on. */
    }
  }

  pid = getpid();   /* Get our process id */
  sprintf(b,"%d", pid);
  write(fd, b, strlen(b));
  close(fd);

  if (!stay_foreground) {

    write(fildes[1],b,sizeof(b)); /* Send our PID to grand parent. */

    signal(SIGHUP,  SIG_IGN);
    signal(SIGINT,  SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
  } else {
    printf( "PID = %d\n", pid );

    signal(SIGHUP,  SIG_DFL);
    signal(SIGINT,  sig_handler);
    signal(SIGQUIT, SIG_DFL);
  }

  signal(SIGTERM, sig_handler);
  signal(SIGTSTP, SIG_DFL);
  signal(SIGCONT, SIG_DFL);
  signal(SIGTTOU, SIG_DFL);
  signal(SIGTTIN, SIG_DFL);
  signal(SIGSEGV, sig_handler); /* Segmentation fault */
  signal(SIGBUS,  sig_handler); /* Bus error */
  signal(SIGSYS,  sig_handler); /* Bad argument to system call */
  signal(SIGPIPE, SIG_IGN);     /* Broken pipe */
}

#else

/* BSD style daemonizing */

static int go_background(int fd)
{
  char b[80];
  int tty, x, y, z;
  char *s, *t;

  /* Go background */
  signal(SIGHUP,  SIG_IGN);
  signal(SIGINT,  SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
  signal(SIGTSTP, SIG_DFL);
  signal(SIGTTOU, SIG_DFL);
  signal(SIGTTIN, SIG_DFL);
  if (!stay_foreground) {
    if (pipe(fildes) < 0) {
      progerror("go_background/pipe");
      exit(1);
    }
    switch (pid = fork()) {
    case -1:
      progerror("fork");
      close(fd);
      return -1;
    case 0:
      break;
    default:
      printf( "PID = %d\n", pid);
      close(fd);
      close(fildes[1]);
      fflush(stdout); /* Flush stdout */
      read(fildes[0], b, sizeof(b) );
      printf( "%s", b );
      exit(0); /* Let our parent process die */
    }
    fclose(stdin);
    fclose(stdout);
    close(fildes[0]);
  }

  pid = getpid();   /* Get our process id */
  sprintf(b,"%d\n", pid);
  write(fd, b, strlen(b));
  close(fd);


  if (!stay_foreground) {

    if ((tty = open("/dev/tty",O_RDWR,S_IRUSR|S_IWUSR)) >= 0) {

      if (ioctl(tty,TIOCNOTTY,0) < 0) {
	progerror("ioctl,TIOCNOTTY");
	return -1;
      }
      close(tty);
      setpgrp(pid,pid); /* Make our own process group */

    } else if (errno != ENXIO) {
      progerror("open,tty");
      return -1;
    }

    signal(SIGHUP,  SIG_IGN);
    signal(SIGINT,  SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
  } else {
    signal(SIGHUP,  SIG_DFL);
    signal(SIGINT,  sig_handler);
    signal(SIGQUIT, SIG_DFL);
  }

  signal(SIGTERM, sig_handler);
  signal(SIGTSTP, SIG_DFL);
  signal(SIGCONT, SIG_DFL);
  signal(SIGTTOU, SIG_DFL);
  signal(SIGTTIN, SIG_DFL);
  signal(SIGSEGV, sig_handler); /* Segmentation fault */
  signal(SIGBUS,  sig_handler); /* Bus error */
  signal(SIGSYS,  sig_handler); /* Bad argument to system call */
  signal(SIGPIPE, SIG_IGN);     /* Broken pipe */
}

#endif

static int check_pid(void)
{
  int fd;
  int pid = -1;
  int c;
  FILE *f;
  char b[80];

  if ((fd = open(PID_FILE,O_WRONLY|O_CREAT|O_EXCL,S_IRUSR|S_IWUSR)) < 0) {
    /* The file exist already */
    if (errno == EEXIST) {
      if ((f = fopen(PID_FILE,"r")) != NULL) {
	fgets(b,sizeof b,f);
	pid = atoi(b);
	fclose(f);
      } else {
	perror("fopen," PID_FILE);
	exit(1);
      }
      if (pid > 0) {
	if (!kill_other_mud) {
	  printf( "There is another mud running, want me to kill it? ");
	  while ((c = getchar()) != 'N' && c != 'n' && c != 'Y' &&
		 c != 'y' && c != '\n' && c != '\r');
	  if (c == 'Y' || c == 'y') kill_other_mud = True;
	}
	if (!kill_other_mud) {
	  printf( "Ok, then I'll die.\n" );
	  exit(0);
	}
	printf( "Ok, Will kill other mud (PID = %d)\n", pid);
	if (kill(pid,SIGTERM) < 0) {
	  if (errno != ESRCH) {
	    perror("kill");
	    exit(1);
	  } else if (unlink(PID_FILE) < 0) { /* PID_FILE without process */
	    perror("unlink");
	    exit(1);
	  }
	}
      } else {
	if (unlink(PID_FILE) < 0) { /* PID_FILE without process */
	  perror("unlink");
	  exit(1);
	}
      }
      c = 6;
      while (True) {
	sleep(1);
	if ((fd = open(PID_FILE,O_WRONLY|O_CREAT|O_EXCL,
		       S_IRUSR|S_IWUSR)) >= 0) break;
	if (--c < 0) {
	  printf( "Timeout, kill it yourself, I give up.\n" );
	  exit(0);
	}
      }
    } else {
      perror("open " PID_FILE);
      exit(1);
    }
  }
  return fd;
} 


static void usage(void);

static void get_options(int argc,char **argv)
{
  char *s;
  char *v;
  int x;

  if (argc == 1) {

/*    data_dir = DATA_DIR; */
    stay_foreground = False;
    clear_syslog_file = False;
    kill_other_mud = False;
    mud_port = PORT;
    max_players = 32;

    return;
  } else {
    /*
     *since we make odd defaults with no args, we have to add
     * an option to say 'use real defaults', I select option a for that.
     * mudd -a is what usually should be just 'mudd'. -Alf
     */
  }

  while (--argc > 0) {
    s = *++argv;
    if (*s++ != '-') {
      usage();
      exit(1);
    }
    x = *s++;
    switch(x) {
    case 'a': break;
    case 'p':
      if (*s != '\0' || --argc > 0 && *(s = *++argv) != '\0') {
	if ((mud_port = atoi(s)) < 1000 || mud_port > 65535) {
	  mud_port = PORT + 5;
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
    case 'v':
      printf("%s\n", VERSION);
      exit(0);
      break;
    case 'n':
      if (*s != '\0' || --argc > 0 && *(s = *++argv) != '\0') {
	if ((max_players = atoi(s)) < 1 || max_players > 1000) {
	  max_players = 32;
	}
      }
      break;
    case 'd':
      if (*s != '\0' || --argc > 0 && *(s = *++argv) != '\0') {
	data_dir = s;
      }
      break;
    default:
      usage();
      exit(1);
    }
  }
  if (argc > 0) {
    usage();
    exit(1);
  }
}

static void usage(void)
{
  fprintf(stderr, "usage: %s [-p port] [-d directory]\n", progname);
  exit(1);
}


static void new_connection(int fd);

static void main_loop(int m_socket)
{
  int w;
  int v, i;
  int fd;
  int plx = 0;
  struct timeval *tv;
  fd_set r_fds, e_fds;

  time(&global_clock);

  /* Reset everything: */
  for (i = 0; i < numzon; i++) {

	  reset_zone(i, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  }
  last_reset = global_clock;

  cur_player = NULL;
  quit_list  = -1;
  signal(SIGALRM, sig_handler);
  set_timer();
  while(!sig_term_happened) {
    switch (v = setjmp(to_main_loop)) {
    case JMP_QUITTING:
      bflush();
      end_connection();
      setup_globals(-1);
      ++fd;
      break;
    }
    if (sig_timer_happened) {
      sig_timer_happened = False;
      signal(SIGALRM, sig_handler);
      setup_globals(-1); /* bflush() and disable mynum */
      set_timer();
      on_timer();
    } else {
      while (quit_list >= 0) end_connection();

      r_fds = e_fds = sockets_fds;

      if (inp_buf_c > 0) tv = &zerotime;
      else tv = NULL;

#ifdef SYS_HP_UX
      if ((v = select(width, (int *)&r_fds, NULL, (int *)&e_fds, tv)) < 0) {
#else

      if ((v = select(width, &r_fds, NULL, &e_fds, tv)) < 0) {
#endif

	if (errno == EINTR) continue;
	progerror("select");
	exit(1);
      }

      for (fd = 0, w = width; fd < w; fd++) {
	if (FD_ISSET(fd,&e_fds)) {
	  mudlog("Exception pending with fd = %d", fd);
	  --v;
	}
	if (FD_ISSET(fd,&r_fds) || FD_ISSET(fd,&buffer_fds)) {
	  --v;
	  if (fd == m_socket) {
	    new_connection(fd);
	  } else {
	    handle_packet(fd);
	  }
	  bflush();
/*	  while (quit_list >= 0) end_connection(); */
	}

	while (quit_list >= 0) end_connection();
      }
    }
  }
  mudlog("SIGNAL: SIGTERM Handled");
  for (plx = 0; plx < max_players; plx++) {
    if (!EMPTY(pname(plx))) {
      setup_globals(plx);
      crapup("\tSomething very unpleasant seems to have happened...",
	     CRAP_UNALIAS|CRAP_SAVE|CRAP_RETURN);
    }
  }
  rm_pid_file();
}

static void rm_pid_file(void)
{
  if (unlink(PID_FILE) < 0) {
    progerror("rm_pid_file");
  }
}

static void end_connection(void)
{
  INP_HANDLER *i, *j;
  int fd;
  int me = real_mynum;
  int x;

  if ( (x = quit_list) < 0) return;
  if (x == real_mynum) me = -1;
  setup_globals(x);
  quit_list = cur_player->quit_next;
  cur_player->quit_next = -2;

  fd = cur_player->fil_des;
  bflush();
  i = cur_player->inp_handler;
  while (i != NULL) {
    j = i;
    i = i->next;
    free(j);
  }
  cur_player->inp_handler = NULL;

  if(FD_ISSET(fd,&buffer_fds))            /* Erkk! Hopefully the fix      */
    inp_buf_c--;                          /* for the processor eating bug */
                                          /* (from Vita* 1/1/93 -Nck)     */
  FD_CLR(fd,&sockets_fds);
  FD_CLR(fd,&buffer_fds);
  close(fd);
  fclose(cur_player->stream);
  cur_player->stream = NULL;
  cur_player->inp_buf_p = cur_player->inp_buf_end = cur_player->inp_buffer;

#if 0
  cur_player->sock_buf_p = cur_player->sock_buf_end = cur_player->sock_buffer;
#endif

  setup_globals(me);
}


#ifdef SYS_INET_NTOA_BUG
/* If the system include file for inet_ntoa contains erroneous
 * prototypes.
 */
static char *my_inet_ntoa(struct in_addr *in)
{
	static char addr[20];

	sprintf(addr, "%d.%d.%d.%d",
		(int)((in->s_addr >> 24) & 0xff),
		(int)((in->s_addr >> 16) & 0xff),
		(int)((in->s_addr >> 8) & 0xff),
		(int)(in->s_addr & 0xff));

	return addr;
}
#endif


static void new_connection(int m_socket)
{
  PLAYER_REC         *pl;
  FILE               *f;
  struct hostent     *h;
  int                plx;
  int                fd;
  int                sin_len;
  Boolean            host_banned = False, host_b2 = False;
  struct sockaddr_in sin;
  char               *host, *s;
  char               hostnum[MAXHOSTNAMELEN];

  sin_len = sizeof(struct sockaddr_in);
  if ((fd = accept(m_socket,&sin,&sin_len)) < 0) {
    progerror("accept");
  } else if ((f = fdopen(fd,"w")) == NULL) {
    progerror("fdopen");
    exit(1);
  } else {

#ifdef SYS_INET_NTOA_BUG
    strcpy(hostnum, my_inet_ntoa(& sin.sin_addr));
#else
    strcpy(hostnum, inet_ntoa(& sin.sin_addr));
#endif

    host_b2 = is_host_banned(hostnum);
    host = hostnum;

    if ((h = gethostbyaddr((char *)&sin.sin_addr,sizeof(sin.sin_addr),
			   AF_INET)) == NULL) {
/*      mudlog( "gethostbyaddr: Couldn't find hostentry for %s", hostnum)*/;
    } else {
      host_banned = is_host_banned(h->h_name);
      host = h->h_name;
    }

    if ((plx = find_free_player_slot()) < 0) {
      fprintf(f, "\nSorry, but this mud is full, please come back later.\n");
      fflush(f);
      fclose(f);
      return;
    }
    setup_globals(plx);
    pl = cur_player;
#if 0
    pl->sock_buf_p = pl->sock_buf_end = pl->sock_buffer;
#endif
    pl->sin_len = sin_len;
    pl->sin = sin;
    pl->fil_des = fd;
    pl->stream = f;
    s = pl->hostname;
    if (host_banned || host_b2) *s++ = '*';
    strcpy(s,host);

    /* Include this socket as a socket to listen to */
    if (fd >= width) width = fd + 1;
    FD_SET(fd,&sockets_fds);
    new_player();
  }
}

void handle_packet(int fd)
{
  int plx = find_pl_index(fd);
  int x , y, g;
  char z;
  char *b, *p, *c  /*, *k */ ;  int k;


  if (plx < 0) return;

  setup_globals(plx);

  if (cur_player->stream == NULL) return;

  if (cur_player->inp_buf_p >= cur_player->inp_buf_end) {
    if ((x = read(fd,cur_player->inp_buffer,MAX_COM_LEN - 5)) < 0) {
      if (errno == ECONNRESET ||     /* Connection reset by peer */
          errno == EHOSTUNREACH ||   /* No route to host */
          errno == ETIMEDOUT ||      /* Connection timed out */
          errno == ENETUNREACH ||    /* Network is unreachable */
          errno == ENETRESET ||      /* Network dropped connection on reset */
          errno == ENETDOWN ) {      /* Network is down */
        crapup(NULL,CRAP_SAVE|CRAP_UNALIAS|CRAP_RETURN);
	errno = 0;
	quit_player();
	return;
      }
      mudlog("ERROR: when reading data from %s\n", pname(mynum));
      progerror(pname(mynum));
      return;
    }

    if (x == 0) {
        crapup(NULL, CRAP_SAVE|CRAP_UNALIAS|CRAP_RETURN /*|CRAP_LINKLOSS*/ );
        errno = 0;
        quit_player();
        return;
    }

    inp_buf_c++;
    FD_SET(fd,&buffer_fds);

    b = cur_player->inp_buf_p = cur_player->inp_buffer;
    cur_player->inp_buf_end = b + x;
  } else {
    b = cur_player->inp_buf_p;
    x = cur_player->inp_buf_end - b;
  }  


#if 0
/* Telnet code corrected for bug in PC (and some other) telnet by ErIC */

  if(*b == '\377') {
	  b += 3;
	  x -= 3;

	  if(x <= 0) {
		  inp_buf_c--;
		  FD_CLR(fd,&buffer_fds);
		  cur_player->inp_buf_end = 0;
		  return;
	  }
  }

  c = cur_player->sock_buf_p;

  for (y = 0; (y < x) && (c < (cur_player->sock_buffer + MAX_COM_LEN - 6))
       && b[y] != '\n' && b[y] != '\r' && b[y] != '\0'; ++y) {

	    if ((b[y] == '\010') || (b[y] == '\177')) {
		    if (c > cur_player->sock_buffer)
		      c--;
	    }
	    else
	            *c++ = b[y];
  }

  cur_player->sock_buf_p = c;

  if ((y < x) && (c < (cur_player->sock_buffer + MAX_COM_LEN - 6))) {

	  while (y < x && ( b[y] == '\n' || b[y] == '\r' || !b[y]) )
	    y++;

	  *c = 0;
	  k = c;
	  c = cur_player->sock_buffer;
	  
	  for (p = c; *p != '\0'; p++) {
		  if (iscntrl(*p))  *p = ' ';
	  }
      
	  if (cur_player->snooped > 0) {
		  *(k++) = '\n';
		  *(k--) = 0;
		  print_buf(c, True);
		  *k = 0;
	  }

	  g = 1;
  }

  cur_player->sock_buf_p = c;
  cur_player->inp_buf_p = b + y;
  
  if (y >= x) {
	  FD_CLR(fd, &buffer_fds);
	  --inp_buf_c;
  }

  if (g) {
	  cur_player->inp_handler->inp_handler(c);
	  cur_player->sock_buf_p=cur_player->sock_buffer;
  }

#else

  for (y = 0; y < x && b[y] != '\n' && b[y] != '\r'; ++y);
  b[k = y] = 0;
  while (++y < x && ( b[y] == '\n' || b[y] == '\r' ) )
    ;
  cur_player->inp_buf_p = b + y;

  if (y >= x) {
     FD_CLR(fd,&buffer_fds);
     --inp_buf_c;
  }

  /* Let us check if it is any response from TELNET */
  if ((*b & 0377) == 0377) {
    /* Yes it is */
    /* If it is ECHO option (001) and the response is IAC DON'T ECHO
     * 0377 0376 0001 and we have no_echo it means that we have requested
     * echo off and that telnet rejects it.
     * All other responses are either 'ok' or may be ignored.
     */
    if (b[2] == 0001 && (b[1] & 0377) == 0376 && cur_player->no_echo) {
      bprintf( "(WARNING: Password will show): ");
    }
  } else {

    /* Remove unwanted control-characters like \001 etc. : */
    for (p = b; *p != '\0'; p++) {
      if (iscntrl(*p))  *p = ' ';
    }

    if (cur_player->snooped > 0) {
      z = b[++k];
      b[k] = 0;
      b[--k] = '\n';
      print_buf(b,True);
      b[k] = 0;
      b[k+1] = z;
    }

    cur_player->inp_handler->inp_handler(b);
  }
#endif
}



void sig_handler(int sig)
{
  int plx;
  FILE *f;

  switch (sig) {
  case SIGTERM:
    mudlog( "SIGNAL: SIGTERM" );
    sig_term_happened = True;
    return;
  case SIGALRM:
    sig_timer_happened = True;
    return;
  case SIGSEGV:
    mudlog( "SIGNAL: SIGSEGV[%d]", sig);
    break;
  case SIGBUS:
    mudlog( "SIGNAL: SIGBUS[%d]", sig);
    break;
  case SIGINT:
    break;
  default:
    mudlog( "SIGNAL: %d", sig);
    break;
  }
  rm_pid_file();
  exit(1);
}








