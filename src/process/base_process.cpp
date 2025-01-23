#include "base_process.h"
#include <unistd.h>
#include "communicator.h"
#include "logger_instance.h"

namespace process
{
    std::atomic<bool> BaseProcess::continue_{true};
    std::atomic<int>  BaseProcess::exitCode_{EXIT_SUCCESS};

    BaseProcess::BaseProcess() : timeManager_() { setupSignalHandling(); }

    void BaseProcess::logLifetime() const
    {
        reason_ += " | exit code: " + std::to_string(exitCode_);
        tools::LoggerManager::getInstance().logInfo(
                "[PROCESS FINISHED] | Lifetime: " + timeManager_.getFormattedElapsedTimeStr() + " | " + reason_);
    }

    void BaseProcess::preWork()
    {
        tools::LoggerManager::createLoggerType();
        Communicator::getInstance().sendCreationMessage();
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
        BaseProcess::continueFlag() = false;
        BaseProcess::exitCode()     = signum;
    }

    void setupSignalHandling()
    {
        signal(SIGTERM, signalHandler);
        signal(SIGINT, signalHandler);
    }
} // namespace process
