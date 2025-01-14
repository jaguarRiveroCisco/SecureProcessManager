#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H

#include "logger.h"

namespace tools
{
    class ConsoleLogger : public Logger 
    {
    public:
        static ConsoleLogger &getInstance()
        {
            static ConsoleLogger instance;
            return instance;
        }

    protected:
        void outputLog(const std::string &message) override;
    };
}



#endif // CONSOLE_LOGGER_H