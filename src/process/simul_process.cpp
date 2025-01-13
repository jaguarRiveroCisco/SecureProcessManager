

#include "simul_process.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>
#include <unistd.h>
#include "communicator.h"
namespace process
{
    int           ProcessSimulator::rndUpper_ = 20;
    constexpr int baseSleepDuration           = 10;

    void ProcessSimulator::setRndUpper(int rndUpper)
    {
        rndUpper_ = rndUpper;
        std::cout << "Children process will simulate work for a random duration between " << baseSleepDuration
                  << " and " << baseSleepDuration + rndUpper_ << " seconds." << std::endl;
    }
    void ProcessSimulator::setSleepDuration()
    {
        std::srand(std::time(nullptr) ^ getpid()); // Seed using time and PID to ensure different seeds
        sleepDuration_ = std::rand() % rndUpper_ + baseSleepDuration; // Random sleep duration between 1 and x seconds
    }

    void ProcessSimulator::work()
    {
        setSleepDuration();
        Communicator::getInstance().sendCreationMessage(sleepDuration_);

        auto msSleepDuration = sleepDuration_ * 1000;
        auto endTime         = startTime_ + std::chrono::milliseconds(msSleepDuration);

        std::cout << "[START] Process ID: " << getpid() << " | Sleep Duration: " << msSleepDuration << " ms"
                  << std::endl;

        // Maximum allowed lifetime to prevent indefinite execution
        auto maxLifetime   = std::chrono::milliseconds(msSleepDuration + 5000); // Add a buffer to the sleep duration
        auto currentTime   = std::chrono::high_resolution_clock::now();
        std::string reason = "Sleep duration reached";

        while (continue_)
        {
            if (currentTime >= endTime)
            {
                break;
            }

            if (currentTime - startTime_ >= maxLifetime)
            {
                reason = "Maximum lifetime reached";
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate some work
            currentTime = std::chrono::high_resolution_clock::now();
        }

        logLifetime(reason);
        _exit(0); // Ensure the child process exits immediately
    }
} // namespace process
