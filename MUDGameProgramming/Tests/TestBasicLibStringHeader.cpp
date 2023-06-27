#include <gtest/gtest.h>
#include <sstream>

#include "BasicLib/BasicLibString.h"

class CustomObj {
public:
    std::string name;

    CustomObj() {}
    CustomObj(const std::string& name) : name(name) {}

    friend std::ostream& operator<<(std::ostream& os, const CustomObj& obj) {
        os << obj.name;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, CustomObj& obj) {
        is >> obj.name;
        return is;
    }

    bool operator==(const CustomObj& other) const {
        return name == other.name;
    }
};

TEST(BasicLibStringHeader, Insert) {
    std::stringstream ss;
    
    int intValue = 10;
    BasicLib::insert(ss, intValue);
    EXPECT_EQ(ss.str(), "10");
    ss.str(std::string());

    float floatValue = 3.1415;
    BasicLib::insert(ss, floatValue);
    EXPECT_EQ(ss.str(), "3.1415");
    ss.str(std::string());

    char charValue = 'a';
    BasicLib::insert(ss, charValue);
    EXPECT_EQ(ss.str(), "a");
    ss.str(std::string());
    
    CustomObj customValue("custom");
    BasicLib::insert(ss, customValue);
    EXPECT_EQ(ss.str(), "custom");
    ss.str(std::string());

    BasicLib::insert(ss, intValue);
    BasicLib::insert(ss, floatValue);
    BasicLib::insert(ss, charValue);
    BasicLib::insert(ss, customValue);
    EXPECT_EQ(ss.str(), "103.1415acustom");
}

TEST(BasicLibStringHeader, Extract) {
    std::stringstream ss;
    ss << 10;
    int intValue;
    BasicLib::extract(ss, intValue);
    EXPECT_EQ(10, intValue);

    ss.str(std::string());
    ss.clear();
    ss << 3.1415;
    float floatValue;
    BasicLib::extract(ss, floatValue);
    EXPECT_FLOAT_EQ(3.1415, floatValue);

    ss.str(std::string());
    ss.clear();
    ss << 'a';
    char charValue;
    BasicLib::extract(ss, charValue);
    EXPECT_EQ('a', charValue);

    ss.str(std::string());
    ss.clear();
    CustomObj customValue, compareCustomValue("custom");
    ss << compareCustomValue;
    BasicLib::extract(ss, customValue);
    EXPECT_EQ(compareCustomValue, customValue);

    ss.str(std::string());
    ss.clear();
    ss << "custom";
    float value = 0;
    ASSERT_THROW(BasicLib::extract(ss, value), std::exception);
}

TEST(BasicLibStringHeader, ToString) {
    EXPECT_EQ(BasicLib::tostring(10, 2), "10");
    EXPECT_EQ(BasicLib::tostring(10.2f, 4), "10.2");
    EXPECT_EQ(BasicLib::tostring('a', 1), "a");

    CustomObj customValue("custom");
    EXPECT_EQ(BasicLib::tostring(customValue, 6), "custom");
    EXPECT_EQ(BasicLib::tostring(10, 5), "10   ");
    EXPECT_EQ(BasicLib::tostring(10, 0), "10");
}

TEST(BasicLibStringHeader, ToType) {
    EXPECT_EQ(BasicLib::totype<int>("10"), 10);
    EXPECT_EQ(BasicLib::totype<float>("10.2"), 10.2f);
    EXPECT_EQ(BasicLib::totype<char>("a"), 'a');
    CustomObj customValue("custom");
    EXPECT_EQ(BasicLib::totype<CustomObj>("custom"), customValue);
    EXPECT_EQ(BasicLib::totype<std::string>("str"), "str");
    EXPECT_EQ(BasicLib::totype<int>("  10"), 10);
    EXPECT_EQ(BasicLib::totype<int>("10  "), 10);
}
