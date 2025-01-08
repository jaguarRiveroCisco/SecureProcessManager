#ifndef PROCESS_BASE_H
#define PROCESS_BASE_H
#include "synchro.h"

class ProcessBase 
{
protected:
    ProcessBase()          = default;
    virtual ~ProcessBase() = default;
    void displayProcessStatus(int &status);
    bool isProcessRunning() const;
    void terminateProcess();
    void killProcess();
    void sendSignal(int signal);
    void createCheckProcessThread();
    void checkProcessState();
    pid_t    pid_;
    Synchro *synchro_;
};

#endif // PROCESS_BASE_H
