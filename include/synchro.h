#ifndef SYNCHRO_H
#define SYNCHRO_H

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>

struct Synchro final
{
    std::mutex              mtx;
    std::condition_variable cv;
    std::queue<pid_t>       eventQueue;
    Synchro()
    {
        std::cout << "CTOR Synchro object created" << std::endl;
    }
    ~Synchro()
    {
        std::cout << "DTOR ~Synchro object destroyed" << std::endl;
    }

    // Delete the new and delete operators to prevent dynamic allocation
    void* operator new(size_t) = delete;
    void operator delete(void*) = delete;
};
#endif // SYNCHRO_H;
