#include "logger_instance.h"
#include "file_logger.h"
#include "console_logger.h"
#include <iostream>
#include "timestamp.h"
namespace tools
{
    Logger* LogOpt::loggerInstance = nullptr;
    std::string LogOpt::loggerType_ = "console";

    Logger &LogOpt::getInstance()
    {
        if (!loggerInstance)
        {
            throw std::runtime_error("Logger not initialized");
        }
        return *loggerInstance;
    }

    void LogOpt::createLoggerType(const std::string &loggerType)
    {
        loggerType_ = loggerType;
        std::cout << TimeStamp::get() + "INFO: [CREATING] LogOpt creating Logger type: " << loggerType_ << std::endl;
        resetLogger();
        createLogger();
    }

    std::string &LogOpt::loggerType()
    {
        return loggerType_;
    }

    void LogOpt::createLogger()
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
    void LogOpt::resetLogger()
    {
        if (loggerInstance)
        {
            loggerInstance = nullptr;
        }
        ConsoleLogger::resetInstance();
        FileLogger::resetInstance();
    }

    Logger &LogOpt::consoleLogger() 
    {
        static ConsoleLogger cl;
        return cl;
    }
} 