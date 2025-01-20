#ifndef PROCESS_BASE_H
#define PROCESS_BASE_H
#include "synchro.h"
namespace process
{
    class BaseHandler 
    {
    public:
        pid_t getPid() const;
        std::atomic<bool>& monitoring() { return monitoring_; }

    protected:
        BaseHandler() = default;
        virtual ~BaseHandler() = default;
        void          displayProcessStatus(int &status);
        bool          isProcessRunning();
        void          terminateProcess();
        void          killProcess();
        void          intProcess();
        void          sendSignal(int signal);
        void          createMonitorProcessThread();
        void          monitorProcessThread();
        pid_t         pid_{0};
        concurrency::Synchro *synchro_{nullptr};
        std::atomic<bool> monitoring_ {true};
    };
} // namespace process

#endif // PROCESS_BASE_H
