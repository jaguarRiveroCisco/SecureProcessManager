#pragma once

#ifndef PROCESS_MONITOR_H
#define PROCESS_MONITOR_H

#include "base_process.h"
#include "synchro.h"

namespace process
{
    class ProcessMonitor 
    {
    public:
        ProcessMonitor()          = default;
        virtual ~ProcessMonitor() = default;
        std::atomic<bool> &monitoring();
        pid_t              getPid() const;
        void               createMonitorProcessThread();
        void               init(std::unique_ptr<IProcess> process);
        virtual void       launchChildProcess();
        void               terminateProcess();
        void               killProcess();
        void               intProcess();
        void               monitorProcessThread();

    protected:

        pid_t                     pid_{0};
        std::unique_ptr<IProcess> process_;

    private:
        std::atomic<bool> monitor_{false};
    };

} // namespace process

#endif // PROCESS_MONITOR_H
