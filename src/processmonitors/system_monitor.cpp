#include "system_monitor.h"
#include <unistd.h>
#include "logger_instance.h"

namespace process
{
    void SystemMonitor::launchChildProcess()
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
}