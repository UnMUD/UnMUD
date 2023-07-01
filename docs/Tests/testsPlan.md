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

Para levantar o ambiente basta executar o comando `docker compose up`, estando na pasta `MUDGameProgramming`. Após o container ser criado, é necessário compilar as bibliotecas e o MUD, o que pode ser feito com o comando:

```
docker compose exec unmud bash Scripts/compile.sh 
```

Após a compilação ser concluída, é possível compilar e executar os testes com o comando:

```
docker compose exec unmud bash Scripts/tests.sh 
```

Caso todos os testes passem, será criado um arquivo informando a cobertura de testes nas pasta `AnalysisLogs/GCOVR`, com o nome `ano-mês-dia-hora-segundo-coverage.html`.

Também é possível executar os comandos de forma independente, basta entrar na pasta `MUDGameProgramming/SimpleMUD`, realizando esse acesso por dentro do container, e utilizar a ferramenta Make para compilar os arquivos, executar o binário resultante e gerar a cobertura de testes.

Para acessar o container basta executar:
```
docker compose exec -it unmud /bin/bash

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

* [Detalhamento dos testes da BasicLib](/Tests/basicLibPlan.md)
* [Detalhamento dos testes da SocketLib](/Tests/socketLibPlan.md)
* [Detalhamento dos testes da ThreadLib](/Tests/threadLibPlan.md)
* [Detalhamento dos testes do SimpleMUD](/Tests/simpleMUDPlan.md)


## Referências

* Everett, Gerald D., and Raymond McLeod Jr. "Software testing." Testing Across the Entire (2007), disponível em <http://worldcolleges.info/sites/default/files/software-testing-testing-across-the-entire-software-development-life-cycle.9780471793717.28214.pdf>, acessado em Abril de 2023.