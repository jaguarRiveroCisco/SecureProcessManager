#include "synchro.h"

    void Synchro::addEvent(pid_t pid)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        eventQueue_.push(pid);
        cv_.notify_one();
    }

    // Get and pop the front element of the queue
    pid_t Synchro::getAndPopFront()
    {
        std::lock_guard<std::mutex> lock(mtx_);
        if (!eventQueue_.empty())
        {
            pid_t frontElement = eventQueue_.front();
            eventQueue_.pop();
            return frontElement;
        }
        return -1; // Return -1 if the queue is empty
    }

    // Wait for an event to be available
    void Synchro::waitForEvent()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this] { return !eventQueue_.empty(); });
    }

    // Check if the queue is empty
    bool Synchro::isEmpty()
    {
        std::lock_guard<std::mutex> lock(mtx_);
        return eventQueue_.empty();
    }