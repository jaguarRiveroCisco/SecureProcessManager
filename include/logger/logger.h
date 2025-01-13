#ifndef LOGGER_H
#define LOGGER_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

namespace tools
{

    enum class LogLevel { INFO, WARNING, ERROR };

    class Logger 
    {
    public:
        Logger();
        ~Logger();

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