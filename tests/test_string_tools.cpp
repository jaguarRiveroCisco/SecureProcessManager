#include <gtest/gtest.h>
#include "string_tools.h"
#include "logger_instance.h"

class StringToolsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize the logger
        tools::LoggerManager::createLoggerType();
    }

    void TearDown() override {
        // Cleanup code if needed
    }
};

TEST_F(StringToolsTest, SplitString) {
    constexpr std::string        input     = "one,two,three";
    constexpr char           delimiter = ',';
    std::vector<std::string> expected  = {"one", "two", "three"};
    std::vector<std::string> result    = tools::string::splitString(input, delimiter);
    ASSERT_EQ(result, expected);
}

TEST_F(StringToolsTest, StrToPidValid) {
    const std::string pidStr   = "12345";
    constexpr pid_t   expected = 12345;
    const pid_t       result   = tools::string::strToPid(pidStr);
    ASSERT_EQ(result, expected);
}

TEST_F(StringToolsTest, StrToPidInvalid) {
    constexpr std::string pidStr   = "invalid";
    constexpr pid_t   expected = -1;
    const pid_t       result   = tools::string::strToPid(pidStr);
    ASSERT_EQ(result, expected);
}

TEST_F(StringToolsTest, StrToPidOutOfRange) {
    const std::string pidStr = "999999999999999999999999999999";
    constexpr pid_t expected = -1;
    const pid_t result = tools::string::strToPid(pidStr);
    ASSERT_EQ(result, expected);
}

TEST_F(StringToolsTest, CreateCStyleArgs) {
    const std::vector<std::string> args = {"arg1", "arg2", "arg3"};
    const std::vector<char*> c_args = tools::string::createCStyleArgs(args);
    ASSERT_EQ(c_args.size(), 4); // 3 args + 1 null-terminator
    ASSERT_STREQ(c_args[0], "arg1");
    ASSERT_STREQ(c_args[1], "arg2");
    ASSERT_STREQ(c_args[2], "arg3");
    ASSERT_EQ(c_args[3], nullptr);
}