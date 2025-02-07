#include <gtest/gtest.h>
#include <thread>
#include <atomic>
#include "base_process.h"
#include "communicator.h"
#include "logger_instance.h"

class BaseProcessTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize the logger
        tools::LoggerManager::createLoggerType();
    }

    void TearDown() override {
        // Cleanup code if needed
    }
};

TEST_F(BaseProcessTest, ContinueFlag) {
    tools::LoggerManager::getInstance().logInfo("process::BaseProcess::continueFlag()");

    std::atomic<bool>& continueFlag = process::BaseProcess::continueFlag();
    continueFlag = false;
    ASSERT_FALSE(continueFlag.load());
}

TEST_F(BaseProcessTest, ExitCode) {
    tools::LoggerManager::getInstance().logInfo("process::BaseProcess::exitCode()");
    std::atomic<int>& exitCode = process::BaseProcess::exitCode();
    exitCode = 1;
    ASSERT_EQ(exitCode.load(), 1);
}
