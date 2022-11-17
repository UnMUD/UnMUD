#include <unistd.h>
#include <stdlib.h>
#include "kernel.h"
#include "objects.h"
#include <malloc.h>
#include "stdinc.h"
/* ==================== Define some things about them ====================== */

#define DELETE_LEVEL LVL_GOD   /* Level when can delete any mess    */
#define OWNER_CAN_DEL          /* Define if can delete what I wrote */
#define MAX_SUBJ_LEN 45        /* Maximum chars allowed in subject  */
#define MAX_MESS_LEN 200       /* Max lines per mess. (Same as mail) */
#define PBB_FILE "bboard_ply"  /* Name of file for player bboard    */
#define WBB_FILE "bboard_wiz"  /* Name of file for wizard bboard    */
#define AWB_FILE "bboard_awiz" /* Arch wizard board file name.      */
#define GDB_FILE "bboard_god"  /* God board file name.              */

/* The instructions, printed at top of board when it is examined by a player. */

static char *Board_instr =
    "You look closely at the board which has the following intructions written\n \
on it.  Commands: WRITE <heading> , MESSAGE #, ERASE #\n";

/* ==================== Type definitions for the boards ==================== */

typedef struct _mess *PMES;

typedef struct _mess
{
  char owner[PNAME_LEN + 1]; /* Name of player who wrote message    */
  char *date;                /* Ascii string for date & time        */
  char *message;             /* Pointer to the message.             */
  char *header;              /* Heading for the message.            */
  PMES next;                 /* Pointer to next message on board    */
} MESS;

typedef struct _board
{
  int nomess;    /* Number of messages on the board      */
  int level;     /* Min level to be able to read it.     */
  char file[31]; /* Name of the file for storage         */
  PMES last;     /* Last message. Just for fast updates  */
  PMES messages; /* Pointer to first message.            */
} BOARD;

/* =============== Function prototypes for use in board.c only ============= */
/* These are declared static because it is IMPORTANT that no outside files   */
/* call these. Reason: The rest of the mud does not know the types BOARD and */
/* MESS. This is to make board code easier to add to a mud.                  */

static BOARD *present_board();
static void load_board(BOARD *b);
static void update_board(BOARD *b);
static Boolean is_owner(PMES pm, int pl);

/* ================ Define the objects that are to be boards =============== */

#define PBBOARD 1
BOARD pbb;
static int bboards[] =
    {
        OBJ_LIMBO_BBOARD,
        -1};

#define WBBOARD 2
BOARD wbb;
static int wbboards[] =
    {
        OBJ_LIMBO_WIZ_BBOARD,
        -1};

#define AWZBBOARD 3
BOARD abb;
static int abboards[] =
    {
        OBJ_LIMBO_AWIZ_BBOARD,
        -1};

#define GDBBOARD 4
BOARD gbb;
static int gbboards[] =
    {
        OBJ_LIMBO_GOD_BBOARD,
        -1};

/*
 * **  Boot the bulletin board files. NOTE, unlike most bootstrap files, the
 * **  name of the file to boot is NOT contained in bootstrap file.
 * **            YOU MAY WANT TO EDIT SETUP VALUES HERE.
 */
void boot_bboards()
{
  pbb.nomess = 0;
  strcpy(pbb.file, PBB_FILE);
  pbb.level = 0;

  wbb.nomess = 0;
  strcpy(wbb.file, WBB_FILE);
  wbb.level = LVL_APPREN;

  abb.nomess = 0;
  strcpy(abb.file, AWB_FILE);
  abb.level = LVL_ARCHWIZARD;

  gbb.nomess = 0;
  strcpy(gbb.file, GDB_FILE);
  gbb.level = LVL_GOD;

  /* boot mortal board first */
  load_board(&(pbb));
  /* boot the wizard board now */
  load_board(&(wbb));
  /* boot the arch_wizard board now */
  load_board(&(abb));
  /* boot the god board now */
  load_board(&(gbb));
  return;
}

/*
 * **  returns 0 id ob is NOT a bulletin board, and returns appropriate
 * **  number to indicate which board object represents. Also note that
 * **  if (isa_board(ob)) will work fine.
 */
