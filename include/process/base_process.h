#ifndef BASE_PROCESS_H
#define BASE_PROCESS_H

#include "process_interface.h"
#include <memory>
#include <string>
namespace process
{

    void signalHandler(int signum);
    void setupSignalHandling();

    enum NapTimeMs { SMALL = 50, SHORT = 100, MEDIUM = 500, LONG = 1000 };
    enum NapTimeSec { SHORTS = 5, MEDIUMS = 15, LONGS = 30 };
    class BaseProcess : public IProcess 
    {
    public:

        static std::atomic<bool> &continueFlag();
        static int               &exitCode();

        static void               sleepRandomMs(bool display = false);
        static void               sleepRandomSec(bool display = false);
        static int                randomMs();
        static int                randomSec();

    protected:
        virtual ~BaseProcess() = default;
        BaseProcess();

        void logLifetime() const;

        std::chrono::time_point<std::chrono::high_resolution_clock> startTime_ =
                std::chrono::high_resolution_clock::now();
        void preWork() override;
        void postWork() override;
        static std::atomic<bool> continue_;
        static int exitCode_;
        std::string reason_ = "End of life";
    };

} // namespace process

#endif // BASE_PROCESS_H