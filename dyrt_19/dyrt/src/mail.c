/* iDIRT Message System Version 1.03/PE (Public Edition)
 * by Illusion (1995)
 *
 * Derived from the DYRT mailer from DYRT 1.1c
 * by Thrace [Valentin Popescu] (vpopesc@calstatela.edu)
 * -------------------------------------------------------------------------
 *                       *** NOTE ***
 * DO NOT CHANGE THE VERSION INFORMATION IN THIS MAILER!!
 * IF YOU CHANGE THE MAILER CONSIDERABLY, GIVE CREDIT TO
 * BOTH ILLUSION AND THRACE. THRACE WORKED HARD ON THIS
 * MAILER, AND ILLUSION WORKED HARD TO DEBUG IT, THEY
 * DESERVE CREDIT FOR DOING SO.
 *
 * Basically what I did was take this source code from the Dyrt 1.1c 
 * source code and started rewriting it and trying to make it work with 
 * the iDIRT (or any other Dirt) code. I had to fix a few bugs that the 
 * mailer had which were:
 *
 * 1.00
 * ----
 *	- A bug in the dump_headers() function that made the whole MUD crash 
 *        when you scan your mail when you typed 'mail' to read it (read
 *        the headers). What was happening was this: the MUD was calling a 
 *        bflush() that seemed to be crashing the MUD. Removing this bflush() 
 *        still killed it. After trying out many different techniques I 
 *        figured (basically guessed *grin*) that the error was coming from 
 *        the bflush() calling fflush() when the mail file was at the EOF (or
 *        past it, that was my idea). So I had the dump_headers() check to see
 *        if the pointer that accesses the mail file is located past the EOF 
 *        or not. Well, this fixed it and it no longer crashes.
 *
 *     - A bug in the write_mail() function that didn't let you abort the 
 *       message if you had a blank subject. 
 *
 *     - Fixed a bug in the mail menu that would cause it to stay in an 
 *       infinite mail loop. This bug was caused by a user running the 
 *       mailer from within the mailer again (*mail).
 *
 *     - Players can now no longer alias from the mail menu.
 *
 * 1.01
 * ----
 *     - Fixed the bug that allowed players to put the EOM text in the 
 *       message. Change the EOM_MARKER #define to whatever you see fit
 *	 and keep it a secret so that nobody can kill players mail files.
 *	 You may even wish to use control characters for the EOM marker, 
 *	 your decision.
 *
 *     - Added some color to the mailer.
 *
 * 1.02
 * ----
 *     - Fixed a bug in which if users entered any format character into 
 *       the subject, it crashed the MUD.
 *
 * 1.03
 * ----
 *     - Fixed a bug in which if users entered any format character into
 *	 the text, it crashed the MUD.
 *
 *     - Now the mailer saves the player's prompt before entering the mailer.
 *   
 *     - Released a Public Version removing the iDIRT specific code.
 *
 * 1.04
 * ----
 *     - Added multiple recipients
 *     - Added forwarding
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include "kernel.h"
#include "objects.h"
#include "mobiles.h"
#include "mobile.h"
#include "locations.h"
#include "sflags.h"
#include "pflags.h"
#include "mflags.h"
#include "cflags.h"
#include "quests.h"
#include "sendsys.h"
#include "levels.h"
#include "parse.h"
#include "exec.h"
#include "rooms.h"
#include "mail.h"
#include "bprintf.h"
#include "stdinc.h"

/* Function Prototypes
Boolean concat_files (char *to, char *from);
*/

void cc_mail( char *u );

/* Mail system defines */
#define EOM_MARKER 	"\001\002\003"
#define MAX_LETTERS	150
#define READ		1
#define WRITE		0
#define SUBJECT		"&+CSubject: &+w"
#define CC		"&+CCC: &+w"
#define SENT		"\n&+CMessage sent.\n"
#define MAILP		"&+c:&+C> &+w"
#define PROMPT		"&+C[&+wMail&+C] &+b:&+w "
#define NOTICE1		"\
&+wEnter your message. To save your message type &+W'=s' &+wor &+W'**' &+won \
a blank line.\nTo abort your message type &+W'=a' &+wor &+W'*abort' &+won \
a blank line.\n"

int count;

/* mailcom(): Main loop for the mail program. This function checks to see if
 * the person trying to access the mailer is aliased, and then checks if a
 * player's name was passed through the command line and will startup the
 * mail sender if they are found.
 */
