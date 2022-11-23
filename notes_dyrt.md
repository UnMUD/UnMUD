Data 29/06/2022

# Ações extras para execução do MUD

- Necessário instalar tcsh para que os

## Instalação GMDB

Rodando o comando `make install`

**1º erro:**

```/usr/bin/install -c -m 644 ./gdbm.3 /usr/local/man/man3/gdbm.3
/usr/bin/install: cannot create regular file '/usr/local/man/man3/gdbm.3': No such file or directory
```

Correção: Criação da pasta man3

**2º erro:**

```
/usr/bin/install -c -m 644 ./gdbm.info /usr/local/info/gdbm.info
/usr/bin/install: cannot create regular file '/usr/local/info/gdbm.info': No such file or directory
```

Correção: Criação da pasta info

Rodando o comando `make progs`

**3º erro:**

```
/usr/bin/ld: tndbm.o: in function 'main':
tndbm.c:(.text+0x1b5): warning: the 'gets' function is dangerous and should not be used.
/usr/bin/ld: tndbm.c:(.text+0x56): undefined reference to `dbm_open'
/usr/bin/ld: tndbm.c:(.text+0x12c): undefined reference to `dbm_firstkey'
/usr/bin/ld: tndbm.c:(.text+0x14b): undefined reference to `dbm_fetch'
/usr/bin/ld: tndbm.c:(.text+0x188): undefined reference to `dbm_close'
/usr/bin/ld: tndbm.c:(.text+0x1e3): undefined reference to `dbm_fetch'
/usr/bin/ld: tndbm.c:(.text+0x2bd): undefined reference to `dbm_store'
/usr/bin/ld: tndbm.c:(.text+0x386): undefined reference to `dbm_delete'
/usr/bin/ld: tndbm.c:(.text+0x3c5): undefined reference to `dbm_nextkey'
/usr/bin/ld: tndbm.c:(.text+0x3e0): undefined reference to `dbm_fetch'
/usr/bin/ld: tndbm.c:(.text+0x42e): undefined reference to `dbm_firstkey'
/usr/bin/ld: tndbm.c:(.text+0x44a): undefined reference to `dbm_nextkey'
```

Não solucionado

Rodando o comando `sudo make install-compat`

**4º erro:**

```
/usr/bin/install -c -m 644 ./dbm.h /usr/local/include/dbm.h
/usr/bin/install -c -m 644
/usr/bin/install: missing file operand
Try '/usr/bin/install --help' for more information.
```

Não solucionado

## Instação do Dyrt

`make depend` funcionou corretamente

Rodando o comando `make`

**1º erro:**

```
generate.c: In function ‘make_verbs’:
generate.c:104:18: error: ‘sys_errlist’ undeclared (first use in this function)
  104 |             buff,sys_errlist[errno]);
      |                  ^~~~~~~~~~~
```

```
generate.c: In function ‘make_data’:
generate.c:2293:18: error: ‘sys_errlist’ undeclared (first use in this function)
 2293 |             buff,sys_errlist[errno]);
      |                  ^~~~~~~~~~~
```

Correção: Substituição do `sys_errlist[errno]` por `strerror(errno)`. `sys_errlist` está depreciado

**2º erro:**

```
gcc -o ..//bin/generate -DDEBUG -g -O4 -I..//include/ -DLINUX -DREBOOT generate.o -lgdbm -lcrypt
Regenerating World....
..//bin/generate data ../
(*) Generating dyrt userfiles...

sh: line 1: //lib/cpp: No such file or directory

Zone not found: limbo in file ..//data/WORLD/limbo.zone.
```

Correção: Atualização da constante `#define CPP "//usr/bin/cpp -P -traditional -I ../include %s"` no arquivo `/include/machine/linux.h`

**3º erro:**

```
gcc -DDEBUG -g -O4 -I../include/ -DLINUX -DREBOOT   -c -o bootstrap.o bootstrap.c
bootstrap.c: In function ‘bootstrap’:
bootstrap.c:68:31: error: ‘sys_errlist’ undeclared (first use in this function)
   68 |       mudlog ("bopen: %s.\n", sys_errlist[errno]);
      |                               ^~~~~~~~~~~
bootstrap.c:68:31: note: each undeclared identifier is reported only once for each function it appears in
make: *** [<builtin>: bootstrap.o] Error 1
```

Correção: Substituição do `sys_errlist[errno]` por `strerror(errno)`. `sys_errlist` está depreciado

> Dei um replace all em `sys_errlist[errno]` e substitui por `strerror(errno)`

**4º erro:**

