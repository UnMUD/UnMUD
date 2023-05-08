#include <gtest/gtest.h>

#include "Tests.h"
#include "BasicLib/BasicLibString.h"

TEST(BasicLibString, UpperCase)
{
    EXPECT_EQ(BasicLib::UpperCase(""), "");
    EXPECT_EQ(BasicLib::UpperCase("hello"), "HELLO");
    EXPECT_EQ(BasicLib::UpperCase("WORLD"), "WORLD");
    EXPECT_EQ(BasicLib::UpperCase("HeLLo"), "HELLO");
    EXPECT_EQ(BasicLib::UpperCase("H&ll0"), "H&LL0");
}

TEST(BasicLibString, LowerCase)
{
    EXPECT_EQ(BasicLib::LowerCase(""), "");
    EXPECT_EQ(BasicLib::LowerCase("hello"), "hello");
    EXPECT_EQ(BasicLib::LowerCase("WORLD"), "world");
    EXPECT_EQ(BasicLib::LowerCase("HeLLo"), "hello");
    EXPECT_EQ(BasicLib::LowerCase("H&ll0"), "h&ll0");
}

TEST(BasicLibString, SearchAndReplace)
{
    EXPECT_EQ(BasicLib::SearchAndReplace("Hello world", "world", "hello"), "Hello hello");
    EXPECT_EQ(BasicLib::SearchAndReplace("Hello world", "w0rld", "hello"), "Hello world");
    ASSERT_DURATION_LE(1, {
        EXPECT_EQ(BasicLib::SearchAndReplace("Hello world", "", "hello"), "Hello world"); // breaking tests
    });
    EXPECT_EQ(BasicLib::SearchAndReplace("Hello world", "world", ""), "Hello ");
    EXPECT_EQ(BasicLib::SearchAndReplace("Hello w@rld!", "w@rld!", "world"), "Hello world");
    EXPECT_EQ(BasicLib::SearchAndReplace("Hello world", "world", "w@rld!"), "Hello w@rld!");
    EXPECT_EQ(BasicLib::SearchAndReplace("", "world", "hello"), "");
}

TEST(BasicLibString, TrimWhitespace)
{
    EXPECT_EQ(BasicLib::TrimWhitespace("Hello world"), "Hello world");
    EXPECT_EQ(BasicLib::TrimWhitespace(" Hello world"), "Hello world");
    EXPECT_EQ(BasicLib::TrimWhitespace("\tHello world"), "Hello world");
    EXPECT_EQ(BasicLib::TrimWhitespace("\nHello world"), "Hello world");
    EXPECT_EQ(BasicLib::TrimWhitespace("\rHello world"), "Hello world");
    EXPECT_EQ(BasicLib::TrimWhitespace("\fHello world"), "Hello world");
    EXPECT_EQ(BasicLib::TrimWhitespace("Hello world "), "Hello world");
    EXPECT_EQ(BasicLib::TrimWhitespace("Hello world\t"), "Hello world");
    EXPECT_EQ(BasicLib::TrimWhitespace("Hello world\n"), "Hello world");
    EXPECT_EQ(BasicLib::TrimWhitespace("Hello world\r"), "Hello world");
    EXPECT_EQ(BasicLib::TrimWhitespace("Hello world\f"), "Hello world");
    EXPECT_EQ(BasicLib::TrimWhitespace(""), "");
}

TEST(BasicLibString, ParseWord)
{
    EXPECT_EQ(BasicLib::ParseWord("Hello world", 1), "world");
    EXPECT_EQ(BasicLib::ParseWord("Hello world", 2), "");
    EXPECT_EQ(BasicLib::ParseWord("Hello world", -1), "");
    EXPECT_EQ(BasicLib::ParseWord("", 0), "");
    EXPECT_EQ(BasicLib::ParseWord("  Hello   world  ", 1), "world");
}

TEST(BasicLibString, RemoveWord)
{
    EXPECT_EQ(BasicLib::RemoveWord("Hello world", 1), "Hello ");
    EXPECT_EQ(BasicLib::RemoveWord("Hello world", 2), "Hello world");
    EXPECT_EQ(BasicLib::RemoveWord("Hello world", -1), "Hello world");
    EXPECT_EQ(BasicLib::RemoveWord("", 0), "");
    EXPECT_EQ(BasicLib::RemoveWord("  Hello   world  ", 1), "  Hello   ");
}