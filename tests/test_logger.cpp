// tests/test_logger.cpp
#include <gtest/gtest.h>
#include "logger_instance.h"

TEST(LoggerTest, LogInfo) {
    tools::LoggerManager::getInstance().logInfo("Test message");
    // Add assertions to verify the behavior
}