int isa_board(int ob)
{
  int i;

  /* Is it a player bboard */
  for (i = 0; bboards[i] != -1; i++)
  {
    if (bboards[i] == ob)
      return PBBOARD;
  }

  /* is it a wiz bboard */
  for (i = 0; wbboards[i] != -1; i++)
  {
    if (wbboards[i] == ob)
      return WBBOARD;
  }

  /* is it an arch_wizard bboard */
  for (i = 0; abboards[i] != -1; i++)
  {
    if (abboards[i] == ob)
      return AWZBBOARD;
  }
  /* is it a god bboard */
  for (i = 0; gbboards[i] != -1; i++)
  {
    if (gbboards[i] == ob)
      return GDBBOARD;
  }
  return 0;
}

/*
 * **  Simply returns the address of the first board it can find in room with
 * **  mynum. Mortal boards first. Null if none. This should NOT be called outside
 * **  this module, the mud does not know what a BOARD is. :)
 */
static BOARD *
present_board()
{
  int i;

  for (i = 0; bboards[i] != -1; i++)
  {
    if (ploc(mynum) == oloc(bboards[i]))
      return &(pbb);
  }

  for (i = 0; wbboards[i] != -1; i++)
  {
    if (ploc(mynum) == oloc(wbboards[i]))
      return &(wbb);
  }

  for (i = 0; abboards[i] != -1; i++)
  {
    if (ploc(mynum) == oloc(abboards[i]))
      return &(abb);
  }

  for (i = 0; gbboards[i] != -1; i++)
  {
    if (ploc(mynum) == oloc(gbboards[i]))
      return &(gbb);
  }
  return ((BOARD *)NULL);
}

/*
 * **  Displays intructs on how to use the board as well as list of messages
 */

void lat_board(int ob)
{
  BOARD *brd = NULL;
  PMES m = NULL;
  int i = 1;

  switch (isa_board(ob))
  {
  case 0: /* NOT a board */
    return;

  case PBBOARD:
    brd = &(pbb);
    break;

  case WBBOARD:
    brd = &(wbb);
    break;

  case AWZBBOARD:
    brd = &(abb);
    break;

  case GDBBOARD:
    brd = &(gbb);
    break;
  }
  if (brd->level > plev(mynum))
  {
    bprintf("You look closely but can not understand the mystical writings\n");
    return;
  }

  bprintf("%s", Board_instr);
  bprintf("There are %d messages on this board\n\n", brd->nomess);

  m = brd->messages;
  while (m != NULL)
  {
    bprintf("%2d %-*s %-*s \n", i, PNAME_LEN, m->owner, MAX_SUBJ_LEN, m->header);
    i++;
    m = m->next;
  }
  return;
}

/*
 * **  dump the boards to their files. See load_board for file format.
 */
static void
update_board(BOARD *b)
{
  FILE *f = NULL;
  PMES m = NULL;

  if (b == (BOARD *)NULL)
  {
    mudlog("BB ERROR: update null board");
    return;
  }
  if ((b->file == (char *)NULL) || (b->file[0] == '\0'))
  {
    mudlog("BB ERROR: update to null file");
    return;
  }
  if ((f = fopen(b->file, "w")) == (FILE *)NULL)
  {
    mudlog("BB ERROR: Could not open file %s for write", b->file);
    return;
  }

  fprintf(f, "%d\n", b->nomess);
  m = b->messages;
  while (m != NULL)
  {
    fprintf(f, "%s\n%s\n%s\n", m->owner, m->date, m->header);
    fprintf(f, "%d\n%s\n^\n", strlen(m->message), m->message);
    m = m->next;
  }
  fclose(f);
  if (f)
    fclose(f);
  return;
}

/*
 * **  Reads the file b->file and loads all the messages into memory that are in
 * **  the file. FORMAT of the files is: (ignore the first ** each line)
 * **
 * **  <#num messages><NL>
 * **  <owner><NL>
 * **  <date><NL>
 * **  <header><NL>
 * **  <#strlen of the message><NL>
 * **  <message><NL><^><NL>
 * **  <owner><NL> ......
 */
