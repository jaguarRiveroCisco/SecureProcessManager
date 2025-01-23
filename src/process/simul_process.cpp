#include "simul_process.h"
#include <chrono>
#include <ctime>
#include <thread>
#include <unistd.h>
#include "communicator.h"
#include "logger_instance.h"
#include "nap_time.h"
#include "random_stuff.h"

namespace process
{
    void ProcessSimulator::setSleepDuration()
    {
        int val = 0;
        switch (tools::SleepTime::type)
        {
            case tools::NapType::SEC:
                val               = tools::randomSec();
                sleepDurationMs_  = val * 1000; // Convert seconds to milliseconds
                sleepDurationStr_ = std::to_string(val) + " seconds (" + std::to_string(sleepDurationMs_) + " ms)";
                break;

            case tools::NapType::MIN:
                val               = tools::randomMin();
                sleepDurationMs_  = val * 60 * 1000; // Convert minutes to milliseconds
                sleepDurationStr_ = std::to_string(val) + " minutes (" + std::to_string(sleepDurationMs_) + " ms)";
                break;

            default:
                val               = tools::randomMs();
                sleepDurationMs_  = val;
                sleepDurationStr_ = std::to_string(val) + " milliseconds";
                break;
        }

        // Calculate end time and maximum allowed lifetime
        endTime_     = startTime_ + std::chrono::milliseconds(sleepDurationMs_);
        maxLifeTime_ = std::chrono::milliseconds(sleepDurationMs_ +
                                                 tools::NapTimeMs::LONG); // Add a buffer to the sleep duration

        // Log the information
        tools::LoggerManager::getInstance().logInfo("[PROCESS EXECUTING] | Simulated Process started. Duration: " +
                                                    std::to_string(sleepDurationMs_ / 60000) + " minutes (" +
                                                    std::to_string(sleepDurationMs_ / 1000) + " seconds, " +
                                                    std::to_string(sleepDurationMs_) + " ms)");
    }

    bool ProcessSimulator::proceed()
    {
        bool res = continue_;
        if (res)
        {
            if (currentTime_ >= endTime_)
            {
                res = false;
            }
            else if (currentTime_ - startTime_ >= maxLifeTime_)
            {
                reason_ = "Maximum allowed lifetime exceeded";
                res = false;
            }
            else if (getppid() == 1)
            {
                reason_ = "Parent process has terminated. Exiting child process.";
                res = false;
            }
        }

        return res;
    }

    void ProcessSimulator::work()
    {
        preWork();
        setSleepDuration();
        while (proceed())
        {
            tools::sleepRandomMs();
            currentTime_ = std::chrono::high_resolution_clock::now();
        }
        postWork();
    }
} // namespace process
