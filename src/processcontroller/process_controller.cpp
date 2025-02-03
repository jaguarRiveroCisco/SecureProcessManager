#include "process_controller.h"
#include <unistd.h> // Include this header for fork
#include "logger_instance.h"
namespace process
{
    int               ProcessController::numProcesses_ = 4; // Default number of processes
    std::string       ProcessController::processType_  = "simul"; // Default process type
    std::atomic<bool> ProcessController::running_      = true;
    std::atomic<bool> ProcessController::respawn_      = true;

    ProcessMonitorMap ProcessController::handlers_;
    std::shared_mutex ProcessController::mutex_;

    // Initialize static members
    LoggingType ProcessController::loggingType_ = LoggingType::Console;

    // Implementation of the new method
    std::string ProcessController::loggingTypeToString()
    {
        switch (loggingType_)
        {
            case LoggingType::Console:
                return "Console";
            case LoggingType::File:
                return "File";
            default:
                return "Unknown";
        }
    }

    void ProcessController::setProcessType(const std::string &processType) { processType_ = processType; }

    void ProcessController::displayAllPids()
    {
        tools::LoggerManager::getInstance() << "[PARENT PROCESS] Current PIDs: ";
        for (const auto &[pid, monThread]: handlers_)
        {
            tools::LoggerManager::getInstance() << monThread->getPid() << " | ";
        }
        tools::LoggerManager::getInstance() << " Total number of processes: " << handlers_.size();
        tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);
    }

    void ProcessController::killAll()
    {
        running() = false;

        for (auto &[pid, monThread]: handlers_)
        {
            monThread->killProcess();
        }
    }

    void ProcessController::terminateAll()
    {
        running() = false;
        for (auto &[pid, monThread]: handlers_)
        {
            monThread->terminateProcess();
        }
    }

    void ProcessController::pauseMonitoring()
    {
        concurrency::Synchro::getInstance().pauseMonitoring(true);
    }

    void ProcessController::continueMonitoring()
    {
        concurrency::Synchro::getInstance().pauseMonitoring(false);
    }

    void ProcessController::intAll()
    {
        running() = false;
        for (auto &[pid, monThread]: handlers_)
        {
            monThread->intProcess();
        }
    }

    std::atomic<bool> &ProcessController::running() { return running_; }

    std::atomic<bool> &ProcessController::respawn() { return respawn_; }

    void ProcessController::killProcessByPid(pid_t pid)
    {

        if (const auto element = findMonitor(pid))
        {
            element->killProcess();
        }
        else
        {
            tools::LoggerManager::getInstance() << "Process with PID: " << pid << " not found.";
            tools::LoggerManager::getInstance().flush(tools::LogLevel::ERROR);
        }
    }

    void ProcessController::terminateProcessByPid(pid_t pid)
    {
        if (const auto element = findMonitor(pid))
        {
            element->terminateProcess();
        }
        else
        {
            tools::LoggerManager::getInstance() << "Process with PID: " << pid << " not found.";
            tools::LoggerManager::getInstance().flush(tools::LogLevel::ERROR);
        }
    }

    void ProcessController::intProcessByPid(pid_t pid)
    {
        if (const auto element = findMonitor(pid))
        {
            element->intProcess();
        }
        else
        {
            tools::LoggerManager::getInstance() << "Process with PID: " << pid << " not found.";
            tools::LoggerManager::getInstance().flush(tools::LogLevel::ERROR);
        }
    }
} // namespace process
