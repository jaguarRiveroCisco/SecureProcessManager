#include "synchro.h"
#include <iostream> // For std::cerr
#include "logger_instance.h"

namespace concurrency
{
    Synchro &Synchro::getInstance()
    {
        static Synchro instance; // Guaranteed to be destroyed, instantiated on first use
        return instance;
    }

    void Synchro::enqueueTerminatedPid(pid_t pid) noexcept
    {
        std::lock_guard<std::mutex> lock(mtx_);
        terminatedPidQueue_.push(pid);
    }

    // Get and pop the front element of the queue
    pid_t Synchro::removeTerminatedProcessPid() noexcept
    {
        std::lock_guard<std::mutex> lock(mtx_);
        if (!terminatedPidQueue_.empty())
        {
            pid_t frontElement = terminatedPidQueue_.front();
            terminatedPidQueue_.pop();
            return frontElement;
        }
        return -1; // Return -1 if the queue is empty
    }

    void Synchro::pauseMonitoring(bool value) noexcept
    {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            pauseMonitoring_ = value;
        }
        try
        {
            tools::LoggerManager::getInstance() << "[SYNCHRO] | Process Monitoring threads state: " << (value ? "Paused" : "Continued");
            tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);
        }
        catch (const std::exception &e)
        {
            // Handle any potential logging exceptions to maintain noexcept guarantee
            std::cerr << "Logging error: " << e.what() << std::endl;
        }
    }

    bool Synchro::pauseMonitoring() const noexcept { return pauseMonitoring_; }

    // Check if the queue is empty
    bool Synchro::isTerminatedPidQueueEmpty() const noexcept
    {
        std::lock_guard<std::mutex> lock(mtx_);
        return terminatedPidQueue_.empty();
    }
} // namespace concurrency
