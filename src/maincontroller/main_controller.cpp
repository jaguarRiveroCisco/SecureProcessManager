#include "main_controller.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include "nap_time.h"
#include "console_loop.h"

namespace process {

    std::thread monitoringThread;
    std::thread terminationThread;
    std::thread restoreHandlerCountThread;

    std::mutex mtx;
    std::condition_variable cv;
    bool processRunning = true;

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

    void MainController::stop()
    {
        ProcessController::running() = false;
        {
            std::lock_guard<std::mutex> lock(mtx);
            processRunning = false;
        }
        cv.notify_all();
    }

    void MainController::processLifecycleLoop()
    {
        startThreads();
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, []{ return  !processRunning; });
        }
        joinThreads();
    }

} // namespace process
