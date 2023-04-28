# Plano de Testes

|  **Data**  | **Versão** | **Descrição** | **Autor(es)** |
| ---------- | ---------- | ------------- | ------------- |
| 26/04/2023 |    1.0     | Criação do plano de testes inicial | [Iuri Severo](https://github.com/iurisevero/) |

## Introdução

Este documento tem como objetivo descrever o planejamento de teste do sistema UnMUD. O objetivo dos testes realizados é diminuir a chance de alterações e contribuições externas insiram novos erros no sistema, além de verificar e validar que o sistema está funcionando como esperado. Aqui serão abordados testes de caixa branca, em especial os testes unitários que podem ser realizados para aumentar a cobertura de linhas testadas, conforme a _Statement Coverage Technique_.

Devido as limitações técnicas dos desenvolvedores e a limitação de tempo para implementação dos testes, este documento irá se aprofundar mais nos casos de teste voltados para as bibliotecas implementadas (_BasicLib_, _SocketLib_ e _ThreadLib_), devido a complexidade encontrada para testar as funções do SimpleMUD, que envolvem conexão de rede e dependepem, em sua maioria, do relógio interno do sistema ou de entradas do usuário.

## Materiais de referência para o planejamento e execução dos testes

O principal material que pode ser consultado para melhor entendimento do sistema analisado é o livro “MUD Game Programming”, escrito por Ron Penton. Nesse livro é possível encontrar, em detalhes, todo processo de elaboração do software em questão, assim como seu objetivo.

Futuramente, a monografia e o relatório de pesquisa resultantes desse projeto também poderão ser consultados.

Atualmente não há uma base dados que pode ser usada como fonte para dados de teste, mas uma base de dados será implementada e estará disponível para acesso dos mantenedores do sistema.

## Ambiente de testes

As ferramentas utilizadas para realização dos testes unitários é a GoogleTest, um framework desenvolvido pela Google para testes em C/C++, em conjunto com a ferramenta gcovr, responsável por analisar a cobertura de testes. Para realização dos testes é possível levantar um ambiente local, utilizando as ferramentas Docker e Docker Compose.

Para levantar o ambiente basta executar o comando `docker-compose up`, estando na pasta `MUDGameProgramming`. Após o container ser criado, é necessário compilar as bibliotecas e o MUD, o que pode ser feito com o comando:

```
docker-compose exec unmud bash Scripts/compile.sh 
```

Após a compilação ser concluída, é possível compilar e executar os testes com o comando:

```
docker-compose exec unmud bash Scripts/tests.sh 
```

Caso todos os testes passem, será criado um arquivo informando a cobertura de testes nas pasta `AnalysisLogs/GCOVR`, com o nome `ano-mês-dia-hora-segundo-coverage.html`.

Também é possível executar os comandos de forma independente, basta entrar na pasta `MUDGameProgramming/SimpleMUD`, realizando esse acesso por dentro do container, e utilizar a ferramenta Make para compilar os arquivos, executar o binário resultante e gerar a cobertura de testes.

Para acessar o container basta executar:
```
docker-compose exec -it unmud /bin/bash

```

Após entrar na pasta `SimpleMUD`, para compilar os testes execute:
```
make tests
```

É possível executar o binário com o comando:

```
make run_tests
```

Por fim, para ver a cobertura dos testes basta rodar o comando a seguir, ainda na pasta `SimpleMUD`:

```
gcovr
```

Observação: A cobertura de testes será apresentada no terminal. Para gerar arquivos de saída é recomendado fazer a leitura da documentação da ferramenta `gcovr`

## Detalhamento dos testes

O detalhamento dos testes será feito com base no resultado apresentado pela ferramenta gcovr, onde cada tópico será referente a um arquivo (.cpp ou .h), que possuirá subtópicos listando as funções presentes nesses arquivos, totalizando 54 arquivos.

### BasicLib/BasicLibFiles.cpp

#### Função GetFileList
```cpp
filelist GetFileList( const std::string p_directory )
```

**Descrição:** Recebe uma `string` contendo o nome do diretório e retorna um `set` de `strings` contendo o nome dos arquivos desse diretório

**Sugestão de teste:**
* Diretório não existente
* Diretório vazio
* Diretório com poucos arquivos dentro
* Diretório com muitos arquivos dentro

### BasicLib/BasicLibFunctions.h

#### Função double_find_if
```cpp
template<class iterator, class firstpass, class secondpass> 
inline iterator double_find_if( iterator begin, 
                                iterator end, 
                                firstpass one, 
                                secondpass two )
```

**Descrição:** Executa uma pesquisa de passagem dupla em uma coleção de iteradores

**Sugestão de teste:** 
* Elemento que deve ser encontrado na primeira passagem
* Elemento que deve ser encontrado na segunda passagem
* Elemento que não deve ser encontrado
* Iteradores de início e fim inválidos

#### Função double_find_if (com qualificador)
```cpp
template<class iterator, class firstpass, class secondpass, class qualify> 
inline iterator double_find_if( iterator begin, 
                                iterator end, 
                                firstpass one, 
                                secondpass two, 
                                qualify q )
```

**Descrição:** Executa uma pesquisa de passagem dupla em uma coleção de iteradores, que, além disso, também devem se qualificar usando uma função qualificadora

**Sugestão de teste:**
* Elemento qualificado que deve ser encontrado na primeira passagem
* Elemento não qualificado que deve ser encontrado na primeira passagem
* Elemento qualificado que deve ser encontrado na segunda passagem
* Elemento não qualificado que deve ser encontrado na segunda passagem
* Elemento qualificado que não deve ser encontrado
* Elemento não qualificado que não deve ser encontrado
* Iteradores de início e fim inválidos
* Qualificador esperando um tipo de elemento diferente do enviado

#### Função operate_on_if
```cpp
template<class iterator, class function, class qualify>
inline function operate_on_if( iterator begin, 
                        iterator end, 
                        function func,
                        qualify q )
```

**Descrição:** Itera por meio de uma coleção e executa a função func em cada item que passa pelo qualificador.

**Sugestão de teste:**
* Coleção sem elementos qualificados e função func válida
* Coleção com todos elementos qualificados e função func válida
* Função func inválida, retornando um elemento diferente do tipo da coleção
* Qualificador esperando um tipo de elemento diferente do enviado

#### Função percent
```cpp
template< typename type >
inline int percent( const type& first, const type& second ) 
```

**Descrição:** Retorna a porcetagem do segundo valor em relação ao primeiro

**Sugestão de teste:**
* Ambos valores positivos
* Um valor positivo e um negativo
* Ambos valores negativos
* Primeiro valor zero
* Segundo valor zero

#### Função ASCIIToHex
```cpp
inline char ASCIIToHex( char c )
```

**Descrição:** Converte um caracter de ASCII para Hexadecimal

**Sugestão de teste:**
* Caracter numérico
* Caracter simples minúsculo
* Caracter simples maiúsculo
* Caracter especial
* Caracter nulo

### BasicLib/BasicLibLogger.h

#### Função TextDecorator::FileHeader
```cpp
static std::string FileHeader( const std::string& p_title )
```

**Descrição:** Recebe uma string contendo um título e a retorna formatada conforme o header definido

**Sugestão de teste:**
* String de título comum
* String com caracteres especiais
* String vazia

#### Função TextDecorator::SessionOpen
```cpp
static std::string SessionOpen()
```

**Descrição:** Retorna uma string com a decoração que indica a abertura de uma sessão

**Sugestão de teste:**
* Verificar retorno da função

#### Função TextDecorator::SessionClose
```cpp
static std::string SessionClose()
```

**Descrição:** Retorna uma string com a decoração que indica o fechamento de uma sessão

**Sugestão de teste:**
* Verificar retorno da função

#### Função TextDecorator::Decorate
```cpp
static std::string Decorate( const std::string& p_string )
```

**Descrição:** Concatena a decoração padrão a uma string

**Sugestão de teste:**
* String vazia
* String sem caracteres especiais
* String com caracteres especiais

#### Função Logger::Logger
```cpp
template<class decorator>
Logger( const std::string& p_filename,
        const std::string& p_logtitle,
        bool p_timestamp = false,
        bool p_datestamp = false );
```

**Descrição:** Cria um objeto do tipo Logger e abre/cria uma sessão de log em um arquivo com o nome correspondente ao passado e adiciona o título definido, caso o arquivo não exista. Define se o log terá timestamp e datestamp.

**Sugestão de teste:**
* Utilizar um nome de arquivo não existente
* Utilizar um nome de arquivo já existente
* Utilizar um nome de arquivo inválido
* Criar uma sessão com timestamp
* Criar uma sessão com datestamp
* Criar uma sessão sem timestamp
* Criar uma sessão sem datestamp


#### Função Logger::~Logger
```cpp
template<class decorator>
~Logger();
```

**Descrição:** Finaliza a sessão de log do objeto referente

**Sugestão de teste:**
* Finalizar uma sessão

#### Função Logger::Log
```cpp
template<class decorator>
void Log( const std::string& p_entry );
```

**Descrição:** Recebe uma string e adiciona ela ao arquivo de log

**Sugestão de teste:**
* String vazia
* String sem caracteres especiais
* String com caracteres especiais


### BasicLib/BasicLibRandom.h

#### Função UpperCase
```cpp
std::string UpperCase( const std::string& p_string )
```

**Descrição:** Retorna uma string com todos caracteres maiúsculos. Não altera a string original.

**Sugestão de teste:**
* String vazia
* String simples
* String com caracteres especiais

### BasicLib/BasicLibString.cpp

#### Função LowerCase
```cpp
std::string LowerCase( const std::string& p_string )
```

**Descrição:** Retorna uma string com todos caracteres minúsculos. Não altera a string original.

**Sugestão de teste:**
* String vazia
* String simples
* String com caracteres especiais

#### Função SearchAndReplace
```cpp
std::string SearchAndReplace( 
    const std::string& p_target,
    const std::string& p_search,
    const std::string& p_replace )
```

**Descrição:** Substitui todas aparições de uma string (p_search) por outra string (p_replace) em uma string alvo (p_target). Não altera a string original.

**Sugestão de teste:**
* Procurar por uma string válida e substituir por uma string válida
* Procurar por uma string que não está na string alvo
* Substituir uma string vazia
* Substituir por uma string vazia
* Substituir uma string com caracteres especiais
* Substituir por uma string com caracteres especiais
* String alvo vazia

#### Função TrimWhitespace
```cpp
std::string TrimWhitespace( const std::string& p_string )
```

**Descrição:** Remove espaços em branco do início e/ou fim de uma string

**Sugestão de teste:**
* String com espaço simples ( ) no início
* String com tabulação (\t) no início
* String com quebra de linha (\n) no início
* String com retorno de carro (\r) no início
* String com avanço de formulário (\f) no início
* String com espaço simples ( ) no final
* String com tabulação (\t) no final
* String com quebra de linha (\n) no final
* String com retorno de carro (\r) no final
* String com avanço de formulário (\f) no final
* String vazia

#### Função ParseWord
```cpp
std::string ParseWord( const std::string& p_string, int p_index )
```

**Descrição:** Retorna a palavra encontrada em uma determinada posição de uma string

**Sugestão de teste:**
* Pesquisar uma palavra em uma posição válida de uma string válida
* Pesquisar um índice maior que a quantidade de palavras na string
* Pesquisar um índice negativo
* Usar uma string vazia
* Usar uma string com múltiplos espaços em branco entre as palavras

#### Função RemoveWord
```cpp
std::string RemoveWord( const std::string& p_string, int p_index )
```

**Descrição:** Remove uma palavra encontrada em uma determinada posição de uma string

**Sugestão de teste:**
* Remover uma palavra em uma posição válida de uma string válida
* Remover um índice maior que a quantidade de palavras na string
* Remover um índice negativo
* Usar uma string vazia
* Usar uma string com múltiplos espaços em branco entre as palavras

### BasicLib/BasicLibString.h

#### Função insert
```cpp
template< typename type >
inline void insert( std::ostream& s, const type& t )  {  s << t; }
```

**Descrição:** Modelo de função (template) que insere um objeto de qualquer tipo na saída de fluxo de dados (std::ostream).

**Sugestão de teste:**
* Inserção de um número inteiro
* Inserção de um número de ponto flutuante
* Inserção de um caractere
* Inserção de um objeto personalizado
* Inserção de vários objetos diferentes e verificar se eles são inseridos corretamente e em ordem na saída

#### Função extract
```cpp
template< typename type >
inline type& extract( std::istream& s, type& t )  {  s >> t; return t; }
```

**Descrição:** Template para extrair um valor do tipo type a partir de um fluxo de entrada std::istream

**Sugestão de teste:**
* Extração de um número inteiro
* Extração de um número de ponto flutuante
* Extração de um caractere
* Extração de um objeto personalizado
* Extração de um valor de um tipo incompatível

#### Função tostring
```cpp
template< typename type >
inline std::string tostring( const type& p_type, size_t p_width = 0 )
```

**Descrição:** Recebe uma variável do tipo type e uma largura máxima de caracteres (p_width) e retorna uma string com tamanho maior ou igual ao determinado

**Sugestão de teste:**
* Conversão de um número inteiro
* Conversão de um número de ponto flutuante
* Conversão de um caractere
* Conversão de um objeto personalizado
* Largura zero
* Largura negativa
* Largura máxima


#### Função totype
```cpp
template< typename type >
inline type totype( const std::string& p_string )
```

**Descrição:** Recebe uma string e tenta convertê-la para um determinado tipo especificado pelo usuário (template)

**Sugestão de teste:**
* Conversão para um número inteiro
* Conversão para um número para ponto flutuante
* Conversão de uma string vazia
* Conversão de uma string com caracteres inválidos
* Conversão de uma string que começa com espaços em branco
* Conversão de uma string que termina com espaços em branco

### BasicLib/BasicLibTime.cpp

#### Função GetTimeMS
```cpp
sint64 GetTimeMS()
```

**Descrição:** Retorna um valor de tempo em milissegundos

**Sugestão de teste:**
* Verificar se o valor retornado é um número inteiro maior ou igual a zero
* Verificar se o valor retornado é consistente ao executar a função várias vezes em um curto espaço de tempo

#### Função GetTimeS
```cpp
sint64 GetTimeS()
```

**Descrição:** Retorna um valor de tempo em segundos

**Sugestão de teste:**
* Verificar se o valor retornado é um número inteiro maior ou igual a zero
* Verificar se o valor retornado é consistente ao executar a função várias vezes em um curto espaço de tempo

#### Função GetTimeM
```cpp
sint64 GetTimeM()
```

**Descrição:** Retorna um valor de tempo em minutos

**Sugestão de teste:**
* Verificar se o valor retornado é um número inteiro maior ou igual a zero
* Verificar se o valor retornado é consistente ao executar a função várias vezes em um curto espaço de tempo

#### Função GetTimeH
```cpp
sint64 GetTimeH()
```

**Descrição:** Retorna um valor de tempo em horas

**Sugestão de teste:**
* Verificar se o valor retornado é um número inteiro maior ou igual a zero
* Verificar se o valor retornado é consistente ao executar a função várias vezes em um curto espaço de tempo

#### Função TimeStamp
```cpp
std::string TimeStamp()
```

**Descrição:** Retorna a hora atual em formato de 24 horas (hh:mm:ss)

**Sugestão de teste:**
* Verificar se a string retornada contém exatamente 8 caracteres no formato "hh:mm:ss";
* Verificar se a string retornada corresponde a hora atual

#### Função DateStamp
```cpp
std::string DateStamp()
```

**Descrição:** Retorna a data atual em formato de YYYY:MM:DD

**Sugestão de teste:**
* Verificar se a string retornada contém exatamente 10 caracteres no formato "YYYY:MM:DD";
* Verificar se a string retornada corresponde a data atual

#### Função Timer::Timer
```cpp
Timer::Timer()
```

**Descrição:** Cria um objeto Timer para medir o tempo que passa entre dois pontos no código

**Sugestão de teste:**
* Verificar se o objeto é instanciado com os valores iniciais definidos corretamente

#### Função Timer::Reset
```cpp
void Timer::Reset( sint64 p_timepassed )
```

**Descrição:** Redefine o tempo inicial do timer

**Sugestão de teste:**
* Verificar se o tempo inicial redefinido está correto

#### Função Timer::GetMS
```cpp
sint64 Timer::GetMS()
```

**Descrição:** Retorna o tempo decorrido desde o último reset em milissegundos

**Sugestão de teste:**
* Verificar se o tempo retornado corresponde ao tempo esperado desde o último reset

#### Função Timer::GetS
```cpp
sint64 Timer::GetS()
```

**Descrição:** Retorna o tempo decorrido desde o último reset em segundos

**Sugestão de teste:**
* Verificar se o tempo retornado corresponde ao tempo esperado desde o último reset

#### Função Timer::GetM
```cpp
sint64 Timer::GetM()
```

**Descrição:** Retorna o tempo decorrido desde o último reset em minutos

**Sugestão de teste:**
* Verificar se o tempo retornado corresponde ao tempo esperado desde o último reset

#### Função Timer::GetH
```cpp
sint64 Timer::GetH()
```

**Descrição:** Retorna o tempo decorrido desde o último reset em horas

**Sugestão de teste:**
* Verificar se o tempo retornado corresponde ao tempo esperado desde o último reset

#### Função Timer::GetD
```cpp
sint64 Timer::GetD()
```

**Descrição:** Retorna o tempo decorrido desde o último reset em dias

**Sugestão de teste:**
* Verificar se o tempo retornado corresponde ao tempo esperado desde o último reset

#### Função Timer::GetY
```cpp
sint64 Timer::GetY()
```

**Descrição:** Retorna o tempo decorrido desde o último reset em anos

**Sugestão de teste:**
* Verificar se o tempo retornado corresponde ao tempo esperado desde o último reset

#### Função Timer::GetString
```cpp
std::string Timer::GetString()
```

**Descrição:** Retorna uma string com o tempo decorrido desde o último reset, no formato "Y years, D days, H hours, M minutes"

**Sugestão de teste:**
* Verificar se a string retornada corresponde ao valor esperado

### BasicLib/BasicLibTime.h

Não há funções para testar

### SocketLib/Connection.h
### SocketLib/ConnectionHandler.h
### SocketLib/ConnectionManager.h
### SocketLib/ListeningManager.h
### SocketLib/SocketLibErrors.cpp
### SocketLib/SocketLibSocket.cpp
### SocketLib/SocketLibSocket.h
### SocketLib/SocketLibSystem.cpp
### SocketLib/SocketSet.cpp
### SocketLib/SocketSet.h
### SocketLib/Telnet.cpp
### SocketLib/Telnet.h

### ThreadLib/ThreadLibFunctions.cpp
### ThreadLib/ThreadLibFunctions.h

### SimpleMUD.cpp
### SimpleMUD/Attributes.h
### SimpleMUD/DatabasePointer.cpp
### SimpleMUD/DatabasePointer.h
### SimpleMUD/Enemy.cpp
### SimpleMUD/Enemy.h
### SimpleMUD/EnemyDatabase.cpp
### SimpleMUD/EnemyDatabase.h
### SimpleMUD/Entity.h
### SimpleMUD/EntityDatabase.h
### SimpleMUD/Game.cpp
### SimpleMUD/Game.h
### SimpleMUD/GameLoop.cpp
### SimpleMUD/GameLoop.h
### SimpleMUD/Item.h
### SimpleMUD/ItemDatabase.cpp
### SimpleMUD/ItemDatabase.h
### SimpleMUD/Logon.cpp
### SimpleMUD/Logon.h
### SimpleMUD/Player.cpp
### SimpleMUD/Player.h
### SimpleMUD/PlayerDatabase.cpp
### SimpleMUD/PlayerDatabase.h
### SimpleMUD/Room.cpp
### SimpleMUD/Room.h
### SimpleMUD/RoomDatabase.cpp
### SimpleMUD/RoomDatabase.h
### SimpleMUD/Store.h
### SimpleMUD/StoreDatabase.cpp
### SimpleMUD/StoreDatabase.h
### SimpleMUD/Train.cpp
### SimpleMUD/Train.h


## Referências

* Everett, Gerald D., and Raymond McLeod Jr. "Software testing." Testing Across the Entire (2007), disponível em <http://worldcolleges.info/sites/default/files/software-testing-testing-across-the-entire-software-development-life-cycle.9780471793717.28214.pdf>, acessado em Abril de 2023.