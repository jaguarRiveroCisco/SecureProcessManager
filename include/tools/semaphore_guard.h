#ifndef TOOLS_SEMAPHORE_GUARD_H
#define TOOLS_SEMAPHORE_GUARD_H

#include <fcntl.h>
#include <iostream>
#include <semaphore.h>
#include <stdexcept>
#include <string>

namespace tools
{

    class SemaphoreGuard 
    {
    public:
        static SemaphoreGuard &getInstance(const std::string &name = "/console_logger_semaphore");
        void lock();
        void unlock();
        // Separate function to unlink semaphore when appropriate
        static void unlinkSemaphore(const std::string &name);
        const std::string &getName() const { return sem_name; }

    private:
        SemaphoreGuard(const std::string &name = "/console_logger_semaphore");
        ~SemaphoreGuard();
        SemaphoreGuard(const SemaphoreGuard &)            = delete;
        SemaphoreGuard &operator=(const SemaphoreGuard &) = delete;
        std::string sem_name;
        sem_t      *sem;
    };

    struct locker 
    {
        SemaphoreGuard *sem_;

        locker(SemaphoreGuard *sem) : sem_(sem) { sem_->lock(); }
        ~locker() { sem_->unlock(); }
    };

} // namespace tools

#endif // TOOLS_SEMAPHORE_GUARD_H