void mailcom (void)
{
  PERSONA p;

  /* Do not let aliased players use the mailer */
  if (cur_player->aliased || cur_player->polymorphed != -1)
    {
      bprintf ("You cannot use the mailer while you are aliased.\n");
      return;
    }
  /* no mailing while in jail. */
  if (ltstflg(ploc(mynum), LFL_JAIL) && plev(mynum) < LVL_ARCHWIZARD)
    {
      bprintf ("Sorry... we jailed the postman also...\n");
      return;
    }
  /* Save the mail header */
  cur_player->mails.old_handler = cur_player->inp_handler->inp_handler;

  /* Save the prompt */
  strcpy(cur_player->mails.prompt, cur_player->prompt);

  if (brkword () != -1) {
    if (!getuaf (wordbuf, &p)) {
      bprintf ("&+WPlayer not found in database.\n");
      return;
    }
    strcpy (cur_player->mails.w_name, p.p_name);
    bprintf ("&+CTo: &+w%s\n", cur_player->mails.w_name);

    bprintf (SUBJECT);
    cur_player->mails.old_handler = cur_player->inp_handler->inp_handler;
    strcpy (cur_player->cprompt, SUBJECT);

    replace_input_handler(write_mail);
    return;
  }
  read_mail (pname(mynum));
  return;
}

/* write_mail(): Gets the message subject and checks to see if to abort 
 * the message (if subject is a blank) and creates the header in the 
 * temporary mail file. Also, now you can abort the message here (before
 * it just asked you for the subject again).
 */
void write_mail (char *u)
{
  if (EMPTY (u) || u[0] == '\n') {
    bprintf ("&+RMessage Aborted.\n");
    replace_input_handler (cur_player->mails.old_handler);
    strcpy(cur_player->prompt, cur_player->mails.prompt);
    strcpy (cur_player->cprompt, build_prompt (mynum));
    bprintf("\r%s", cur_player->cprompt);
    setup_globals (mynum);
    return;
  } else {
    bprintf (CC);
    open_mail (WRITE, True);
    if (cur_player->mails.write == (FILE *) 0)
      return;
    fprintf (cur_player->mails.write, "%s\n%s\n%d\n", pname (mynum), u, time (0));
    fflush (cur_player->mails.write);
    strcpy (cur_player->cprompt, CC);
    replace_input_handler (cc_mail);
  }
  return;
}

void cc_mail( char *u ) {
  if( EMPTY( u ) || u[0] == '\n' ) {
    cur_player->mails.cc_list[ 0 ] = '\0';
  } else {
    strncpy( cur_player->mails.cc_list, u, MAX_COM_LEN );
  }
  bprintf(NOTICE1);
  bprintf(MAILP);
  strcpy( cur_player->cprompt, MAILP );
  replace_input_handler( input_mail );
}

/* input_mail(): Handles the input of the message text */
void input_mail (char *x)
{
  int p;
  char to[256];
  char from[256];
  Boolean abort = False;

  if (!strcasecmp(x,"=s") || !strcasecmp(x,"=a") ||
      !strcasecmp(x,"**") || !strcasecmp(x,"*abort")) {
    fprintf( cur_player->mails.write, "\n**CC:%s", cur_player->mails.cc_list );
    fprintf (cur_player->mails.write, "\n%s\n", EOM_MARKER);
    close_mail (WRITE);
    strcpy(cur_player->prompt, cur_player->mails.prompt);
    strcpy (cur_player->cprompt, (char *) build_prompt (mynum));
    sprintf (to, "%s/%s", MAIL_DIR, cur_player->mails.w_name);
    sprintf (from, "%s/%s.from-%s", MAIL_DIR, cur_player->mails.w_name, pname (mynum));

    if (!strcasecmp(x,"=a") || !strcasecmp(x, "*abort")) {
      bprintf("&+RMail aborted.\n");
      abort = True;
    } else {
      PERSONA pe;
      char *pCCName;

      bprintf( "Sending mail to %s\n", cur_player->mails.w_name );
      concat_files (to, from);

      /** Send mail to the cc list as well... */

      for( pCCName = strtok( cur_player->mails.cc_list, " ," );
           pCCName != NULL; pCCName = strtok( NULL, " ," ) ) {
        if (!getuaf (pCCName, &pe)) {
          bprintf ("&+WPlayer %s not found in database.\n", pCCName );
        } else {
          sprintf( to, "%s/%s", MAIL_DIR, pe.p_name );
          bprintf( "Sending mail to %s\n", pe.p_name );
          concat_files( to, from );
          if( ( p = fpbns( pe.p_name ) ) >= 0 && ( p < max_players ) ) {
            sendf (p, 
	           "&+C* &+WYou have new mail from %s &+C*\n", pname (mynum));
          }
        }
      } 
    }

    unlink (from);

    if ((p = fpbns (cur_player->mails.w_name)) >= 0) {
      if (p < max_players && !abort)
        sendf (p, "&+C* &+WYou have new mail from %s &+C*\n", pname (mynum));
    }

    if (cur_player->mails.level)
      replace_input_handler (mail_menu);
    else {
      replace_input_handler (cur_player->mails.old_handler);
      strcpy(cur_player->prompt, cur_player->mails.prompt);
      strcpy (cur_player->cprompt, build_prompt (mynum));
      bprintf("\r%s", cur_player->cprompt);
    }
    return;
  }

  fprintf (cur_player->mails.write, "%s\n", x);
  bprintf (MAILP);
  fflush (cur_player->mails.write);
  return;
}

