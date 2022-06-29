/*
 * MUD writer
 *
 * Note that the mud writer is reentrant, you may start one writer
 * while another is active, the writer will save the previous writer's state
 * prompt et.c the writer tries to save all info about the previous
 * input handler, cprompt is also saved.
 * This means that for example while doing 'mail xyzzy' in mud
 * you may do '*mail zyxxy' inside there, to mail another person.
 * by typing '**' you will then terminate the last mail and you will
 * get back to the previous mail and yet another ** will terminate your
 * mail to xyzzy and get you back to whereever you were when you did
 * mail xyzzy.
 *
 *
 * Also note that it is not an editor, you may write text and the text
 * can replace old text or may be appended to old text, but you don't have
 * the old text available while inside the writer.
 *
 *
 * The MUD writer is used like this:
 *
 * example a simple mail writer for mud.
 *
 * mailcom(void)
 * {
 *    if (brkword() != -1) { /* send mail
 *        start_writer( "Write your mail, terminate with '**'",
 *                      "MAIL>",
 *                      wordbuf,  /* The name who we want to send mail to
 *                      strlen(wordbuf) + 1, /* length of argument
 *                      mail_handler,
 *                      WR_CMD|'*',          /* allow commands
 *                      500);                /* Max 500 lines
 *
 *        return;
 *    } else { /* read mail
 *      read_mail();
 *   }
 * }
 *
 * mail_handler(void *w,void *ad, int adlen)
 * {
 *    FILE *f;
 *    char b[100];
 *    char a[100];
 *
 *    strcpy(b,"MAIL/");
 *    strcpy(a,ad); /* save the address, wgets will destroy ad
 *    strcat(b,a);
 *    if ((f = fopen(b,"a")) == NULL) {
 *       progerror(b);
 *       terminate_writer(w);
 *       return;
 *    } else {
 *       while (wgets(b,sizeof(b),w) != NULL) {
 *           fputs(b,f);
 *       }
 *       /* notify the person he has mail
 *       notify_mail(a); /* can't use ad here as it is destroyed by wgets
 *    }
 * }
 *
 */

void start_writer(char *h,        /* Header line */
		  char *p,        /* Prompt string */
		  void *arg,      /* Argument data to handler. */
		  int  arglen,    /* Length of argument data */
		  void handler(void *x,void *arg,int arglen), /* handler */
		  int  flags,     /* Flags */
		  int max_lines); /* Max number of lines */

int wnum_lines(void *w); /* Get number of lines left in buffer */

int wnum_chars(void *w); /* Get number of chars (no \n) in buffer */


/* These functions are used by handler to read the text */
/* Get one character from writer, all memory is released
 * when EOF is returned. including the memory area for arg.
 * if arg is to be used after reading EOF, make sure to copy it
 * to a safe place before EOF is read by wgetc.
 */
int  wgetc(void *w);     /* get one character from writer */

/* Unget one character from writer, unget EOF is legal and effectively
 * empties the buffer
 */
int  wungetc(int c, void *w);
char *wgets(char *buf,int buflen,void *w); /* Get one line of characters */

/* terminate writer terminates the writer by first doing wungetc(EOF,x);
 * followed by reading that same EOF character and thereby
 * releasing all memory used by the writer.
 */
void terminate_writer(void *w);

/* Terminates all writers used by player plx. */
void terminate_all_writers(int plx);


/* flags */
#define WR_CMD    0x0100    /* Commands are permitted */
#define WR_CMDCH    0x7f    /* Mask for special character, default '*' */
