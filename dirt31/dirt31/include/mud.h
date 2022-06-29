void    push_input_handler(void (*h)(char *str));
void    pop_input_handler(void);
void    replace_input_handler(void (*h)(char *str));
int     find_free_player_slot(void);
int     find_pl_index(int fd);
void    xsetup_globals(int plx);
void    setup_globals(int plx);
Boolean is_host_banned(char *hostname);
Boolean is_player_banned(char *name);
void    new_player(void);
void    get_command(char *cmd);
void    quit_player(void);
void    do_motd(char *cont);

