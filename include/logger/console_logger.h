#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H

#include <memory> // For std::unique_ptr
#include "logger.h"
#include "semaphore_guard.h" // Ensure SemaphoreGuard is defined
namespace tools
{
    class ConsoleLogger final : public Logger 
    {
        friend LoggerManager;
    public:
        ConsoleLogger() 
        {
            sem_ = std::make_unique<concurrency::SemaphoreGuard>("/console_logger");
        }

    protected:
        void outputLog(const std::string &message) override;

    private:
        std::unique_ptr <concurrency::SemaphoreGuard>      sem_;
        static std::unique_ptr<ConsoleLogger> instance;


        // Public method to get the instance
        static ConsoleLogger &getInstance();

        // Reset the singleton instance
        static void resetInstance();
    };
} // namespace tools

#endif // CONSOLE_LOGGER_H
