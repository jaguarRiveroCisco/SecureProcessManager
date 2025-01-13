#include "logger.h"
#include <atomic>
#include <sstream>
#include <string>
#include "timestamp.h"
#include <iostream>

namespace tools
{

    Logger::Logger() : done(false) 
    {
        std::cout << "[Logger] Initialized at " << TimeStamp::get() << std::endl;
        logThread = std::thread(&Logger::processLogs, this);
    }

    Logger::~Logger()
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            done = true;
            condVar.notify_all();
        }
        logThread.join();
        std::cout << "[Logger] Destroyed at " << TimeStamp::get() << std::endl;
    }

    void Logger::log(LogLevel level, const std::string &message)
    {
        std::string logEntry = "[" + TimeStamp::get() + "] " + logLevelToString(level) + ": " + message;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            logQueue.push(logEntry);
        }
        condVar.notify_one();
    }

    void Logger::flush(LogLevel level)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::string                 message = currentMessageStream.str();
        if (!message.empty())
        {
            log(level, message);
            currentMessageStream.str(""); // Clear the stream
        }
    }

    void Logger::processLogs()
    {
        while (!done)
        {
            std::unique_lock<std::mutex> lock(mutex_);
            condVar.wait(lock, [this] { return !logQueue.empty() || done; });

            while (!logQueue.empty())
            {
                std::string message = logQueue.front();
                logQueue.pop();
                lock.unlock();

                // Output log message to the console or a file
                std::cout << message << std::endl;

                lock.lock();
            }
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

    int example()
    {
        tools::Logger logger;

        logger.log(tools::LogLevel::INFO, "This is an info message.");
        logger.log(tools::LogLevel::WARNING, "This is a warning message.");
        logger.log(tools::LogLevel::ERROR, "This is an error message.");

        logger.logInfo("This is an info message.");
        logger.logWarning("This is a warning message.");
        logger.logError("This is an error message.");

        logger << "Starting a new log entry with operator<< ";
        logger << "and adding more details.";
        logger.flush(tools::LogLevel::INFO);

        return 0;
    }

} // namespace tools