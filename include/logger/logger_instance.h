#ifndef LOGGER_INSTANCE_H
#define LOGGER_INSTANCE_H
#include "logger.h"

namespace tools
{
    struct LoggerManager 
    {
        static void         createLoggerType();
        static std::string &loggerType();
        static Logger      &getInstance();
    private:
        static Logger     *loggerInstance;
        static std::string loggerType_;
        static void        createLogger();
        static void        resetLogger();
    };


} // namespace tools

#endif // LOGGER_INSTANCE_H
