#include "logger_instance.h"
#include "file_logger.h"
#include "console_logger.h"
#include <iostream>
#include "timestamp.h"
namespace tools
{
    Logger* LoggerManager::loggerInstance = nullptr;
    std::string LoggerManager::loggerType_ = "console";

    Logger &LoggerManager::getInstance()
    {
        if (!loggerInstance)
        {
            throw std::runtime_error("Logger not initialized");
        }
        return *loggerInstance;
    }

    void LoggerManager::createLoggerType()
    {
        if(loggerType_.empty())
        {
            throw std::invalid_argument("Logger type cannot be empty");
        }
        std::cout << TimeStamp::get() + "INFO: [CREATING LOGGER] LoggerManager creating Logger type: " << loggerType_
                  << std::endl;
        resetLogger();
        createLogger();
    }

    std::string &LoggerManager::loggerType()
    {
        return loggerType_;
    }

    void LoggerManager::createLogger()
    {
        if (loggerType_ == "console")
        {
            loggerInstance = &ConsoleLogger::getInstance();
        }
        else if (loggerType_ == "file")
        {
            loggerInstance = &FileLogger::getInstance();
        }
        else
        {
            throw std::invalid_argument("Invalid logging choice");
        }
    }
    void LoggerManager::resetLogger()
    {
        if (loggerInstance)
        {
            loggerInstance = nullptr;
        }
        ConsoleLogger::resetInstance();
        FileLogger::resetInstance();
    }

    Logger &LoggerManager::consoleLogger() 
    {
        static ConsoleLogger cl;
        return cl;
    }
} 