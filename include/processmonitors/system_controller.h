#pragma once

#ifndef SYSTEM_CONTROLLER_H
#define SYSTEM_CONTROLLER_H

#include "process_controller.h"

namespace process
{
    class SystemController : public ProcessMonitor 
    {
    protected:
        void forkAndExecuteChildProcess() override;
    };
}

#endif // SYSTEM_CONTROLLER_H