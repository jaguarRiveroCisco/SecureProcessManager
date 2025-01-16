#include "semaphore_guard.h"

namespace tools
{
    SemaphoreGuard::SemaphoreGuard(const std::string &name) : sem_name(name)
    {
        sem = sem_open(sem_name.c_str(), O_CREAT, 0644, 1);
        if (sem == SEM_FAILED)
        {
            perror("sem_open");
            throw std::runtime_error("Failed to open semaphore");
        }
    }

    SemaphoreGuard ::~SemaphoreGuard()
    {
        // Close the semaphore
        if (sem != SEM_FAILED && sem_close(sem) == -1)
        {
            perror("sem_close");
        }
        // Unlinking should be managed separately to ensure safe removal
    }

    void SemaphoreGuard::lock()
    {
        if (sem_wait(sem) == -1)
        {
            perror("sem_wait");
            throw std::runtime_error("Failed to lock semaphore");
        }
    }

    void SemaphoreGuard::unlock()
    {
        if (sem_post(sem) == -1)
        {
            perror("sem_post");
            throw std::runtime_error("Failed to unlock semaphore");
        }
    }

    // Separate function to unlink semaphore when appropriate
    void SemaphoreGuard::unlinkSemaphore(const std::string &name)
    {
        if (sem_unlink(name.c_str()) == -1)
        {
            perror("sem_unlink");
        }
    }
}