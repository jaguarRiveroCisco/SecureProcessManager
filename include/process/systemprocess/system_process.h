#pragma once

#ifndef SYSTEM_PROCESS_H
#define SYSTEM_PROCESS_H
#include "simul_process.h"
namespace process
{

    class SystemProcess : public ProcessSimulator 
    {
    public:
        ~SystemProcess() override = default;
        void work() override;
    };
}

#endif // SYSTEM_PROCESS_H