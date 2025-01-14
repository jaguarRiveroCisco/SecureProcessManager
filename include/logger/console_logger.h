#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H

#include "logger.h"

namespace tools
{
    class ConsoleLogger final : public Logger 
    {
        friend struct LogOpt;
    protected:
        void outputLog(const std::string &message) override;
    private:
        static ConsoleLogger &getInstance()
        {
            static ConsoleLogger instance;
            return instance;
        }
        ConsoleLogger() = default;
    };
}



#endif // CONSOLE_LOGGER_H