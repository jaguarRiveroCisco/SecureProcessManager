
#include <ctime>
#include <thread>
#include <chrono>
#include <unistd.h>
#include "synchro.h"
#include "process_base.h"
#include "simul_process.h"

void SimulProcess::setSleepDuration()
{
    std::srand(std::time(nullptr) ^ getpid()); // Seed using time and PID to ensure different seeds
    int x          = 10;
    sleepDuration_ = std::rand() % x + 1; // Random sleep duration between 1 and x seconds
}

void SimulProcess::work()
{
    setSleepDuration();
    sendCreationMessage(sleepDuration_);
    // Child process
    std::this_thread::sleep_for(std::chrono::seconds(sleepDuration_)); // Simulate some work
    _exit(0); // Ensure the child process exits immediately
}
std::string SimulProcess::receiveCreationMessage() { return messenger_.receiveMessage(Message::CREATION_MSG); }
