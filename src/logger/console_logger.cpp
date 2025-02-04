#include "console_logger.h"
#include <iostream>
#include <semaphore.h>
#include <memory>
#include "process_controller.h"

namespace tools
{

    class BufferedConsoleLogger
    {
    public:
        static BufferedConsoleLogger& getInstance() {
            static BufferedConsoleLogger instance;
            return instance;
        }

        void log(const std::string& message)
        {
            buffer_.push_back(message);
            if (buffer_.size() >= bufferLimit_)
            {
                flush(); // Write out the buffer if it reaches the limit
            }
        }

        void flush() {
            for (const auto& msg : buffer_)
            int{
                std::cout << msg << std::endl;
            }
            buffer_.clear();
        }

        ~BufferedConsoleLogger()
        {
            flush(); // Ensure all messages are written out on destruction
        }

    private:
        BufferedConsoleLogger() : bufferLimit_(process::ProcessController::numProcesses() * 2)
        {
        }
        std::vector<std::string> buffer_;
        size_t bufferLimit_{};
    };



    // Initialize the static member
    std::unique_ptr<ConsoleLogger> ConsoleLogger::instance = nullptr;
    sem_t* ConsoleLogger::sem_ = nullptr;

    void ConsoleLogger::outputLog(const std::string &message) 
    {
        static auto& logger = BufferedConsoleLogger::getInstance();
        sem_wait(sem_); // Wait (lock the semaphore)
        logger.log(message);
        sem_post(sem_); // Signal (unlock the semaphore)
    }

    ConsoleLogger &ConsoleLogger::getInstance()
    {
        if (!instance)
        {
            instance = std::make_unique<ConsoleLogger>();
        }
        return *instance;
    }

    // Reset the singleton instance
    void ConsoleLogger::resetInstance()
    {
        instance.reset(); // Automatically deletes the instance and sets to nullptr
    }
}