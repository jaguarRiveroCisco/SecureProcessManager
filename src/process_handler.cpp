
#include <unistd.h>
#include "synchro.h"
#include "process_handler.h"

int ProcessHandler::numProcesses_ = 4; // Default number of processes
std::vector<std::unique_ptr<ProcessHandler>> ProcessHandler::handlers_;

void ProcessHandler::start(Synchro *synchro)
{
    synchro_ = synchro;
    createChild();
}

void ProcessHandler::numProcesses(int numProcesses) { numProcesses_ = numProcesses; }

int ProcessHandler::numProcesses() { return numProcesses_; }

void ProcessHandler::createChild()
{
    pid_ = fork();
    if (pid_ == 0)
    {
        setSleepDuration();
        sendCreationMessage(sleepDuration_);
        work();
    }
    else if (pid_ < 0)
    {
        // Fork failed
        perror("fork");
    }
    else
    {
        createCheckProcessThread();
    }
}
