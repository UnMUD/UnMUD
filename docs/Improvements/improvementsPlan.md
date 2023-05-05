# Plano de Melhorias

|  **Data**  | **Versão** | **Descrição** | **Autor(es)** |
| ---------- | ---------- | ------------- | ------------- |
| 03/05/2023 |    0.1     | Criação do documento de plano de melhorias | [Iuri Severo](https://github.com/iurisevero/) |
| 04/05/2023 |    0.2     | Finalizando sistema de classificação | [Iuri Severo](https://github.com/iurisevero/) |

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

| Identificação | Fonte | Classificação | Conhecimento (1 - 5) | Tempo (min) | Complexidade (1 - 5) | Nível |
| ------------- | ----- | ------------- | -------------------- | ----------- | -------------------- | ----- |
| Undefined error | - | error | 1 | 90 < x < 180 | 4 | B |
| A has not been declared | gcc | error | 4 | 30 < x < 90 | 3 | F |
| A was not declared in this scope | gcc | error | 4 | 30 < x < 90 | 2 | G |
| dependent-name A is parsed as a non-type, but instantiation yields a type | gcc | error | 3 | 30 < x < 90 | 3 | E |
| invalid storage class for function | gcc | error | 3 | 30 < x < 90 | 3 | E |
| multiple definition of | gcc | error | 5 | x < 30 | 3 | G |
| need ‘typename’ before A because B is a dependent scope | gcc | error | 3 | 30 < x < 90 | 4 | D |
| no matching function for call to A | gcc | error | 2 | 30 < x < 90 | 3 | D |
| No such file or directory | bash | error | 5 | x < 30 | 2 | H |
| Segmentation fault (core dumped) | gcc | error | 2 | 90 < x < 180 | 4 | B |
| specializing member A requires ‘template<>’ syntax | gcc | error | 2 | 30 < x < 90 | 3 | D |
| static declaration of A follows non-static declaration | gcc | error | 4 | x < 30 | 3 | G |
| ‘sys_errlist’ undeclared | gcc | error | 5 | x < 30 | 2 | H |
| there are no arguments to A that depend on a template parameter, so a declaration of A must be available | gcc | error | 5 | x < 30 | 2 | H |
| errno: TLS definition in A section .tbss mismatches non-TLS reference in B | gcc | error | 3 | x < 30 | 3 | F |
| variable or field A declared void | gcc | error | 3 | 30 < x < 90 | 3 | E |
| catching polymorphic type | gcc | warning | 3 | x < 30 | 3 | F |
| conversion from A to B may change value | gcc | warning | 4 | x < 30 | 2 | H |
| comparison of integer expressions of different signedness | gcc | warning | 4 | x < 30 | 1 | H |
| conversion to B from A may change the sign of the result | gcc | warning | 4 | x < 30 | 2 | H |
| declaration of A shadows a global declaration | gcc | warning | 4 | x < 30 | 3 | G |
| has pointer data members A but does not override B or C | gcc | warning | 3 | 30 < x < 90 | 2 | F |
| should be initialized in the member initialization list | gcc | warning | 5 | x < 30 | 1 | H |
| unused parameter | gcc | warning | 5 | x < 30 | 3 | G |
| use of old-style cast | gcc | warning | 4 | x < 30 | 2 | H |
| constParameter | cppcheck | warning | 5 | x < 30 | 1 | H |
| cstyleCast | cppcheck | warning | 4 | x < 30 | 3 | G |
| noExplicitConstructor | cppcheck | warning | 3 | x < 30 | 2 | G |
| passedByValue | cppcheck | warning | 4 | x < 30 | 2 | H |
| selfAssignment | cppcheck | warning | 3 | x < 30 | 2 | G |
| uninitMemberVar | cppcheck | warning | 5 | x < 30 | 1 | H |
| unreadVariable | cppcheck | warning | 5 | x < 30 | 3 | G |
| unusedFunction | cppcheck | warning | 5 | 30 < x < 90 | 3 | F |
| useInitializationList | cppcheck | warning | 5 | x < 30 | 1 | H |

| android-cloexec-accept | clang-tidy | warning | 4 | x < 30 | 1 | H |
| array-to-pointer-decay | clang-tidy | warning | 3 | x < 30 | 1 | G |
| avoid-c-arrays | clang-tidy | warning | 5 | x < 30 | 1 | H |
| bounds-constant-array-index | clang-tidy | warning | 3 | x < 30 | 2 | G |
| bounds-pointer-arithmetic | clang-tidy | warning | 5 | x < 30 | 3 | G |
| braces-around-statements | clang-tidy | warning | 5 | x < 30 | 1 | H |
| bugprone-macro-parentheses | clang-tidy | warning | 4 | x < 30 | 1 | H |
| build-using-namespace | clang-tidy | warning | 3 | x < 30 | 1 | G |
| casting | clang-tidy | warning | 4 | x < 30 | 2 | H |
| cert-err58-cpp | clang-tidy | warning | 3 | x < 30 | 3 | F |
| container-size-empty | clang-tidy | warning | 5 | x < 30 | 1 | H |
| convert-member-functions-to-static | clang-tidy | warning | 4 | x < 30 | 3 | G |
| default-arguments-calls | clang-tidy | warning | 5 | x < 30 | 3 | G |
| deprecated-headers | clang-tidy | warning | 5 | x < 30 | 1 | H |
| else-after-return | clang-tidy | warning | 5 | x < 30 | 3 | G |
| header-guard | clang-tidy | warning | 3 | x < 30 | 2 | G |
| implicit-bool-conversion | clang-tidy | warning | 5 | x < 30 | 2 | H |
| include-order | clang-tidy | warning | 5 | x < 30 | 1 | H |
| init-variables | clang-tidy | warning | 5 | x < 30 | 1 | H |
| isolate-declaration | clang-tidy | warning | 5 | x < 30 | 1 | H |
| loop-convert | clang-tidy | warning | 5 | x < 30 | 2 | H |
| macro-usage | clang-tidy | warning | 3 | x < 30 | 3 | F |
| magic-numbers | clang-tidy | warning | 5 | x < 30 | 2 | H |
| make-member-function-const | clang-tidy | warning | 4 | x < 30 | 2 | H |
| member-init | clang-tidy | warning | 5 | x < 30 | 1 | H |
| namespace-comments | clang-tidy | warning | 4 | x < 30 | 1 | H |
| no-assembler | clang-tidy | warning | 3 | x < 30 | 3 | F |
| non-const-parameter | clang-tidy | warning | 4 | x < 30 | 2 | H |
| non-private-member-variables-in-classes | clang-tidy | warning | 5 | x < 30 | 3 | G |
| overloaded-operator | clang-tidy | warning | 5 | 30 < x < 90 | 3 | F |
| owning-memory | clang-tidy | warning | 3 | 30 < x < 90 | 3 | E |
| performance-unnecessary-value-param | clang-tidy | warning | 4 | x < 30 | 3 | G |
| return-braced-init-list | clang-tidy | warning | 4 | x < 30 | 2 | H |
| runtime-references | clang-tidy | warning | 5 | x < 30 | 3 | G |
| signed-bitwise | clang-tidy | warning | 4 | x < 30 | 2 | H |
| simplify-boolean-expr | clang-tidy | warning | 5 | x < 30 | 3 | G |
| statically-constructed-objects | clang-tidy | warning | 4 | 30 < x < 90 | 3 | F |
| type-cstyle-cast | clang-tidy | warning | 4 | x < 30 | 3 | G |
| unused-using-decls | clang-tidy | warning | 4 | x < 30 | 2 | H |
| use-auto | clang-tidy | warning | 5 | x < 30 | 1 | H |
| use-emplace | clang-tidy | warning | 5 | x < 30 | 2 | H |
| use-nullptr | clang-tidy | warning | 5 | x < 30 | 2 | H |
| use-trailing-return-type | clang-tidy | warning | 4 | x < 30 | 2 | H |
| vararg | clang-tidy | warning | 3 | x < 30 | 2 | G |