static void
load_board(BOARD *b)
{
  FILE *f = NULL;
  int i = 0, j = 0, size, c;
  PMES m = NULL;
  char buf[81];
  char *p;

  if ((f = fopen(b->file, "r")) == NULL)
  {
    /*    printf("BB ERROR: can't open %s.\n",b->file); */
    b->nomess = 0;
    /*fclose (f); */
    if (f)
      fclose(f);
    return;
  }
  fscanf(f, "%d\n", &i);
  b->nomess = i;
  b->messages = NULL;
  for (; i > 0; i--)
  {
    m = (PMES)xmalloc(1, sizeof(MESS));
    m->next = (PMES)NULL;

    /* Load the message */
    fscanf(f, "%s\n", m->owner);
    fgets(buf, 81, f);
    if ((p = strchr(buf, '\n')) != NULL)
    {
      *p = '\0';
    }
    m->date = COPY(buf);
    fgets(buf, 81, f);
    if ((p = strchr(buf, '\n')) != NULL)
    {
      *p = '\0';
    }
    m->header = COPY(buf);
    fscanf(f, "%d\n", &j);
    j += 10; /* Just in case */
    m->message = (char *)xmalloc(1, j);
    m->message[0] = '\0';
    size = 0;
    while (((c = getc(f)) != EOF) && (size < j - 1))
    {
      if (c == '\n')
      {
        c = getc(f);
        if (c == '^')
        {
          m->message[size] = '\0';
          size = j + 1;
        }
        else
        {
          m->message[size++] = '\n';
          ungetc(c, f);
        }
      }
      else
      {
        m->message[size++] = (char)c;
      }
    }

    /* Check errors and report/recover */
    if (c == EOF) /* Eeek!! */
    {
      printf("BB ERROR: Eeek!! %s ended before expected\n", b->file);
      i = -1;
    }
    else if (size < j)
    {
      m->message[size] = '\0';
    }

    /* Copy message to the board list */
    if (b->messages == NULL) /* special first case */
    {
      b->messages = m;
    }
    else
    {
      b->last->next = m;
    }
    b->last = m; /* Remember last message */
  }
  return;
}

/*
 * **  The command that actually reads message from the board for the players.
 * **  VERB: message
 */
void do_read()
{
  BOARD *b;
  PMES m;
  int num = 0, i;

  b = present_board();
  if (b == NULL) /* No board present               */
  {
    bprintf("I don't see the board here.\n");
    return;
  }
  if ((brkword() == -1) && !isdigit(wordbuf[0]))
  {
    bprintf("Which board would that message be written on then?\n");
    return;
  }

  if (b->level > plev(mynum)) /* Not able to read cause of level */
  {
    bprintf("You look closely but can not understand the mystical writings\n");
    return;
  }

  num = atoi(wordbuf);
  if ((num > b->nomess) || (num < 1)) /* Number out of valid range      */
  {
    bprintf("That message only exist in your imagination\n");
    return;
  }
  num--;
  for (i = 0, m = b->messages; (i < num) && (m != NULL); m = m->next, i++)
    ;
  if (m == NULL)
  {
    bprintf("The message just vanished.\n");
    mudlog("BB ERROR: could not find message in do_read()");
    return;
  }
  bprintf("Message by:%s (%s)\nSubject:%s\n",
          m->owner, m->date, m->header);
  bprintf("%s\n", m->message);
  return;
}

/*
 * **  Erase a message from the board in this room. Checks for permission.
 * **  You can erase if you can read the board and you wrote the message or
 * **  you are at or above the level defined above where you can erase any message
 * **  You must still be able to see the board of course.
 */
void do_erase()
{
  BOARD *b;
  PMES m, m2;
  int num = 0, i;

  if ((b = present_board()) == NULL)
  {
    bprintf("Erase a message from what?\n");
    return;
  }

  if ((brkword() == -1) && !isdigit(wordbuf[0]))
  {
    bprintf("Which board would that message be written on then?\n");
    return;
  }

  if (b->nomess <= 0)
  {
    bprintf("Nothing written on it to delete.\n");
    return;
  }

  if (plev(mynum) < b->level)
  {
    bprintf("You look closely but can not understand the mystical writings\n");
    return;
  }

  num = atoi(wordbuf);
  if ((num > b->nomess) || (num < 1)) /* Number out of valid range      */
  {
    bprintf("That message only exist in your imagination\n");
    return;
  }
  num--;
  for (i = 0, m2 = NULL, m = b->messages; (i < num) && (m != NULL); i++)
  {
    m2 = m;
    m = m->next;
  }
  if (m == NULL) /* Someone beat us to it */
  {
    bprintf("The message just vanished.\n");
    mudlog("BB ERROR: could not find message in do_erase()");
    return;
  }
  if ((plev(mynum) >= DELETE_LEVEL) || is_owner(m, mynum))
  {
    if (m2 == NULL) /* del first message */
    {
      b->messages = m->next;
    }
    else
    {
      m2->next = m->next;
    }
    if (m == b->last) /* deleting the last message */
      b->last = m2;

    free(m->message);
    free(m->date);
    free(m->header);
    free(m);
    b->nomess--;
    update_board(b);
  }
  else
  {
    bprintf("I don't think so!!\n");
  }
  return;
}

