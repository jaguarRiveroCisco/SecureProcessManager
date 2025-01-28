#include "semaphore_guard.h"

namespace concurrency
{
    std::vector<std::string> SemaphoreGuard::sem_names;

    SemaphoreGuard::SemaphoreGuard(const std::string& name) : sem_name(name)
    {
        sem = sem_open(sem_name.c_str(), O_CREAT, 0644, 1);
        if (sem == SEM_FAILED)
        {
            perror("sem_open");
            throw std::runtime_error("Failed to open semaphore");
        }
        sem_names.push_back(sem_name);
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
    void SemaphoreGuard::unlinkSemaphores()
    {
        for(const auto &sem_name : sem_names)
        {
            unlinkSemaphore(sem_name);
        }
    }
    void SemaphoreGuard::unlinkSemaphore(const std::string &sem_name)
    {
        std::string strError;
        sem_t      *sem = sem_open(sem_name.c_str(), 0);
        if (sem == SEM_FAILED)
        {
            //strError = "sem_open: Failed to open semaphore: " + sem_name;
            //perror(strError.c_str());
            return;
        }
        if (sem_close(sem) == -1)
        {
            strError = "sem_close: Failed to close semaphore: " + sem_name;
            perror(strError.c_str());
        }
        if (sem_unlink(sem_name.c_str()) == -1)
        {
            strError = "sem_unlink: Failed to unlink semaphore: " + sem_name;
            perror(strError.c_str());
        }
        else
        {
            //std::cout << "Unlinked semaphore: " << sem_name << std::endl;
        }
    }
}