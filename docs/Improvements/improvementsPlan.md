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
| A has not been declared | gcc | error | | | | |
| A was not declared in this scope | gcc | error | | | | |
| dependent-name A is parsed as a non-type, but instantiation yields a type | gcc | error | | | | |
| invalid storage class for function | gcc | error | | | | |
| multiple definition of | gcc | error | | | | |
| need ‘typename’ before A because B is a dependent scope | gcc | error | | | | |
| no matching function for call to A | gcc | error | | | | |
| No such file or directory | bash | error | | | | |
| Segmentation fault (core dumped) | gcc | error | | | | |
| specializing member A requires ‘template<>’ syntax | gcc | error | | | | |
| static declaration of A follows non-static declaration | gcc | error | | | | |
| ‘sys_errlist’ undeclared | gcc | error | | | | |
| there are no arguments to ‘log’ that depend on a template parameter, so a declaration of ‘log’ must be available | gcc | error | | | | |
| TLS definition in A section .tbss mismatches non-TLS reference in B | gcc | error | | | | |
| variable or field A declared void | gcc | error | | | | |
| catching polymorphic type | gcc | warning | | | | |
| conversion from A to B may change value | gcc | warning | | | | |
| comparison of integer expressions of different signedness | gcc | warning | | | | |
| conversion to B from A may change the sign of the result | gcc | warning | | | | |
| declaration of A shadows a global declaration | gcc | warning | | | | |
| has pointer data members A but does not override B or C | gcc | warning | | | | |
| should be initialized in the member initialization list | gcc | warning | | | | |
| unused parameter | gcc | warning | | | | |
| use of old-style cast | gcc | warning | | | | |
| constParameter | cppcheck | warning | | | | |
| cstyleCast | cppcheck | warning | | | | |
| noExplicitConstructor | cppcheck | warning | | | | |
| passedByValue | cppcheck | warning | | | | |
| selfAssignment | cppcheck | warning | | | | |
| uninitMemberVar | cppcheck | warning | | | | |
| unreadVariable | cppcheck | warning | | | | |
| unusedFunction | cppcheck | warning | | | | |
| useInitializationList | cppcheck | warning | | | | |
| android-cloexec-accept | clang-tidy | warning | | | |
| array-to-pointer-decay | clang-tidy | warning | | | |
| avoid-c-arrays | clang-tidy | warning | | | |
| avoid-c-arrays | clang-tidy | warning | | | |
| bounds-constant-array-index | clang-tidy | warning | | | |
| bounds-pointer-arithmetic | clang-tidy | warning | | | |
| braces-around-statements | clang-tidy | warning | | | |
| bugprone-macro-parentheses | clang-tidy | warning | | | |
| build-using-namespace | clang-tidy | warning | | | |
| casting | clang-tidy | warning | | | |
| cert-err58-cpp | clang-tidy | warning | | | |
| container-size-empty | clang-tidy | warning | | | |
| convert-member-functions-to-static | clang-tidy | warning | | | |
| default-arguments-calls | clang-tidy | warning | | | |
| deprecated-headers | clang-tidy | warning | | | |
| else-after-return | clang-tidy | warning | | | |
| header-guard | clang-tidy | warning | | | |
| implicit-bool-conversion | clang-tidy | warning | | | |
| include-order | clang-tidy | warning | | | |
| init-variables | clang-tidy | warning | | | |
| isolate-declaration | clang-tidy | warning | | | |
| loop-convert | clang-tidy | warning | | | |
| macro-usage | clang-tidy | warning | | | |
| magic-numbers | clang-tidy | warning | | | |
| make-member-function-const | clang-tidy | warning | | | |
| member-init | clang-tidy | warning | | | |
| namespace-comments | clang-tidy | warning | | | |
| no-assembler | clang-tidy | warning | | | |
| non-const-parameter | clang-tidy | warning | | | |
| non-private-member-variables-in-classes | clang-tidy | warning | | | |
| overloaded-operator | clang-tidy | warning | | | |
| owning-memory | clang-tidy | warning | | | |
| performance-unnecessary-value-param | clang-tidy | warning | | | |
| return-braced-init-list | clang-tidy | warning | | | |
| runtime-references | clang-tidy | warning | | | |
| signed-bitwise | clang-tidy | warning | | | |
| simplify-boolean-expr | clang-tidy | warning | | | |
| statically-constructed-objects | clang-tidy | warning | | | |
| type-cstyle-cast | clang-tidy | warning | | | |
| unused-using-decls | clang-tidy | warning | | | |
| use-auto | clang-tidy | warning | | | |
| use-emplace | clang-tidy | warning | | | |
| use-nullptr | clang-tidy | warning | | | |
| use-trailing-return-type | clang-tidy | warning | | | |
| vararg | clang-tidy | warning | | | |