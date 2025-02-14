#include "base_process.h"
#include <unistd.h>
#include "communicator.h"
#include "logger_instance.h"
#include <csignal>

namespace process
{
    std::atomic<bool> BaseProcess::continue_{true};
    std::atomic<int>  BaseProcess::exitCode_{EXIT_SUCCESS};

    BaseProcess::BaseProcess() : timeManager_() { setupSignalHandling(); }

    void BaseProcess::logLifetime() const
    {
        reason_ += " | exit code: " + std::to_string(exitCode_);
        tools::LoggerManager::getInstance().logInfo(
                "[PROCESS FINISHED]  | Lifetime: " + timeManager_.getFormattedElapsedTimeStr() + " | " + reason_);
    }

    void BaseProcess::preWork(pid_t pid)
    {
        pid_ = pid;
        
        timeManager_.setSleepDuration();
        tools::LoggerManager::createLoggerType();

        concurrency::Communicator::getInstance().sendCreationMessage(timeManager_.getSleepDurationStr(), pid_);
    }

    void BaseProcess::postWork()
    {
        logLifetime();
        if (getppid() != 1)
            concurrency::Communicator::getInstance().sendTerminationMessage(timeManager_.getFormattedElapsedTimeStr(),
                                                                            pid_, exitCode_);
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