/* read_mail(): Lets the player read a message */
void read_mail (char *u)
{
  strcpy (cur_player->mails.r_name, pname (mynum));

  open_mail (READ, False);
  if (!(cur_player->mails.read)) {
    bprintf ("&+CSorry, you have no mail.\n");
    return;
  }

  strcpy (cur_player->cprompt, PROMPT);
  dump_headers (cur_player->mails.read);
  bprintf (PROMPT);
  replace_input_handler (mail_menu);
  return;
}

/* dump_headers(): Displays the message headers for your message file. 
 * There was a bug with the original code that caused the MUD to crash when
 * the bflush() was called and if the EOF of the mail file had been reached.
 * This was fixed by Illusion for iDIRT.
 */
void dump_headers (FILE * m)
{
  char   b[MAX_COM_LEN];
  char   from[MAX_COM_LEN];
  time_t time_sent;
  char   s[MAX_COM_LEN];
  count = 1;

  rewind (cur_player->mails.read);

  bprintf ("&+C No.  From         Date Sent                 Subject\n");
  bprintf ("&+b----- ------------ ------------------------  ---------------------------\n");

  while (!feof (cur_player->mails.read)) {
    fgets (b, MAX_COM_LEN, cur_player->mails.read);	/* From Field */
    if (feof(cur_player->mails.read)) 
      return;
    b[strlen (b) - 1] = '\0';
    strncpy (from, b, 12);

    fgets (s, MAX_COM_LEN, cur_player->mails.read);	/* Subject Field */

    fgets (b, 40, cur_player->mails.read);		/* Date Send Field */
    time_sent = atol (b);

    for (;;) {						/* Find the EOF text */
      if (feof (cur_player->mails.read))
        break;
      fgets (b, MAX_COM_LEN, cur_player->mails.read);
      if (strncmp (b, EOM_MARKER, 3) == 0)
	break;
    }

    if (strlen (s) > 27) {				/* Dump Header */
      s[24] = ' ';
      s[25] = s[26] = s[27] = '.';
      s[28] = '\n';
      s[29] = '\0';
    }
    bprintf("&+w[%3.3d] %-12.12s %24.24s  %s", 
            count, from, ctime (&time_sent),
	    (cur_player->mails.data)[count - 1] ? "&+R[ To Be Deleted ]\n" : s);
      
    count++;
    if (!feof(cur_player->mails.read)) /* [iDIRT] Bug fix. The MUD used */
      bflush();                        /* to crash here until the mail  */
    }                                  /* file was checked for EOF.     */
    return;
}

