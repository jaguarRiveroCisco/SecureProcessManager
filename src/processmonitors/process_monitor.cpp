#include "process_monitor.h"
#include <csignal>
#include <process_controller.h>
#include <thread>
#include <unistd.h> // Include this header for _exit
#include "logger_instance.h"
#include "nap_time.h"
#include "communicator.h"
#include "process_status.h"
#include <sys/types.h>
#include <sys/wait.h>

namespace process
{

    pid_t ProcessMonitor::getPid() const
    {
        return pid_;
    }
    
    void  ProcessMonitor::terminateProcess() const { tools::sendSignal(SIGTERM, pid_); }

    void ProcessMonitor::killProcess() const { tools::sendSignal(SIGKILL, pid_); }

    void ProcessMonitor::intProcess() const { tools::sendSignal(SIGINT, pid_); }

    std::atomic<bool>& ProcessMonitor::monitoring() { return monitor_; }

    void ProcessMonitor::createMonitorProcessThread()
    {
        if( monitoring() )
        {
            tools::LoggerManager::getInstance().logWarning("[PARENT PROCESS] | Monitoring thread already running.");
            return;
        }
        // Parent process
        // Create a thread to check the state of the child process
        try
        {
            std::thread checkThread(&ProcessMonitor::monitorProcessThread, this);
            checkThread.detach();
        }
        catch (const std::system_error &e)
        {
            tools::LoggerManager::getInstance() << "[PARENT PROCESS] | Thread creation failed: " << e.what();
            tools::LoggerManager::getInstance().flush(tools::LogLevel::ERROR);
            exit(EXIT_FAILURE); // Ensure the child process exits
        }
    }

    void ProcessMonitor::monitorProcessThread()
    {
        int  status   = -1;
        monitoring() = true;
        bool monPrinted = false;
        while (ProcessController::running() && monitoring())
        {
            if (concurrency::Synchro::getInstance().pauseMonitoring())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::VERYSMALL));
                if (!monPrinted)
                {
                    monPrinted = true;
                    tools::LoggerManager::getInstance().logInfo("[MONITOR THREAD] | Not monitoring " + std::to_string(pid_));
                }
                continue;
            }
            monPrinted = false;

            // Check if the process with PID = pid_ is running
            if (!tools::isProcessRunning(pid_))
                break;

            pid_t result = waitpid(pid_, &status, WNOHANG);
            if (result == 0)
            {
                // Child still running
                std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::VERYSMALL));
            }
            else if (result == pid_)
            {
                if(auto res = tools::displayProcessStatus(status, pid_); res != tools::STATUS_OK)
                {
                    concurrency::Communicator::getInstance().sendTerminationMessage("", pid_, status);
                }
                break;
            }
            else
            {
                // Error occurred
                perror("waitpid");
                tools::LoggerManager::getInstance().logError("[MONITOR THREAD] | Error occurred while waiting for process " + std::to_string(pid_) + ".");
                break;
            }
        }
        monitoring() = false;
    }

    void ProcessMonitor::collectAndLaunch(std::unique_ptr<IProcess> process)
    {
        process_ = std::move(process);
        launchChildProcess();
    }

    void ProcessMonitor::launchChildProcess()
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

} // namespace process
