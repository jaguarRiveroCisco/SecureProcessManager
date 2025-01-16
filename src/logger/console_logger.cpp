#include "console_logger.h"
#include <iostream>
#include "semaphore_guard.h"

namespace tools
{
    // Initialize the static member
    std::unique_ptr<ConsoleLogger> ConsoleLogger::instance = nullptr;
    void ConsoleLogger::outputLog(const std::string &message) 
    {
        locker lock(&SemaphoreGuard::getInstance());
        std::cout << message << std::endl; 
    }
}