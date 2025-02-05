#include "main_controller.h"
#include <thread>
#include "nap_time.h"
#include "console_loop.h"

namespace process {

    std::thread monitoringThread;
    std::thread terminationThread;
    std::thread restoreHandlerCountThread;

    void MainController::initializeController(const std::string &processType, int numProcesses)
    {
        ProcessController::setProcessType(processType);
        ProcessController::setNumProcesses(numProcesses);
        initializeFactory();
        createHandlers(ProcessController::numProcesses());
    }

    void MainController::startThreads()
    {
        monitoringThread = std::thread(&MainController::CreateMonitoringThreads);
        terminationThread = std::thread(&MainController::MonitorProcessTermination);
        restoreHandlerCountThread = std::thread(&MainController::restoreHandlerCount);
    }

    void MainController::joinThreads()
    {
        if (monitoringThread.joinable())
            monitoringThread.join();
        if (terminationThread.joinable())
            terminationThread.join();
        if (restoreHandlerCountThread.joinable())
            restoreHandlerCountThread.join();
    }

    void MainController::startControlLoop()
    {
        cli::driver::consoleLoop();
    }

    void MainController::endControlLoop()
    {
        cli::driver::consoleLoop(false);
    }

    void MainController::processLifecycleLoop()
    {
        startThreads();
        while (ProcessController::running())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::LONG));
        }
        joinThreads();
    }

} // namespace process
