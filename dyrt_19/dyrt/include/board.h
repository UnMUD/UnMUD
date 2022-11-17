#ifndef __BOARD_H__
#define __BOARD_H__

void boot_bboards();
int isa_board(int ob);
void lat_board(int ob);
void do_read();
void do_erase();
void write_board();

#endif /* Add nothing past this line... */
