#include <gtest/gtest.h>
#include <thread>
#include <condition_variable>
#include "main_controller.h"
#include "ConfigReader.h"
#include "logger_instance.h"
#include "ProcessRegistry.h"
#include "communicator.h"

class MainControllerTest : public ::testing::Test {
protected:
    process::MainController mainController;

    void SetUp() override {
        // Initialize the logger
        tools::LoggerManager::createLoggerType();

    }

    void TearDown() override {
        // Cleanup code if needed
    }
};

TEST_F(MainControllerTest, InitializeController)
{
    process::MainController::initializeController("simul", 5);
    ASSERT_EQ(process::ProcessController::numProcesses(), 5);
}

TEST_F(MainControllerTest, ProcessLifecycleLoop)
{
    std::thread lifecycleThread(&process::MainController::processLifecycleLoop);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate some processing time
    process::MainController::stop();
    lifecycleThread.join();
    ASSERT_FALSE(process::ProcessController::running());
}