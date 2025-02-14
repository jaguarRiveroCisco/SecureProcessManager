#pragma once

#ifndef NETWORK_PROCESS_H
#define NETWORK_PROCESS_H

#include "process_simulator.h"

namespace process
{

    class NetworkProcess final : public ProcessSimulator
    {
    public:
        ~NetworkProcess() override = default;
        void work() override;
    };

} // namespace process

#endif // NETWORK_PROCESS_H
