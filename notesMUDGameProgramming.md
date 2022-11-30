# MUD Game Programming

## Simple MUD

O readme do Simple MUD define os seguintes passos para compilação do programa:
* GCC: type "make libs", "make simplemud", and "make link"

### make libs
Na primeira tentativa de execução do ```make libs``` diversos erros foram levantados na biblioteca básica e na de sockets.

```
In file included from ../Libraries/BasicLib/BasicLib.h:15,
                 from ../Libraries/SocketLib/SocketLibSocket.h:14,
                 from ../Libraries/SocketLib/SocketLibSocket.cpp:8:
../Libraries/BasicLib/BasicLibRandom.h: In member function ‘double BasicLib::normal_generator<inclusive, generator>::operator()()’:
../Libraries/BasicLib/BasicLibRandom.h:132:31: error: there are no arguments to ‘log’ that depend on a template parameter, so a declaration of ‘log’ must be available [-fpermissive]
  132 |         m_rho = sqrt(-2 * log(1- m_rho2));
      |                           ^~~

../Libraries/BasicLib/BasicLibRandom.h:132:31: note: (if you use ‘-fpermissive’, G++ will accept your code, but allowing the use of an undeclared name is deprecated)
../Libraries/BasicLib/BasicLibRandom.h:140:36: error: there are no arguments to ‘cos’ that depend on a template parameter, so a declaration of ‘cos’ must be available [-fpermissive]
  140 | turn m_rho * ( m_valid ? cos(2 * pi * m_rho1) : sin( 2 * pi * m_rho1)) * m_sigma + m_mean;
      |                          ^~~

../Libraries/BasicLib/BasicLibRandom.h:140:59: error: there are no arguments to ‘sin’ that depend on a template parameter, so a declaration of ‘sin’ must be available [-fpermissive]
  140 | ? cos(2 * pi * m_rho1) : sin( 2 * pi * m_rho1)) * m_sigma + m_mean;
      |                          ^~~

../Libraries/SocketLib/SocketLibSocket.cpp: In member function ‘void SocketLib::DataSocket::Connect(SocketLib::ipaddress, SocketLib::port)’:
../Libraries/SocketLib/SocketLibSocket.cpp:141:9: error: ‘memset’ was not declared in this scope
  141 |         memset( &(m_remoteinfo.sin_zero), 0, 8 );
      |         ^~~~~~
../Libraries/SocketLib/SocketLibSocket.cpp:9:1: note: ‘memset’ is defined in header ‘<cstring>’; did you forget to ‘#include <cstring>’?
    8 | #include "SocketLibSocket.h"
  +++ |+#include <cstring>
    9 | 
../Libraries/SocketLib/SocketLibSocket.cpp: In member function ‘void SocketLib::ListeningSocket::Listen(SocketLib::port)’:
../Libraries/SocketLib/SocketLibSocket.cpp:289:9: error: ‘memset’ was not declared in this scope
  289 |         memset( &(m_localinfo.sin_zero), 0, 8 );
      |         ^~~~~~
../Libraries/SocketLib/SocketLibSocket.cpp:289:9: note: ‘memset’ is defined in header ‘<cstring>’; did you forget to ‘#include <cstring>’?
In file included from ../Libraries/BasicLib/BasicLib.h:15,
                 from ../Libraries/SocketLib/SocketSet.h:12,
                 from ../Libraries/SocketLib/SocketSet.cpp:10:
../Libraries/BasicLib/BasicLibRandom.h: In member function ‘double BasicLib::normal_generator<inclusive, generator>::operator()()’:
../Libraries/BasicLib/BasicLibRandom.h:132:31: error: there are no arguments to ‘log’ that depend on a template parameter, so a declaration of ‘log’ must be available [-fpermissive]
  132 |         m_rho = sqrt(-2 * log(1- m_rho2));
      |                           ^~~

../Libraries/BasicLib/BasicLibRandom.h:132:31: note: (if you use ‘-fpermissive’, G++ will accept your code, but allowing the use of an undeclared name is deprecated)
../Libraries/BasicLib/BasicLibRandom.h:140:36: error: there are no arguments to ‘cos’ that depend on a template parameter, so a declaration of ‘cos’ must be available [-fpermissive]
  140 | turn m_rho * ( m_valid ? cos(2 * pi * m_rho1) : sin( 2 * pi * m_rho1)) * m_sigma + m_mean;
      |                          ^~~

../Libraries/BasicLib/BasicLibRandom.h:140:59: error: there are no arguments to ‘sin’ that depend on a template parameter, so a declaration of ‘sin’ must be available [-fpermissive]
  140 | ? cos(2 * pi * m_rho1) : sin( 2 * pi * m_rho1)) * m_sigma + m_mean;
      |                          ^~~

In file included from ../Libraries/BasicLib/BasicLib.h:15,
                 from ../Libraries/SocketLib/Connection.h:15,
                 from ../Libraries/SocketLib/Telnet.h:14,
                 from ../Libraries/SocketLib/Telnet.cpp:9:
../Libraries/BasicLib/BasicLibRandom.h: In member function ‘double BasicLib::normal_generator<inclusive, generator>::operator()()’:
../Libraries/BasicLib/BasicLibRandom.h:132:31: error: there are no arguments to ‘log’ that depend on a template parameter, so a declaration of ‘log’ must be available [-fpermissive]
  132 |             m_rho = sqrt(-2 * log(1- m_rho2));
      |                               ^~~
../Libraries/BasicLib/BasicLibRandom.h:132:31: note: (if you use ‘-fpermissive’, G++ will accept your code, but allowing the use of an undeclared name is deprecated)
../Libraries/BasicLib/BasicLibRandom.h:140:36: error: there are no arguments to ‘cos’ that depend on a template parameter, so a declaration of ‘cos’ must be available [-fpermissive]
  140 |         return m_rho * ( m_valid ? cos(2 * pi * m_rho1) : sin( 2 * pi * m_rho1)) * m_sigma + m_mean;
      |                                    ^~~
../Libraries/BasicLib/BasicLibRandom.h:140:59: error: there are no arguments to ‘sin’ that depend on a template parameter, so a declaration of ‘sin’ must be available [-fpermissive]
  140 | ho * ( m_valid ? cos(2 * pi * m_rho1) : sin( 2 * pi * m_rho1)) * m_sigma + m_mean;
      |                                         ^~~

In file included from ../Libraries/SocketLib/Connection.h:17,
                 from ../Libraries/SocketLib/Telnet.h:14,
                 from ../Libraries/SocketLib/Telnet.cpp:9:
../Libraries/SocketLib/ConnectionManager.h: At global scope:
../Libraries/SocketLib/ConnectionManager.h:37:13: error: need ‘typename’ before ‘std::__cxx11::list<SocketLib::Connection<protocol> >::iterator’ because ‘std::__cxx11::list<SocketLib::Connection<protocol> >’ is a dependent scope
   37 |     typedef std::list< Connection<protocol> >::iterator clistitr;
      |             ^~~
      |             typename 
../Libraries/SocketLib/ConnectionManager.h:110:17: error: ‘clistitr’ has not been declared
  110 |     void Close( clistitr p_itr );
      |                 ^~~~~~~~
../Libraries/SocketLib/ConnectionManager.h: In destructor ‘SocketLib::ConnectionManager<protocol, defaulthandler>::~ConnectionManager()’:
../Libraries/SocketLib/ConnectionManager.h:161:5: error: ‘clistitr’ was not declared in this scope; did you mean ‘clist’?
  161 |     clistitr itr;
      |     ^~~~~~~~
      |     clist
../Libraries/SocketLib/ConnectionManager.h:163:10: error: ‘itr’ was not declared in this scope
  163 |     for( itr = m_connections.begin(); itr != m_connections.end(); ++itr )
      |          ^~~
../Libraries/SocketLib/ConnectionManager.h: At global scope:
../Libraries/SocketLib/ConnectionManager.h:213:58: error: variable or field ‘Close’ declared void
  213 | anager<protocol, defaulthandler>::Close( clistitr p_itr )
      |                                          ^~~~~~~~

../Libraries/SocketLib/ConnectionManager.h:213:58: error: ‘clistitr’ was not declared in this scope
../Libraries/SocketLib/ConnectionManager.h: In member function ‘void SocketLib::ConnectionManager<protocol, defaulthandler>::Listen()’:
../Libraries/SocketLib/ConnectionManager.h:248:9: error: ‘clistitr’ was not declared in this scope; did you mean ‘clist’?
  248 |         clistitr itr = m_connections.begin();
      |         ^~~~~~~~
      |         clist
../Libraries/SocketLib/ConnectionManager.h:249:17: error: expected ‘;’ before ‘c’
  249 |         clistitr c;
      |                 ^~
      |                 ;
../Libraries/SocketLib/ConnectionManager.h:252:16: error: ‘itr’ was not declared in this scope
  252 |         while( itr != m_connections.end() )
      |                ^~~
../Libraries/SocketLib/ConnectionManager.h:256:13: error: ‘c’ was not declared in this scope
  256 |             c = itr++;
      |             ^
../Libraries/SocketLib/ConnectionManager.h: In member function ‘void SocketLib::ConnectionManager<protocol, defaulthandler>::Send()’:
../Libraries/SocketLib/ConnectionManager.h:309:5: error: ‘clistitr’ was not declared in this scope; did you mean ‘clist’?
  309 |     clistitr itr = m_connections.begin();
      |     ^~~~~~~~
      |     clist
../Libraries/SocketLib/ConnectionManager.h:310:13: error: expected ‘;’ before ‘c’
  310 |     clistitr c;
      |             ^~
      |             ;
../Libraries/SocketLib/ConnectionManager.h:313:12: error: ‘itr’ was not declared in this scope
  313 |     while( itr != m_connections.end() )
      |            ^~~
../Libraries/SocketLib/ConnectionManager.h:317:9: error: ‘c’ was not declared in this scope
  317 |         c = itr++;
      |         ^
../Libraries/SocketLib/ConnectionManager.h: In member function ‘void SocketLib::ConnectionManager<protocol, defaulthandler>::CloseConnections()’:
../Libraries/SocketLib/ConnectionManager.h:352:5: error: ‘clistitr’ was not declared in this scope; did you mean ‘clist’?
  352 |     clistitr itr = m_connections.begin();
      |     ^~~~~~~~
      |     clist
../Libraries/SocketLib/ConnectionManager.h:353:13: error: expected ‘;’ before ‘c’
  353 |     clistitr c;
      |             ^~
      |             ;
../Libraries/SocketLib/ConnectionManager.h:355:12: error: ‘itr’ was not declared in this scope
  355 |     while( itr != m_connections.end() )
      |            ^~~
../Libraries/SocketLib/ConnectionManager.h:358:9: error: ‘c’ was not declared in this scope
  358 |         c = itr++;
      |         ^
make: *** [makefile:18: libs] Error 1
```

A correção dos erros será iniciado na biblioteca básica e depois avançará para biblioteca de sockets.

**BasicLib**

O erro ocorria no arquivo ```BasicLibRandom.h```, nas chamadas das funções de _log_, _cos_ e _sin_ . A correção foi simples, bastou incluir a biblioteca ```<math.h>```.

**SocketLib**

A indicação de erros da SocketLib apontaram para 2 arquivos: SocketLibSocket.h, que apresenta erro ao tentar executar a função ```memset```, e ConnectionManager.h, cujo o erro parece envolver o iterador ```clistitr```.

A correção do SocketLibSocket.h foi simples, bastou incluir a biblioteca ```<cstring>```.

Já a correção do ConnectionManager.h exigiu mais pesquisa, porém no fim, bastou adicionar o identificador ```typename``` antes de declaração do iterador ```std::list< Connection<protocol> >::iterator clistitr```. Alteração feita na linha 37.
A solução foi encontrada no Stackoverflow, [Nested templates with dependent scope](https://stackoverflow.com/questions/3311633/nested-templates-with-dependent-scope/3311640#3311640)

---

Após as correções o comando ```make libs``` rodou corretamente.

## Better MUD