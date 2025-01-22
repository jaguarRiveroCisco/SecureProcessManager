#ifndef PROCESS_BASE_H
#define PROCESS_BASE_H
#include "synchro.h"
#include "locked_boolean.h"
namespace process
{
    class BaseHandler 
    {
    public:
        pid_t getPid() const;
        std::atomic<bool>  monitoring();
        void               monitoring(bool value);
        void               createMonitorProcessThread();

    protected:
        BaseHandler() = default;
        virtual ~BaseHandler() = default;
        void          displayProcessStatus(int &status);
        bool          isProcessRunning();
        void          terminateProcess();
        void          killProcess();
        void          intProcess();
        void          sendSignal(int signal);
        void          monitorProcessThread();
        pid_t         pid_{0};
    private:
        concurrency::LockedBoolean monitor_;
    };

} // namespace process

#endif // PROCESS_BASE_H
