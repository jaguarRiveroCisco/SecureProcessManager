#pragma once

#ifndef SYSTEM_PROCESS_H
#define SYSTEM_PROCESS_H
#include "process_simulator.h"
namespace process
{

    class SystemProcess : public ProcessSimulator 
    {
    public:
        ~SystemProcess() override = default;
        void work() override;

    private:
        bool spawnChildProcess(const std::vector<char *> &args);
    };
}

#endif // SYSTEM_PROCESS_H