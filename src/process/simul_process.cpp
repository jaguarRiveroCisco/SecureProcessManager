#include "simul_process.h"
#include <chrono>
#include <ctime>
#include <thread>
#include <unistd.h>
#include "communicator.h"
#include "logger_instance.h"
namespace process
{
    void ProcessSimulator::setSleepDuration()
    {
       sleepDuration_ = randomSec();
       msSleepDuration = sleepDuration_ * 1000;
       endTime_        = startTime_ + std::chrono::milliseconds(msSleepDuration);
       // Maximum allowed lifetime to prevent indefinite execution
       maxLifeTime_ = std::chrono::milliseconds(msSleepDuration + NapTimeMs::LONG); // Add a buffer to the sleep duration
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
        tools::LoggerManager::getInstance().logInfo(
                "[PROCESS EXECUTING] | Simulated Process started. Duration : " + std::to_string(sleepDuration_) + " seconds (" +
                std::to_string(msSleepDuration) + " ms)");

        while (proceed())
        {
            sleepRandomMs();
            currentTime_ = std::chrono::high_resolution_clock::now();
        }
        postWork();
    }
} // namespace process
