#include "logger.h"
#include <atomic>
#include <sstream>
#include <string>
#include "timestamp.h"
#include <iostream>

namespace tools
{

    Logger::Logger()
    {
        std::cout << "[Logger] Initialized at " << TimeStamp::get() << std::endl;
    }

    Logger::~Logger()
    {
        std::cout << "[Logger] Destroyed at " << TimeStamp::get() << std::endl;
    }

    void Logger::log(LogLevel level, const std::string &message)
    {
        std::string logEntry = "[" + TimeStamp::get() + "] " + logLevelToString(level) + ": " + message;
        outputLog(logEntry);
    }

    void Logger::flush(LogLevel level)
    {
        std::string                 message = currentMessageStream.str();
        if (!message.empty())
        {
            log(level, message);
            currentMessageStream.str(""); // Clear the stream
        }
    }
    void Logger::logInfo(const std::string &message)
    {
        log(LogLevel::INFO, message);
    }
    void Logger::logWarning(const std::string &message)
    {
        log(LogLevel::WARNING, message);
    }
    void Logger::logError(const std::string &message)
    {
        log(LogLevel::ERROR, message);
    }
    std::string Logger::logLevelToString(LogLevel level) const
    {
        switch (level)
        {
            case LogLevel::INFO:
                return "INFO";
            case LogLevel::WARNING:
                return "WARNING";
            case LogLevel::ERROR:
                return "ERROR";
            default:
                return "UNKNOWN";
        }
    }

    //int example()
    //{
    //    tools::ConsoleLogger logger;
//
    //    logger.log(tools::LogLevel::INFO, "This is an info message.");
    //    logger.log(tools::LogLevel::WARNING, "This is a warning message.");
    //    logger.log(tools::LogLevel::ERROR, "This is an error message.");
//
    //    logger.logInfo("This is an info message.");
    //    logger.logWarning("This is a warning message.");
    //    logger.logError("This is an error message.");
//
    //    logger << "Starting a new log entry with operator<< ";
    //    logger << "and adding more details.";
    //    logger.flush(tools::LogLevel::INFO);
//
    //    return 0;
    //}

} // namespace tools