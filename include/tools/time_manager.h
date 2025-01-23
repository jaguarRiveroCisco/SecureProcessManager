#pragma once

#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <chrono>
#include <string>

namespace tools
{
    class TimeManager final
    {
    public:
        TimeManager() = default;
        TimeManager(const TimeManager&) = delete;
        TimeManager& operator=(const TimeManager&) = delete;

        void                           setSleepDuration();
        std::chrono::milliseconds::rep getElapsedMilliseconds() const;
        std::string                    getSleepDurationStr() const { return sleepDurationStr_; }

        // Reference accessor functions
        std::chrono::time_point<std::chrono::high_resolution_clock>& currentTime() { return currentTime_; }
        int sleepDurationMs() const { return sleepDurationMs_; }
        bool endOfLife() const
        {
            return currentTime_ >= endTime_;
        }
        bool maxLifeExceeded() const
        {
            return currentTime_ - startTime_ >= maxLifeTime_;
        }

    private:
        std::chrono::milliseconds                                   maxLifeTime_ = std::chrono::milliseconds(0);
        std::chrono::time_point<std::chrono::high_resolution_clock> startTime_= std::chrono::high_resolution_clock::now();
        std::chrono::time_point<std::chrono::high_resolution_clock> endTime_= std::chrono::high_resolution_clock::now();
        std::chrono::time_point<std::chrono::high_resolution_clock> currentTime_ = std::chrono::high_resolution_clock::now();

        int         sleepDurationMs_ = 0;
        std::string sleepDurationStr_ = "0 milliseconds";
    };
}

#endif // TIME_MANAGER_H
