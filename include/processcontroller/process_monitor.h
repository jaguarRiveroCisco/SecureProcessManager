#pragma once

#ifndef PROCESS_MONITOR_H
#define PROCESS_MONITOR_H

#include "synchro.h"
namespace process
{
    class ProcessMonitor 
    {
    public:

        std::atomic<bool>        &monitoring();
        pid_t                     getPid() const;
        void                      createMonitorProcessThread();

    protected:
        ProcessMonitor()          = default;
        virtual ~ProcessMonitor() = default;
        void  terminateProcess();
        void  killProcess();
        void  intProcess();
        void  monitorProcessThread();
        pid_t pid_{0};

    private:
        std::atomic<bool>        monitor_{false};

    };

} // namespace process

#endif // PROCESS_MONITOR_H
