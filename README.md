# UnMUD

O UnMUD é um Multi-User Dungeon (MUD) desenvolvido como Projeto de Pesquisa e Trabalho de Conclusão de Curso do aluno [Iuri Severo](https://github.com/iurisevero), da Universidade de Brasília. Seu código teve como base o SimpleMUD, disponibilizado por Ron Penton em seu livro “MUD Game Programming”, e foi evoluído para se enquadrar na diretrizes do C++, além de seguir a boas práticas adotadas pela comunidade.

Esse projeto foi desenvolvido com o objetivo principal de tornar a base de códigos de um MUD implantável, para que outros estudantes possam explorá-la e evoluí-la conforme desejarem, dando a eles a oportunidade de jogar, testar e analisar o jogo, a fim de compreender mais profundamente seu desenvolvimento, arquitetura e outros aspectos relevantes de seu software. Devido ao seu aspecto multidisciplinar e seu potencial evolutivo, MUDs se enquandram bem nessa função de "material de estudo".

A monografia e o relatório de pesquisa resultantes desse projeto podem ser acessados nos links: [TCC](); [PIBIC]().

![Imagem do Software em funcionamento]()

## Documentação

Toda documentação referente ao projeto pode ser visto em seu [site no GitHub Pages](https://unmud.github.io/UnMUD/#/). Caso você seja novo por aqui, alguns links interessantes de dar uma olhada caso você esteja interessado em:

* [Contribuir com o projeto](https://unmud.github.io/UnMUD/#/contributionGuide)
* [Jogar UnMUD](https://unmud.github.io/UnMUD/#/)

É sempre bom também dar uma olhada nosso [Código de Conduta](https://unmud.github.io/UnMUD/#/) antes de interagir com a comunidade!

## Área do suporte

A área de discussões do GitHub está aberta para o esclarecimento de quaisquer dúvidas relacionadas ao projeto.

<!-- ## Requisitos mínimos: 
nesse tópico, são apresentados quais são os requisitos mínimos para se executar o projeto, e -->

## Tecnologias utilizadas

O UnMUD foi desenvolvido utilizando C++ como sua principal linguagem, na versão C++XX. O código foi atualizado para seguir as diretrizes especificadas no [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#main) e deve funcionar em versões superiores a especificada, porém é possível que haja erros de compilação em versões anteriores.

Para criar um ambiente de desenvolvimento isolado foram utilizadas as ferramentas [Docker](https://www.docker.com/) e [Docker-compose](https://docs.docker.com/compose/). Caso você não possua essas ferramentas, é possível instalá-la seguindo suas documentações oficiais:

* Arch: [Docker Desktop](https://docs.docker.com/desktop/install/archlinux/); [Docker Engine](https://wiki.archlinux.org/title/docker#Installation); [Docker-compose](https://wiki.archlinux.org/title/docker#Docker_Compose)
* Ubuntu: [Docker Desktop](https://docs.docker.com/desktop/install/ubuntu/); [Docker Engine](https://docs.docker.com/engine/install/ubuntu/); [Docker-compose](https://docs.docker.com/compose/install/linux/#install-using-the-repository)
* Windows: [Docker Desktop](https://docs.docker.com/desktop/install/windows-install/)
* Mac: [Docker Desktop](https://docs.docker.com/desktop/install/mac-install/)

Também foi utilizada a ferramenta (Makefile ou CMake) para compilação e linkagem dos códigos, assim como a geração de arquivo um executável. Não é necessária a instalação dessas ferramentas para execução do programa, uma vez que o ambiente de desenvolvimento é levantado de forma isolada pelo Docker. De todo modo, caso seja do seu interesse utilizá-la para rodar o programa, é possível acessar o guia de instalação nos links abaixo:

* Arch:
* Ubuntu:
* Windows:
* Mac:

Para análise estática de código foram utilizadas as ferramentas:

* Ferramenta 1
* Ferramenta 2
* Ferramenta 3

E para realização de testes unitários foi utilizado o framework [Framework](), em conjunto com a ferramenta [Ferramenta]() para análise da cobertura de testes.

A integração contínua e o deploy foram realizados com o auxílio do [GitHub Actions](https://github.com/features/actions).