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

### make simplemud

Assim como o ```make libs```, o comando apresentou erros ao ser executado. Alguns semelhantes aos já resolvidos, o que torna a correção mais rápida.

```
SimpleMUD/EntityDatabase.h:47:25: error: need ‘typename’ before ‘SimpleMUD::EntityDatabase<datatype>::container::iterator’ because ‘SimpleMUD::EntityDatabase<datatype>::container’ is a dependent scope
   47 |         iterator( const container::iterator& p_itr ) // copy constructor
      |                         ^~~~~~~~~
      |                         typename 
```

O retorno completo da compilação pode ser visto abaixo:

```
g++ -I../Libraries *.cpp -c;
In file included from SimpleMUD/ItemDatabase.h:15,
                 from SimpleMUD.cpp:12:
SimpleMUD/EntityDatabase.h:47:25: error: need ‘typename’ before ‘SimpleMUD::EntityDatabase<datatype>::container::iterator’ because ‘SimpleMUD::EntityDatabase<datatype>::container’ is a dependent scope
   47 |         iterator( const container::iterator& p_itr ) // copy constructor
      |                         ^~~~~~~~~
      |                         typename 
SimpleMUD/EntityDatabase.h: In constructor ‘SimpleMUD::EntityDatabase<datatype>::iterator::iterator(const int&)’:
SimpleMUD/EntityDatabase.h:49:34: error: ‘itr’ was not declared in this scope
   49 |             container::iterator& itr = *this; // also needed because VC6 sucks
      |                                  ^~~
SimpleMUD/EntityDatabase.h: In member function ‘datatype& SimpleMUD::EntityDatabase<datatype>::iterator::operator*()’:
SimpleMUD/EntityDatabase.h:59:34: error: ‘itr’ was not declared in this scope
   59 |             container::iterator& itr = *this; // also needed because VC6 sucks
      |                                  ^~~
SimpleMUD/EntityDatabase.h: In member function ‘datatype* SimpleMUD::EntityDatabase<datatype>::iterator::operator->()’:
SimpleMUD/EntityDatabase.h:69:34: error: ‘itr’ was not declared in this scope
   69 |             container::iterator& itr = *this; // also needed because VC6 sucks
      |                                  ^~~
SimpleMUD/EntityDatabase.h: In static member function ‘static SimpleMUD::entityid SimpleMUD::EntityDatabase<datatype>::FindOpenID()’:
SimpleMUD/EntityDatabase.h:159:9: error: need ‘typename’ before ‘std::map<unsigned int, datatype>::iterator’ because ‘std::map<unsigned int, datatype>’ is a dependent scope
  159 |         std::map<entityid,datatype>::iterator itr = m_map.begin();
      |         ^~~
SimpleMUD/EntityDatabase.h:159:46: error: expected ‘;’ before ‘itr’
  159 |         std::map<entityid,datatype>::iterator itr = m_map.begin();
      |                                              ^~~~
      |                                              ;
SimpleMUD/EntityDatabase.h:163:17: error: ‘itr’ was not declared in this scope
  163 |             if( itr->first != previous + 1 )
      |                 ^~~
SimpleMUD/EntityDatabase.h:167:15: error: ‘itr’ was not declared in this scope
  167 |             ++itr;
      |               ^~~
SimpleMUD/EntityDatabase.h: At global scope:
SimpleMUD/EntityDatabase.h:186:13: error: need ‘typename’ before ‘std::vector<datatype>::iterator’ because ‘std::vector<datatype>’ is a dependent scope
  186 |     typedef std::vector<datatype>::iterator iterator;
      |             ^~~
      |             typename 
SimpleMUD/EntityDatabase.h:188:19: error: ‘iterator’ does not name a type
  188 |     inline static iterator begin()  { return m_vector.begin() + 1; }
      |                   ^~~~~~~~
SimpleMUD/EntityDatabase.h:189:19: error: ‘iterator’ does not name a type
  189 |     inline static iterator end()    { return m_vector.end(); }
      |                   ^~~~~~~~
SimpleMUD/EntityDatabase.h: In instantiation of ‘static SimpleMUD::EntityDatabase<datatype>::iterator SimpleMUD::EntityDatabase<datatype>::begin() [with datatype = SimpleMUD::Player]’:
SimpleMUD/PlayerDatabase.h:49:19:   required from here
SimpleMUD/EntityDatabase.h:80:16: error: no matching function for call to ‘SimpleMUD::EntityDatabase<SimpleMUD::Player>::iterator::iterator(std::map<unsigned int, SimpleMUD::Player, std::less<unsigned int>, std::allocator<std::pair<const unsigned int, SimpleMUD::Player> > >::iterator)’
   80 |         return iterator( m_map.begin() );
      |                ^~~~~~~~~~~~~~~~~~~~~~~~~
SimpleMUD/EntityDatabase.h:47:9: note: candidate: ‘SimpleMUD::EntityDatabase<datatype>::iterator::iterator(const int&) [with datatype = SimpleMUD::Player]’
   47 |         iterator( const container::iterator& p_itr ) // copy constructor
      |         ^~~~~~~~
SimpleMUD/EntityDatabase.h:47:46: note:   no known conversion for argument 1 from ‘std::map<unsigned int, SimpleMUD::Player, std::less<unsigned int>, std::allocator<std::pair<const unsigned int, SimpleMUD::Player> > >::iterator’ {aka ‘std::_Rb_tree_iterator<std::pair<const unsigned int, SimpleMUD::Player> >’} to ‘const int&’
   47 |         iterator( const container::iterator& p_itr ) // copy constructor
      |                   ~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~
SimpleMUD/EntityDatabase.h:46:9: note: candidate: ‘SimpleMUD::EntityDatabase<datatype>::iterator::iterator() [with datatype = SimpleMUD::Player]’
   46 |         iterator() {};  // default constructor
      |         ^~~~~~~~
SimpleMUD/EntityDatabase.h:46:9: note:   candidate expects 0 arguments, 1 provided
SimpleMUD/EntityDatabase.h:38:11: note: candidate: ‘constexpr SimpleMUD::EntityDatabase<SimpleMUD::Player>::iterator::iterator(const SimpleMUD::EntityDatabase<SimpleMUD::Player>::iterator&)’
   38 |     class iterator : public container::iterator
      |           ^~~~~~~~
SimpleMUD/EntityDatabase.h:38:11: note:   no known conversion for argument 1 from ‘std::map<unsigned int, SimpleMUD::Player, std::less<unsigned int>, std::allocator<std::pair<const unsigned int, SimpleMUD::Player> > >::iterator’ {aka ‘std::_Rb_tree_iterator<std::pair<const unsigned int, SimpleMUD::Player> >’} to ‘const SimpleMUD::EntityDatabase<SimpleMUD::Player>::iterator&’
SimpleMUD/EntityDatabase.h:38:11: note: candidate: ‘constexpr SimpleMUD::EntityDatabase<SimpleMUD::Player>::iterator::iterator(SimpleMUD::EntityDatabase<SimpleMUD::Player>::iterator&&)’
SimpleMUD/EntityDatabase.h:38:11: note:   no known conversion for argument 1 from ‘std::map<unsigned int, SimpleMUD::Player, std::less<unsigned int>, std::allocator<std::pair<const unsigned int, SimpleMUD::Player> > >::iterator’ {aka ‘std::_Rb_tree_iterator<std::pair<const unsigned int, SimpleMUD::Player> >’} to ‘SimpleMUD::EntityDatabase<SimpleMUD::Player>::iterator&&’
SimpleMUD/EntityDatabase.h: In instantiation of ‘static SimpleMUD::EntityDatabase<datatype>::iterator SimpleMUD::EntityDatabase<datatype>::end() [with datatype = SimpleMUD::Player]’:
SimpleMUD/PlayerDatabase.h:49:26:   required from here
SimpleMUD/EntityDatabase.h:89:16: error: no matching function for call to ‘SimpleMUD::EntityDatabase<SimpleMUD::Player>::iterator::iterator(std::map<unsigned int, SimpleMUD::Player, std::less<unsigned int>, std::allocator<std::pair<const unsigned int, SimpleMUD::Player> > >::iterator)’
   89 |         return iterator( m_map.end() );
      |                ^~~~~~~~~~~~~~~~~~~~~~~
SimpleMUD/EntityDatabase.h:47:9: note: candidate: ‘SimpleMUD::EntityDatabase<datatype>::iterator::iterator(const int&) [with datatype = SimpleMUD::Player]’
   47 |         iterator( const container::iterator& p_itr ) // copy constructor
      |         ^~~~~~~~
SimpleMUD/EntityDatabase.h:47:46: note:   no known conversion for argument 1 from ‘std::map<unsigned int, SimpleMUD::Player, std::less<unsigned int>, std::allocator<std::pair<const unsigned int, SimpleMUD::Player> > >::iterator’ {aka ‘std::_Rb_tree_iterator<std::pair<const unsigned int, SimpleMUD::Player> >’} to ‘const int&’
   47 |         iterator( const container::iterator& p_itr ) // copy constructor
      |                   ~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~
SimpleMUD/EntityDatabase.h:46:9: note: candidate: ‘SimpleMUD::EntityDatabase<datatype>::iterator::iterator() [with datatype = SimpleMUD::Player]’
   46 |         iterator() {};  // default constructor
      |         ^~~~~~~~
SimpleMUD/EntityDatabase.h:46:9: note:   candidate expects 0 arguments, 1 provided
SimpleMUD/EntityDatabase.h:38:11: note: candidate: ‘constexpr SimpleMUD::EntityDatabase<SimpleMUD::Player>::iterator::iterator(const SimpleMUD::EntityDatabase<SimpleMUD::Player>::iterator&)’
   38 |     class iterator : public container::iterator
      |           ^~~~~~~~
SimpleMUD/EntityDatabase.h:38:11: note:   no known conversion for argument 1 from ‘std::map<unsigned int, SimpleMUD::Player, std::less<unsigned int>, std::allocator<std::pair<const unsigned int, SimpleMUD::Player> > >::iterator’ {aka ‘std::_Rb_tree_iterator<std::pair<const unsigned int, SimpleMUD::Player> >’} to ‘const SimpleMUD::EntityDatabase<SimpleMUD::Player>::iterator&’
SimpleMUD/EntityDatabase.h:38:11: note: candidate: ‘constexpr SimpleMUD::EntityDatabase<SimpleMUD::Player>::iterator::iterator(SimpleMUD::EntityDatabase<SimpleMUD::Player>::iterator&&)’
SimpleMUD/EntityDatabase.h:38:11: note:   no known conversion for argument 1 from ‘std::map<unsigned int, SimpleMUD::Player, std::less<unsigned int>, std::allocator<std::pair<const unsigned int, SimpleMUD::Player> > >::iterator’ {aka ‘std::_Rb_tree_iterator<std::pair<const unsigned int, SimpleMUD::Player> >’} to ‘SimpleMUD::EntityDatabase<SimpleMUD::Player>::iterator&&’
SimpleMUD/EntityDatabase.h: In instantiation of ‘datatype& SimpleMUD::EntityDatabase<datatype>::iterator::operator*() [with datatype = SimpleMUD::Player]’:
../Libraries/BasicLib/BasicLibFunctions.h:45:16:   required from ‘iterator BasicLib::double_find_if(iterator, iterator, firstpass, secondpass, qualify) [with iterator = SimpleMUD::EntityDatabase<SimpleMUD::Player>::iterator; firstpass = SimpleMUD::matchentityfull; secondpass = SimpleMUD::matchentity; qualify = SimpleMUD::playeractive]’
SimpleMUD/PlayerDatabase.h:50:51:   required from here
SimpleMUD/EntityDatabase.h:59:32: error: dependent-name ‘SimpleMUD::EntityDatabase<datatype>::container::iterator’ is parsed as a non-type, but instantiation yields a type
   59 |             container::iterator& itr = *this; // also needed because VC6 sucks
SimpleMUD/EntityDatabase.h:59:32: note: say ‘typename SimpleMUD::EntityDatabase<datatype>::container::iterator’ if a type is meant
make: *** [makefile:23: simplemud] Error 1
```

