#pragma once

#ifndef CONTROLLER_BASE_H
#define CONTROLLER_BASE_H

#include "process_monitor.h"
#include "process_interface.h"

namespace process
{
    enum class LoggingType { Console, File };

    class ProcessController : public ProcessMonitor 
    {
    public:

        static void terminateAll();
        static void terminateProcessByPid(pid_t pid);
        static void intProcessByPid(pid_t pid);
        static void intAll();
        static void killAll();
        static void killProcessByPid(pid_t pid);
        static void displayAllPids();
        static void pauseMonitoring();
        static void continueMonitoring();
        static void setProcessType(const std::string &processType);
        static std::string  loggingTypeToString(); // New method

               void init(std::unique_ptr<IProcess> process);

    protected:
        static std::vector<std::unique_ptr<ProcessController>> handlers_;
        virtual void forkAndExecuteChildProcess();
        static void setNumProcesses(int numProcesses) { numProcesses_ = numProcesses; }
        static int         numProcesses_;
        static std::string processType_;
        static LoggingType loggingType_;
        std::unique_ptr<IProcess> process_;

    private:

    };
} // namespace process

#endif // CONTROLLER_BASE_H