/* mail_menu(): The menuing system for the mailer */
void mail_menu (char *u)
{
  time_t tim;
  char   buffer[MAX_COM_LEN + 5];
  int    quitting_mail = False;
  int    do_cc = False;

  cur_player->inmailer = True;

  if (cur_player->mails.level) {
    bprintf (PROMPT);
    cur_player->mails.level = 0;
  }

  strcpy (cur_player->cprompt, PROMPT);

  if (isupper (u[0]))
    u[0] = tolower (u[0]);

  switch (u[0]) {
    case 'R': do_cc = True;
    case 'r':			/* Reply */
      tim = time (0);
      if (EMPTY (cur_player->mails.last))
	bprintf ("&+WYou haven't read any messages yet.\n");
      else {
	if (strncasecmp (cur_player->mails.subject, "Re: ", 4) != 0)
	  sprintf (buffer, "&+CRe: &+w%s", cur_player->mails.subject);
	else
	  strcpy (buffer, cur_player->mails.subject);

	strcpy (cur_player->mails.w_name, cur_player->mails.last);
	open_mail (WRITE, True);
	if (!cur_player->mails.write) {
          close_mail (READ);
	  return;
	}

	bprintf ("&+CTo: &+w%s\n&+CSubject: &+w%s\n", 
		  cur_player->mails.last, buffer);
        bprintf (CC);
        fprintf (cur_player->mails.write, "%s\n%s%d\n", 
                 pname (mynum), buffer, tim);

        cur_player->mails.level = 1;
        replace_input_handler (cc_mail);
	return;
      }
    break;
    
    case 'd':			/* Delete */
      if (u[1] != ' ' || !isdigit (u[2]))
	bprintf ("\nUsage: d [message number]\n");
      else
						/*or statement added by jwn*/
        if ((atoi(&u[2]) - 1) > MAX_LETTERS || atoi(&u[2]) > (count-1) )  
          {
          bprintf("Message number cannot be greater than %d\n", (count-1));
          mudlog("Bad &+rMAIL&* by %s, Number was %d",
                 pname(mynum), (atoi(&u[2]) - 1));
          bprintf (PROMPT);
          return;
          }     
	if((atoi(&u[2]) - 1) < 0) {
	  bprintf("Message number must be at least 1.\n");
	  mudlog("Bad &+rMAIL&* by %s, Number was %d",
		 pname(mynum), (atoi(&u[2]) - 1));
	  bprintf(PROMPT);
	  return;
	}

	(cur_player->mails.data)[atoi (&u[2]) - 1] = True;
    break;

    case 'u':			/* Undelete */
      if (u[1] != ' ' || !isdigit (u[2]))
	bprintf ("\nUsage: u [message number]\n");
      else
        if ((atoi(&u[2]) - 1) > MAX_LETTERS || atoi(&u[2]) > (count-1) )  
          {
          bprintf("Message number cannot be greater than %d\n", MAX_LETTERS);
          mudlog("Bad &+rMAIL&* by %s, Number was %d",
                 pname(mynum), (atoi(&u[2]) - 1));
          bprintf (PROMPT);
          return;
          }     
	if((atoi(&u[2]) - 1) < 0) {
	  bprintf("Message number must be at least 1.\n");
	  mudlog("Bad &+rMAIL&* by %s, Number was %d",
		 pname(mynum), (atoi(&u[2]) - 1));
	  bprintf(PROMPT);
	  return;
	}
	(cur_player->mails.data)[atoi (&u[2]) - 1] = False;
    break;
    
    case '*':			/* Game command */
      gamecom (&u[1], False);
    break;
 
    case 'i':			/* Info */
      bprintf ("\001f%s\003\n", "mail.info");
    break;
    
    case '?':
      bprintf ("\001f%s\003\n", "mail.help");
    break;
    
    case 'h':
      bprintf ("\n");
      dump_headers (cur_player->mails.read);
    break;

    case 'v':
      bprintf("iDIRT Mailer Version 1.04/PE (Public Edition)\n");
      bprintf("1994, 95 by Illusion\n\n");
      bprintf("Derived from the DYRT mailer from DYRT 1.1c\n");
      bprintf("by Valentin Popescu (vpopesc@calstatela.edu)\n");
      bprintf("Multiple recepient & forwarding capabilities added 1998\n" );
      bprintf(" by DuRandir\n" );
    break;

    case '\0':
    
    case '\n':
    break;
    
    case 'x':
    
    case 'q':			/* quit the mailer */
      replace_input_handler (cur_player->mails.old_handler); 
      close_mail (WRITE);
      delete_mail ();
      close_mail (READ);
      strcpy(cur_player->prompt, cur_player->mails.prompt);
      strcpy (cur_player->cprompt, build_prompt (mynum));
      setup_globals (mynum);
      cur_player->inmailer = False;
      quitting_mail = True;
    break;
    
    default:
    
    /* Check to see if a number was inputed */
    {
      int number;
      number = atoi (u);
      if (number < 1 || number > MAX_LETTERS) {
        bprintf ("\nUnknown command. Type '?' for help.\n");
        break;
      }
      read_msg (number);
    }
    break;
  }

  if (!quitting_mail) {
    bprintf (PROMPT);
  } else {
    strcpy(cur_player->prompt, cur_player->mails.prompt);
    bprintf ("%s", (char *) build_prompt (mynum));
  }

  return;
}

