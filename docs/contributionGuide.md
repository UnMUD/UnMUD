# Guia de Contribuição

|  **Data**  | **Versão** | **Descrição** | **Autor(es)** |
| ---------- | ---------- | ------------- | ------------- |
| 08/04/2023 |  1.0   | Criação do Guia de Contribuição Iniical | [Iuri Severo](https://github.com/iurisevero) |

Este documento irá abordar o seguintes tópicos, para auxiliar aqueles que desejam contribuir com o projeto:

* Criando/Escolhendo uma Issue
* Padrões adotados para contribuições
* Criando um Pull Request
* Como rodar o UnMUD localmente
* Como rodar os testes de UnMUD
* Como rodar o site do UnMUD localmente

## Criando/Escolhendo uma Issue

### Criando novas Issues

Caso você perceba algum erro no projeto e queira corrigí-lo ou caso você tenha uma ideia de nova funcionalidade para o jogo, crie uma _issue_ para documentar o problema ou a ideia antes de começar a desenvolver e abrir o tão esperado Pull Request.

Três templates foram desenvolvidos para criação de _issues_:

* Bug: Para quando algum erro no programa ou na documentação do projeto
* História de usuário: Esse é mais exclusiva para os gerentes do projeto, e servem para quando novas histórias de usuário forem adicionadas ao backlog do produto
* Solicitação de Feature: Para caso você tenha alguma ideia que queira adicionar ao projeto. Nesse caso, é interessante esperar a validação de um dos desenvolvedores principais ou gerentes do projeto antes de começar a desenvolver.

![Templates de Issue](/Images/issuesTemplates.png)

O template "Report a security vulnerability" foi criado automaticamente pelo GitHub, para relatórios referentes a vulnerabilidades de segurança.

### Escolhendo uma Issue

Caso você queira colaborar, mas ainda não sabe como, dê uma olhada nas _issues_ já existentes! Elas estarão listadas na [aba de Issues](https://github.com/UnMUD/UnMUD/issues) do GitHub e possuirão **Labels** que servem para classificar a _issue_ e facilitar a escolha por parte do contribuidor.

![Labels de Issues](/Images/labels.png)

## Padrões adotados para contribuições

Após escolher a _issue_ é hora de começar a contribuir! Antes de começar a mexer nos arquivos, crie um **Fork** do repositório, faça um clone dele localmente e crie uma _branch_ nova para começar as alterações, seguindo a política de branchs e de commits.

### Política de Branchs

**1.** A branch deverá possuir o padrão CamelCase x-NomeDaBranch, em que o 'x' é o número da issue;

**2.** O nome deve ser escolhido com base na tarefa descrita na _issue_;

**Exemplo:**

```
1-GuiaDeContribuicao
```

### Política de Commits

**1.** Os commits devem ser escritos em português, no gerundio

**2.** A issue deve ser citada no commit por questões de rastreabilidade, para isso, basta adicionar:
```
#<numeroDaIssue> 
```

**Exemplo:**
```
#1 - Criando guia de contribuição
```

**Observação:** Por padrão, o caracter '#' define uma linha de comentário na mensagem do commit. Para resolver esse problema, altere o caracter de comentário utilizando o comando:
```
git config --local core.commentChar '!'
```

Para commits com a flag ```-m``` não é necessário realizar essa alteração

**3.** Para que uma pessoa seja inclusa como contribuinte no gráfico de commits do GitHub, basta incluir a instrução _Co-authored-By:_ na mensagem:
**Exemplo:**
```
#01 - Revisando guia de contribuição


Co-authored-By: Iuri Severo <nobreiuri@gmail.com>
```

## Criando um Pull Request

**1.** O Pull Request deve indicar em seu título a tag referente a alteração feita e o nome da issue ao qual se refere.
**Exemplo:**
```
US09 - Criar guia de contribuição
```

**2.** O corpo de texto do Pull Request deve ser escrito de forma clara e objetiva, seguindo o template fornecido no GitHub

**3.** O Pull Request deve ser realizado para branch `main`

![Criando pull request](/Images/comparePR.png)

![Template de pull request](/Images/templatePR.png)

## Como rodar o UnMUD localmente

Para uma maior praticidade e segurança no desenvolvimento foi criado um ambiente containerizado para rodar o UnMUD. Para executá-lo basta entrar na pasta `MUDGameProgramming` e executar o comando:

```
docker compose up
```

Após o container ser levantado, é possível acessá-lo com o comando:

```
docker compose exec -it unmud /bin/bash

```

Dentro do container é possível realizar qualquer operação desejada, no entanto, algumas funções foram previamente definidas para que sejam realizadas com maior facilidade.

Para compilar o código do MUD basta executar:

```
docker compose exec unmud bash Scripts/compile.sh 
```

Para executar o MUD basta executar:

```
docker compose exec unmud bash Scripts/run.sh 
```

Após isso, deve ser possível entrar no UnMUD a partir da porta 5100 utilizando a ferramenta `telnet`.

Para realizar a análise estática do código execute:

```
docker compose exec unmud bash Scripts/static_test.sh 
```

Para realizar a verificação do lint execute:

```
docker compose exec unmud bash Scripts/lint.sh
```

O resultado das análises será exibido no terminal e também ficará salvo na pasta `AnalysisLogs`.

Para formatar os arquivos, basta executar:

```
docker compose exec unmud bash Scripts/format.sh
```

A formatação é realizada pela ferramenta [clang-format](https://clang.llvm.org/docs/ClangFormat.html) e possui uma [extensão disponível para Visual Studio Code](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format).

### Como rodar os testes de UnMUD

Para rodar os testes é necessário compilar as bibliotecas e o jogo. Após isso basta executar o comando:

```
docker compose up exec unmud bash Scripts/tests.sh
```

Esse scripts irá realizar a compilação, execução e análise de cobertura dos testes, gerando um arquivo na pasta `AnalysisLogs/GCOVR`, com o nome `ano-mês-dia-hora-segundo-coverage.html`.

Caso o desejado seja apenas compilar os testes ou executá-los, é necessário realizar a execução do Makefile que está dentro da pasta `SimpleMUD`, realizando esse acesso por dentro do container. Após entrar na pasta é possível compilar os testes com o comando:

```
make tests
```

E executá-los com o comando:

```
make run_tests
```

Para ver a cobertura dos testes basta rodar o comando a seguir, ainda na pasta `SimpleMUD`:

```
gcovr
```

## Como subir o site do UnMUD localmente

Caso as alterações realizadas sejam na documentação, é possível rodar o site de documentação localmente para ter uma pré-visualização dos arquivos alterados. Para isso, basta seguir os passos abaixo:

* [Instale o Docsify](https://docsify.now.sh/quickstart)
* Vá para pasta do projeto
* Digite o seguinte comando
```
docsify serve docs/
```

Após isso o site deve estar funcionando na url "http://localhost:3000"! A partir de agora deve ser possível navegar nele e ver todas alterações realizadas.

## Referências

* iGado - Políticas, disponível em: <https://github.com/UnBArqDsw/2020.1_G13_iGado/blob/master/docs/Policies/Policies.md/>. Acesso em 08 de abril de 2023.
* Rasa - Guia de Contribuição, disponível em: <https://github.com/RasaHQ/rasa/blob/main/CONTRIBUTING.md>. Acessado em 08 de abril de 2023.