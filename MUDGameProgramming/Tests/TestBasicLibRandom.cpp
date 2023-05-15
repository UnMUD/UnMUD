#include <gtest/gtest.h>

#include "BasicLib/BasicLibRandom.h"

using namespace BasicLib;

// Teste para random_percent_inclusive
TEST(RandomPercentInclusiveTest, GeneratesNumbersInRange) {
    BasicLib::random_percent_inclusive generator;

    double result = generator();
    EXPECT_GE(result, 0.0);  // Verifica se o número gerado é maior ou igual a 0.0
    EXPECT_LE(result, 1.0);  // Verifica se o número gerado é menor ou igual a 1.0
}

TEST(RandomPercentInclusiveTest, GeneratesNumbersInRangeWithSeededGenerator) {
    BasicLib::random generator;
    generator.seed(1234);
    BasicLib::random_percent_inclusive percentInclusive(generator);

    double result = percentInclusive();
    EXPECT_GE(result, 0.0);
    EXPECT_LE(result, 1.0);
}

// Teste para random_range_exclusive
TEST(RandomRangeExclusiveTest, GeneratesNumbersInRange) {
    BasicLib::random_range_exclusive generator;

    double result = generator();
    EXPECT_GE(result, 0.0);
    EXPECT_LT(result, 1.0);  // Verifica se o número gerado é estritamente menor que 1.0
}

TEST(RandomRangeExclusiveTest, GeneratesNumbersInRangeWithSeededGenerator) {
    BasicLib::random generator;
    generator.seed(1234);
    BasicLib::random_range_exclusive rangeExclusive;

    double result = rangeExclusive();
    EXPECT_GE(result, 0.0);
    EXPECT_LT(result, 1.0);
}

// Teste para random_int_range
TEST(RandomIntRangeTest, GeneratesNumbersInRange) {
    BasicLib::random_int_range<> generator;

    int result = generator();
    EXPECT_GE(result, 0);
    EXPECT_LE(result, 1);  // Verifica se o número gerado está entre 0 e 1, inclusive
}

TEST(RandomIntRangeTest, GeneratesNumbersInRangeWithSeededGenerator) {
    BasicLib::random generator;
    generator.seed(1234);
    BasicLib::random_int_range<> intRange;

    int result = intRange();
    EXPECT_GE(result, 0);
    EXPECT_LE(result, 1);

    result = intRange(5, 3);
    EXPECT_GE(result, 3);
    EXPECT_LE(result, 5);
}
