#ifndef LOGGER_H
#define LOGGER_H

#include "logger_interface.h"
#include <sstream>

namespace tools
{
    class Logger : public ILogger
    {
    public:
        Logger();
        virtual ~Logger();

        void log(LogLevel level, const std::string &message);

        template<typename T> Logger &operator<<(const T &message)
        {
            currentMessageStream << message;
            return *this;
        }

        void flush(LogLevel level);

        void logInfo(const std::string &message);
        void logWarning(const std::string &message);
        void logError(const std::string &message);

    protected:
        virtual void outputLog(const std::string &message) = 0; // Pure virtual function

    private:
        std::ostringstream      currentMessageStream;
        std::string logLevelToString(LogLevel level) const;
    };
} // namespace tools

#endif // LOGGER_H