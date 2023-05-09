#include <gtest/gtest.h>
#include <vector>

#include "BasicLib/BasicLibFunctions.h"

TEST(BasicLibFunctions, DoubleFindIf) {
    auto is_five = [](int i) { return i == 5; };
    auto is_seven = [](int i) { return i == 7; };
    std::vector<int> v = {0, 1, 2, 3, 4, 5};
    auto v_iterator = v.begin() + 5;
    EXPECT_EQ(BasicLib::double_find_if(v.begin(), v.end(), is_five, is_seven), v_iterator);
    
    v[3] = 7;
    v[5] = 3;
    v_iterator = v.begin() + 3;
    EXPECT_EQ(BasicLib::double_find_if(v.begin(), v.end(), is_five, is_seven), v_iterator);
    
    v[3] = 8;
    EXPECT_EQ(BasicLib::double_find_if(v.begin(), v.end(), is_five, is_seven), v.end());

    auto invalid_iter = v.begin() - 1;
    EXPECT_EQ(BasicLib::double_find_if(invalid_iter, v.end(), is_five, is_seven), v.end());
}

TEST(BasicLibFunctions, DoubleFindIfWithQualifier) {
    auto is_five = [](int i) { return i == 5; };
    auto is_seven = [](int i) { return i == 7; };
    auto is_even = [](int i) { return i % 2 == 0; };
    auto is_odd = [](int i) { return i % 2 == 1; };

    std::vector<int> v = {0, 1, 2, 3, 4, 5};
    auto v_iterator = v.begin() + 5;
    EXPECT_EQ(BasicLib::double_find_if(
        v.begin(), v.end(), is_five, is_seven, is_odd
    ), v_iterator);

    EXPECT_EQ(BasicLib::double_find_if(
        v.begin(), v.end(), is_five, is_seven, is_even
    ), v.end());
    
    v[3] = 7;
    v[5] = 3;
    v_iterator = v.begin() + 3;
    EXPECT_EQ(BasicLib::double_find_if(
        v.begin(), v.end(), is_five, is_seven, is_odd
    ), v_iterator);

    EXPECT_EQ(BasicLib::double_find_if(
        v.begin(), v.end(), is_five, is_seven, is_even
    ), v.end());

    
    v[3] = 8;
    EXPECT_EQ(BasicLib::double_find_if(
        v.begin(), v.end(), is_five, is_seven, is_odd
    ), v.end());

    EXPECT_EQ(BasicLib::double_find_if(
        v.begin(), v.end(), is_five, is_seven, is_even
    ), v.end());

    auto invalid_iter = v.begin() - 1;
    EXPECT_EQ(BasicLib::double_find_if(
        invalid_iter, v.end(), is_five, is_seven, is_odd
    ), v.end());
}

TEST(BasicLibFunctions, OperateOnIf) {
    auto is_five = [](int i) { return i == 5; };
    auto is_seven = [](int i) { return i == 7; };
    
    bool check = false;
    auto confirm_check = [&check](int i) { check = true; };
    std::vector<int> v = {0, 1, 2, 3, 4, 5};
    BasicLib::operate_on_if(v.begin(), v.end(), confirm_check, is_five);
    EXPECT_TRUE(check);
    check = false;
    BasicLib::operate_on_if(v.begin(), v.end(), confirm_check, is_seven);
    EXPECT_FALSE(check);
}

TEST(BasicLibFunctions, Percent) {
    EXPECT_EQ(BasicLib::percent(5, 10), 50);
    EXPECT_EQ(BasicLib::percent(-5, 10), -50);
    EXPECT_EQ(BasicLib::percent(-5, -10), 50);
    EXPECT_EQ(BasicLib::percent(0, 10), 0);
    ASSERT_THROW(BasicLib::percent(5, 0), std::exception);
}

TEST(BasicLibFunctions, ASCIIToHex) {
    EXPECT_EQ(BasicLib::ASCIIToHex('0'), 0);
    EXPECT_EQ(BasicLib::ASCIIToHex('a'), 10);
    EXPECT_EQ(BasicLib::ASCIIToHex('A'), 10);
    EXPECT_EQ(BasicLib::ASCIIToHex('@'), 0);
    EXPECT_EQ(BasicLib::ASCIIToHex('\0'), 0);
}

TEST(BasicLibFunctions, Always) {
    BasicLib::always<int> a;
    int *i;
    EXPECT_TRUE(a(*i));
}
