#ifndef SIMUL_PROCESS_H
#define SIMUL_PROCESS_H
#include "process.h"

namespace process
{
    class ProcessSimulator : public Process 
    {
    public:
        void work() override;
        virtual ~ProcessSimulator() = default;

    protected:
        void setSleepDuration();
        bool proceed();
        int        sleepDurationMs_ = 0;
        std::string sleepDurationStr_;
        int        msSleepDuration = 0;
        std::chrono::time_point<std::chrono::high_resolution_clock> endTime_;
        std::chrono::time_point<std::chrono::high_resolution_clock> currentTime_ = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds                                    maxLifeTime_;
    };
} // namespace process

#endif // SIMUL_PROCESS_H
