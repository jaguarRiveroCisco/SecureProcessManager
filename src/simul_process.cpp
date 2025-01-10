
#include "simul_process.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>
#include <unistd.h>
#include "process_helper.h"
namespace process
{
    int           ProcessSimulator::rndUpper_ = 10;
    constexpr int baseSleepDuration       = 10;

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
        // Child process
        std::this_thread::sleep_for(std::chrono::seconds(sleepDuration_)); // Simulate some work
        _exit(0); // Ensure the child process exits immediately
    }
} // namespace process
