#include "console_logger.h"
#include <iostream>
#include "semaphore_guard.h"
namespace tools
{

    // Initialize the static member
    std::unique_ptr<ConsoleLogger> ConsoleLogger::instance = nullptr;
    void ConsoleLogger::outputLog(const std::string &message) 
    {
        concurrency::locker lock(sem_.get());
        std::cout << message << std::endl; 
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