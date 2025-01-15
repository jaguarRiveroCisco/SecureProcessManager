#ifndef SYNCHRO_H
#define SYNCHRO_H
#include <condition_variable>
#include <queue>
#include "logger_instance.h"

struct Synchro final
{
    std::mutex              mtx;
    std::condition_variable cv;
    std::queue<pid_t>       eventQueue;

    Synchro()
    {
        tools::LogOpt::getInstance() << "[CREATED] Synchro object created: " << this;
        tools::LogOpt::getInstance().flush(tools::LogLevel::INFO);
    }

    ~Synchro()
    {
        tools::LogOpt::getInstance() << "[DESTROYED] ~Synchro object destroyed: " << this;
        tools::LogOpt::getInstance().flush(tools::LogLevel::INFO);
    }

    // Delete the new and delete operators to prevent dynamic allocation
    void* operator new(size_t) = delete;
    void operator delete(void*) = delete;

    // Function to get and pop the front element of the queue
    pid_t getAndPopFront()
    {
        if (!eventQueue.empty())
        {
            pid_t frontElement = eventQueue.front();
            eventQueue.pop();
            return frontElement;
        }
        return -1; // Return -1 if the queue is empty
    }
};

#endif // SYNCHRO_H