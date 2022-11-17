#ifndef __MUD_H__
#define __MUD_H__

void push_input_handler(void (*h)(char *str));
void pop_input_handler(void);
void replace_input_handler(void (*h)(char *str));
int find_free_player_slot(void);
int find_pl_index(int fd);
void xsetup_globals(int plx);
void setup_globals(int plx);
Boolean is_host_banned(char *hostname);
Boolean is_player_banned(char *name);
Boolean privileged_user(char *name);
extern Boolean a_new_player;
void new_player(void);
void get_command(char *cmd);
void gc(char *w);
void quit_player(void);
void do_motd(char *cont);
void get_yn(void (*yn_func)(int yn), char *prompt, int assume);
#endif /* Add nothing past this line... */
