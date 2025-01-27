#ifndef PROCESS_BASE_H
#define PROCESS_BASE_H
#include "synchro.h"
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
        std::atomic<bool> monitor_ {false};
    };

} // namespace process

#endif // PROCESS_BASE_H
