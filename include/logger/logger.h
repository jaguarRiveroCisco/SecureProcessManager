#ifndef LOGGER_H
#define LOGGER_H

#include "logger_interface.h"
#include <sstream>

namespace tools
{
    struct LoggerManager;
    class Logger : public ILogger 
    {
        friend struct LoggerManager;
    public:
        void flush(LogLevel level) override;
        void log(LogLevel level, const std::string &message) override;
        void logInfo(const std::string &message) override;
        void logWarning(const std::string &message) override;
        void logError(const std::string &message) override;
        void logException(const std::string &message) override;

        template <typename T>
        Logger &operator<<(const T &message)
        {
            currentMessageStream << message;
            return *this;
        }
        ~Logger() override = default;
        Logger(const Logger &)                            = delete;
        Logger                 &operator=(const Logger &) = delete;
    protected:
        virtual void outputLog(const std::string &message) = 0; // Pure virtual function
        Logger() = default;
    private:


        std::ostringstream      currentMessageStream;
        static std::string logLevelToString(LogLevel level);
    };
} // namespace tools

#endif // LOGGER_H