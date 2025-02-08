#pragma once

#ifndef CONTROLLER_CLI_CONTROLLER_H
#define CONTROLLER_CLI_CONTROLLER_H
#include "thread_controller.h"
namespace cli::driver
{
    class CLIController final : public ThreadController
    {
    protected:
        void runThread() override;
    };
} // namespace cli::driver
#endif // CONTROLLER_CLI_CONTROLLER_H
