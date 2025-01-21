#include "process.h"
#include <iostream>
#include <thread>
#include "communicator.h"
#include "logger_instance.h"

namespace process
{
    void Process::work()
    {
        preWork();  

        // Add real process work code here
        while (continue_)
        {
            /* code */
            if (getppid() == 1)
            {
                reason_ = "Parent process has terminated. Exiting child process.";
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(NapTimeMs::SHORT)); // Simulate some work
        }
        postWork();

    }
} // namespace process
