#pragma once

#ifndef PROCESS_CONTROLLER_H
#define PROCESS_CONTROLLER_H

#include <functional>
#include <shared_mutex>
#include <unordered_map>
#include "process_monitor.h"
#include "ConfigReader.h"
namespace process
{
    enum class LoggingType { Console, File };

    class ProcessController 
    {
    public:
        static std::atomic<bool> &running();
        static std::atomic<bool> &respawn();
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
        static void setNumProcesses(int numProcesses) { numProcesses_ = numProcesses; }
        static int  numProcesses() { return numProcesses_; }
        static std::string loggingTypeToString(); // New method
        static std::string &processType() { return processType_; }
        static void readConfigFile(const std::string &configFilePath);
        static config::ConfigReader &configReader() { return *configReader_; }


    protected:
        static int               numProcesses_;
        static std::string       processType_;
        static LoggingType       loggingType_;

    private:

        static std::atomic<bool> running_;
        static std::atomic<bool> respawn_;
        static std::shared_mutex mutex_;
        static std::unique_ptr<config::ConfigReader> configReader_;

    };
} // namespace process

#endif // PROCESS_CONTROLLER_H
