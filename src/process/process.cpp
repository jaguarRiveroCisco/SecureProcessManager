#include "process.h"
#include <iostream>
#include <thread>
#include "communicator.h"
#include "logger_instance.h"

namespace process
{
    void Process::work()
    {
        tools::LogOpt::createLoggerType(tools::LogOpt::loggerType());
        // Real process work implementation
        Communicator::getInstance().sendCreationMessage();
        tools::LogOpt::getInstance() << "[EXECUTING] Process work started";
        tools::LogOpt::getInstance().flush(tools::LogLevel::INFO);
        // Add real process work code here
        while (continue_)
        {
            /* code */
            if (getppid() == 1)
            {
                tools::LogOpt::getInstance() << "Parent process has terminated. Exiting child process.";
                tools::LogOpt::getInstance().flush(tools::LogLevel::INFO);
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate some work
        }
        logLifetime("Process ended");
        _exit(0); // Ensure the child process exits immediately
    }
} // namespace process