```
main.c: In function ‘get_options’:
main.c:634:15: error: invalid storage class for function ‘usage’
  634 |   static void usage (void);
      |               ^~~~~
main.c: In function ‘main_loop’:
main.c:767:15: error: invalid storage class for function ‘new_connection’
  767 |   static void new_connection (int fd);
      |               ^~~~~~~~~~~~~~
```

Correção: Aparentemente as funções `usage` e `new_connection` já estão declaradas no header (linhas 40 e 45), então eu comentei a redeclaração que estava sendo feita dentro das funções (linhas 634 e 767)

**5º erro:**

```
sflag.c:565:1: error: static declaration of ‘_wiz’ follows non-static declaration
sflag.c:522:3: note: previous implicit declaration of ‘_wiz’ with type ‘void(int,  char *)’
  522 |   _wiz (LVL_APPRENTICE, wordbuf);
      |   ^~~~
```

Correção: Adição de header para função wiz `static void _wiz (int level, char *text);` no arquivo `sflag.c`

**6º erro:**

```
/usr/bin/ld: errno: TLS definition in /usr/lib/libc.so.6 section .tbss mismatches non-TLS reference in bootstrap.o
/usr/bin/ld: /usr/lib/libc.so.6: error adding symbols: bad value
collect2: error: ld returned 1 exit status
make: *** [Makefile:158: ..//bin/aberd] Error 1
```

Correção: Remoção da linha `extern int errno;` dos arquivos `bootstrap.c`, `mail.c`, `misc.c` e `uaf.c`, e inclusão do `include <errno.h>` no arquivo `bootstrap.c`.

**7º erro:**

```
gcc -o aberd -DDEBUG -g -O4 -I..//include/ -DLINUX -DREBOOT actions.o admin.o board.o bootstrap.o bprintf.o calendar.o change.o clone.o commands.o communicate.o condition.o fight.o flags.o frob.o game.o hate.o log.o magic.o mail.o main.o misc.o mobile.o move.o mud.o objsys.o parse.o party.o rooms.o s_socket.o sendsys.o sflag.o timing.o uaf.o utils.o wizard.o wizlist.o writer.o zones.o  -lgdbm -lcrypt
/usr/bin/ld: admin.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: 		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: board.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: 		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: bootstrap.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: 	 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: bprintf.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:	 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: calendar.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:    multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: change.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: clone.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: commands.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:	 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: communicate.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: condition.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:	 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: fight.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: flags.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: frob.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: game.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: hate.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: log.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: magic.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: mail.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: main.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: mobile.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: move.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: mud.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: objsys.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: parse.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: rooms.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: sendsys.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:	 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: sflag.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: timing.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/timing.c:56:		 		 multiple definition of `next_event'; mud.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/global.h:86: first defined here
/usr/bin/ld: timing.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: uaf.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: utils.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: wizard.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: wizlist.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:	 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: writer.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: zones.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15:		 multiple definition of `a_new_player'; actions.o:/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/..//include/mud.h:15: first defined here
/usr/bin/ld: main.o: in function `main':
/home/iuri/Desktop/pibic/Pibic/dyrt_19/dyrt/src/main.c:89: warning: the `getwd' function is dangerous and should not be used.
collect2: error: ld returned 1 exit status
make: *** [Makefile:158: ..//bin/aberd] Error 1
```

Correção: Adição de `include "mud.h"` no arquivo `mud.c`; adição d `extern Boolean a_new_player` no arquivo `mud.h`; Adição d `extern time_t next_event;` no arquivo `timing.c`

**8º erro:**
O jogo não quis rodar, então segui as instruções relacionadas aos `verbs` e rodei o comando `./verbgen` na pasta `/src`.

## Rodando o Dyrt

### Erro 1

O comando `aberd &` citado no `README.FIRST.DAMMIT` não foi encontrado. Rodar o executável `aberd`, da pasta `/src` teve o seguinte _output_:

```
data_dir = "../data/".
max_players = 40.
port = 6715.
Do not clear syslog file.
Debugging is on.
Kill other mud.
Bootstrap... ID-table & ID-counter used 8192 bytes.
Bootstrap... 'players' used 602880 bytes.
Bootstrap... A:actions used 61780 bytes.
Bootstrap... Z:zones used 7856 bytes.
Bootstrap... L:locations used 923949 bytes.
Bootstrap... C:mobiles used 192716 bytes.
Bootstrap... E:levels used 0 bytes.
Bootstrap... H:hours used 0 bytes.
Bootstrap... I:intermud.conf used 0 bytes.
Bootstrap... O:objects used 279263 bytes.
Bootstrap... P:pflags used 0 bytes.
Bootstrap... V:verbs used 3200 bytes.
Bootstrap... W:wizlist ---->Can't open W file wizlist used 0 bytes.

A total of 2071644 bytes used.
Connected to port 6715 on lucas-tkp.
Segmentation fault (core dumped)
```

