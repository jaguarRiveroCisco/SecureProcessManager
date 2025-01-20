#ifndef SYNCHRO_H
#define SYNCHRO_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include "base_process.h"
#include "logger_instance.h"

class Synchro final 
{
public:
    // Add an event to the queue and notify waiting threads
    void addEvent(pid_t pid)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        eventQueue_.push(pid);
        cv_.notify_one();
    }

    // Get and pop the front element of the queue
    pid_t getAndPopFront()
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
    void waitForEvent()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this] { return !eventQueue_.empty(); });
    }

    // Check if the queue is empty
    bool isEmpty()
    {
        std::lock_guard<std::mutex> lock(mtx_);
        return eventQueue_.empty();
    }

private:
    std::mutex              mtx_;
    std::condition_variable cv_;
    std::queue<pid_t>       eventQueue_;
};

#endif // SYNCHRO_H
