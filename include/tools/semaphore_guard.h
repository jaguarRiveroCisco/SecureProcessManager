#ifndef TOOLS_SEMAPHORE_GUARD_H
#define TOOLS_SEMAPHORE_GUARD_H

#include <fcntl.h>
#include <iostream>
#include <semaphore.h>
#include <stdexcept>
#include <string>

namespace tools
{

    class SemaphoreGuard {
    public:
        SemaphoreGuard(const std::string &name = "/console_logger_semaphore") : sem_name(name)
        {
            sem = sem_open(sem_name.c_str(), O_CREAT, 0644, 1);
            if (sem == SEM_FAILED)
            {
                perror("sem_open");
                throw std::runtime_error("Failed to open semaphore");
            }
        }

        ~SemaphoreGuard()
        {
            // Close the semaphore
            if (sem != SEM_FAILED && sem_close(sem) == -1)
            {
                perror("sem_close");
            }
            // Unlinking should be managed separately to ensure safe removal
        }

        void lock()
        {
            if (sem_wait(sem) == -1)
            {
                perror("sem_wait");
                throw std::runtime_error("Failed to lock semaphore");
            }
        }

        void unlock()
        {
            if (sem_post(sem) == -1)
            {
                perror("sem_post");
                throw std::runtime_error("Failed to unlock semaphore");
            }
        }

        // Separate function to unlink semaphore when appropriate
        static void unlinkSemaphore(const std::string &name)
        {
            if (sem_unlink(name.c_str()) == -1)
            {
                perror("sem_unlink");
            }
        }

    private:
        std::string sem_name;
        sem_t      *sem;
    };

    struct locker {
        SemaphoreGuard *sem_;

        locker(SemaphoreGuard *sem) : sem_(sem) { sem_->lock(); }
        ~locker() { sem_->unlock(); }
    };

} // namespace tools

#endif // TOOLS_SEMAPHORE_GUARD_H
