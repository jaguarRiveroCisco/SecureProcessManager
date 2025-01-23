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

    void Synchro::pushPid(pid_t pid) noexcept
    {
        std::lock_guard<std::mutex> lock(mtx_);
        pidQueue_.push(pid);
    }

    // Get and pop the front element of the queue
    pid_t Synchro::removeFrontPidQueue() noexcept
    {
        std::lock_guard<std::mutex> lock(mtx_);
        if (!pidQueue_.empty())
        {
            pid_t frontElement = pidQueue_.front();
            pidQueue_.pop();
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
    bool Synchro::isPidQueueEmpty() const noexcept
    {
        std::lock_guard<std::mutex> lock(mtx_);
        return pidQueue_.empty();
    }
} // namespace concurrency