/*
 * **  if owners are allowed to delete their messages then returns True if
 * **  mynum is the owner, False otherwise
 */
static Boolean
is_owner(PMES pm, int pl)
{
#ifdef OWNER_CAN_DEL
  if (strcasecmp(pname(pl), pm->owner) == 0)
    return True;
  else
    return False;

#else
  return False;
#endif
}

/*
 * **  Allow players to write on the board. Uses the same writer as mail,
 * **  except that it's handler buffers the input in memory, not writing it
 * **  to a file.
 */
void write_board()
{
  BOARD *b;
  char subj[MAX_COM_LEN];

  void board_handler(void *w, void *ad, int adlen);

  if (cur_player->aliased || cur_player->polymorphed != -1)
  {
    bprintf("Not while aliased.\n");
    return;
  }
  if (cur_player->board != NULL)
  {
    bprintf("You're writing on one already, don't be greedy.\n");
    return;
  }
  if ((b = present_board()) == NULL)
  {
    bprintf("There is no board to write on.\n");
    return;
  }
  if (plev(mynum) < b->level)
  {
    bprintf("Strange, your pen doesn't seem to work.\n");
    return;
  }

  getreinput(subj);
  if (strlen(subj) > MAX_SUBJ_LEN)
    bprintf("Warning: Subject to long, truncated.\n");
  subj[MAX_SUBJ_LEN + 1] = '\0';

  cur_player->board = b; /* Remember the board + stop 2 writes at same time */

  ssetflg(mynum, SFL_POSTING);
  start_writer("End the message with ** on the beginning of a new line",
               "BOARD>",
               subj,
               MAX_SUBJ_LEN + 1,
               board_handler,
               WR_CMD | '*',
               MAX_MESS_LEN);
  sclrflg(mynum, SFL_POSTING);
  return;
}

/*
 * **  Handle the data we get from the writer, create a message, put it in
 * **  the board, save the board to a file, and finally free the cur_player->board
 * **  variable, allowing them to start another write.
 */
void board_handler(void *w, void *ad, int adlen)
{
  PMES msg;
  char b[100], subj[MAX_SUBJ_LEN + 1];
  char bigbuf[(MAX_COM_LEN + 1) * MAX_MESS_LEN]; /* More than we could want */
  BOARD *brd = (BOARD *)cur_player->board;
  int size = 0;

  if (brd == NULL) /* Should never happen, but better than crashing mud   */
  {                /* if it does.                                         */
    mudlog("Board went null on %s\n", pname(mynum));
    return;
  }

  strcpy(subj, (char *)ad); /* Seems to work better if copy this here */
  bigbuf[0] = '\0';
  while ((wgets(b, sizeof(b), w)) != NULL)
  {
    if (!EMPTY(b))
      strcat(bigbuf, b);
  }

  if (EMPTY(bigbuf))
  {
    bprintf("Ok, I won't bother posting then\n");
    cur_player->board = NULL;
    return;
  }

  /* Check for trailing \n or \r and get rid of them */
  size = strlen(bigbuf);
  if ((bigbuf[size - 1] == '\n') || (bigbuf[size - 1] == '\r'))
    bigbuf[size - 1] = '\0';
  else if ((bigbuf[size] == '\n') || (bigbuf[size] == '\r'))
    bigbuf[size] = '\0';

  /* Lets create this message */
  msg = (PMES)xmalloc(1, sizeof(MESS));
  msg->message = COPY(bigbuf);
  msg->date = COPY(time2ascii(TIME_CURRENT));
  msg->header = COPY(subj);
  strcpy(msg->owner, pname(mynum));
  msg->next = NULL;

  if (brd->last == NULL)
    brd->messages = msg;
  else
    brd->last->next = msg;
  brd->last = msg;
  brd->nomess++;
  update_board(brd);

  cur_player->board = NULL; /* Free up the board(locking write for mynum) */

  return;
}
