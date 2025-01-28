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

        using HandlerFactory = std::function<std::unique_ptr<ProcessMonitor>()>;
        static std::unordered_map<std::string, HandlerFactory> handlerFactoryMap_;

        template<typename MonitorType, typename ProcessType> 
        static std::unique_ptr<ProcessMonitor> createHandler();

        static void initializeFactory();
    };
    // Template function to create a handler
    template<typename MonitorType, typename ProcessType> 
    std::unique_ptr<ProcessMonitor> MainController::createHandler()
    {
        auto handler = std::make_unique<MonitorType>();
        handler->init(std::make_unique<ProcessType>());
        return handler;
    }


} // namespace process

#endif // MAIN_CONTROLLER_H
