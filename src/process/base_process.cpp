#include "base_process.h"
#include <unistd.h>
namespace process
{
    std::atomic<bool> BaseProcess::continue_{true};
    void BaseProcess::logLifetime(const std::string &reason) const
    {
        auto endTime  = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime_).count();
        std::cout << "[END]   Process ID: " << getpid() << " | Lifetime: " << duration << " ms"
                  << " | Reason: " << reason << std::endl;
    }
}