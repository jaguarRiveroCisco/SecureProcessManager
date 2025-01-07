#ifndef SIMUL_PROCESS_H
#define SIMUL_PROCESS_H
class SimulProcess 
{
protected:
    SimulProcess()          = default;
    virtual ~SimulProcess() = default;
    int  sleepDuration_     = 0;
    void setSleepDuration()
    {
        std::srand(std::time(nullptr) ^ getpid()); // Seed using time and PID to ensure different seeds
        int x          = 10;
        sleepDuration_ = std::rand() % x + 1; // Random sleep duration between 1 and x seconds
    }
    virtual void work()
    {
        // Child process
        std::this_thread::sleep_for(std::chrono::seconds(sleepDuration_)); // Simulate some work
        _exit(0); // Ensure the child process exits immediately
    }
};

#endif // SIMUL_PROCESS_H
