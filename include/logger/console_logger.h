#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H

#include <memory> // For std::unique_ptr
#include "logger.h"

namespace tools
{
    class ConsoleLogger final : public Logger 
    {
        friend LoggerManager;
    public:
        ConsoleLogger() = default; // Private constructor

    protected:
        void outputLog(const std::string &message) override;

    private:
        static std::unique_ptr<ConsoleLogger> instance;


        // Public method to get the instance
        static ConsoleLogger &getInstance()
        {
            if (!instance)
            {
                instance = std::make_unique<ConsoleLogger>();
            }
            return *instance;
        }

        // Reset the singleton instance
        static void resetInstance()
        {
            instance.reset(); // Automatically deletes the instance and sets to nullptr
        }
    };
} // namespace tools

#endif // CONSOLE_LOGGER_H
