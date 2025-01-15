#include "base_process.h"
#include <unistd.h>
#include "logger_instance.h"

namespace process
{
    std::atomic<bool> BaseProcess::continue_{true};
    void BaseProcess::logLifetime() const
    {
        auto endTime  = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime_).count();
        tools::LoggerManager::getInstance().logInfo("[DONE]   | Lifetime: " + std::to_string(duration) + " ms" + " | Reason: " + reason_);
    }

    void signalHandler(int signum)
    {
        tools::LoggerManager::getInstance().logInfo("Received signal: " + std::to_string(signum));
        BaseProcess::continueFlag() = false;
    }

    void setupSignalHandling()
    {
        signal(SIGTERM, signalHandler);
        signal(SIGINT, signalHandler);
    }
}