#pragma once

#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include "process_monitor.h"

namespace process
{
    class SystemMonitor : public ProcessMonitor 
    {
    protected:
        void launchChildProcess() override;
    };
}

#endif // SYSTEM_MONITOR_H