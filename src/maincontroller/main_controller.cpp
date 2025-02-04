
#include "main_controller.h"
#include <thread>
#include "console_control.h"
#include "nap_time.h"

namespace process
{
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
