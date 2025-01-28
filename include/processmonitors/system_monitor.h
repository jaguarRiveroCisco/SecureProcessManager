#pragma once

#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include "process_controller.h"

namespace process
{
    class SystemMonitor : public ProcessMonitor 
    {
    protected:
        void forkAndExecuteChildProcess() override;
    };
}

#endif // SYSTEM_MONITOR_H