# Plano de Testes

|  **Data**  | **Versão** | **Descrição** | **Autor(es)** |
| ---------- | ---------- | ------------- | ------------- |
| 28/04/2023 |    1.0     | Criação do plano de testes da SocketLib | [Iuri Severo](https://github.com/iurisevero/) |

## Detalhamento dos testes da SocketLib

### SocketLib/Connection.h

#### Classe Connection
```cpp
template< class protocol >
class Connection : public DataSocket
```

#### Função Connection
```cpp
template< class protocol >
Connection<protocol>::Connection()
```

**Descrição:** Construtor do objeto do tipo Connection

**Sugestão de teste:**
* Verificar se o objeto está sendo criado com os valores inicias corretos

#### Função Connection( DataSocket& p_socket )
```cpp
template< class protocol >
Connection<protocol>::Connection( DataSocket& p_socket )
```

**Descrição:** Construtor do objeto do tipo Connection que recebe um Datasocket para usar como "template" da conexão

**Sugestão de teste:**
* Verificar se o objeto criado utiliza o Datasocket passado como referência
* Criar uma conexão utilizando um Datasocket inválido

#### Função Initialize
```cpp
template< class protocol >
void Connection<protocol>::Initialize()
```

**Descrição:** Inicializa o objeto instanciado

**Sugestão de teste:**
* Verificar se os valores iniciais da conexão estão corretos

#### Função GetLastSendTime
```cpp
template< class protocol >
BasicLib::sint64 Connection<protocol>::GetLastSendTime() const
```

**Descrição:** Obtém a quantidade de tempo que passou, em segundos, desde a última vez que os dados foram enviados com sucesso, se ainda houver dados na fila ou 0 se não houver dados na fila

**Sugestão de teste:**
* Verificar retorno sem dados na fila
* Verificar retorno com dados na fila

#### Função BufferData
```cpp
template< class protocol >
void Connection<protocol>::BufferData( const char* p_buffer, int p_size )
```

**Descrição:** Adiciona dados na fila de envio

**Sugestão de teste:**
* Adicionar dado inválido na fila
* Verificar se o dado foi adicionado corretamente na fila
* Adicionar um dado grande na fila

#### Função SendBuffer
```cpp
template< class protocol >
void Connection<protocol>::SendBuffer()
```

**Descrição:** Envia o conteúdo do buffer de envio

**Sugestão de teste:**
* Verificar se a fila ficou vazia após o envio
* Verificar se os dados foram enviados corretamente
* Verificar se a ordem dos dados enviados estava correta

#### Função Receive
```cpp
template< class protocol >
void Connection<protocol>::Receive()
```

**Descrição:** Recebe um buffer de dados e retorna o número de bytes recebidos. Um valor de retorno de 0 é válido.

**Sugestão de teste:**
* Receber um buffer vazio
* Receber um buffer preenchido e verificar se o retorno da função é equivalente ao tamanho do buffer

#### Função GetLastReceiveTime
```cpp
inline BasicLib::sint64 GetLastReceiveTime() const
```

**Descrição:** Obtém a quantidade de tempo em segundos desde que os dados foram recebidos pela última vez

**Sugestão de teste:**
* Verificar se o valor retornado corresponde ao tempo esperado

#### Função Close
```cpp
inline void Close()
```

**Descrição:** Define um booleano informando à conexão que ela deve ser fechada quando o gerenciador de conexões chegar a ela

**Sugestão de teste:**
* Verificar valor do booleano após chamada da função

#### Função CloseSocket
```cpp
inline void CloseSocket()
```

**Descrição:** Fecha fisicamente o socket subjacente

**Sugestão de teste:**
* Tentar fechar um socket nulo
* Tentar acessar o scoket após fechá-lo

#### Função GetDataRate
```cpp
inline int GetDataRate() const
```

**Descrição:** Obtém o datarate de recebimento para o socket, em bytes por segundo, calculado no intervalo anterior

**Sugestão de teste:**
* Verficar se o tempo retornado corresponde ao esperado

#### Função GetCurrentDataRate
```cpp
inline int GetCurrentDataRate() const
```

**Descrição:** Obtém a taxa de dados de recebimento atual para o socket, em bytes por segundo

**Sugestão de teste:**
* Verficar se o tempo retornado corresponde ao esperado

#### Função GetBufferedBytes
```cpp
inline int GetBufferedBytes() const
```

**Descrição:** Obtém a quantidade de bytes dentro do buffer de envio

**Sugestão de teste:**
* Verficar se a quantidade de bytes retornada corresponde ao esperado

#### Função SwitchHandler
```cpp
inline void SwitchHandler( typename protocol::handler* p_handler )
```

**Descrição:** Altera o handler atual, que está no topo da pilha, pelo novo handler passado como parâmetro

**Sugestão de teste:**
* Testar a função com a pilha de handlers vazia
* Testar a função passando um handler inválido
* Testar a função passando um handler válido e com a pilha de handlers cheia

#### Função AddHandler
```cpp
inline void AddHandler( typename protocol::handler* p_handler )
```

**Descrição:** Sai do estado do handler atual, entra no estado definido pelo novo handler e o adiciona na pilha

**Sugestão de teste:**
* Adicionar um handler inválido
* Verificar estado da conexão após a adição de um handler válido

#### Função RemoveHandler
```cpp
inline void RemoveHandler()
```

**Descrição:**  Sai do estado atual e o deleta e, em seguida, remove o handler da pilha. Entra no estado definido pelo handler anterior, caso exista

**Sugestão de teste:**
* Testar a função com a pilha de handlers vazia
* Testar a função com a pilha de handlers contendo mais de 2 handlers
* Testar a função com apenas um handler na pilha

#### Função Handler
```cpp
inline typename protocol::handler* Handler()
```

**Descrição:** Retorna o handler que está no topo da pilha

**Sugestão de teste:**
* Testar a função com a pilha de handlers vazia
* Testar a função com a pilha de handlers cheia

#### Função ClearHandlers
```cpp
void ClearHandlers()
```

**Descrição:** Sai do estado definido pelo handler atual e apaga todos handler que estiverem na pilha

**Sugestão de teste:**
* Testar a função com a pilha de handlers vazia
* Testar a função com a pilha de handlers cheia e verificar se ela fica vazia após a execução

### SocketLib/ConnectionHandler.h

#### Classe ConnectionHandler
```cpp
template<typename protocol, typename command>
class ConnectionHandler
```

**Descrição:** A classe ConnectionHandler é um modelo genérico para manipular ações em uma conexão de rede, onde protocol e command são tipos genéricos que devem ser especificados ao instanciar a classe. A classe tem quatro funções virtuais puras que são chamadas para manipular eventos de conexão, e uma variável m_connection que armazena um ponteiro para a conexão.

Os eventos de conexão que são tratados pela classe ConnectionHandler são:

* Handle: manipula um comando recebido do cliente.
* Enter: notifica quando a conexão está entrando em um estado.
* Leave: notifica quando a conexão está deixando um estado.
* Hungup: notifica quando a conexão foi desligada inesperadamente.
* Flooded: notifica quando a conexão está sendo encerrada devido ao envio excessivo de tráfego.
* Sugestões de testes unitários incluem:


**Sugestão de teste:**
* Testar a instanciação da classe ConnectionHandler com diferentes tipos de protocolo e comando para garantir que a classe possa ser usada com sucesso com qualquer tipo
* Testar a função Handle passando diferentes tipos de comandos para garantir que a função esteja manipulando corretamente cada um deles.
* Testar cada uma das funções virtuais puras (Enter, Leave, Hungup e Flooded) para garantir que elas estejam sendo chamadas corretamente nos momentos apropriados. Isso pode ser feito usando um mock da conexão que chama as funções da classe ConnectionHandler quando eventos de conexão ocorrem.

### SocketLib/ConnectionManager.h

#### Classe ConnectionManager
```cpp
template<typename protocol, typename defaulthandler>
class ConnectionManager
```

**Descrição:** Gerencia conexões, todas identificadas por um ID. Ela é responsável por lidar com a adição de novas conexões, contagem de conexões, envio e recebimento de dados e fechamento de conexões

**Sugestão de teste:**
* Testar a criação de uma nova conexão: Criar um objeto ConnectionManager e adicionar uma nova conexão a ele. Verificar se a contagem total de conexões aumentou em 1 e se a nova conexão foi adicionada com sucesso.
* Testar o envio de dados: Adicionar algumas conexões ao ConnectionManager e enviar dados por meio delas. Verificar se os dados foram enviados com sucesso e se a quantidade de bytes enviados está dentro do limite permitido pelo ConnectionManager.
* Testar o recebimento de dados: Enviar dados para uma conexão gerenciada pelo ConnectionManager e verificar se os dados foram recebidos corretamente.
* Testar o fechamento de conexões: Fechar uma ou mais conexões gerenciadas pelo ConnectionManager e verificar se a contagem total de conexões diminuiu em 1 para cada conexão fechada.
* Testar a adição de muitas conexões: Tentar adicionar mais conexões do que o ConnectionManager pode lidar e verificar se o ConnectionManager negou a conexão.
* Testar a remoção de muitas conexões: Adicionar várias conexões ao ConnectionManager e remover todas elas de uma só vez. Verificar se a contagem total de conexões é zero após a remoção.

### SocketLib/ListeningManager.h

#### Função 
```cpp

```

**Descrição:** Gerencia até 64 sockets de escuta, monitorando todos eles usando `select()`. Sempre que uma nova conexão é aceita, ela é enviada para o gerenciador de conexões associado.

Os métodos públicos são:

* `void AddPort (port p_port)`: Adiciona uma porta ao gerenciador.
* `void SetConnectionManager (ConnectionManager<protocol, defaulthandler>* p_manager)`: Diz ao gerenciador qual gerenciador de conexões usar sempre que uma nova conexão é aceita.
* `void Listen()`: Aguarda novas conexões em qualquer uma das portas abertas.

Os métodos protegidos são:

* `std::vector<ListeningSocket> m_sockets`: Um vetor que contém todos os sockets que o gerenciador está ouvindo.
* `SocketSet m_set`: Um conjunto de conexões que será usado para polling.
* `ConnectionManager<protocol, defaulthandler>* m_manager`: Um ponteiro para o gerenciador de soquetes que gerenciará os sockets sempre que um novo soquete for aceito.

**Sugestão de teste:**
* Verifique se a função `AddPort()` adiciona corretamente uma nova porta ao gerenciador.
* Verifique se a função `SetConnectionManager()` define corretamente o gerenciador de conexões.
* Verifique se o `Listen()` é capaz de aceitar novas conexões corretamente.

### SocketLib/SocketLibErrors.cpp

#### Função GetError  
```cpp
Error GetError( bool p_errno )
```

**Descrição:** Atua como um wrapper simples para recuperar erros de biblioteca de soquete de errno ou h_errno

**Sugestão de teste:**
* Verifique se a função retorna o tipo `ESeriousError` se não houver um erro
* Verifique se a função retorna o tipo de erro correto para um erro conhecido
* Verifique se a função retorna o tipo de erro correto para um erro desconhecido

#### Função TranslateError
```cpp
Error TranslateError( int p_error, bool p_errno )
```

**Descrição:** Traduz códigos de erro do formato nativo da plataforma para o formato SocketLib

**Sugestão de teste:**
* Verifique se a função retorna o tipo de erro correto, para cada um dos códigos de erro listados na função

#### Função Exception
```cpp
Exception::Exception( Error p_code )
```

**Descrição:** Inicializa a exceção de socket com um código de erro específico

**Sugestão de teste:**
* Verificar se a exceção retorna o código de erro esperado

#### Função ErrorCode
```cpp
Error Exception::ErrorCode()
```

**Descrição:** Retorna o código de erro do socket

**Sugestão de teste:**
* Verificar se o código de erro retornado corresponde ao erro levantado

#### Função PrintError
```cpp
std::string Exception::PrintError()
```

**Descrição:** Imprime a mensagem de erro em uma string

**Sugestão de teste:**
* Verificar se a string retornada corresponde ao erro levantado

### SocketLib/SocketLibSocket.cpp

#### Função Socket::Close
```cpp
void Socket::Close()
```

**Descrição:** Fecha o socket

**Sugestão de teste:**
* Verificar se o método fecha o socket corretamente.
* Verificar se o método faz o socket ter um valor inválido após ser fechado.

#### Função Socket::SetBlocking
```cpp
void Socket::SetBlocking(bool p_blockmode)
```

**Descrição:** Configura o modo de bloqueio do socket

**Sugestão de teste:**
* Verificar se o método configura o socket corretamente como bloqueante.
* Verificar se o método configura o socket corretamente como não-bloqueante.

#### Função Socket::Socket
```cpp
Socket::Socket(sock p_socket)
```

**Descrição:** Construtor da classe Socket que recebe um socket.

**Sugestão de teste:**
* Verificar se o socket é configurado como bloqueante após ser criado.
* Verificar se o socket é configurado como não-bloqueante após ser criado.

#### Função DataSocket::DataSocket
```cpp
DataSocket::DataSocket(sock p_socket)
```

**Descrição:** Construtor da classe DataSocket que recebe um socket.

**Sugestão de teste:**
* Verificar se o socket é configurado como bloqueante após ser criado.
* Verificar se o socket é configurado como não-bloqueante após ser criado.

#### Função DataSocket::Connect
```cpp
void DataSocket::Connect(ipaddress p_addr, port p_port)
```

**Descrição:** Conecta o socket a outro socket. Este método falha caso o socket já esteja conectado ou o servidor rejeite a conexão.

**Sugestão de teste:**
* Verificar se o método conecta corretamente dois sockets.
* Verificar se o método lança uma exceção caso o socket já esteja conectado.
* Verificar se o método lança uma exceção caso a conexão seja rejeitada pelo servidor.

#### Função DataSocket::Send
```cpp
int DataSocket::Send(const char* p_buffer, int p_size)
```

**Descrição:** Tenta enviar dados e retorna o número de bytes enviados

**Sugestão de teste:**
* Verificar se o método envia corretamente dados por um socket.
* Verificar se o método lança uma exceção caso o socket não esteja conectado.

#### Função DataSocket::Receive
```cpp
int DataSocket::Receive( char* p_buffer, int p_size )
```

**Descrição:** Usada para receber dados de um soquete e retorna o número de bytes recebidos. 

**Sugestão de teste:**
* Verificar se uma exceção é lançada se Receive é chamada em um socket não conectado.
* Verificar se uma exceção é lançada se a conexão foi fechada antes ou durante a chamada da função.
* Verificar se a função recebe a quantidade correta de dados quando dados são enviados para o socket.
* Verificar se a função retorna corretamente 0 se nenhum dado é recebido.

#### Função DataSocket::Close
```cpp
void DataSocket::Close()
```

**Descrição:** Fecha o socket

**Sugestão de teste:**
* Verificar se a função Close fecha corretamente o socket.
* Verificar se uma exceção é lançada se Close é chamada em um socket não inicializado.

#### Classe ListeningSocket::ListeningSocket
```cpp
ListeningSocket::ListeningSocket()
```

**Descrição:** Herda da classe Socket e é usada para configurar um soquete para escutar conexões entrantes.

**Sugestão de teste:**
* Verificar se o objeto da classe está sendo instanciado corretamente

#### Função ListeningSocket::Listen
```cpp
void ListeningSocket::Listen( port p_port )
```

**Descrição:** Usada para configurar o soquete para escutar em uma determinada porta. 

**Sugestão de teste:**
* Verificar se uma exceção é lançada ao tentar ouvir em uma porta inválida, como uma porta menor que 1024.
* Verificar se o socket está realmente escutando na porta correta, tentando se conectar a ela a partir de outro processo ou de um telnet.
* Verificar se a exceção é lançada quando o socket não pode ser associado à porta desejada, como quando a porta já está em uso ou se o processo não tem permissão para usar a porta.

#### Função ListeningSocket::Accept
```cpp
DataSocket ListeningSocket::Accept()
```

**Descrição:** Usada para aceitar uma conexão entrante em um soquete de escuta e retorna um DataSocket que representa a nova conexão. 

**Sugestão de teste:**
* Verificar se uma exceção é lançada se a função Accept é chamada antes de Listen.
* Verificar se uma exceção é lançada quando ocorre um erro ao aceitar a conexão.
* Verificar se o objeto DataSocket retornado pela função Accept é válido e tem informações corretas sobre a conexão.

#### Função ListeningSocket::Close
```cpp
void ListeningSocket::Close()
```

**Descrição:** Fecha o socket de escuta

**Sugestão de teste:**
* Verificar se a função Close fecha corretamente o socket.
* Verificar se uma exceção é lançada se Close é chamada em um socket não inicializado.

### SocketLib/SocketLibSocket.h

#### Classe Socket
```cpp
class Socket
```

**Descrição:** Uma classe base de socket muito básica que dará ao usuário a capacidade de obter informações de porta e IP, mas não muito mais.

#### Classe DataSocket
```cpp
class DataSocket : public Socket
```

**Descrição:** Uma variação da classe base BasicSocket que lida com conexões de dados TCP/IP.

#### Classe ListeningSocket
```cpp
class ListeningSocket : public Socket
```

**Descrição:** Uma variação da classe base BasicSocket que lida com solicitações de conexão TCP/IP de entrada.

### SocketLib/SocketLibSystem.cpp

#### Função GetIPAddress
```cpp
ipaddress GetIPAddress( const std::string p_address )
```

**Descrição:** Recebe uma string que pode ser um endereço IP ou um nome DNS, e retorna o endereço IP na forma de uma estrutura ipaddress. Se o endereço não puder ser convertido, uma exceção será lançada.

**Sugestão de teste:**
* Testar a conversão de um endereço IP válido para ipaddress.
* Testar a conversão de um nome DNS válido para ipaddress.
* Testar a exceção lançada quando o endereço não puder ser convertido.

#### Função GetIPString
```cpp
std::string GetIPString( ipaddress p_address )
```

**Descrição:** Recebe um endereço IP na forma de uma estrutura ipaddress e retorna uma string contendo o endereço IP em formato numérico.

**Sugestão de teste:**
* Testar a conversão de um ipaddress válido para string.
* Testar a string retornada quando o ipaddress é inválido.

#### Função GetHostNameString
```cpp
std::string GetHostNameString( ipaddress p_address )
```

**Descrição:** Recebe um endereço IP na forma de uma estrutura ipaddress e retorna o nome DNS correspondente. Essa função pode bloquear.

**Sugestão de teste:**
* Testar a conversão de um endereço IP válido para um nome DNS.
* Testar a exceção lançada quando o nome DNS não puder ser resolvido.

#### Função IsIPAddress
```cpp
bool IsIPAddress( const std::string p_address )
```

**Descrição:** Recebe uma string e verifica se ela representa um endereço IP válido ou um nome DNS.

**Sugestão de teste:**
* Testar a função com um endereço IP válido.
* Testar a função com um nome DNS válido.
* Testar a função com uma string que não é nem um endereço IP válido nem um nome DNS válido.

### SocketLib/SocketSet.cpp

#### Função SocketSet
```cpp
SocketSet::SocketSet()
```

**Descrição:** Construtor da classe SocketSet

**Sugestão de teste:**
* Testar se os conjuntos de sockets `m_set` e `m_activityset` estão inicializados corretamente com a função `FD_ZERO`

#### Função AddSocket
```cpp
void SocketSet::AddSocket( const Socket& p_sock )
```

**Descrição:** Adiciona um socket ao set

**Sugestão de teste:**
* Testar se o socket é adicionado corretamente ao conjunto de sockets `m_set` com a função `FD_SET`.
* Testar se o conjunto de descritores de sockets `m_socketdescs` é atualizado corretamente

#### Função RemoveSocket
```cpp
void SocketSet::RemoveSocket( const Socket& p_sock )
```

**Descrição:** Remove um socket do set

**Sugestão de teste:**
* Testar se o socket é removido corretamente do conjunto de sockets `m_set` com a função `FD_CLR`.
* Testar se o conjunto de descritores de sockets `m_socketdescs` é atualizado corretamente

### SocketLib/SocketSet.h

#### Classe SocketSet
```cpp
class SocketSet
```

**Descrição:** A classe SocketSet é utilizada para gerenciar um conjunto de sockets. Ela utiliza as funções `FD_ZERO`, `FD_SET` e `FD_CLR` para manipular os conjuntos de sockets, que são representados pela estrutura `fd_set` da biblioteca de sockets.

#### Função Poll
```cpp
inline int Poll( long p_time = 0 )
```

**Descrição:** Realiza a chamada select() do sistema operacional em um conjunto de sockets, aguardando por atividade em um período de tempo determinado. Ela retorna o número de descritores de arquivo prontos para leitura, gravação ou exceção, ou 0 se nenhum estiver pronto ou ocorrer um erro.

**Sugestão de teste:**
* Verificar se a função retorna 0 quando não há atividade em nenhum dos sockets adicionados ao conjunto
* Adicionar um socket ao conjunto e verificar se a função retorna 1 quando o socket tiver atividade
* Adicionar múltiplos sockets ao conjunto e verificar se a função retorna o número correto de sockets com atividade
* Verificar se a função retorna -1 em caso de erro

#### Função HasActivity
```cpp
inline bool HasActivity( const Socket& p_sock )
```

**Descrição:** verifica se um socket em particular teve atividade desde a última chamada a `Poll()`. Ela retorna `true` se o socket tiver atividade e `false` caso contrário.

**Sugestão de teste:**
* Verificar se a função retorna `false` quando nenhum socket teve atividade desde a última chamada a `Poll()`
* Adicionar um socket ao conjunto, chamar `Poll()`, verificar se a função retorna `true` para o socket com atividade, chamar `Poll()` novamente e verificar se a função retorna `false` para o mesmo socket.

### SocketLib/Telnet.cpp

#### Função Translate
```cpp
void Telnet::Translate( Connection<Telnet>& p_conn, char* p_buffer, int p_size )
```

**Descrição:** Responsável por traduzir dados brutos de bytes em dados de telnet e pode enviar para o manipulador de protocolo atual da conexão

**Sugestão de teste:**
* Testar o tamanho do buffer de entrada sendo zero.
* Testar o tamanho do buffer de entrada sendo igual a `BUFFERSIZE`.
* Testar caracteres normais no buffer de entrada.
* Testar caracteres de retrocesso no buffer de entrada.
* Testar caracteres de nova linha no buffer de entrada.
* Testar o manipulador de protocolo atual da conexão sendo nulo.
* Testar o manipulador de protocolo atual da conexão não sendo nulo.

#### Função SendString
```cpp
void Telnet::SendString( Connection<Telnet>& p_conn, string p_string )
```

**Descrição:** Envia uma string de texto simples para a conexão, fazendo qualquer tradução necessária. Ela faz isso adicionando os dados ao buffer de saída da conexão.

**Sugestão de teste:**
* Testar string de entrada vazia.
* Testar string de entrada contendo apenas caracteres normais.
* Testar string de entrada contendo caracteres de nova linha.
* Testar buffer de saída da conexão vazio.
* Testar buffer de saída da conexão contendo dados.

### SocketLib/Telnet.h

#### Classe Telnet
```cpp
class Telnet
```

**Descrição:** A classe Telnet define o protocolo Telnet, que recebe dados brutos de conexões e remove ou lida com quaisquer códigos de comando. A classe oferece métodos para traduzir dados brutos em dados Telnet e enviar strings para a conexão. A classe também define constantes para cores e formatação de texto, bem como constantes para controle de tela, como limpar a tela e a linha atual.

#### Função Buffered
```cpp
inline int Buffered()
```

**Descrição:** Retonar o tamanho do buffer de dados do objeto

**Sugestão de teste:**
* Testar se a função retorna corretamente o tamanho do buffer após as traduções serem aplicadas.