#include <gtest/gtest.h>
#include <regex>
#include <string>
#include <unistd.h>

#include "BasicLib/BasicLibTime.h"

const BasicLib::sint64 TOLERANCE_MS = 10;
const BasicLib::sint64 TOLERANCE_S = 1000;
const BasicLib::sint64 TOLERANCE_M = 60000;
const BasicLib::sint64 TOLERANCE_H = 3600000;

TEST(BasicLibTime, GetTime) {
    EXPECT_GT(BasicLib::GetTimeMS(), 0);
    EXPECT_NEAR(
        BasicLib::seconds(BasicLib::GetTimeS()), BasicLib::GetTimeMS(), TOLERANCE_S
    );
    EXPECT_NEAR(
        BasicLib::minutes(BasicLib::GetTimeM()), BasicLib::GetTimeMS(), TOLERANCE_M
    );
    EXPECT_NEAR(
        BasicLib::hours(BasicLib::GetTimeH()), BasicLib::GetTimeMS(), TOLERANCE_H
    );
}

TEST(BasicLibTime, TimeStamp) {
    std::regex regex("(\\d{2}):(\\d{2}):(\\d{2})");
    std::string timeStamp = BasicLib::TimeStamp();
    EXPECT_TRUE(std::regex_match(timeStamp, regex));
}

TEST(BasicLibTime, DateStamp) {
    std::regex regex("(\\d{4}).(\\d{2}).(\\d{2})");
    std::string dateStamp = BasicLib::DateStamp();
    EXPECT_TRUE(std::regex_match(dateStamp, regex));
}


TEST(BasicLibTime, Timmer) {
    BasicLib::Timer t;
    EXPECT_GT(t.GetMS(), 0);

    t.Reset(0);
    sleep(1);
    EXPECT_NEAR(t.GetMS(), 1000, TOLERANCE_MS);
}

TEST(BasicLibTime, TimmerGet) {
    BasicLib::Timer t;
    EXPECT_GT(t.GetMS(), 0);

    t.Reset(0);
    EXPECT_NEAR(t.GetMS(), 0, TOLERANCE_MS);
    EXPECT_EQ(t.GetS(), 0);
    EXPECT_EQ(t.GetM(), 0);
    EXPECT_EQ(t.GetH(), 0);
    EXPECT_EQ(t.GetD(), 0);
    EXPECT_EQ(t.GetY(), 0);

    BasicLib::sint64 testTimeMS = BasicLib::years(1);
    testTimeMS += BasicLib::days(5);
    testTimeMS += BasicLib::hours(3);
    testTimeMS += BasicLib::minutes(1);
    t.Reset(testTimeMS);
    std::string stamp = t.GetString();
    std::string expectedStamp = "1 years, 5 days, 3 hours, 1 minutes";
    EXPECT_EQ(stamp, expectedStamp);
}