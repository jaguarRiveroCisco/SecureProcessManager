#include <gtest/gtest.h>
#include <condition_variable>
#include <mutex>
#include "cli_controller.h"
#include "logger_instance.h"

namespace {
    class TestThreadController : public cli::driver::ThreadController {
    public:
        void runThread() override {
            std::unique_lock<std::mutex> lock(mutex);
            while (!stopFlag) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                if (commandFunc) {
                    commandFunc("test command");
                }
                stopFlag = true;
            }
            stopped = true;
            cv.notify_one();
        }

        bool hasStopped() {
            std::unique_lock<std::mutex> lock(mutex);
            cv.wait(lock, [this] { return stopped; });
            return stopped;
        }

    private:
        std::mutex mutex;
        std::condition_variable cv;
        bool stopped = false;
    };

    void DummyCommandFunction(const std::string& command) {
        // Handle the command
        tools::LoggerManager::getInstance().logInfo("TestThreadController | Command received: " + command);
    }

    TEST(ThreadControllerTest, TestRunAndStop) {
        TestThreadController controller;
        controller.run(DummyCommandFunction);

        // Let the thread run for a short period
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        controller.stop();

        // Ensure the thread is properly stopped
        ASSERT_TRUE(controller.hasStopped());
    }
}
