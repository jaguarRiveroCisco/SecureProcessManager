#include "logger_instance.h"
#include "file_logger.h"
#include "console_logger.h"
#include <iostream>
#include "timestamp.h"
namespace tools
{
    // Definition of the static member
    Logger* LogOpt::loggerInstance = nullptr;
    void LogOpt::initializeLogger(const std::string &choice)
    {
        std::cout << TimeStamp::get() + "INFO: [START] LogOpt Initialized as: " << choice << std::endl;

        if (choice == "console")
        {
            loggerInstance = &ConsoleLogger::getInstance();
        }
        else if (choice == "file")
        {
            loggerInstance = &FileLogger::getInstance();
        }
        else
        {
            throw std::invalid_argument("Invalid logging choice");
        }
    }

    Logger &LogOpt::getInstance()
    {
        if (!loggerInstance)
        {
            //initializeLogger("console");
            throw std::runtime_error("Logger not initialized");
        }
        return *loggerInstance;
    }
} 