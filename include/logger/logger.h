#ifndef LOGGER_H
#define LOGGER_H

#include "logger_interface.h"
#include <sstream>

namespace tools
{
    struct LogOpt;
    class Logger : public ILogger 
    {
        friend struct LogOpt;
    public:
        void flush(LogLevel level);
        void log(LogLevel level, const std::string &message);
        void logInfo(const std::string &message);
        void logWarning(const std::string &message);
        void logError(const std::string &message);

        template <typename T>
        Logger &operator<<(const T &message)
        {
            currentMessageStream << message;
            return *this;
        }
        virtual ~Logger();

    protected:
        virtual void outputLog(const std::string &message) = 0; // Pure virtual function
        Logger();
    private:

        Logger(const Logger &)                            = delete;
        Logger                 &operator=(const Logger &) = delete;
        std::ostringstream      currentMessageStream;
        std::string logLevelToString(LogLevel level) const;
    };
} // namespace tools

#endif // LOGGER_H