#ifndef ILOGGER_H
#define ILOGGER_H

#include <string>

namespace tools
{

    enum class LogLevel { INFO, WARNING, ERROR };

    struct ILogger 
    {
        virtual ~ILogger() = default;
        virtual void log(LogLevel level, const std::string &message) = 0;
        virtual void logInfo(const std::string &message)             = 0;
        virtual void logWarning(const std::string &message)          = 0;
        virtual void logError(const std::string &message)            = 0;
    };

} // namespace tools

#endif // ILOGGER_H
