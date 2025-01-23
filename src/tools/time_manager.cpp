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
                break;
            case tools::NapType::MIN:
                val               = tools::randomMin();
                sleepDurationMs_  = val * 60 * 1000;
                break;
            default:
                val               = tools::randomMs();
                sleepDurationMs_  = val;
                break;
        }

        sleepDurationStr_ = timeToStr(sleepDurationMs_);
        endTime_     = startTime_ + std::chrono::milliseconds(sleepDurationMs_);
        maxLifeTime_ = std::chrono::milliseconds(sleepDurationMs_ + tools::NapTimeMs::LONG);

        // Log the information
        tools::LoggerManager::getInstance().logInfo("[PROCESS EXECUTING] | Simulated Process started. Duration: " +
                                                    sleepDurationStr_);
    }

    std::chrono::milliseconds::rep TimeManager::getElapsedMilliseconds() const
    {
        currentTime_ = std::chrono::high_resolution_clock::now();
        auto duration    = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime_ - startTime_);
        return duration.count();
    }

    std::chrono::time_point<std::chrono::high_resolution_clock> &TimeManager::currentTime() { return currentTime_; }

    int         TimeManager::sleepDurationMs() const { return sleepDurationMs_; }

    bool        TimeManager::endOfLife() const { return currentTime_ >= endTime_; }

    bool        TimeManager::maxLifeExceeded() const { return currentTime_ - startTime_ >= maxLifeTime_; }

    std::string TimeManager::getSleepDurationStr() const { return sleepDurationStr_; }

    std::string TimeManager::getFormattedElapsedTimeStr() const
    {
        auto elapsedMilliseconds = getElapsedMilliseconds();
        return timeToStr(elapsedMilliseconds);
    }

    std::string TimeManager::timeToStr(long long elapsedMilliseconds) const
    {
        return std::to_string(elapsedMilliseconds / 60000) + " minutes (" + std::to_string(elapsedMilliseconds / 1000) +
               " seconds, " + std::to_string(elapsedMilliseconds) + " ms)";
    }
} // namespace tools
