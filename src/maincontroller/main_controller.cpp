
#include "main_controller.h"
#include <mutex>
#include <thread>
#include "communicator.h"
#include "console_control.h"
#include "logger_instance.h"
#include "nap_time.h"
#include "network_process.h"
#include "process.h"
#include "process_simulator.h"
#include "random_stuff.h"
#include "string_tools.h"
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

    void MainController::createHandler()
    {
        const std::string &processType = ProcessController::processType();
        auto it          = handlerFactoryMap_.find(processType);
        if (it != handlerFactoryMap_.end())
        {
            // second() is the templatized function that creates the relevant handler. 
            // See the header for the templatized function
            ProcessMonitorPtr handler = it->second();
            ProcessController::addMonitorProcess(handler->getPid(), std::move(handler));
            ++counter_; 
            tools::LoggerManager::getInstance().logInfo("[HANDLER CREATED] | Counter value: " +
                                                        std::to_string(counter_));
        }
        else
        {
            throw std::runtime_error("Invalid process type");
        }
    }

    void MainController::createHandlers(int numHandlers)
    {
        for (int i = 0; i < numHandlers; ++i)
        {
            try
            {
                createHandler();
            }
            catch (const std::exception &e)
            {

                tools::LoggerManager::getInstance() << "Error creating process handler: " << e.what();
                tools::LoggerManager::getInstance().flush(tools::LogLevel::ERROR);
            }
        }
    }

    void MainController::run(const std::string &processType, int numProcesses)
    {
        ProcessController::setProcessType(processType);
        ProcessController::setNumProcesses(numProcesses);
        initializeFactory();
        createHandlers(ProcessController::numProcesses());
        std::thread monitoringThread(&MainController::CreateMonitoringThreads);
        monitoringThread.detach();
        std::thread terminationThread(&MainController::MonitorProcessTermination);
        terminationThread.detach();
        cli::driver::consoleLoop();
        processLifecycleLoop();
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

    void MainController::restoreHandlerCount()
    {
        // Check if the number of handlers is less than numProcesses_
        if (ProcessController::running() && ProcessController::respawn() &&
            (counter_.load() < ProcessController::numProcesses()))
        {
            int numHandlersToCreate = ProcessController::numProcesses() - counter_;
            createHandlers(numHandlersToCreate);
        }
    }

} // namespace process
