#include "controller_base.h"
#include <unistd.h> // Include this header for fork
#include "logger_instance.h"
namespace process
{
    int         ProcessController::numProcesses_ = 4; // Default number of processes
    std::string ProcessController::processType_  = "simul"; // Default process type

    std::vector<std::unique_ptr<ProcessController>> ProcessController::handlers_;
    // Initialize static members
    LoggingType ProcessController::loggingType_ = LoggingType::Console;

    void ProcessController::init(std::unique_ptr<IProcess> process)
    {
        process_ = std::move(process);
        forkAndExecuteChildProcess();
    }
    
    void ProcessController::forkAndExecuteChildProcess()
    {
        pid_ = fork();
        if (pid_ == 0)
        {
            try
            {
                process_->work();
            }
            catch (const std::exception &e)
            {
                // Handle exceptions in child process
                tools::LoggerManager::getInstance() << "[PARENT PROCESS] Exception in child process: " << e.what();
                tools::LoggerManager::getInstance().flush(tools::LogLevel::EXCEPTION);
                exit(EXIT_FAILURE); // Ensure child process exits
            }
        }
        else if (pid_ < 0)
        {
            // Fork failed
            perror("fork");
            throw std::runtime_error("[PARENT PROCESS] Failed to fork process");
        }
        else
        {
        }
    }
    
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

    void  ProcessController::displayAllPids()
    {
        tools::LoggerManager::getInstance() << "[PARENT PROCESS] Current PIDs: ";
        for (const auto &handler: handlers_)
        {
            tools::LoggerManager::getInstance() << handler->getPid() << " | ";
        }
        tools::LoggerManager::getInstance() << " Total number of processes: " << handlers_.size();
        tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);
    }

    void ProcessController::killAll()
    {
        running(false);

        for (auto &handler: handlers_)
        {
            handler->killProcess();
        }
    }

    void ProcessController::terminateAll()
    {
        running(false);
        for (auto &handler: handlers_)
        {
            handler->terminateProcess();
        }
    }

    void ProcessController::pauseMonitoring()
    {
        concurrency::Synchro::getInstance().pauseMonitoring(true);
        for(auto &handler: handlers_)
        {
            handler->monitoring(false);
        }
    }

    void ProcessController::continueMonitoring()
    {
        concurrency::Synchro::getInstance().pauseMonitoring(false);
    }

    void ProcessController::intAll()
    {
        running(false);
        for (auto &handler: handlers_)
        {
            handler->intProcess();
        }
    }

    void ProcessController::killProcessByPid(pid_t pid)
    {
        auto it =
                std::find_if(handlers_.begin(), handlers_.end(), [pid](const std::unique_ptr<ProcessController> &handler) {
                    return handler->getPid() == pid;
                });
        if (it != handlers_.end())
        {
            (*it)->killProcess();
        }
        else
        {
            tools::LoggerManager::getInstance() << "Process with PID: " << pid << " not found.";
            tools::LoggerManager::getInstance().flush(tools::LogLevel::ERROR);
        }
    }

    void ProcessController::terminateProcessByPid(pid_t pid)
    {
        auto it = std::find_if(handlers_.begin(), handlers_.end(),
                               [pid](const std::unique_ptr<ProcessController> &handler) { return handler->getPid() == pid; });
        if (it != handlers_.end())
        {
            (*it)->terminateProcess();
        }
        else
        {
            tools::LoggerManager::getInstance() << "[PARENT PROCESS] Process with PID: " << pid << " not found.";
            tools::LoggerManager::getInstance().flush(tools::LogLevel::ERROR);
        }
    }

    void ProcessController::intProcessByPid(pid_t pid)
    {
        auto it = std::find_if(handlers_.begin(), handlers_.end(),
                               [pid](const std::unique_ptr<ProcessController> &handler) { return handler->getPid() == pid; });
        if (it != handlers_.end())
        {
            (*it)->intProcess();
        }
        else
        {
            tools::LoggerManager::getInstance() << "[PARENT PROCESS] Process with PID: " << pid << " not found.";
            tools::LoggerManager::getInstance().flush(tools::LogLevel::ERROR);
        }
    }
}