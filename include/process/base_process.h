#ifndef BASE_PROCESS_H
#define BASE_PROCESS_H

#include "process_interface.h"
#include "console_logger.h"
#include <memory>
namespace process
{

    void signalHandler(int signum);
    void setupSignalHandling();

    class BaseProcess : public IProcess 
    {
    public:
        virtual ~BaseProcess() = default;
        static std::atomic<bool> continue_;

    protected:
        std::chrono::time_point<std::chrono::high_resolution_clock> startTime_ =
                std::chrono::high_resolution_clock::now();

        BaseProcess() { setupSignalHandling(); }

        void logLifetime(const std::string &reason = "End of life") const;

        static std::unique_ptr<tools::ILogger> logger_;


    };

} // namespace process

#endif // BASE_PROCESS_H