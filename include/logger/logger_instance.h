#ifndef LOGGER_INSTANCE_H
#define LOGGER_INSTANCE_H
#include <memory>
#include <stdexcept>
#include <string>
#include "logger.h"

namespace tools
{
    struct LoggerManager {
    public:
        static void         createLoggerType();
        static std::string &loggerType();
        static Logger      &getInstance();
        static Logger      &consoleLogger();

    private:
        static Logger     *loggerInstance;
        static std::string loggerType_;
        static void        createLogger();
        static void        resetLogger();
    };


} // namespace tools

#endif // LOGGER_INSTANCE_H