**Segmentation Fault**: Inicialmente estava ocorrendo na função open_gdbm, chamada a partir da função init_userfile, chamada a partir da função xmain
**Correção**: Atualização da biblioteca gdbm da versão 1.7.3 para versão 1.23

### Erro 2

```
data_dir = "../data/".
max_players = 40.
port = 6715.
Do not clear syslog file.
Debugging is on.
Kill other mud.
Ok, Will kill other mud (PID = 73891)
Bootstrap... ID-table & ID-counter used 8192 bytes.
Bootstrap... 'players' used 602880 bytes.
Bootstrap... A:actions used 61780 bytes.
Bootstrap... Z:zones used 7856 bytes.
Bootstrap... L:locations used 923949 bytes.
Bootstrap... C:mobiles used 192716 bytes.
Bootstrap... E:levels used 0 bytes.
Bootstrap... H:hours used 0 bytes.
Bootstrap... I:intermud.conf used 0 bytes.
Bootstrap... O:objects used 279263 bytes.
Bootstrap... P:pflags used 0 bytes.
Bootstrap... V:verbs used 3200 bytes.
Bootstrap... W:wizlist ---->Can't open W file wizlist used 0 bytes.

A total of 2071644 bytes used.
Connected to port 6715 on ubuntu-focal.
Segmentation fault (core dumped)
```

**Segmentation fault**: Acontecendo na função main_loop, chamada a partir da função xmain.
Após o debug foi definido q o segmentation fault está acontecendo na função consid_move, chamada a partir da função move_mobiles, do arquivo mobile.c, chamada a partir da função on_timer, do arquivo timing.c, que é chamada na função main_loop.

**Correção**: send_g_msg ; gsendf

