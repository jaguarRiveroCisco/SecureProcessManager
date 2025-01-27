#pragma once

#ifndef SYSTEM_CONTROLLER_H
#define SYSTEM_CONTROLLER_H

#include "controller_base.h"

namespace process
{
    class SystemController : public ProcessController 
    {
    protected:
        void forkAndExecuteChildProcess() override;
    };
}

#endif // SYSTEM_CONTROLLER_H