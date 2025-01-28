#pragma once

#ifndef SIMUL_PROCESS_H
#define SIMUL_PROCESS_H
#include "process.h"

namespace process
{
    class ProcessSimulator : public Process 
    {
    public:
        void work() override;
        virtual ~ProcessSimulator() = default;

    protected:
        bool proceed();

    };
} // namespace process

#endif // SIMUL_PROCESS_H
