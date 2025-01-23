#include "time_manager.h"
#include "logger_instance.h"
#include "nap_time.h"
#include "random_stuff.h"

namespace tools
{
    void TimeManager::setSleepDuration()
    {
        int val = 0;
        switch (tools::SleepTime::type)
        {
            case tools::NapType::SEC:
                val               = tools::randomSec();
                sleepDurationMs_  = val * 1000;
                sleepDurationStr_ = std::to_string(val) + " seconds (" + std::to_string(sleepDurationMs_) + " ms)";
                break;
            case tools::NapType::MIN:
                val               = tools::randomMin();
                sleepDurationMs_  = val * 60 * 1000;
                sleepDurationStr_ = std::to_string(val) + " minutes (" + std::to_string(sleepDurationMs_) + " ms)";
                break;
            default:
                val               = tools::randomMs();
                sleepDurationMs_  = val;
                sleepDurationStr_ = std::to_string(val) + " milliseconds";
                break;
        }

        endTime_     = startTime_ + std::chrono::milliseconds(sleepDurationMs_);
        maxLifeTime_ = std::chrono::milliseconds(sleepDurationMs_ + tools::NapTimeMs::LONG);

        // Log the information
        tools::LoggerManager::getInstance().logInfo("[PROCESS EXECUTING] | Simulated Process started. Duration: " +
                                                    std::to_string(sleepDurationMs_ / 60000) + " minutes (" +
                                                    std::to_string(sleepDurationMs_ / 1000) + " seconds, " +
                                                    std::to_string(sleepDurationMs_) + " ms)");
    }

    std::chrono::milliseconds::rep TimeManager::getElapsedMilliseconds() const
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto duration    = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime_);
        return duration.count();
    }
}