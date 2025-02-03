#include "console_logger.h"
#include <iostream>
#include <semaphore.h>
#include <memory>


namespace tools
{

    // Initialize the static member
    std::unique_ptr<ConsoleLogger> ConsoleLogger::instance = nullptr;
    sem_t* ConsoleLogger::sem_ = nullptr;

    void ConsoleLogger::outputLog(const std::string &message) 
    {
        sem_wait(sem_); // Wait (lock the semaphore)
        std::cout << message << std::endl;
        sem_post(sem_); // Signal (unlock the semaphore)
    }

    ConsoleLogger &ConsoleLogger::getInstance()
    {
        if (!instance)
        {
            instance = std::make_unique<ConsoleLogger>();
        }
        return *instance;
    }

    // Reset the singleton instance
    void ConsoleLogger::resetInstance()
    {
        instance.reset(); // Automatically deletes the instance and sets to nullptr
    }
}