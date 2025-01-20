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
    void addEvent(pid_t pid);
    // Get and pop the front element of the queue
    pid_t getAndPopFront();

    // Wait for an event to be available
    void waitForEvent();
    // Check if the queue is empty
    bool isEmpty();

private:
    std::mutex              mtx_;
    std::condition_variable cv_;
    std::queue<pid_t>       eventQueue_;
};

#endif // SYNCHRO_H
