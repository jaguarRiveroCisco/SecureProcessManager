#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H

#include "logger.h"

namespace tools
{
    class ConsoleLogger : public Logger 
    {
    protected:
        void outputLog(const std::string &message) override;
    };
}



#endif // CONSOLE_LOGGER_H