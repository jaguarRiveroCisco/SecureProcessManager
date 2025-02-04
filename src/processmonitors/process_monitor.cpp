#include "process_monitor.h"
#include <chrono>
#include <csignal>
#include <process_controller.h>
#include <thread>
#include <unistd.h> // Include this header for _exit
#include "logger_instance.h"
#include "nap_time.h"
#include "communicator.h"
namespace process
{
    enum Status { STATUS_OK, STATUS_SIGNAL, STATUS_UNKNOWN };

    int displayProcessStatus(int status, pid_t pid)
    {
        if (WIFEXITED(status))
        {
            return STATUS_OK;
        }
        else if (WIFSIGNALED(status))
        {
            int signal = WTERMSIG(status);
            tools::LoggerManager::getInstance().logWarning("[PARENT PROCESS] | Child process " + std::to_string(pid) +
                                                           " was terminated by signal " + std::to_string(signal) + ".");
            return STATUS_SIGNAL;
        }
        else
        {
            tools::LoggerManager::getInstance().logError("[PARENT PROCESS] | Child process " + std::to_string(pid) +
                                                           " exited with an unknown status.");
            return STATUS_UNKNOWN;
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
    
    void  ProcessMonitor::terminateProcess() const { sendSignal(SIGTERM, pid_); }

    void ProcessMonitor::killProcess() const { sendSignal(SIGKILL, pid_); }

    void ProcessMonitor::intProcess() const { sendSignal(SIGINT, pid_); }

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
            if (!isProcessRunning(pid_))
                break;

            pid_t result = waitpid(pid_, &status, WNOHANG);
            if (result == 0)
            {
                // Child still running
                std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::VERYSMALL));
            }
            else if (result == pid_)
            {
                auto res = displayProcessStatus(status, pid_);
                if(res != STATUS_OK)
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
