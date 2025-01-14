#ifndef SYNCHRO_H
#define SYNCHRO_H
#include <condition_variable>
#include <queue>
#include <console_logger.h>

struct Synchro final
{
    std::mutex              mtx;
    std::condition_variable cv;
    std::queue<pid_t>       eventQueue;

    Synchro()
    {
        tools::ConsoleLogger::getInstance() << "[START] Synchro object created: " << this;
        tools::ConsoleLogger::getInstance().flush(tools::LogLevel::INFO);
    }

    ~Synchro()
    {
        tools::ConsoleLogger::getInstance() << "[END] ~Synchro object destroyed: " << this;
        tools::ConsoleLogger::getInstance().flush(tools::LogLevel::INFO);
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