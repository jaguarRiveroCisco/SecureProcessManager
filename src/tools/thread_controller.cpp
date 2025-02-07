#include "thread_controller.h"
#include <nap_time.h>
#include <random_stuff.h>
#include "console_logger.h"

namespace cli::driver
{
    static tools::ConsoleLogger cl;

    void ThreadController::run(std::function<void(const std::string &)> commandFunc)
    {
        this->commandFunc = std::move(commandFunc);
        stopFlag          = false;
        readerThread      = std::thread(&ThreadController::runThread, this);
    }

    void ThreadController::stop()
    {
        const auto     start   = std::chrono::steady_clock::now();
        constexpr auto timeout = std::chrono::milliseconds(tools::NapTimeMs::SMALL); // Set timeout duration
        stopFlag = true;
        if (readerThread.joinable())
        {
            readerThread.join();
        }
        while (!hasStopped())
        {
            if (std::chrono::steady_clock::now() - start > timeout)
            {
                cl << "[TIMEOUT] Stopping the console loop due to timeout.";
                cl.flush(tools::LogLevel::ERROR);
                break;
            }
            sleepMs(tools::NapTimeMs::VERYSMALL);
        }
    }

    bool ThreadController::hasStopped()
    {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [this] { return stopped; });
        return stopped;
    }
}