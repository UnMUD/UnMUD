extern char     *pwait;
extern jmp_buf  to_main_loop;

extern FILE	*log_fl;	/* NULL if no logging	*/
extern LINE	event[];
extern LINE	inter[];
extern LINE	status[];
extern char    *Exits[];
extern char    *exits[];

extern Location	   *room_data;
extern Object	   *objects;
extern UBLOCK_REC  *ublock;

extern ZONE	*zoname;
extern char	**messages;
extern char	**verbtxt;
extern char	*txt1;
extern char	*txt2;
extern char	item1[];
extern char	item2[];
extern char	*TF[];
extern char	*OO[];
extern int	levels[LVL_WIZARD + 1];
extern char	strbuf[MAX_COM_LEN];
extern char	wordbuf[MAX_COM_LEN];

extern char     		*progname;
extern char			*data_dir;
extern char			my_hostname[MAXHOSTNAMELEN];
extern struct hostent		*my_hostent;
extern struct sockaddr_in	s_in;
extern char			**envp;
extern int			pid;

extern PLAYER_REC       *players;

extern WORLD_REC        the_world_rec;
extern WORLD_REC	*the_world;

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
** the above is not true when the player is aliased. Then real_mynum
** remember who he is, while mynum is the index of the character he
** is aliasing or polymorphing. cur_player is always pointing to
** real me and cur_ublock as well.
**
*************************
*/
extern int	   mynum;         /* current player slot-number */
extern int         real_mynum;    /* real mynum, see global.h */
extern int         quit_list;     /* List of players to quit */
extern PLAYER_REC *cur_player;    /* Current player info. */
extern UBLOCK_REC *cur_ublock;    /* Current ublock info. */

extern int	max_players;
extern int      num_const_chars;
extern int	numchars;       /* Number of players + mobiles */
extern int      char_array_len;

extern int	numobs;		/* Number of objects in the game */
extern int	num_const_obs;	/* Number of constant (not created in-game) */
extern int      obj_array_len;

extern int      numzon;		/* Number of zones in the world		*/
extern int      num_const_zon;
extern int      zon_array_len;

extern int	numloc;		/* Number of locations */
extern int      num_const_locs;
extern int      loc_array_len;

extern long int id_counter;
extern int_table  id_table;

extern int	*verbnum;
extern int	ob1;
extern int	ob2;
extern int	pl1;
extern int	pl2;
extern int	pptr;		/* The parameter pointer		*/
extern int	prep;

extern int	stp;
extern int	verbcode;
extern time_t   next_event;     /* check mud.c */
extern time_t   last_reset;
extern time_t   global_clock;

extern char *Pflags[];
extern char *Sflags[];
extern char *Mflags[];
extern char *MLevels[]; /* Male mortal level names */
extern char *FLevels[]; /* Female mortal level names */
extern char *Quests[];
