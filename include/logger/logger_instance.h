#ifndef LOGGER_INSTANCE_H
#define LOGGER_INSTANCE_H
#include <memory>
#include <stdexcept>
#include <string>
#include "logger.h"

namespace tools
{
    struct LogOpt 
    {
    public:
        static void initializeLogger(const std::string &choice);

        static Logger &getInstance();

    private:
        static Logger* loggerInstance;
    };


} // namespace tools

#endif // LOGGER_INSTANCE_H
