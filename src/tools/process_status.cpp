//
// Created by Jaguar Rivero on 2025-02-04.
//
#include "process_status.h"
#include "logger_instance.h"
#include <csignal>

namespace tools
{
    int displayProcessStatus(int status, const pid_t pid)
    {
        if (WIFEXITED(status))
        {
            return STATUS_OK;
        }
        else if (WIFSIGNALED(status))
        {
            const int signal = WTERMSIG(status);
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

    void sendSignal(const int signal, const pid_t pid)
    {
        if (kill(pid, signal) == -1)
        {
            perror("kill");
        }
    }
}
