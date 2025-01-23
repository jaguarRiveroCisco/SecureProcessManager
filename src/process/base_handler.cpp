#include "base_handler.h"
#include <chrono>
#include <csignal>
#include <thread>
#include <unistd.h> // Include this header for _exit
#include "logger_instance.h"
#include "nap_time.h"

namespace process
{

    void BaseHandler::displayProcessStatus(int &status)
    {
        if (WIFEXITED(status))
        {
            tools::LoggerManager::getInstance().logInfo("[PARENT PROCESS] Child process " + std::to_string(pid_)
                                                        + " exited normally with status " + std::to_string(WEXITSTATUS(status)) + ".");
        }
        else if (WIFSIGNALED(status))
        {
            tools::LoggerManager::getInstance().logWarning("[PARENT PROCESS] Child process " + std::to_string(pid_)
                                                           + " was terminated by signal " + std::to_string(WTERMSIG(status)) + ".");
        }
        else
        {
            tools::LoggerManager::getInstance().logWarning("[PARENT PROCESS] Child process " + std::to_string(pid_) + " exited with an unknown status.");
        }
    }
    
    pid_t BaseHandler::getPid() const { return pid_; }

    bool BaseHandler::isProcessRunning()
    {
        if (kill(pid_, 0) == -1 && errno == ESRCH)
        {
            tools::LoggerManager::getInstance() << "[PARENT PROCESS] Process " << pid_ << " is not running.";
            tools::LoggerManager::getInstance().flush(tools::LogLevel::ERROR);
            return false;
        }
        return true;
    }

    void BaseHandler::terminateProcess() { sendSignal(SIGTERM); }

    void BaseHandler::killProcess() { sendSignal(SIGKILL); }

    void BaseHandler::intProcess() { sendSignal(SIGINT); }

    void BaseHandler::sendSignal(int signal)
    {
        if (kill(pid_, signal) == -1)
        {
            perror("kill");
        }
    }


    std::atomic<bool> BaseHandler::monitoring() 
    {
        return monitor_ == true;
    }

    void BaseHandler::monitoring(bool value) 
    { 
        monitor_ = value;
    }

    void BaseHandler::createMonitorProcessThread()
    {
        if( monitoring() )
        {
            tools::LoggerManager::getInstance().logWarning("[PARENT PROCESS] Monitoring thread already running.");
            return;
        }
        // Parent process
        // Create a thread to check the state of the child process
        try
        {
            std::thread checkThread(&BaseHandler::monitorProcessThread, this);
            checkThread.detach();
        }
        catch (const std::system_error &e)
        {
            tools::LoggerManager::getInstance() << "[PARENT PROCESS] Thread creation failed: " << e.what();
            tools::LoggerManager::getInstance().flush(tools::LogLevel::ERROR);
            exit(EXIT_FAILURE); // Ensure the child process exits
        }
    }

    void BaseHandler::monitorProcessThread()
    {
        auto threadId = std::this_thread::get_id();
        std::ostringstream oss;
        oss << threadId;
        tools::LoggerManager::getInstance().logInfo("[MONITORING THREAD] Monitoring thread started with ID: " + oss.str());
        int  status   = -1;
        monitoring(true);
        while (monitoring())
        {
            // Check if the process with PID = pid_ is running
            if (!isProcessRunning())
                break;

            pid_t result = waitpid(pid_, &status, WNOHANG);
            if (result == 0)
            {
                // Child still running
                std::this_thread::sleep_for(std::chrono::milliseconds(NapTimeMs::SHORT));
            }
            else if (result == pid_)
            {
                displayProcessStatus(status);
                concurrency::Synchro::getInstance().pushPid(pid_);
                break;
            }
            else
            {
                // Error occurred
                perror("waitpid");
                break;
            }
        }
        monitoring(false);
        tools::LoggerManager::getInstance().logInfo("[MONITORING THREAD] Monitoring thread stopped with ID: " + oss.str());}
} // namespace process
