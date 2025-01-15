#include "console_logger.h"
#include <iostream>

namespace tools
{
    // Initialize the static member
    std::unique_ptr<ConsoleLogger> ConsoleLogger::instance = nullptr;
    void ConsoleLogger::outputLog(const std::string &message) 
    { 
        std::cout << message << std::endl; 
    }
}