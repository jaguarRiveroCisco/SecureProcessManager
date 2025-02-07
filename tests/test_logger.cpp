// tests/test_logger.cpp
#include <gtest/gtest.h>
#include "logger_instance.h"

class LoggerTest : public ::testing::Test {
    protected:
    void SetUp() override {
        // Initialize the logger
        tools::LoggerManager::createLoggerType();
    }

    void TearDown() override {
        // Cleanup code if needed
    }
};

TEST_F(LoggerTest, LogInfo) {
    tools::LoggerManager::getInstance().logInfo("Test message");
    // Add assertions to verify the behavior
}