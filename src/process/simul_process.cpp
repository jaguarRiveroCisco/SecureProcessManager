#include "simul_process.h"
#include <chrono>
#include <ctime>
#include <thread>
#include <unistd.h>
#include "communicator.h"
#include "logger_instance.h"
#include <iostream>
namespace process
{
    int           ProcessSimulator::rndUpper_ = 20;
    constexpr int baseSleepDuration           = 10;

    void ProcessSimulator::setRndUpper(int rndUpper)
    {
        rndUpper_ = rndUpper;
        tools::LoggerManager::getInstance().logInfo("Random work duration set between " + std::to_string(baseSleepDuration) +
                                         " and " + std::to_string(baseSleepDuration + rndUpper_) + " seconds for child processes.");
    }

    void ProcessSimulator::setSleepDuration()
    {
        std::srand(std::time(nullptr) ^ getpid()); // Seed using time and PID for unique seeds
        sleepDuration_ = std::rand() % rndUpper_ + baseSleepDuration; // Random sleep duration between 1 and x seconds
    }

    void ProcessSimulator::work()
    {
        tools::LoggerManager::createLoggerType(tools::LoggerManager::loggerType());

        setSleepDuration();
        Communicator::getInstance().sendCreationMessage(sleepDuration_);

        auto msSleepDuration = sleepDuration_ * 1000;
        auto endTime         = startTime_ + std::chrono::milliseconds(msSleepDuration);

        tools::LoggerManager::getInstance().logInfo(
                "[EXECUTING] | Simulated Process started. Duration : " + std::to_string(sleepDuration_) + " seconds (" +
                std::to_string(msSleepDuration) + " ms)");

        // Maximum allowed lifetime to prevent indefinite execution
        auto maxLifetime   = std::chrono::milliseconds(msSleepDuration + 5000); // Add a buffer to the sleep duration
        auto currentTime   = std::chrono::high_resolution_clock::now();
        std::string reason = "Sleep duration completed";

        while (continue_)
        {
            if (currentTime >= endTime)
            {
                break;
            }

            if (currentTime - startTime_ >= maxLifetime)
            {
                reason = "Maximum allowed lifetime exceeded";
                break;
            }

            if (getppid() == 1)
            {
                tools::LoggerManager::getInstance() << "Parent process has terminated. Exiting child process.";
                tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);
                break;
            }


            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate some work
            currentTime = std::chrono::high_resolution_clock::now();
        }

        logLifetime(reason);
        _exit(0); // Ensure the child process exits immediately
    }
} // namespace process
