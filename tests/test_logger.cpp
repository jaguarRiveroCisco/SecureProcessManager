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
    tools::LoggerManager::getInstance().logInfo("LogInfo: Test message");
    // Add assertions to verify the behavior
}

TEST_F(LoggerTest, LogWarning) {
    tools::LoggerManager::getInstance().logWarning("LogWarning: Test message");
    // Add assertions to verify the behavior
}

TEST_F(LoggerTest, LogError) {
    tools::LoggerManager::getInstance().logError("LogError: Test message");
    // Add assertions to verify the behavior
}

TEST_F(LoggerTest, LogException) {
    tools::LoggerManager::getInstance().logException("LogException: Test message");
    // Add assertions to verify the behavior
}