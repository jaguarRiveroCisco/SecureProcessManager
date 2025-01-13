#include "base_process.h"
#include <unistd.h>
namespace process
{
    void BaseProcess::logLifetime(const std::string &reason) const
    {
        auto endTime  = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime_).count();
        std::cout << "[END]   Process ID: " << getpid() << " | Lifetime: " << duration << " ms"
                  << " | Reason: " << reason << std::endl;
    }
    void BaseProcess::setupSignalHandling()
    {
        signal(SIGTERM, BaseProcess::signalHandler);
        signal(SIGINT, BaseProcess::signalHandler);
    }

    void BaseProcess::signalHandler(int signum)
    {
        std::cout << "Process " << getpid() << " received signal " << signum << std::endl;
        exit(signum);
    }
}