/* read_msg(): Reads in a message number, taking the number as a parameter */
void read_msg (int num)
{
  /* to get to a particular message num, we must skip exactly num - 1 EOFs */

  char   buffer[MAX_COM_LEN];
  time_t time_sent;

  if (num <= 0 || num > MAX_LETTERS)
    return;

  rewind (cur_player->mails.read);

  for (num = num - 1; num != 0; num--) {
    for (;;) {
      fgets (buffer, MAX_COM_LEN, cur_player->mails.read);
        if (feof (cur_player->mails.read))
          break;
        if (strncmp (buffer, EOM_MARKER, 3) == 0)
	    break;
    }
  }

  /* Now we should be at the right place, and we dump the letter */

  fgets (buffer, MAX_COM_LEN, cur_player->mails.read);
  if (feof (cur_player->mails.read)) {
    bprintf ("\n&+WMessage out of range.\n", num);
    return;
  }
  buffer[strlen (buffer) - 1] = '\0';
  strncpy (cur_player->mails.last, buffer, 12);

  if ((cur_player->mails.data)[num] == True)
    bprintf ("\n&+R* Note: This letter is marked for deletion *\n");

  /* dump the header */
  bprintf ("\n&+CFrom: &+w%s\n", buffer);
  fgets (buffer, MAX_COM_LEN, cur_player->mails.read);
  strncpy (cur_player->mails.subject, buffer, MAX_COM_LEN);
  bprintf ("&+CSubject: &+w%s", buffer);
  fgets (buffer, MAX_COM_LEN, cur_player->mails.read);
  time_sent = atoi (buffer);
  bprintf ("&+CSent: &+w%s", ctime (&time_sent));
  bprintf ("\n");

  /* Dump the message body */

  for (;;) {
    fgets (buffer, MAX_COM_LEN, cur_player->mails.read);
    if (strncmp (buffer, EOM_MARKER, 3) == 0)
      break;
    if( buffer[0] == '*' && buffer[1] == '*' ) {
      bprintf( "%s", buffer + 2 );
      /* strncpy (cur_player->mails.cc_list, buffer, MAX_COM_LEN); */
    } else { 
      bprintf("%s", buffer);
    }
    if (feof (cur_player->mails.read))
      break;
  }
  bflush ();
  return;
}

/* open_mail(): Handles the opening of a mail file. */
void open_mail (char mode, Boolean complain)
{
  char buf[1024];
  FILE *tf;
  Boolean fail = False;

  switch (mode) {
    case READ:
      sprintf (buf, "%s/%s", MAIL_DIR, cur_player->mails.r_name);
      tf = fopen (buf, "r");
      if (!tf)
        fail = True;
      else
        cur_player->mails.read = tf;
      break;
    case WRITE:
      sprintf (buf, "%s/%s.from-%s", MAIL_DIR, cur_player->mails.w_name, pname (mynum));

      tf = fopen (buf, "w");
      if (!tf) {
        progerror ("mail-write");
        fail = True;
      }
      cur_player->mails.write = tf;
      break;
    }

  if (complain && fail) {
    bprintf ("We are experiencing technical difficulties. Try again later.\n");
    progerror ("open_mail");
    replace_input_handler (cur_player->mails.old_handler);
  }
  return;
}

/* close_mail(): Handles the closing of the mail files */
void close_mail (char mode)
{
  switch (mode) {
    case READ:
      if (cur_player->mails.read)
        fclose (cur_player->mails.read);
      cur_player->mails.read = 0;
      break;
    case WRITE:
      if (cur_player->mails.write)
        fclose (cur_player->mails.write);
      cur_player->mails.write = 0;
      break;
    }
}

