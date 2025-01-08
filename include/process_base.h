#ifndef PROCESS_BASE_H
#define PROCESS_BASE_H
#include "synchro.h"

class ProcessBase 
{
protected:
    ProcessBase();
    virtual ~ProcessBase();
    void displayProcessStatus(int &status);
    bool isProcessRunning() const;
    void terminateProcess();
    void killProcess();
    void sendSignal(int signal);
    void createCheckProcessThread();
    void checkProcessState();
    pid_t    pid_ {0};
    Synchro *synchro_ {nullptr};

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime_;
};

#endif // PROCESS_BASE_H
