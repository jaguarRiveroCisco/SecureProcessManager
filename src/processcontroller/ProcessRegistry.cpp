//
// Created by Jaguar Rivero on 2025-02-04.
//

#include "ProcessRegistry.h"

namespace process
{
    ProcessMonitorMap ProcessRegistry::handlers_;
    DeceasedPidsSet   ProcessRegistry::deceasedProcesses_;
    std::shared_mutex ProcessRegistry::mutex_;

    ProcessMonitorMap ProcessRegistry::handlers()
    {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        ProcessMonitorMap activeHandlers;
        for (const auto &[pid, monitor] : handlers_)
        {
            if (!deceasedProcesses_.contains(pid))
            {
                // Use the existing shared_ptr without creating a new instance
                activeHandlers.emplace(pid, monitor);
            }
        }
        return activeHandlers;
    }

bool ProcessRegistry::addMonitorProcess(pid_t pid, ProcessMonitorPtr processMonitor)
    {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        if (!handlers_.contains(pid))
        {
            // If the key does not exist, insert the new element
            auto res = handlers_.emplace(pid, std::move(processMonitor));
            return res.second; // Return true if insertion was successful
        }
        return false; // Return false if the key already exists
    }

    ProcessMonitor *ProcessRegistry::findMonitor(const pid_t pid)
    {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        if (handlers_.contains(pid))
        {
            return handlers_.at(pid).get(); // Return the raw pointer to the ProcessMonitor
        }
        return nullptr; // Return nullptr if not found
    }

    bool ProcessRegistry::markProcessAsDeceased(const pid_t pid)
    {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        if (handlers_.contains(pid))
        {
            deceasedProcesses_.insert(pid);
            return true;
        }
        return false;
    }

} // namespace process
