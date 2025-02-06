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
        TimeManager()                               = default;
        TimeManager(const TimeManager &)            = delete;
        TimeManager &operator=(const TimeManager &) = delete;

        std::chrono::milliseconds::rep                               getElapsedMilliseconds() const;
        std::chrono::time_point<std::chrono::high_resolution_clock> &currentTime();

        bool                                                         endOfLife() const;
        bool                                                         maxLifeExceeded() const;

        int                                                          sleepDurationMs() const;
        void                                                         setSleepDuration();
        std::string                                                  getSleepDurationStr() const;
        std::string                                                  getFormattedElapsedTimeStr() const;

        std::string timeToStr(long long elapsedMilliseconds) const;

    private:
        std::chrono::milliseconds                                   maxLifeTime_ = std::chrono::milliseconds(0);
        std::chrono::time_point<std::chrono::high_resolution_clock> startTime_ =
                std::chrono::high_resolution_clock::now();
        std::chrono::time_point<std::chrono::high_resolution_clock> endTime_ =
                std::chrono::high_resolution_clock::now();
        mutable std::chrono::time_point<std::chrono::high_resolution_clock> currentTime_ =
                std::chrono::high_resolution_clock::now();

        int         sleepDurationMs_  = 0;
        std::string sleepDurationStr_ = "0 milliseconds";
        std::string units_            = " milliseconds";
    };
} // namespace tools

#endif // TIME_MANAGER_H
