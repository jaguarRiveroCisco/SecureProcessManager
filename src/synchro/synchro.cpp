#include "synchro.h"
#include "logger_instance.h"

namespace concurrency
{
    Synchro &Synchro::getInstance()
    {
        static Synchro instance; // Guaranteed to be destroyed, instantiated on first use
        return instance;
    }

    void Synchro::pushPid(pid_t pid)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        pidQueue_.push(pid);
        cv_.notify_one();
    }

    // Get and pop the front element of the queue
    pid_t Synchro::removeFrontPidQueue()
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

    // Wait for an event to be available
    void Synchro::blockUntilPidAvailable()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this] { return !pidQueue_.empty() || pauseMonitoring_; });
    }

    void Synchro::pauseMonitoring(bool value) 
    {
        tools::LoggerManager::getInstance() << "[SYNCHRO] Stop monitoring set to " << (value ? "[ON]" : "[OFF]");
        tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);
        pauseMonitoring_ = value; 
        cv_.notify_all();
    }

    bool Synchro::pauseMonitoring() const
    { 
        return pauseMonitoring_; 
    }

    // Check if the queue is empty
    bool Synchro::isPidQueueEmpty()
    {
        std::lock_guard<std::mutex> lock(mtx_);
        return pidQueue_.empty();
    }
} // namespace sync