> ## Warnings
> Dps d corrigir o erro 2
>
> ```
> gcc -DDEBUG -g -O4 -I..//include/ -DLINUX -DREBOOT   -c -o generate.o generate.c
> generate.c:26:13: warning: conflicting types for built-in function ‘log’; expected ‘double(double)’ [-Wbuiltin-declaration-mismatch]
>   26 | static void log (char t, XOBJ * O, XZON * Z, char *f,
>      |             ^~~
> generate.c:21:1: note: ‘log’ is declared in header ‘<math.h>’
>   20 | #include "generate.h"
>  +++ |+#include <math.h>
>   21 |
> generate.c: In function ‘read_loc’:
> generate.c:1350:29: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
> 1350 |               l->exits[k] = (XLOC *) get_int (F);
>      |                             ^
> generate.c: In function ‘write_loc’:
> generate.c:2200:23: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
> 2200 |                   y = (int) (L->exits[x]);
>      |                       ^
> generate.c: In function ‘xmalloc’:
> generate.c:2482:61: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
> 2482 |       fprintf (stderr, "No room to allocate bytes. [%d]\n", (int)p);
> ```
>
> Dps de corrigir o erro 4
>
> ```
> gcc -DDEBUG -g -O4 -I..//include/ -DLINUX -DREBOOT   -c -o main.o main.c
> main.c: In function ‘main’:
> main.c:83:3: warning: ‘sigsetmask’ is deprecated [-Wdeprecated-declarations]
>    83 |   sigsetmask(0);
>       |   ^~~~~~~~~~
> In file included from /usr/include/sys/param.h:28,
>                  from ..//include/kernel.h:6,
>                  from main.c:6:
> /usr/include/signal.h:176:12: note: declared here
>   176 | extern int sigsetmask (int __mask) __THROW __attribute_deprecated__;
>       |            ^~~~~~~~~~
> main.c:89:3: warning: ‘getwd’ is deprecated [-Wdeprecated-declarations]
>    89 |   getwd (path);
>       |   ^~~~~
> In file included from main.c:10:
> /usr/include/unistd.h:545:14: note: declared here
>   545 | extern char *getwd (char *__buf)
>       |              ^~~~~
> main.c: In function ‘xmain_reboot’:
> main.c:214:3: warning: implicit declaration of function ‘init_userfile’ [-Wimplicit-function-declaration]
>   214 |   init_userfile();
>       |   ^~~~~~~~~~~~~
> main.c:215:3: warning: implicit declaration of function ‘reorg_userfile’ [-Wimplicit-function-declaration]
>   215 |   reorg_userfile();
>       |   ^~~~~~~~~~~~~~
> main.c: In function ‘sysreboot’:
> main.c:1305:3: warning: implicit declaration of function ‘close_userfile’; did you mean ‘close_logfile’? [-Wimplicit-function-declaration]
>  1305 |   close_userfile();
>       |   ^~~~~~~~~~~~~~
>       |   close_logfile
> make: *** [<builtin>: main.o] Error 1
> ```
>
> Dps de corrigir o erro 5
>
> ```
> gcc -DDEBUG -g -O4 -I..//include/ -DLINUX -DREBOOT   -c -o sflag.o sflag.c
> sflag.c: In function ‘gossip_test’:
> sflag.c:469:14: warning: implicit declaration of function ‘build_voice_text’ [-Wimplicit-function-declaration]
>   469 |     buffer = build_voice_text(see_name(player, sender), "gossips", text, "'",
>       |              ^~~~~~~~~~~~~~~~
> sflag.c:469:12: warning: assignment to ‘char *’ from ‘int’ makes pointer from integer without a cast [-Wint-conversion]
>   469 |     buffer = build_voice_text(see_name(player, sender), "gossips", text, "'",
>       |            ^
> sflag.c: In function ‘_wiz’:
> sflag.c:605:20: warning: assignment to ‘char *’ from ‘int’ makes pointer from integer without a cast [-Wint-conversion]
>   605 |             buffer = build_voice_text (see_name(t, plx),
>       |                    ^
> sflag.c:616:20: warning: assignment to ‘char *’ from ‘int’ makes pointer from integer without a cast [-Wint-conversion]
>   616 |             buffer = build_voice_text (see_name(t, plx),
>       |                    ^
> sflag.c:627:20: warning: assignment to ‘char *’ from ‘int’ makes pointer from integer without a cast [-Wint-conversion]
>   627 |             buffer = build_voice_text (see_name(t,plx),
>       |                    ^
> sflag.c:638:20: warning: assignment to ‘char *’ from ‘int’ makes pointer from integer without a cast [-Wint-conversion]
>   638 |             buffer = build_voice_text (see_name(t, plx),
>       |                    ^
> gcc -DDEBUG -g -O4 -I..//include/ -DLINUX -DREBOOT   -c -o timing.o timing.c
> gcc -DDEBUG -g -O4 -I..//include/ -DLINUX -DREBOOT   -c -o uaf.o uaf.c
> gcc -DDEBUG -g -O4 -I..//include/ -DLINUX -DREBOOT   -c -o utils.o utils.c
> utils.c: In function ‘tlookup’:
> utils.c:311:47: warning: passing argument 4 of ‘glookup’ from incompatible pointer type [-Wincompatible-pointer-types]
>   311 |   return glookup (elem, strlen (elem), table, strncasecmp);
>       |                                               ^~~~~~~~~~~
>       |                                               |
>       |                                               int (*)(const char *, const char *, size_t) {aka int (*)(const char *, const char *, long unsigned int)}
> utils.c:290:16: note: expected ‘int (*)(char *, char *, int)’ but argument is of type ‘int (*)(const char *, const char *, size_t)’ {aka ‘int (*)(const char *, const char *, long unsigned int)’}
>   290 |          int (*strcmpfun) (char *s1, char *s2, int n))
>       |          ~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
> utils.c: In function ‘xmalloc’:
> utils.c:361:61: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
>   361 |       fprintf (stderr, "No room to allocate bytes. [%d]\n", (int)p);
>       |                                                             ^
> gcc -DDEBUG -g -O4 -I..//include/ -DLINUX -DREBOOT   -c -o wizard.o wizard.c
> wizard.c: In function ‘syslogcom’:
> wizard.c:90:3: warning: implicit declaration of function ‘filehandle’ [-Wimplicit-function-declaration]
>    90 |   filehandle (buff,1);
>       |   ^~~~~~~~~~
> wizard.c: At top level:
> wizard.c:109:6: warning: conflicting types for ‘filehandle’; have ‘void(char *, int)’
>   109 | void filehandle(char *filename, int mode)
>       |      ^~~~~~~~~~
> wizard.c:90:3: note: previous implicit declaration of ‘filehandle’ with type ‘void(char *, int)’
>    90 |   filehandle (buff,1);
>       |   ^~~~~~~~~~
> gcc -DDEBUG -g -O4 -I..//include/ -DLINUX -DREBOOT   -c -o wizlist.o wizlist.c
> wizlist.c: In function ‘parse_wizlevel’:
> wizlist.c:208:18: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
>   208 |           *low = (int)p;
>       |                  ^
> wizlist.c: In function ‘wizlistcom’:
> wizlist.c:261:15: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
>   261 |           p = (struct _w *) low;
>       |               ^
> ```
