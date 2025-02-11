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
        ~ProcessSimulator() override = default;

    protected:
        bool proceed() const;

    };
} // namespace process

#endif // SIMUL_PROCESS_H
