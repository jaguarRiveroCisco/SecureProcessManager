#pragma once

#ifndef PROCESS_CONTROLLER_H
#define PROCESS_CONTROLLER_H

#include "process_monitor.h"
#include "process_interface.h"

namespace process
{
    enum class LoggingType { Console, File };

    class ProcessController 
    {
    public:
        static std::atomic<bool>                               &running();
        static std::atomic<bool>                               &respawn();
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
        static std::string &processType() { return processType_; }
        static std::vector<std::unique_ptr<ProcessMonitor>> &handlers() { return handlers_; }

        static void setNumProcesses(int numProcesses) { numProcesses_ = numProcesses; }
        static int numProcesses() { return numProcesses_; }
    protected:
        static std::vector<std::unique_ptr<ProcessMonitor>> handlers_;
        static int         numProcesses_;
        static std::string processType_;
        static LoggingType loggingType_;

    private:
        static std::atomic<bool> running_;
        static std::atomic<bool> respawn_;
    };
} // namespace process

#endif // PROCESS_CONTROLLER_H