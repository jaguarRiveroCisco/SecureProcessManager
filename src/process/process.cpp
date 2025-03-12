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
        if ( 1 == getpid())
        {
            // we have been cloned with CLONE_NEWPID flag so we think we are PID == 1
            // At this point we should have pid_ set to the real pid
            preWork(pid_);
        }
        else
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
