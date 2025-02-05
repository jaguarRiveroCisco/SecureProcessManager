#pragma once

#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include "process_controller.h"
#include "ProcessRegistry.h"

namespace process
{
    using HandlerFactory = std::function<ProcessMonitorPtr()>;
    using FactoryMap     = std::unordered_map<std::string, HandlerFactory>;
    class MainController final
    {
    public:
        static void initializeController(const std::string &processType, int numProcesses);
        static void processLifecycleLoop();
        static void stop();
    private:
        static void restoreHandlerCount();
        static void createHandlers(int numHandlers);
        static void createHandler();
        static void CreateMonitoringThreads();
        static void createMonitorThread(const std::string &);
        static void terminateMonitorThread(const std::string &);
        static void MonitorProcessTermination();
        static void startThreads();
        static void joinThreads();


        static FactoryMap handlerFactoryMap_;

        template<typename MonitorType, typename ProcessType> 
        static ProcessMonitorPtr manufacture();

        static void initializeFactory();

        static std::atomic<int> counter_;
    };
    // Template function to create a handler
    template<typename MonitorType, typename ProcessType> 
    ProcessMonitorPtr MainController::manufacture()
    {
        auto handler = std::make_unique<MonitorType>();
        handler->collectAndLaunch(std::make_unique<ProcessType>());
        return handler;
    }


} // namespace process

#endif // MAIN_CONTROLLER_H
