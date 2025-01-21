#include "base_process.h"
#include <unistd.h>
#include "logger_instance.h"
#include "communicator.h"
#include <thread>
#include <random>
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

    void BaseProcess::preWork() 
    {
        tools::LoggerManager::createLoggerType();
        // Real process work implementation
        Communicator::getInstance().sendCreationMessage();
        tools::LoggerManager::getInstance().logInfo("[PROCESS EXECUTING] | Process work started");
    }
    void BaseProcess::postWork() 
    {
        logLifetime();
        _exit(exitCode_); // Ensure the child process exits immediately
    }

    void BaseProcess::sleepRandom()
    {
        // Initialize random number generator
        std::random_device rd;
        std::mt19937       gen(rd());

        // Sleep for a random duration in milliseconds
        std::uniform_int_distribution<> disMs(NapTimeMs::SMALL, NapTimeMs::LONG);
        auto nappy = disMs(gen);
        tools::LoggerManager::getInstance().logInfo("Sleeping for: " + std::to_string(nappy) + " mS.");
        std::this_thread::sleep_for(std::chrono::milliseconds(nappy));
    }

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