
#include "main_controller.h"
#include <thread>
#include "communicator.h"
#include "console_control.h"
#include "logger_instance.h"
#include "nap_time.h"
#include "network_process.h"
#include "process.h"
#include "simul_process.h"
#include "random_stuff.h"
#include "system_controller.h"
#include "system_process.h"

namespace process
{
    void MainController::createHandler()
    {
        auto handler = std::make_unique<ProcessController>();
        if (ProcessController::processType() == "real")
        {
            handler->init(std::make_unique<Process>());
        }
        else if (ProcessController::processType() == "simul")
        {
            handler->init(std::make_unique<ProcessSimulator>());
        }
        else if (ProcessController::processType() == "network")
        {
            handler->init(std::make_unique<NetworkProcess>());
        }
        else if (ProcessController::processType() == "system")
        {
            handler.reset();
            handler = std::make_unique<SystemController>();
            handler->init(std::make_unique<SystemProcess>());
        }
        else
        {
            throw std::runtime_error("Invalid process type");
        }

        Communicator::getInstance().receiveCreationMessage();
        ProcessController::handlers().push_back(std::move(handler));
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

    void MainController::CreateMonitoringThreads()
    {
        if (concurrency::Synchro::getInstance().pauseMonitoring())
            return;
            
        for (auto &handler: ProcessController::handlers())
        {
            if (!handler->monitoring())
                handler->createMonitorProcessThread();
        }
    }

    void MainController::run(const std::string &processType, int numProcesses)
    {
        ProcessController::setProcessType(processType);
        ProcessController::setNumProcesses(numProcesses);
        createHandlers(ProcessController::numProcesses());
        CreateMonitoringThreads();
        cli::driver::consoleLoop();
        processLifecycleLoop();
        cli::driver::consoleLoop(false);
    }
    bool MainController::removeHandler()
    {
        if (!ProcessController::handlers().empty())
        {
            try
            {
                pid_t pid = concurrency::Synchro::getInstance().removeTerminatedProcessPid();
                if (pid != -1)
                {
                    // Find and remove the handler with the matching PID
                    auto it = std::remove_if(
                            ProcessController::handlers().begin(), ProcessController::handlers().end(),
                            [pid](const std::unique_ptr<ProcessController> &handler) { return handler->getPid() == pid; });

                    if (it != ProcessController::handlers().end())
                    {
                        ProcessController::handlers().erase(it, ProcessController::handlers().end());
                        return true; // Handler was removed
                    }
                    else
                    {
                        // Log a warning if no handler was found for the given PID
                        tools::LoggerManager::getInstance().logWarning("[PARENT PROCESS] Warning: No handler found for PID: " + std::to_string(pid));
                    }
                }
                else
                {
                    // Log a warning if the PID is invalid
                    tools::LoggerManager::getInstance().logWarning("[PARENT PROCESS] Warning: Invalid PID returned from queue");
                }
            }
            catch (const std::exception &e)
            {
                // Handle any potential exceptions and log the error
                tools::LoggerManager::getInstance().logException("[PARENT PROCESS] Error in removeHandler: " + std::string(e.what()));
            }
        }
        else
        {
            // Log a message if the handlers list is empty
            tools::LoggerManager::getInstance().logInfo("[PARENT PROCESS] No handlers to remove, the list is empty.");
        }
        return false; // No handler was removed
    }

    void MainController::processLifecycleLoop()
    {
        while (ProcessController::running())
        {
            if (concurrency::Synchro::getInstance().pauseMonitoring())
            {
                // Sleep for a random short duration to simulate pause
                tools::sleepRandomMs();
                continue; // Check again if monitoring has been resumed
            }

            bool processedEvent = false;

            while (!concurrency::Synchro::getInstance().isTerminatedPidQueueEmpty())
            {
                if(removeHandler())
                    processedEvent = true;
                else
                    break;
            }

            if (!processedEvent)
            {
                // Sleep briefly if no events were processed to prevent tight looping
                std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::SMALL));
                continue;
            }

            restoreHandlerCount();

            if (ProcessController::handlers().empty())
            {
                ProcessController::running() = false;
                tools::LoggerManager::getInstance() << "[PARENT PROCESS] | All handlers removed, exiting...";
                tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);
            }
        }
    }

    void MainController::restoreHandlerCount()
    {
        // Check if the number of handlers is less than numProcesses_
        if (ProcessController::running() && ProcessController::respawn() &&
            (ProcessController::handlers().size() < ProcessController::numProcesses()))
        {
            int numHandlersToCreate = ProcessController::numProcesses() - ProcessController::handlers().size();
            createHandlers(numHandlersToCreate);
            CreateMonitoringThreads();
        }
    }

} // namespace process
