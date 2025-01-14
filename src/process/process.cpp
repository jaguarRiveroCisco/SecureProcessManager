#include "process.h"
#include <iostream>
#include <thread>
#include "communicator.h"
#include "console_logger.h"
namespace process
{
    void Process::work()
    {
        // Real process work implementation
        Communicator::getInstance().sendCreationMessage();
        tools::ConsoleLogger::getInstance() << "[START] Process ID: " << getpid();
        tools::ConsoleLogger::getInstance().flush(tools::LogLevel::INFO);
        // Add real process work code here
        while (continue_)
        {
            /* code */
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate some work
        }
        logLifetime("Process ended");
        _exit(0); // Ensure the child process exits immediately
    }
} // namespace process
