#pragma once

#ifndef PROCESS_CONTROLLER_H
#define PROCESS_CONTROLLER_H

#include <functional>
#include <memory>
#include <shared_mutex>
#include <unordered_map>
#include "process_monitor.h"
namespace process
{
    enum class LoggingType { Console, File };

    using ProcessMonitorPtr = std::unique_ptr<ProcessMonitor>;
    using ProcessMonitorMap = std::unordered_map<pid_t, ProcessMonitorPtr>;


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

        // Updated to return a const reference to handlers_
        static const ProcessMonitorMap &handlers()
        {
            std::shared_lock<std::shared_mutex> lock(mutex_);
            return handlers_;
        }

        static bool addMonitorProcess(pid_t pid, ProcessMonitorPtr processMonitor)
        {
            std::unique_lock<std::shared_mutex> lock(mutex_);
            if (handlers_.find(pid) == handlers_.end())
            {
                // If the key does not exist, insert the new element
                auto res = handlers_.emplace(pid, std::move(processMonitor));
                return true;
            }
            else
            {
                return false;
            }
            return false;
        }

        static ProcessMonitor *findMonitor(pid_t pid)
        {
            std::shared_lock<std::shared_mutex> lock(mutex_);
            auto it = handlers_.find(pid);
            if (it != handlers_.end())
            {
                return it->second.get(); // Return the raw pointer to the ProcessMonitor
            }
            return nullptr; // Return nullptr if not found
        }

        static bool removeMonitorProcess(pid_t pid)
        {
            std::unique_lock<std::shared_mutex> lock(mutex_);
            return handlers_.erase(pid) > 0;
        }

    protected:
        static ProcessMonitorMap handlers_;
        static int               numProcesses_;
        static std::string       processType_;
        static LoggingType       loggingType_;

    private:
        static std::atomic<bool> running_;
        static std::atomic<bool> respawn_;
        static std::shared_mutex mutex_;
    };
} // namespace process

#endif // PROCESS_CONTROLLER_H
