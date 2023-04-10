# Guia de Contribuição

|  **Data**  | **Versão** | **Descrição** | **Autor(es)** |
| ---------- | ---------- | ------------- | ------------- |
| 08/04/2023 |  1.0   | Criação do Guia de Contribuição Iniical | [Iuri Severo](https://github.com/iurisevero) |

Este documento irá abordar o seguintes tópicos, para auxiliar aqueles que desejam contribuir com o projeto:

* Criando/Escolhendo uma Issue
* Padrões adotados para contribuições
* Criando um Pull Request
* Como rodar o #nome localmente
* Como rodar os testes de #nome
* Como rodar o site do #nome localmente

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

Caso você queira colaborar, mas ainda não sabe como, dê uma olhada nas _issues_ já existentes! Elas estarão listadas na [aba de Issues](https://github.com/iurisevero/Pibic/issues) do GitHub e possuirão **Labels** que servem para classificar a _issue_ e facilitar a escolha por parte do contribuidor.

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

**1.** O Pull Request deve indicar em seu título o número e nome da issue ao qual se refere.
**Exemplo:**
```
1 - Criar guia de contribuição
```

**2.** O corpo de texto do Pull Request deve ser escrito de forma clara e objetiva, seguindo o template fornecido no GitHub

**3.** O Pull Request deve ser realizado para branch `main`

![Criando pull request](/Images/comparePR.png)

![Template de pull request](/Images/templatePR.png)

## Como rodar o #nome localmente

Para uma maior praticidade e segurança no desenvolvimento foi criado um ambiente containerizado para rodar o #nome. Para executá-lo basta executar o comando:

```
$sudo docker-compose up
```

A partir disso, deve ser possível entrar no #nome a partir da porta ####, utilizando a ferramenta `telnet`.

## Como rodar os testes de #nome

Para rodar os testes, basta executar o comando:

```
$sudo docker-compose up exec
```

## Como subir o site do #nome localmente

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