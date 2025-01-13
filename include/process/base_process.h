#ifndef BASE_PROCESS_H
#define BASE_PROCESS_H

#include "process_interface.h"
namespace process
{

    class BaseProcess : public IProcess 
    {
    public:
        virtual ~BaseProcess() = default;
        static std::atomic<bool> continue_;

    protected:
        std::chrono::time_point<std::chrono::high_resolution_clock> startTime_ =
                std::chrono::high_resolution_clock::now();

        BaseProcess() = default;

        void logLifetime(const std::string &reason = "End of life") const;
    };
}

#endif // BASE_PROCESS_H