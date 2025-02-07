#include <gtest/gtest.h>
#include "cli_controller.h"
#include "logger_instance.h"
#include "nap_time.h"
#include "random_stuff.h"

namespace
{
    class TestThreadController final : public cli::driver::ThreadController
    {
    public:
        void runThread() override
        {
            std::unique_lock<std::mutex> lock(mutex);
            while (!stopFlag)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                if (commandFunc)
                {
                    commandFunc("test command");
                }
                stopFlag = true;
            }
            stopped = true;
            cv.notify_one();
        }

    };

    void DummyCommandFunction(const std::string& command)
    {
        // Handle the command
        tools::LoggerManager::getInstance().logInfo("TestThreadController | Command received: " + command);
    }

    TEST(ThreadControllerTest, TestRunAndStop)
    {
        tools::LoggerManager::getInstance().logInfo("ThreadControllerTest, TestRunAndStop");
        TestThreadController controller;
        controller.run(DummyCommandFunction);

        sleepMs(tools::NapTimeMs::SMALL);

        controller.stop();

        // Ensure the thread is properly stopped
        ASSERT_TRUE(controller.hasStopped());
    }
}
