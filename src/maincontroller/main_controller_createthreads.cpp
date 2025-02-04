
#include <mutex>
#include <thread>
#include "communicator.h"
#include "console_control.h"
#include "logger_instance.h"
#include "main_controller.h"
#include "nap_time.h"
#include "string_tools.h"
#include "system_monitor.h"
#include "ProcessRegistry.h"
#include "network_process.h"
#include "process.h"
#include "process_simulator.h"
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

    void MainController::createHandler()
    {
        const std::string &processType = ProcessController::processType();
        auto it          = handlerFactoryMap_.find(processType);
        if (it != handlerFactoryMap_.end())
        {
            // second() is the templatized function that creates the relevant handler.
            // See the header for the templatized function
            ProcessMonitorPtr handler = it->second();
            ProcessRegistry::addMonitorProcess(handler->getPid(), std::move(handler));
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

    void MainController::createMonitorThread(const std::string& pidStr)
    {
        pid_t pid = tools::string::strToPid(pidStr);
        if (pid == -1)
        {
            return;
        }

        auto it = ProcessRegistry::findMonitor(pid);

        if(it &&  !it->monitoring())
        {
            it->createMonitorProcessThread();
        }
        else
        {
            tools::LoggerManager::getInstance().logWarning("[CREATE MONITOR THREAD] | Handler not found for PID: " + pidStr);
        }
    }

    /// @brief This function is executed in its own thread.
    /// It listens for creation messages from the child processes and creates monitoring threads for them.
    /// The handlers are created in createHandler() function. Once the handler is created, the process
    /// sends a creation message and this function receives it and generates the monitoring thread for
    // the process.
    void MainController::CreateMonitoringThreads()
    {
        while (ProcessController::running())
        {
            try
            {
                auto creationMessage = concurrency::Communicator::getInstance().receiveCreationMessage();

                if (creationMessage.empty())
                {
                    continue;
                }

                tools::LoggerManager::getInstance().logInfo("[CREATE MONITOR THREAD] | [CREATION MESSAGE]: " +
                                                            creationMessage);
                auto vec = tools::string::splitString(creationMessage);

                if (vec.size() < 2)
                {
                    tools::LoggerManager::getInstance().logWarning(
                            "[CREATE MONITOR THREAD] | Unexpected format in creation message: " + creationMessage);
                    continue;
                }
                createMonitorThread(vec[1]);
            }
            catch (const std::exception &e)
            {
                tools::LoggerManager::getInstance().logException("[CREATE MONITOR THREAD] | Exception occurred: " +
                                                                 std::string(e.what()));
            }

            // Optional sleep to reduce CPU usage in case of tight loop
            std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::VERYSMALL));
        }
    }
} // namespace process
