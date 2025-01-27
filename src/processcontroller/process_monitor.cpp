#include "process_monitor.h"
#include <chrono>
#include <csignal>
#include <thread>
#include <unistd.h> // Include this header for _exit
#include "logger_instance.h"
#include "nap_time.h"

namespace process
{


    void displayProcessStatus(int &status, pid_t pid)
    {
        if (WIFEXITED(status))
        {
        }
        else if (WIFSIGNALED(status))
        {
            tools::LoggerManager::getInstance().logWarning("[PARENT PROCESS] | Child process " + std::to_string(pid)
                                                           + " was terminated by signal " + std::to_string(WTERMSIG(status)) + ".");
        }
        else
        {
            tools::LoggerManager::getInstance().logWarning("[PARENT PROCESS] | Child process " + std::to_string(pid) + " exited with an unknown status.");
        }
    }
    
    bool isProcessRunning(pid_t pid)
    {
        if (kill(pid, 0) == -1 && errno == ESRCH)
        {
            tools::LoggerManager::getInstance() << "[PARENT PROCESS] | Process " << pid << " is not running.";
            tools::LoggerManager::getInstance().flush(tools::LogLevel::ERROR);
            return false;
        }
        return true;
    }

    void sendSignal(int signal, pid_t pid)
    {
        if (kill(pid, signal) == -1)
        {
            perror("kill");
        }
    }

    pid_t ProcessMonitor::getPid() const { return pid_; }
    
    void  ProcessMonitor::terminateProcess() { sendSignal(SIGTERM, pid_); }

    void ProcessMonitor::killProcess() { sendSignal(SIGKILL, pid_); }

    void ProcessMonitor::intProcess() { sendSignal(SIGINT, pid_); }

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
        while (monitoring())
        {
            // Check if the process with PID = pid_ is running
            if (!isProcessRunning(pid_))
                break;

            pid_t result = waitpid(pid_, &status, WNOHANG);
            if (result == 0)
            {
                // Child still running
                std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::SHORT));
            }
            else if (result == pid_)
            {
                displayProcessStatus(status, pid_);
                concurrency::Synchro::getInstance().enqueueTerminatedPid(pid_);
                break;
            }
            else
            {
                // Error occurred
                perror("waitpid");
                break;
            }
        }
        monitoring() = false;
        tools::LoggerManager::getInstance().logInfo("[MONITORING THREAD] | Monitoring for PID: " + std::to_string(pid_) + " has stopped.");

    }

    void ProcessMonitor::init(std::unique_ptr<IProcess> process)
    {
        process_ = std::move(process);
        forkAndExecuteChildProcess();
    }

    void ProcessMonitor::forkAndExecuteChildProcess()
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
