# Introspecção

|  **Data**  | **Versão** | **Descrição** | **Autor(es)** |
| ---------- | ---------- | ------------- | ------------- |
| 30/03/2023 |    1.0     | Criação do documento, com definição, aplicação e referências | [Iuri Severo](https://github.com/iurisevero/) |

## Definição

A técnica da **instrospecção** consiste em entender quais propriedades um sistema deve possuir para que funcione corretamente. Ela demanda o Engenheiro de Requisitos imaginar quais funcionalidades e recursos o sistema deveria possuir para que determinadas tarefas sejam desempenhadas.

Apesar de sua utilidade, essa técnica se limita aos conhecimentos do especialista que a está realizando, podendo este não ser o mais adequado para refletir a experiência/atuação real dos interessados.

## Aplicação

Ao analisar o projeto desenvolvido é possível dividí-lo em dois contextos principais: a exploração e desenvolvimento do código do sistema disponibilizado, e a jogabilidade do MUD gerado a partir dessa base de códigos. Por conta disso, para uma aplicação mais completa da introspecção, foi optado por dividí-la em dois pontos de vista: do desenvolvedor e do jogador.

### Desenvolvedor

O primeiro passo de um desenvolvedor ao acessar um repositório público no qual ele planeja contribuir é procurar na documentação do projeto o que é o projeto, quais subsistemas ele possui, como eles são organizados e um **guia de contribuição**. Dessa forma, ele consegue estudar uma área do projeto pela qual ele se interessou e contribuir corretamente.

A parte de contribuição pode ser dividida em três etapas por parte do desenvolvedor: escolha de uma _issue_, correção/implementação do problema selecionado, e abertura do _pull request_ (PR). Outras duas etapas são adicionadas quando consideramos a parte de gereciamento do repositório, que seria a revisão e aceitação do PR. Para realização dessas atividades é necessário pensar em diversos pontos:

* **Escolha de _issues_**: É necessário que essas _issues_ estejam bem descritas, com detalhamento da tarefa a ser realizada e os critérios de aceitação. Também é interessante que elas estejam catalogadas de forma a facilitar a busca por parte de novos contribuidores, como **_labels_** de "bug", "difficulty: hard" ou "good first issue".
* **Implementação**: A princípio é necessário que o desenvolvedor entenda as funções na qual ele está mexendo. Para isso, é preciso de uma boa **documentação do código** e de um código limpo. Também é necessário que esse código seja **fácil de executar**, para que o desenvolvedor seja capaz de verificar as alterações realizadas. Por fim, também deve ser fácil para o desenvolvedor rodar os testes necessários para validar se seu código é aceitável.
* **Pull Request**: Assim como na escolha de _issues_, a parte de abrir um PR requer uma boa descrição da tarefa realizada e do problema solucionado. Um **_template_ de PR** auxiliaria o desenvolvedor nessa parte. Além disso, é interessante ter no guia de contribuição detalhes de como abrir corretamente um PR, caso haja alguma especificação para revisor, _labels_ ou _milestones_.
* **Revisão do PR**: Para o processo de revisão, é necessário a **automação de testes**, tanto unitários quanto estáticos, por meio de softwares como [_GitHub Actions_](https://docs.github.com/pt/actions). Essa verificação automática ajudaria o desenvolvedor a corrigir erros antes dos revisor analisar os códigos inseridos. Também seria útil uma boa definição de como indicar erros e dar _feedbacks_ para as alterações realizadas, criando um padrão de linguagem e ação utilizada pelos revisores.
* **Aceitação do PR**: Essa seria a última etapa da contribuição. Após a aceitação do PR, o código desenvolvido é mesclado com o código base do repositório e lançado em produção. Para essa etapa é importante estar bem definido como será o processo de implantação do sistema, se será por meio de  **_deploy_ contínuo** a cada _merge_ realizado na _branch_ principal, se será a cada período de tempo pré-definido, ou se será realizado manualmente.

Os pontos levantados para auxiliar o desenvolvedor que quer contribuir para o projeto também se aplicam para aqueles que querem apenas estudar o código, salvo que um desenvolvedor que pretende apenas estudar o sistema pode tirar maior proveito de uma documentação mais robusta, que possua um detalhamento maior do produto, da visão de mercado, aplicação, arquitetura, entre outros.

### Jogador

Diferente do desenvolvedor, a pessoa que pretende apenas jogar o MUD oferecido não tem a intenção de mergulhar de cabeça na documentação e ler o código-fonte do projeto para entendê-lo. Ele está interessado em saber **como entrar no jogo** e espera que todas outras informações necessárias para jogar lhe sejam fornecidas por ele.

A primeira ação ao entrar em um jogo novo é **criar uma conta**. A expectativa de um jogador é que seja simples realizar essa ação, que sua conta seja única e que seja salva de maneira segura, uma vez que ela conterá dados dele, como a senha. Após criar a conta e feita a conexão (automatica ou manualmente). No decorrer do jogo, é de se esperar que sejam esclarecidas as ações fundamentais, tais como locomoção, combate e comunicação via chat, bem como a forma de acessar a **ajuda ou o tutorial**. É esperado que o progresso persista após a saída do jogo, assim como os items e dinheiro adquiridos. 

A criação de uma lista de amigos, grupos e outras comunidades sociais é interessante de se possuir em um RPG, assim como um mapa do mundo explorado e, talvez, uma lista de monstros conhecidos ou de todos os monstros. A lista de monstros e o mapa estarem disponíveis em um ambiente diferente do jogo, como em um site, é uma possibilidade, uma vez que outros jogos já utilizaram essa abordagem. De todo modo, isso pode prejudicar a experiência do usuário.

Outras funções básicas de jogos esperadas em um RPG seriam: Acesso ao inventário; equipar e desequipar items; coletar items do chão e de monstros; realizar quests; o personagem possuir atributos customizáveis; evoluir conforme ganhar experiência.

> É válido ressaltar que no projeto desenvolvido, o MUD já está elaborado e suas mecânicas já foram definidas, o que torna a implementação/revisão dessas mecânicas algo não prioritário, visto que o foco dessa pesquisa não é na jogabilidade, mas no desenvolvimento.

## Referências

* Milene Serrano e Maurício Serrano, Requisitos - Aula 07, acessado em 2019
* PUC-Rio, Elicitação de Requisitos, https://www.maxwell.vrac.puc-rio.br/15760/15760_3.PDF, acessado em março de 2023