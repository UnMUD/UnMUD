Data 29/06/2022

# Ações extras para execução do MUD

## Instalação GMDB

Rodando o comando ```make install```

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

Rodando o comando ```make progs```

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

Rodando o comando ```sudo make install-compat```

**4º erro:** 
```
/usr/bin/install -c -m 644 ./dbm.h /usr/local/include/dbm.h
/usr/bin/install -c -m 644 
/usr/bin/install: missing file operand
Try '/usr/bin/install --help' for more information.
```

## Instação do Dyrt

```make depend``` funcionou corretamente

Rodando o comando ```make```

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

Correção: Substituição do ```sys_errlist[errno]``` por ```strerror(errno)```. ```sys_errlist``` está depreciado

**2º erro:** 
```
gcc -o ..//bin/generate -DDEBUG -g -O4 -I..//include/ -DLINUX -DREBOOT generate.o -lgdbm -lcrypt
Regenerating World....
..//bin/generate data ../
(*) Generating dyrt userfiles...

sh: line 1: //lib/cpp: No such file or directory                               

Zone not found: limbo in file ..//data/WORLD/limbo.zone.
```

Não solucionado

> Warnings 
> ```
>gcc -DDEBUG -g -O4 -I..//include/ -DLINUX -DREBOOT   -c -o generate.o generate.c
>generate.c:26:13: warning: conflicting types for built-in function ‘log’; expected ‘double(double)’ [-Wbuiltin-declaration-mismatch]
>   26 | static void log (char t, XOBJ * O, XZON * Z, char *f,
>      |             ^~~
>generate.c:21:1: note: ‘log’ is declared in header ‘<math.h>’
>   20 | #include "generate.h"
>  +++ |+#include <math.h>
>   21 | 
>generate.c: In function ‘read_loc’:
>generate.c:1350:29: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
> 1350 |               l->exits[k] = (XLOC *) get_int (F);
>      |                             ^
>generate.c: In function ‘write_loc’:
>generate.c:2200:23: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
> 2200 |                   y = (int) (L->exits[x]);
>      |                       ^
>generate.c: In function ‘xmalloc’:
>generate.c:2482:61: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
> 2482 |       fprintf (stderr, "No room to allocate bytes. [%d]\n", (int)p);
> ```