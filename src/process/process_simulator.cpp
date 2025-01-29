#include "process_simulator.h"
#include <chrono>
#include <ctime>
#include <thread>
#include <unistd.h>
#include "communicator.h"
#include "logger_instance.h"
#include "random_stuff.h"

namespace process
{
    bool ProcessSimulator::proceed()
    {
        bool res = continue_;
        if (res)
        {
            if (timeManager_.endOfLife())
            {
                res = false;
            }
            else if (timeManager_.maxLifeExceeded())
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
        while (proceed())
        {
            tools::sleepRandomMs();
            timeManager_.currentTime() = std::chrono::high_resolution_clock::now();
        }
        postWork();
    }
} // namespace process
