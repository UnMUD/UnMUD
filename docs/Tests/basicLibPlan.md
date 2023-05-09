# Plano de Testes

|  **Data**  | **Versão** | **Descrição** | **Autor(es)** |
| ---------- | ---------- | ------------- | ------------- |
| 27/04/2023 |    1.0     | Criação do plano de testes da BasicLib | [Iuri Severo](https://github.com/iurisevero/) |

## Detalhamento dos testes da BasicLib

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

#### Função percent
```cpp
template< typename type >
inline int percent( const type& first, const type& second ) 
```

**Descrição:** Retorna a porcetagem do primeiro valor em relação ao segundo

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

### BasicLib/BasicLibString.cpp

#### Função UpperCase
```cpp
std::string UpperCase( const std::string& p_string )
```

**Descrição:** Retorna uma string com todos caracteres maiúsculos. Não altera a string original.

**Sugestão de teste:**
* String vazia
* String simples
* String com caracteres especiais

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

**Descrição:** Recebe uma variável do tipo type e uma largura mínima de caracteres (p_width) e retorna uma string com tamanho maior ou igual ao determinado

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