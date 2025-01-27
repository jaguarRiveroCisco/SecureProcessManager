#pragma once

#ifndef PROCESS_MONITOR_H
#define PROCESS_MONITOR_H

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
        void    terminateProcess();
        void    killProcess();
        void    intProcess();
        void    monitorProcessThread();
        pid_t pid_{0};
    private:
        std::atomic<bool> monitor_ {false};
        static std::atomic<bool> running_;
        static std::atomic<bool> respawn_;
    };

} // namespace process

#endif // PROCESS_MONITOR_H
