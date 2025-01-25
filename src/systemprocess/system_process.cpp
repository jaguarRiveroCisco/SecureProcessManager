#include "system_process.h"
#include "random_stuff.h"
#include "logger_instance.h"

namespace process
{
    void SystemProcess::work()
    {
        preWork();
        timeManager_.setSleepDuration();
        while (proceed())
        {
            // Execute an OS command
            int result = std::system("ls -l"); // Replace "ls -l" with the desired command
            if (result == -1)
            {
                tools::LoggerManager::getInstance().logInfo("Failed to execute command");
            }
            tools::sleepRandomMs(true);
            timeManager_.currentTime() = std::chrono::high_resolution_clock::now();
        }
        postWork();
    }
}
