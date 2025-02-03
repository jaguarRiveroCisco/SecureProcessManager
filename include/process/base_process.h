#ifndef BASE_PROCESS_H
#define BASE_PROCESS_H

#include "process_interface.h"
#include <string>
#include "time_manager.h"
namespace process
{

    void signalHandler(int signum);
    void setupSignalHandling();

    class BaseProcess : public IProcess 
    {
    public:
        static std::atomic<bool> &continueFlag();
        static std::atomic<int>  &exitCode();
    protected:
        pid_t pid_{0};


    protected:
        ~BaseProcess() override = default;
        BaseProcess();
        void logLifetime() const;
        void preWork(pid_t pid) override;
        void postWork() override;

        tools::TimeManager timeManager_;

        static std::atomic<bool> continue_;
        static std::atomic<int>  exitCode_;
        mutable std::string reason_ = "End of life";
    };

} // namespace process

#endif // BASE_PROCESS_H