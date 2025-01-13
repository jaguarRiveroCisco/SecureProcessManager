#include "console_logger.h"
#include <iostream>

namespace tools
{
    void ConsoleLogger::outputLog(const std::string &message) 
    { 
        std::cout << message << std::endl; 
    }
}