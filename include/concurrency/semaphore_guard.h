#pragma once

#ifndef TOOLS_SEMAPHORE_GUARD_H
#define TOOLS_SEMAPHORE_GUARD_H

#include <fcntl.h>
#include <iostream>
#include <semaphore.h>
#include <stdexcept>
#include <string>
#include <vector>

namespace tools
{
    class SemaphoreGuard 
    {
    public:
        void lock();
        void unlock();
        // Separate function to unlink semaphore when appropriate
        static void        unlinkSemaphores();
        static void        unlinkSemaphore(const std::string &sem_name);
        const std::string &getName() const { return sem_name; }
        SemaphoreGuard(const std::string &name);
        ~SemaphoreGuard();

    private:
        SemaphoreGuard(const SemaphoreGuard &)            = delete;
        SemaphoreGuard &operator=(const SemaphoreGuard &) = delete;
        std::string sem_name;
        sem_t      *sem;
        static std::vector<std::string> sem_names;
    };

    struct locker 
    {
        SemaphoreGuard *sem_;

        locker(SemaphoreGuard *sem) : sem_(sem) { sem_->lock(); }
        ~locker() { sem_->unlock(); }
    };

} // namespace tools

#endif // TOOLS_SEMAPHORE_GUARD_H