**EntityDatabase.h**

A primeira alteração para corrigir os erros foi definição do tipo ```containeritr``` que faz acesso ao iterador do tipo ```container``` definido no código original.

```
typedef typename std::map<entityid, datatype>::iterator containeritr;
```

Após isso, todos acessos à ```container::iterator``` foram atualizados para o novo tipo.

A segunda correção foi na linha 160, onde a variável ```ìtr``` estava sendo declarada utilizando a escrita direta do container ```std::map<entityid,datatype>::iterator```. A declaração foi atualizada para ```containeritr itr```.

Por fim, foi adicionado o identificador ```typename``` antes de declaração do iterador ```std::vector<datatype>::iterator iterator;``` na linha 187.

**‘template<>’ syntax**

Após as correções realizadas na biblioteca ```EntityDatabase.h```, novos erros surgiram.

```
g++ -I../Libraries *.cpp -c;
g++ -I../Libraries ./SimpleMUD/*.cpp -c;
./SimpleMUD/EnemyDatabase.cpp:25:28: error: specializing member ‘SimpleMUD::EntityDatabaseVector<SimpleMUD::EnemyTemplate>::m_vector’ requires ‘template<>’ syntax
   25 | std::vector<EnemyTemplate> EntityDatabaseVector<EnemyTemplate>::m_vector;
      |                            ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
./SimpleMUD/EnemyDatabase.cpp:28:27: error: specializing member ‘SimpleMUD::EntityDatabase<SimpleMUD::Enemy>::m_map’ requires ‘template<>’ syntax
   28 | std::map<entityid, Enemy> EntityDatabase<Enemy>::m_map;
      |                           ^~~~~~~~~~~~~~~~~~~~~
./SimpleMUD/ItemDatabase.cpp:20:26: error: specializing member ‘SimpleMUD::EntityDatabase<SimpleMUD::Item>::m_map’ requires ‘template<>’ syntax
   20 | std::map<entityid, Item> EntityDatabase<Item>::m_map;
      |                          ^~~~~~~~~~~~~~~~~~~~
./SimpleMUD/PlayerDatabase.cpp:24:28: error: specializing member ‘SimpleMUD::EntityDatabase<SimpleMUD::Player>::m_map’ requires ‘template<>’ syntax
   24 | std::map<entityid, Player> EntityDatabase<Player>::m_map;
      |                            ^~~~~~~~~~~~~~~~~~~~~~
./SimpleMUD/RoomDatabase.cpp:25:19: error: specializing member ‘SimpleMUD::EntityDatabaseVector<SimpleMUD::Room>::m_vector’ requires ‘template<>’ syntax
   25 | std::vector<Room> EntityDatabaseVector<Room>::m_vector;
      |                   ^~~~~~~~~~~~~~~~~~~~~~~~~~
./SimpleMUD/StoreDatabase.cpp:20:27: error: specializing member ‘SimpleMUD::EntityDatabase<SimpleMUD::Store>::m_map’ requires ‘template<>’ syntax
   20 | std::map<entityid, Store> EntityDatabase<Store>::m_map;
      |                           ^~~~~~~~~~~~~~~~~~~~~
make: *** [makefile:24: simplemud] Error 1
```

O erros foram solucionados com a adição de ```template< class T >``` antes da declaração das variáveis apontadas pelo compilador, sendo T a classe referente a cada variável. Solução encontrada na documentação da IBM [Static data members and templates (C++ only)](https://www.ibm.com/docs/en/zos/2.4.0?topic=only-static-data-members-templates-c)

A primeira solução tentada foi com a adição de ```template<>```, sem especificação de classe, o que resultou em problemas de linkagem.

### make links

Não ocorreram erros na linkagem.

## Better MUD