/* delete_mail(): Deletes messages that have been marked for removal */
void delete_mail (void)
{
  char tempfile[128];
  char tempfile2[128];
  char b[MAX_COM_LEN];
  FILE *fout;
  FILE *fin;
  struct stat s;
  register int t;
  int pid;

  pid = getpid ();

  sprintf (tempfile, "%s/TEMP.%s.%d", MAIL_DIR, pname (mynum), pid);
  sprintf (tempfile2, "%s/%s", MAIL_DIR, pname (mynum));
  (void) unlink (tempfile);

  /* copy the file, filtering out the offending message */

  fout = fopen (tempfile, "w");
  if (!fout) {
    bprintf ("System error detected, mail NOT deleted.\n");
    progerror ("delete_file write");
    return;
  }

  fin = fopen (tempfile2, "r");
  if (!fin) {
    bprintf ("System error detected, mail NOT deleted.\n");
    progerror ("delete_file read");
    fclose (fout);
    unlink (tempfile);
    errno = 0;		/* unlink may cause an errorr */
    return;
  }

  /* now we have both files open */

  for (t = 0; t != MAX_LETTERS; t++) {

    if (feof (fin) != 0)
      break;

    /* Should we delete messages? */
    if ((cur_player->mails.data)[t] != 0) { 	/* Yes, delete messages */
      while (1) {				/* Search for 'EOF'     */
        bzero (b, MAX_COM_LEN);
	fgets (b, MAX_COM_LEN, fin);
	if (feof (fin) != 0)
          break;
	if (strncmp (b, EOM_MARKER, 3) == 0)
          break;
      }
    } else {					/* No, do not delete   */
      while (1) {
        bzero (b, MAX_COM_LEN);
        fgets (b, MAX_COM_LEN, fin);
        fprintf (fout, "%s", b);
        if (feof (fin) != 0)
          break;
        if (strncmp (b, EOM_MARKER, 3) == 0)
          break;
      }
    }
    /* unset the marker */
    (cur_player->mails.data)[t] = 0;
  }

  rewind (fin);

  fclose (fin);
  fclose (fout);

  /* copy the file over */
  unlink (tempfile2);
  rename (tempfile, tempfile2);

  if (stat (tempfile2, &s) != -1)
    if (s.st_size < 4)
      unlink (tempfile2);

  return;
}

/* check_files(): Checks if any files have been updated since login. A 
 * problem with iDIRT (and any DIRT for that matter) has been fixed here and 
 * it uses all of the correct DIRT user variables now. 
 */
void check_files (void)
{
  PERSONA me;
  struct stat s;
  time_t last;
  char buff[512];

  getuaf(pname(mynum), &me);
  last = me.p_last_on;

  sprintf (buff, "%s/%s", MAIL_DIR, pname (mynum));

#if 0	/* If you do have bulletins, this code is helpful */
  if (stat(BULLETIN_DIR"/"MORTALBULL, &s) != -1) {
    if (s.st_mtime > last)
      bprintf ("New bulletin as of %19.19s.\nType 'bulletin' to read it.\n", ctime (&s.st_mtime));
  }

  if ((stat (BULLETIN_DIR"/"WIZARDBULL, &s) != -1) && plev (mynum) >= LVL_WIZARD) {
    if (s.st_mtime > last)
      bprintf ("New Wizard bulletin as of %19.19s.\nType 'wbull' to read it.\n", ctime (&s.st_mtime));
  }

  if ((stat (BULLETIN_DIR"/"ARCHWIZBULL, &s) != -1) && plev (mynum) >= LVL_ARCHWIZARD) {
    if (s.st_mtime > last)
      bprintf ("New Arch-Wizard+ bulletin as of %19.19s.\nType 'abull' to read it.\n", ctime (&s.st_mtime));
  }

  if ((stat (BULLETIN_DIR"/"DEMIGODBULL, &s) != -1) && plev (mynum) >= LVL_DEMI) {
    if (s.st_mtime > last)
      bprintf ("New DemiGod+ bulletin as of %19.19s.\nType 'gbull' to read it.\n", ctime (&s.st_mtime));
  }
#endif

  if (stat (buff, &s) != -1) {
    if (s.st_size > 3) {
      if (s.st_mtime > last)
        bprintf ("&+WYou have new mail.\n");
      else
        bprintf ("&+WYou have old mail in your mailbox.\n");
    }
  }
  bprintf("\n");
}

/* concat_files(): Puts two files together (like the 'cat' shell command)
Boolean concat_files (char *to, char *from)
{
  FILE *fin;
  FILE *fout;
  char x[1024];

  fin = fopen (from, "r");
  if (!fin) {
    fprintf (stderr, "error on opening source file for concat_files.\n");
    return (False);
  }
  fout = fopen (to, "a+");
  if (!fout) {
    fprintf (stderr, "error on opening destination file for concat_files.\n");
    fclose (fin);
    return (False);
  }

  while (!feof (fin)) {
    fgets (x, 1024, fin);
    fprintf (fout, "%s", x);
    x[0] = '\0';
  }
  fclose (fin);
  fclose (fout);
  return (True);
}
*/
