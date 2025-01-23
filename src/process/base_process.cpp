#include "base_process.h"
#include <unistd.h>
#include "communicator.h"
#include "logger_instance.h"

namespace process
{
    std::atomic<bool> BaseProcess::continue_{true};
    std::atomic<int>  BaseProcess::exitCode_{-1};

    BaseProcess::BaseProcess() : timeManager_() { setupSignalHandling(); }

    void BaseProcess::logLifetime() const
    {
        tools::LoggerManager::getInstance().logInfo(
                "[PROCESS FINISHED] | Lifetime: " + timeManager_.getFormattedElapsedTimeStr() + reason_);
    }

    void BaseProcess::preWork()
    {
        tools::LoggerManager::createLoggerType();
        Communicator::getInstance().sendCreationMessage();
        tools::LoggerManager::getInstance().logInfo("[PROCESS EXECUTING] | Process work started");
    }

    void BaseProcess::postWork()
    {
        logLifetime();
        _exit(exitCode_); // Ensure the child process exits immediately
    }

    std::atomic<bool> &BaseProcess::continueFlag() { return continue_; }
    std::atomic<int>  &BaseProcess::exitCode() { return exitCode_; }

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
} // namespace process
