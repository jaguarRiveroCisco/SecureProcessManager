
#include "main_controller.h"
#include <thread>
#include "console_control.h"
#include "nap_time.h"
#include "network_process.h"
#include "process.h"
#include "process_simulator.h"
#include "system_monitor.h"
#include "system_process.h"

namespace process
{
    // Define the handler factory map
    FactoryMap MainController::handlerFactoryMap_;

    std::atomic<int> MainController::counter_ = 0;
    // Initialize the factory map
    void MainController::initializeFactory()
    {
        handlerFactoryMap_["real"]    = []() { return manufacture<ProcessMonitor, Process>(); };
        handlerFactoryMap_["simul"]   = []() { return manufacture<ProcessMonitor, ProcessSimulator>(); };
        handlerFactoryMap_["network"] = []() { return manufacture<ProcessMonitor, NetworkProcess>(); };
        handlerFactoryMap_["system"]  = []() { return manufacture<SystemMonitor, SystemProcess>(); };
    }

    void MainController::run(const std::string &processType, int numProcesses)
    {
        ProcessController::setProcessType(processType);
        ProcessController::setNumProcesses(numProcesses);
        initializeFactory();
        createHandlers(ProcessController::numProcesses());
        std::thread monitoringThread(&MainController::CreateMonitoringThreads);
        std::thread terminationThread(&MainController::MonitorProcessTermination);
        cli::driver::consoleLoop();
        processLifecycleLoop();
        if (monitoringThread.joinable())
            monitoringThread.join();
        if (terminationThread.joinable())
            terminationThread.join();
        cli::driver::consoleLoop(false);
    }
    
    void MainController::processLifecycleLoop()
    {
        while (ProcessController::running())
        {
            if (!concurrency::Synchro::getInstance().pauseMonitoring())
            {
                restoreHandlerCount();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::SMALL));
        }
    }

} // namespace process
