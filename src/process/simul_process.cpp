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
        // Get random duration in minutes
        int sleepDurationMin = tools::randomMin();
        
        // Convert minutes to seconds and milliseconds
        int sleepDurationSec = sleepDurationMin * 60;
        int msSleepDuration = sleepDurationSec * 1000;
        
        // Calculate end time and maximum allowed lifetime
        endTime_ = startTime_ + std::chrono::milliseconds(msSleepDuration);
        maxLifeTime_ = std::chrono::milliseconds(msSleepDuration + tools::NapTimeMs::LONG); // Add a buffer to the sleep duration
        
        // Log the information
        tools::LoggerManager::getInstance().logInfo(
            "[PROCESS EXECUTING] | Simulated Process started. Duration : " + std::to_string(sleepDurationMin) +
            " minutes (" + std::to_string(sleepDurationSec) + " seconds, " + std::to_string(msSleepDuration) + " ms)");
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
