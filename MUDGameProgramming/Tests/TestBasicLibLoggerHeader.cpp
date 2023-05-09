#include <gtest/gtest.h>
#include <fstream>
#include <regex>

#include "BasicLib/BasicLibLogger.h"

std::string GetLineFromFile(const std::string& filename, int lineNumber) {
  std::ifstream inputFile(filename);
  std::string line;
  int currentLineNumber = 0;
  
  while (std::getline(inputFile, line)) {
    currentLineNumber++;
    if (currentLineNumber == lineNumber) {
      return line;
    }
  }

  return "";
}

class BasicLibLoggerTest : public testing::Test {
protected:
    void TearDown() override {
        ASSERT_TRUE(remove((m_tempLogFileName).c_str()) == 0);
    }

    std::string m_tempLogFileName = "test_log.txt";
    std::string m_tempLogFileHeader = "Test Log";
    std::string m_tempLogFileLogText = "Testing Log Function";
};

TEST_F(BasicLibLoggerTest, GetFileList) {
    BasicLib::TextLog *TESTLOG = new BasicLib::TextLog( m_tempLogFileName, m_tempLogFileHeader, true, true );
    TESTLOG->Log(m_tempLogFileLogText);
    delete(TESTLOG);

    std::string testLine = GetLineFromFile(m_tempLogFileName, 2);
    EXPECT_NE(testLine.find(m_tempLogFileHeader), std::string::npos);

    testLine = GetLineFromFile(m_tempLogFileName, 6);
    EXPECT_NE(testLine.find("Session opened."), std::string::npos);

    std::regex regex("\\[(\\d{4}).(\\d{2}).(\\d{2})\\] \\[(\\d{2}):(\\d{2}):(\\d{2})\\] .+");
    testLine = GetLineFromFile(m_tempLogFileName, 7);
    EXPECT_TRUE(std::regex_match(testLine, regex));

    testLine = GetLineFromFile(m_tempLogFileName, 8);
    EXPECT_NE(testLine.find("Session closed."), std::string::npos);

    TESTLOG = new BasicLib::TextLog( m_tempLogFileName, m_tempLogFileHeader, false, false );
    TESTLOG->Log(m_tempLogFileLogText);
    delete(TESTLOG);

    testLine = GetLineFromFile(m_tempLogFileName, 11);
    EXPECT_NE(testLine.find("Session opened."), std::string::npos);

    testLine = GetLineFromFile(m_tempLogFileName, 12);
    EXPECT_FALSE(std::regex_match(testLine, regex));

    testLine = GetLineFromFile(m_tempLogFileName, 13);
    EXPECT_NE(testLine.find("Session closed."), std::string::npos);
}