//
// Created by Jaguar Rivero on 2025-02-04.
//

#ifndef PROCESSREGISTRY_H
#define PROCESSREGISTRY_H

#include <functional>
#include <shared_mutex>
#include <unordered_map>
#include "process_monitor.h"
#include <unordered_set>

namespace process
{
    using ProcessMonitorPtr = std::shared_ptr<ProcessMonitor>;
    using ProcessMonitorMap = std::unordered_map<pid_t, ProcessMonitorPtr>;
    using DeceasedPidsSet = std::unordered_set<pid_t>;

    class ProcessRegistry
    {
        static ProcessMonitorMap handlers_;
        static DeceasedPidsSet deceasedProcesses_;
        static std::shared_mutex mutex_;

    public:
        static ProcessMonitorMap handlers();

        static bool addMonitorProcess(pid_t pid, ProcessMonitorPtr processMonitor);

        static ProcessMonitor *findMonitor(pid_t pid);

        static bool markProcessAsDeceased(pid_t pid);

    };

}


#endif //PROCESSREGISTRY_H
