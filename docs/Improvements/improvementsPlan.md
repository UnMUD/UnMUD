# Planejamento de Melhorias

|  **Data**  | **Versão** | **Descrição** | **Autor(es)** |
| ---------- | ---------- | ------------- | ------------- |
| 03/05/2023 |    0.1     | Criação do documento de plano de melhorias | [Iuri Severo](https://github.com/iurisevero/) |
| 04/05/2023 |    0.2     | Finalizando sistema de classificação | [Iuri Severo](https://github.com/iurisevero/) |
| 05/05/2023 |    1.0     | Classificação dos erros; Adição da contagem de erros; Detalhamento da realização das melhorias | [Iuri Severo](https://github.com/iurisevero/) |

## Introdução

Este documento tem como objetivo apresentar um sistema de classificação para os erros e warnings encontrados no projeto, assim como uma lista detalhada de como cada tipo de erro e warning se encaixa nessa classificação e um planejamento para a correção dos mesmos. A divisão de tipos será realizada com base nos relatórios das ferramentas `clang-tidy` e `cppcheck`, enquanto a classificação será baseada no nível de dificuldade de resolução de cada um desses tipos.

## Sistema de classificação

A classificação dos erros e warnings será baseada nos seguintes fatores:
* Conhecimento a respeito do erro/warning
* Tempo necessário para resolvê-lo
* Complexidade da solução

Dessa forma, um erro/warning que se repete diversas vezes possuiria um nível de conhecimento maior, o que resultaria em um menor tempo necessário para resolução, enquanto um erro que só aparece uma vez teria requisitaria maior tempo necessário para resolução, uma vez que a exploração para entendimento do erro também faria parte desse tempo.

Por fim, os erros serão divididos em níveis, com base nos valores atribuídos aos fatores classificatórios, conforme apresentado na tabela abaixo:

| Conhecimento (1 - 5) | Tempo (min) | Complexidade (1 - 5) | Nível |
| -------------------- | ----------- | -------------------- | ----- |
| [4, 5] |       x < 30  | [1, 2] | H |
| [4, 5] |       x < 30  |    3   | G |
| [4, 5] |       x < 30  | [4, 5] | F |
| [4, 5] |  30 < x < 90  | [1, 2] | G |
| [4, 5] |  30 < x < 90  |    3   | F |
| [4, 5] |  30 < x < 90  | [4, 5] | E |
| [4, 5] |  90 < x < 180 | [1, 2] | F |
| [4, 5] |  90 < x < 180 |    3   | E |
| [4, 5] |  90 < x < 180 | [4, 5] | D |
| [4, 5] | 180 < x       | [1, 2] | E |
| [4, 5] | 180 < x       |    3   | D |
| [4, 5] | 180 < x       | [4, 5] | C |
|    3   |       x < 30  | [1, 2] | G |
|    3   |       x < 30  |    3   | F |
|    3   |       x < 30  | [4, 5] | E |
|    3   |  30 < x < 90  | [1, 2] | F |
|    3   |  30 < x < 90  |    3   | E |
|    3   |  30 < x < 90  | [4, 5] | D |
|    3   |  90 < x < 180 | [1, 2] | E |
|    3   |  90 < x < 180 |    3   | D |
|    3   |  90 < x < 180 | [4, 5] | C |
|    3   | 180 < x       | [1, 2] | D |
|    3   | 180 < x       |    3   | C |
|    3   | 180 < x       | [4, 5] | B |
| [1, 2] |       x < 30  | [1, 2] | F |
| [1, 2] |       x < 30  |    3   | E |
| [1, 2] |       x < 30  | [4, 5] | D |
| [1, 2] |  30 < x < 90  | [1, 2] | E |
| [1, 2] |  30 < x < 90  |    3   | D |
| [1, 2] |  30 < x < 90  | [4, 5] | C |
| [1, 2] |  90 < x < 180 | [1, 2] | D |
| [1, 2] |  90 < x < 180 |    3   | C |
| [1, 2] |  90 < x < 180 | [4, 5] | B |
| [1, 2] | 180 < x       | [1, 2] | C |
| [1, 2] | 180 < x       |    3   | B |
| [1, 2] | 180 < x       | [4, 5] | A |

## Lista de erros e warnings

| Identificação | Fonte | Classificação | Conhecimento (1 - 5) | Tempo (min) | Complexidade (1 - 5) | Nível | Ocorrências |
| ------------- | ----- | ------------- | -------------------- | ----------- | -------------------- | ----- | ----------- |
| Undefined error | - | error | 1 | 90 < x < 180 | 4 | B | 1 |
| A has not been declared | gcc | error | 4 | 30 < x < 90 | 3 | F | 1 |
| A was not declared in this scope | gcc | error | 4 | 30 < x < 90 | 2 | G | 19 |
| dependent-name A is parsed as a non-type, but instantiation yields a type | gcc | error | 3 | 30 < x < 90 | 3 | E | 1 |
| invalid storage class for function | gcc | error | 3 | 30 < x < 90 | 3 | E | 2 |
| multiple definition of | gcc | error | 5 | x < 30 | 3 | G | 35 |
| need ‘typename’ before A because B is a dependent scope | gcc | error | 3 | 30 < x < 90 | 4 | D | 5 |
| no matching function for call to A | gcc | error | 2 | 30 < x < 90 | 3 | D | 2 |
| No such file or directory | bash | error | 5 | x < 30 | 2 | H | 3 |
| Segmentation fault (core dumped) | gcc | error | 2 | 90 < x < 180 | 4 | B | 3 |
| specializing member A requires ‘template<>’ syntax | gcc | error | 2 | 30 < x < 90 | 3 | D | 6 |
| static declaration of A follows non-static declaration | gcc | error | 4 | x < 30 | 3 | G | 1 |
| ‘sys_errlist’ undeclared | gcc | error | 5 | x < 30 | 2 | H | 3 |
| there are no arguments to A that depend on a template parameter, so a declaration of A must be available | gcc | error | 5 | x < 30 | 2 | H | 9 |
| errno: TLS definition in A section .tbss mismatches non-TLS reference in B | gcc | error | 3 | x < 30 | 3 | F | 1 |
| variable or field A declared void | gcc | error | 3 | 30 < x < 90 | 3 | E | 1 |
| catching polymorphic type | gcc | warning | 3 | x < 30 | 3 | F | 3 |
| conversion from A to B may change value | gcc | warning | 4 | x < 30 | 2 | H | 78 |
| comparison of integer expressions of different signedness | gcc | warning | 4 | x < 30 | 1 | H | 4 |
| conversion to B from A may change the sign of the result | gcc | warning | 4 | x < 30 | 2 | H | 36 |
| declaration of A shadows a global declaration | gcc | warning | 4 | x < 30 | 3 | G | 1 |
| has pointer data members A but does not override B or C | gcc | warning | 3 | 30 < x < 90 | 2 | F | 16 |
| should be initialized in the member initialization list | gcc | warning | 5 | x < 30 | 1 | H | 298 |
| unused parameter | gcc | warning | 5 | x < 30 | 3 | G | 1 |
| use of old-style cast | gcc | warning | 4 | x < 30 | 2 | H | 227 |
| constParameter | cppcheck | warning | 5 | x < 30 | 1 | H | 3 |
| cstyleCast | cppcheck | warning | 4 | x < 30 | 3 | G | 1 |
| noExplicitConstructor | cppcheck | warning | 3 | x < 30 | 2 | G | 46 |
| passedByValue | cppcheck | warning | 4 | x < 30 | 2 | H | 3 |
| selfAssignment | cppcheck | warning | 3 | x < 30 | 2 | G | 1 |
| uninitMemberVar | cppcheck | warning | 5 | x < 30 | 1 | H | 23 |
| unreadVariable | cppcheck | warning | 5 | x < 30 | 3 | G | 1 |
| unusedFunction | cppcheck | warning | 5 | 30 < x < 90 | 3 | F | 26 |
| useInitializationList | cppcheck | warning | 5 | x < 30 | 1 | H | 26 |
| android-cloexec-accept | clang-tidy | warning | 4 | x < 30 | 1 | H | 1 |
| array-to-pointer-decay | clang-tidy | warning | 3 | x < 30 | 1 | G | 22 |
| avoid-c-arrays | clang-tidy | warning | 5 | x < 30 | 1 | H | 6 |
| bounds-constant-array-index | clang-tidy | warning | 3 | x < 30 | 2 | G | 15 |
| bounds-pointer-arithmetic | clang-tidy | warning | 5 | x < 30 | 3 | G | 5 |
| braces-around-statements | clang-tidy | warning | 5 | x < 30 | 1 | H | 180 |
| bugprone-macro-parentheses | clang-tidy | warning | 4 | x < 30 | 1 | H | 6 |
| build-using-namespace | clang-tidy | warning | 3 | x < 30 | 1 | G | 17 |
| casting | clang-tidy | warning | 4 | x < 30 | 2 | H | 19 |
| cert-err58-cpp | clang-tidy | warning | 3 | x < 30 | 3 | F | 15 |
| container-size-empty | clang-tidy | warning | 5 | x < 30 | 1 | H | 1 |
| convert-member-functions-to-static | clang-tidy | warning | 4 | x < 30 | 3 | G | 5 |
| default-arguments-calls | clang-tidy | warning | 5 | x < 30 | 3 | G | 131 |
| deprecated-headers | clang-tidy | warning | 5 | x < 30 | 1 | H | 4 |
| else-after-return | clang-tidy | warning | 5 | x < 30 | 3 | G | 4 |
| header-guard | clang-tidy | warning | 3 | x < 30 | 2 | G | 42 |
| implicit-bool-conversion | clang-tidy | warning | 5 | x < 30 | 2 | H | 1 |
| include-order | clang-tidy | warning | 5 | x < 30 | 1 | H | 54 |
| init-variables | clang-tidy | warning | 5 | x < 30 | 1 | H | 23 |
| isolate-declaration | clang-tidy | warning | 5 | x < 30 | 1 | H | 3 |
| loop-convert | clang-tidy | warning | 5 | x < 30 | 2 | H | 5 |
| macro-usage | clang-tidy | warning | 3 | x < 30 | 3 | F | 1 |
| magic-numbers | clang-tidy | warning | 5 | x < 30 | 2 | H | 98 |
| make-member-function-const | clang-tidy | warning | 4 | x < 30 | 2 | H | 3 |
| member-init | clang-tidy | warning | 5 | x < 30 | 1 | H | 12 |
| namespace-comments | clang-tidy | warning | 4 | x < 30 | 1 | H | 2 |
| no-assembler | clang-tidy | warning | 3 | x < 30 | 3 | F | 2 |
| non-const-parameter | clang-tidy | warning | 4 | x < 30 | 2 | H | 1 |
| non-private-member-variables-in-classes | clang-tidy | warning | 5 | x < 30 | 3 | G | 1 |
| overloaded-operator | clang-tidy | warning | 5 | 30 < x < 90 | 3 | F | 16 |
| owning-memory | clang-tidy | warning | 3 | 30 < x < 90 | 3 | E | 3 |
| performance-unnecessary-value-param | clang-tidy | warning | 4 | x < 30 | 3 | G | 7 |
| return-braced-init-list | clang-tidy | warning | 4 | x < 30 | 2 | H | 1 |
| runtime-references | clang-tidy | warning | 5 | x < 30 | 3 | G | 1 |
| signed-bitwise | clang-tidy | warning | 4 | x < 30 | 2 | H | 6 |
| simplify-boolean-expr | clang-tidy | warning | 5 | x < 30 | 3 | G | 7 |
| statically-constructed-objects | clang-tidy | warning | 4 | 30 < x < 90 | 3 | F | 11 |
| type-cstyle-cast | clang-tidy | warning | 4 | x < 30 | 3 | G | 11 |
| unused-using-decls | clang-tidy | warning | 4 | x < 30 | 2 | H | 10 |
| use-auto | clang-tidy | warning | 5 | x < 30 | 1 | H | 20 |
| use-emplace | clang-tidy | warning | 5 | x < 30 | 2 | H | 4 |
| use-nullptr | clang-tidy | warning | 5 | x < 30 | 2 | H | 24 |
| use-trailing-return-type | clang-tidy | warning | 4 | x < 30 | 2 | H | 81 |
| vararg | clang-tidy | warning | 3 | x < 30 | 2 | G | 4 |

## Detalhamento da realização das melhorias

As melhorias do projeto serão realizadas utilizando as ferramentas `clang-format`, `clang-tidy`e `cppcheck`em conjunto com a tabela de classificação dos erros e warnings. Tanto o `clang-format` quanto o `clang-tidy` realizam alteração no código automaticamente, o que irá facilitar parte das correções. O warnings que persistirem após a utilização dessas ferramentas serão corrigidos com base na classificação deles e na quantidade de ocorrências, indo dos mais simples para os mais complexos, e dos com maior ocorrência para os com menor ocorrência, tendo como prioridade os warnings apontados pelo GCC.

Desse modo, os passos realizados para correção serão:
1. Utilização do `clang-format`
2. Geração de novos relatórios de análise utilizando o `cppcheck` e o `clang-tidy`
3. Compilação das bibliotecas e do MUD com flags de warning ativas
4. Correção de warnings

O erros elicitados já foram corrigidos, por conta disso o foco das melhorias será na correção dos warnings