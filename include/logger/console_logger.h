#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H

#include <memory> // For std::unique_ptr
#include "logger.h"
#include <semaphore.h>
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

namespace tools
{
    class ConsoleLogger final : public Logger 
    {
        friend LoggerManager;
    public:
        ConsoleLogger() 
        {
            sem_ = sem_open("/console_logger_sem", O_CREAT, 0644, 1);
            if (sem_ == SEM_FAILED)
            {
                std::cerr << "Failed to open semaphore" << std::endl;
                std::exit(EXIT_FAILURE);
            }
        }
        ~ConsoleLogger() override
        {
            // Close the named semaphore
            if (sem_ != nullptr)
            {
                sem_close(sem_);
                sem_unlink("/console_logger_sem"); // Unlink the semaphore
            }
        }

    protected:
        void outputLog(const std::string &message) override;

    private:
        static sem_t*      sem_;
        static std::unique_ptr<ConsoleLogger> instance;


        // Public method to get the instance
        static ConsoleLogger &getInstance();

        // Reset the singleton instance
        static void resetInstance();
    };
} // namespace tools

#endif // CONSOLE_LOGGER_H
