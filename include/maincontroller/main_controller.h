#pragma once

#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include "process_controller.h"

namespace process
{
    class MainController final
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
        static std::unique_ptr<ProcessMonitor> createHandlerByType(const std::string &processType);
    };
} // namespace process

#endif // MAIN_CONTROLLER_H
