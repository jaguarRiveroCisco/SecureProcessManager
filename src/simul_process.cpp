
#include <ctime>
#include <thread>
#include <chrono>
#include <unistd.h>
#include "synchro.h"
#include "process_base.h"
#include "simul_process.h"



int SimulProcess::rndUpper_ = 10;
constexpr int baseSleepDuration = 20;

void SimulProcess::setRndUpper(int rndUpper)
{
    rndUpper_ = rndUpper;
    std::cout << "Children process will simulate work for a random duration between " << baseSleepDuration
              << " and " << baseSleepDuration + rndUpper_ << " seconds." << std::endl;
}
void SimulProcess::setSleepDuration()
{
    std::srand(std::time(nullptr) ^ getpid()); // Seed using time and PID to ensure different seeds
    sleepDuration_ = std::rand() % rndUpper_ + baseSleepDuration ; // Random sleep duration between 1 and x seconds
}

void SimulProcess::work()
{
    setSleepDuration();
    sendCreationMessage(sleepDuration_);
    // Child process
    std::this_thread::sleep_for(std::chrono::seconds(sleepDuration_)); // Simulate some work
    _exit(0); // Ensure the child process exits immediately
}
