# Plano de Testes

|  **Data**  | **Versão** | **Descrição** | **Autor(es)** |
| ---------- | ---------- | ------------- | ------------- |
| 28/04/2023 |    1.0     | Criação do plano de testes da ThreadLib | [Iuri Severo](https://github.com/iurisevero/) |

## Detalhamento dos testes da ThreadLib

### ThreadLib/ThreadLibFunctions.cpp

#### Função DummyRun
```cpp
void* DummyRun( void* p_data )
```

**Descrição:** Função de thread "dummy" que será usada para traduzir transparentemente ponteiros de função para o sistema em que o usuário está compilando no momento

**Sugestão de teste:**
* Verifique a conversão dos dados recebidos: Crie um objeto `DummyData` e inicialize seus membros `m_func` e `m_data`. Passe esse objeto como argumento para a função `DummyRun` e verifique se a conversão do ponteiro de dados p_data para um ponteiro `DummyData` foi realizada com sucesso e se os membros `m_func` e `m_data` foram corretamente copiados.
* Verifique a execução da função: Crie uma função dummy e passe-a como argumento para a função `DummyRun`. Verifique se a função foi executada corretamente com os dados passados e se a função `DummyRun` retornou 0 após a execução.
* Verifique a liberação de memória: Crie um objeto `DummyData` com alocação de memória dinâmica para o membro `m_data` e inicialize seus membros `m_func` e `m_data`. Passe esse objeto como argumento para a função `DummyRun` e verifique se a memória alocada para esse objeto foi corretamente liberada após a execução da função `DummyRun`.
* Verifique a chamada de função com ponteiro nulo: Crie um objeto `DummyData` com o membro `m_func` apontando para um ponteiro nulo e inicialize o membro `m_data` com um valor qualquer. Passe esse objeto como argumento para a função `DummyRun` e verifique se a função `DummyRun` retorna 0 sem causar erros ou falhas de segmentação.

### ThreadLib/ThreadLibFunctions.h

#### Função 
```cpp
inline ThreadID Create( ThreadFunc p_func, void* p_param )
```

**Descrição:** Cria uma thread e retorna seu ID

**Sugestão de teste:**
* Criar uma thread com uma função que imprime um valor na tela e verificar se o valor é impresso corretamente.
* Tentar criar uma thread com um ponteiro de função nulo
* Tentar criar uma thread com um ponteiro de dados nulo

#### Função 
```cpp
inline ThreadID GetID()
```

**Descrição:** Obtém o ID da thread atual

**Sugestão de teste:**
* Verificar se o ID da thread atual está correto

#### Função 
```cpp
inline void WaitForFinish( ThreadID p_thread )
```

**Descrição:** Espera a thread terminar de executar antes de retornar

**Sugestão de teste:**
* Criar uma thread que espera um tempo antes de retornar e verificar se a função WaitForFinish espera a thread terminar antes de continuar.
* Tentar esperar por uma thread que não existe

#### Função 
```cpp
inline void Kill( ThreadID& p_thread )
```

**Descrição:** Termina uma thread à força

**Sugestão de teste:**
* Criar uma thread que não termina e chamar a função Kill para terminá-la. Verificar se a thread é terminada corretamente.
* Tentar terminar uma thread que não existe

#### Função 
```cpp
inline void YieldThread( int p_milliseconds = 1 )
```

**Descrição:** Cede a thread para o sistema operacional e libera sua fatia de tempo atual

**Sugestão de teste:**
* Chamar a função com um tempo de espera e verificar se a thread atual é suspensa por esse tempo.