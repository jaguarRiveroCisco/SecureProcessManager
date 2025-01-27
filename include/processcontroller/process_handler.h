#pragma once

#ifndef PROCESS_HANDLER_H
#define PROCESS_HANDLER_H

#include "process_controller.h"

namespace process
{
    class MainController final : public ProcessController 
    {
    public:
        static void run(const std::string &processType, int numProcesses);
        static void CreateMonitoringThreads();

    private:
        static void processLifecycleLoop();
        static void restoreHandlerCount();
        static void createHandlers(int numHandlers);
        static void createHandler();
        static bool removeHandler();
    };
} // namespace process

#endif // PROCESS_HANDLER_H
