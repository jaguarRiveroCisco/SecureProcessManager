#ifndef PROCESS_BASE_H
#define PROCESS_BASE_H
#include "synchro.h"
namespace process
{
    class ProcessMonitor 
    {
    public:
        pid_t getPid() const;
        static bool running();
        static bool respawn();
        static void running(bool);
        static void respawn(bool);
        bool        monitoring() const;
        void        monitoring(bool);
        void        createMonitorProcessThread();

    protected:
        ProcessMonitor() = default;
        virtual ~ProcessMonitor() = default;
        void    displayProcessStatus(int &status);
        bool    isProcessRunning();
        void    terminateProcess();
        void    killProcess();
        void    intProcess();
        void    sendSignal(int signal);
        void    monitorProcessThread();
        pid_t pid_{0};
    private:
        std::atomic<bool> monitor_ {false};
        static std::atomic<bool> running_;
        static std::atomic<bool> respawn_;
    };

} // namespace process

#endif // PROCESS_BASE_H
