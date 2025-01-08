
#include <unistd.h>
#include "synchro.h"
#include "process_handler.h"

int ProcessHandler::numProcesses_ = 4; // Default number of processes
std::vector<std::unique_ptr<ProcessHandler>> ProcessHandler::handlers_;

Synchro *ProcessHandler::synchro()
{
    static Synchro instance;
    return &instance;
}

void ProcessHandler::init(Synchro *synchro)
{
    synchro_ = synchro;
    if (synchro_ == nullptr)
    {
        throw std::runtime_error("Synchro object is null");
    }
    createChild();
}
void ProcessHandler::start() { createCheckProcessThread(); }

void ProcessHandler::numProcesses(int numProcesses) { numProcesses_ = numProcesses; }

int ProcessHandler::numProcesses() { return numProcesses_; }

void ProcessHandler::createChild()
{
    pid_ = fork();
    if (pid_ == 0)
    {
        try
        {
            setSleepDuration();
            sendCreationMessage(sleepDuration_);
            work();
        }
        catch (const std::exception &e)
        {
            // Handle exceptions in child process
            std::cerr << "Exception in child process: " << e.what() << std::endl;
            _exit(EXIT_FAILURE); // Ensure child process exits
        }
    }
    else if (pid_ < 0)
    {
        // Fork failed
        perror("fork");
        throw std::runtime_error("Failed to fork process");
    }
    else
    {
        // Parent process
        //std::cout << "Parent process created child process with PID: " << pid_ << std::endl;
    }
}
