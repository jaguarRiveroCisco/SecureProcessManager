#include "process.h"
#include <iostream>
#include <thread>
#include "communicator.h"
#include "logger_instance.h"
#include "random_stuff.h"
namespace process
{
    void Process::work()
    {
        preWork(getpid());  

        // Add real process work code here
        while (continue_)
        {
            /* code */
            if (getppid() == 1)
            {
                reason_ = "Parent process has terminated.";
                break;
            }
            tools::sleepRandomMs();
        }
        postWork();
    }
} // namespace process
