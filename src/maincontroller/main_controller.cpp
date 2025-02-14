#include "main_controller.h"
#include <thread>
#include <condition_variable>
#include "logger_instance.h"

namespace process
{
    std::thread monitoringThread;
    std::thread terminationThread;
    std::thread restoreHandlerCountThread;

    std::mutex mtx;
    std::condition_variable cv;

    void MainController::initializeController(const std::string &processType, int numProcesses)
    {
        try
        {
            ProcessController::setProcessType(processType);
            ProcessController::setNumProcesses(numProcesses);
            initializeFactory();
            createHandlers(ProcessController::numProcesses());
        }
        catch (const std::exception &e)
        {
            tools::LoggerManager::getInstance().logException("Initialization failed: " + std::string(e.what()));
            std::exit(EXIT_FAILURE);
        }
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

    void MainController::stop()
    {
        ProcessController::running() = false;
        cv.notify_all();
    }

    void MainController::processLifecycleLoop()
    {
        startThreads();
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, []{ return !ProcessController::running(); });
        }
        joinThreads();
    }

} // namespace process