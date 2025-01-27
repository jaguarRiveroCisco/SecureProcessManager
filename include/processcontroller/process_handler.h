#pragma once

#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

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

#endif // MAIN_CONTROLLER_H
