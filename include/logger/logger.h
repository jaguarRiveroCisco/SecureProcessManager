#ifndef LOGGER_H
#define LOGGER_H

#include "logger_interface.h"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

namespace tools
{
    class Logger : protected ILogger
    {
    public:
        Logger();
        virtual ~Logger();

        void log(LogLevel level, const std::string &message);

        template<typename T> Logger &operator<<(const T &message)
        {
            std::lock_guard<std::mutex> lock(mutex_);
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
        std::queue<std::string> logQueue;
        std::mutex              mutex_;
        std::condition_variable condVar;
        std::thread             logThread;
        std::atomic<bool>       done;
        std::ostringstream      currentMessageStream;

        void processLogs();
        std::string logLevelToString(LogLevel level) const;
    };




    int example();

} // namespace tools

#endif // LOGGER_H