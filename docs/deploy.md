# Implantação

|  **Data**  | **Versão** | **Descrição** | **Autor(es)** |
| ---------- | ---------- | ------------- | ------------- |
| 27/06/2023 |  1.0   | Criação do documento de Implantação | [Iuri Severo](https://github.com/iurisevero) |

Este documento tem como objetivo reunir as informações levantadas a respeito da implantação do sistema desenvolvido.

## Testes na AWS

Para realização da implantação do sistema foram realizados testes em uma máquina t2.micro nos servidores virtuais que a Amazon disponibiliza. A máquina t2.micro possui um processador Intel Xeon escalável de até 3,3 GHz (Haswell E5-2676 v3 ou Broadwell E5-2686 v4), uma CPU e 1 GiB de memória.

Após a instação da ferramentas git e docker, necessárias para executar o MUD, o código foi compilado, executado e testado com 5 usuários jogando simultaneamente. O momento de maior esforço da CPU foi durante a compilação do código, onde ela alcançou 26.76% de consumo. Durante os momentos de conexão dos jogadores e o tempo que eles estiveram no jogo não afetaram a CPU de maneira significativa, sendo o menor valor atingido nesse período 4.86% e o maior valor 5.54%.

Durante esse teste também foi verificado que o jogo fica disponível para conexão a partir do momento que o container docker é executado.

## Implantação manual no servidor físico

Após os testes, a principal alteração necessária para implantar o código era a criação de variáveis de ambiente para guardar dados sigilosos, como senhas de jogadores administradores, que permitem o provisionamento de usuários. Para isso foi criado um arquivo ```.env``` com as variáveis apresentadas no exemplo abaixo. Essas variáveis são passadas para o programa por meio do docker-compose e permitiram também a variação da porta utilizada pelo MUD.

```
# Database environment variables
PG_HOST=db
PG_PORT=5432
PG_DATABASE=unmud
PG_USER=postgres
PG_PASSWORD=postgres

# SQL environment variables https://stackoverflow.com/questions/18725880/using-an-environment-variable-in-a-psql-script
IURI_PASSWORD=123456789
MAURICIO_PASSWORD=123456789

# MUD environment variables
MUD_PORT=5100
```

Para realizar a implantação de maneira manual é necessário seguir os seguintes passos:

1. Instalar o Git no servidor
2. Clonar o repositório do MUD com o comando ```git clone https://github.com/UnMUD/UnMUD.git```
3. Instalar o Docker e o Docker Compose
4. Entrar na pasta ```MUDGameProgramming``` com o comando ```cd UnMUD/MUDGameProgramming```
5. Criar e executar os containers com o comando ```sudo docker compose up --build -d```
6. Compilar o MUD com o comando ```docker-compose exec unmud bash Scripts/compile.sh```
7. Executar o binário gerado com o comando ```docker-compose exec unmud bash Scripts/run.sh```

Após isso deve ser possível conectar ao jogo utilizando o ip do servidor e a porta especificada nas variáveis de ambiente.

## Criação de Usuários Privilegiados

Para criação de usuários com privilégios é necessário realizar alterações em 3 documentos: ```.env```, ```docker-compose.yml``` e ```Database/DML.sql```. 

No arquivo de variáveis de ambiente é necessário adicionar a senha do usuário que se deseja criar. Ex: ```NOVO_USUARIO_PASSWORD=novaSenha```. Após isso é necessário adicionar essa variável nas variáveis do container ```db```, através da tag ```environment```, seguindo o padrão esperado pelo ```docker-compose```. Ex:

```
    environment:
      - IURI_PASSWORD=${IURI_PASSWORD}
      - MAURICIO_PASSWORD=${MAURICIO_PASSWORD}
      - NOVO_USUARIO_PASSWORD=${NOVO_USUARIO_PASSWORD}
```

Por fim, no arquivo de população do banco é necessário acessar a variável de ambiente criada e adicioná-la como senha para o novo usuário. Para acessar a variável adicione a linha ```\set novoUsuarioPassword `echo "$NOVO_USUARIO_PASSWORD"` ``` no inicio do documento, e para criar um novo usuário com essa senha, adicione ao final do documento ```(xxxxxxx, 'novousuario', :'novoUsuarioPassword', 'ROLE', 0, 0, 1, 1, 0, 10, 0, ROW(100, 100, 100, 0, 0, 0, 0, 0, 0), NULL, NULL);```. É necessário definir um ID e um nome de usuário válidos para a nova conta criada, além de definir qual papel ele terá no jogo: 'REGULAR', 'GOD' ou 'ADMIN'. 

> Lembrando que a população de jogadores acontece passando uma lista de tuplas para a função ```INSERT INTO``` do SQL, então apenas o último jogador adicionado deve possuir um ";" após sua declaração, os outros devem conter "," apenas.