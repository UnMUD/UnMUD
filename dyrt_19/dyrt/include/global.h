#ifndef __GLOBAL_H__ 
#define __GLOBAL_H__ 


jmp_buf to_main_loop;

FILE *log_fl;

Location   *room_data;
Object     *objects;
UBLOCK_REC *ublock;

ZONE	*zoname;
char	**messages;
char	**verbtxt;
char	*txt1;
char	*txt2;
int	levels[LVL_APPREN + 1];
char	strbuf[MAX_COM_LEN];
char	wordbuf[MAX_COM_LEN];
char    rawbuf[MAX_COM_LEN];
char	item1[MAX_COM_LEN];
char	item2[MAX_COM_LEN];

char			*progname;
char			*data_dir = DATA_DIR;
char			my_hostname[MAXHOSTNAMELEN];
struct hostent		*my_hostent;
struct sockaddr_in      s_in;
char			**envp;
int			pid;

PLAYER_REC      *players;
WORLD_REC       the_world_rec;
WORLD_REC	*the_world = &the_world_rec;

/************************
**
** mynum is now used to designate the index of the current player, i.e.
** the player who's message we are currently handling.
** the name should perhaps be 'cur_player' or something like that
** but 'mynum' is used in the old source and since we try to copy it over
** with minimum number of changes, we keep the name.
** new variables are cur_player which is set to &players[mynum] and
** cur_ublock which is set to &ublock[mynum].
**
*************************
*/
int	    mynum;          /* current player slot-number */
int         real_mynum;     /* real mynum if mynum is fake due to aliasing */
int         quit_list;      /* real mynum of player to quit */
PLAYER_REC *cur_player;     /* Current player info. */
UBLOCK_REC *cur_ublock;     /* Current ublock info. */


int	max_players = 32;
int     num_const_chars;
int	numchars;       /* Number of players + mobiles */
int     char_array_len;

int	num_const_obs;  /* Number of constant (not created in-game) objects */
int	numobs;		/* Number of objects in the game */
int     obj_array_len;

int     numzon;		/* Number of zones in the world		*/
int     num_const_zon;
int     zon_array_len;

int     num_const_locs;
int	numloc;		/* Number of locations */
int     loc_array_len;

long int   id_counter;  /* Next ID number to be given to a wiz-creation */
int_table  id_table;    /* Lookup table for [ID numbers -> game indexes] */

int	*verbnum;
int	ob1 = -1;
int	ob2 = -1;
int	pl1 = -1;
int	pl2 = -1;
int	pptr;		/* The parameter pointer		*/
int	prep;

int	stp;
int	verbcode;
time_t   next_event;     /* check mud.c */
time_t   last_reset;     /* Last reset time */
time_t   global_clock;
time_t	 last_healall;

#endif /* Add nothing past this line... */
