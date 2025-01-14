#include "base_process.h"
#include <unistd.h>
namespace process
{
    std::atomic<bool> BaseProcess::continue_{true};
    std::unique_ptr<tools::ILogger> BaseProcess::logger_ = std::make_unique<tools::ConsoleLogger>();
    void BaseProcess::logLifetime(const std::string &reason) const
    {
        auto endTime  = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime_).count();
        logger_->logInfo("[END]  | Lifetime: " + std::to_string(duration) + " ms" +
                         " | Reason: " + reason);
    }

    void signalHandler(int signum)
    {
        std::cout << "[INFO] Process " << getpid() << " received signal: " << signum << std::endl;
        BaseProcess::continue_ = false;
    }

    void setupSignalHandling()
    {
        signal(SIGTERM, signalHandler);
        signal(SIGINT, signalHandler);
    }
}