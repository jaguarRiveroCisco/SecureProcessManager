#include "base_process.h"
#include <unistd.h>
#include "logger_instance.h"

namespace process
{
    std::atomic<bool> BaseProcess::continue_{true};
    int              BaseProcess::exitCode_{-1};
 
    void BaseProcess::logLifetime() const
    {
        auto endTime  = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime_).count();
        tools::LoggerManager::getInstance().logInfo("[PROCESS FINISHED] | Lifetime: " + std::to_string(duration) + " ms" + " | " + reason_);
    }

    std::atomic<bool> &BaseProcess::continueFlag() { return continue_; }
    int &BaseProcess::exitCode() { return exitCode_; }

    BaseProcess::BaseProcess() { setupSignalHandling(); }


    void signalHandler(int signum)
    {
        tools::LoggerManager::getInstance().logInfo("[SIGNAL] Received signal | " + std::to_string(signum));
        BaseProcess::continueFlag() = false;
        BaseProcess::exitCode()     = signum;
    
    }

    void setupSignalHandling()
    {
        signal(SIGTERM, signalHandler);
        signal(SIGINT, signalHandler);
    }
}