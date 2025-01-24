
#include "process_handler.h"
#include <thread>
#include "communicator.h"
#include "console_control.h"
#include "logger_instance.h"
#include "nap_time.h"
#include "network_process.h"
#include "process.h"
#include "simul_process.h"
#include "random_stuff.h"

namespace process
{
    void Controller::createHandler()
    {
        auto handler = std::make_unique<ControllerBase>();
        if (processType_ == "real")
        {
            handler->init(std::make_unique<Process>());
        }
        else if (processType_ == "simul")
        {
            handler->init(std::make_unique<ProcessSimulator>());
        }
        else if (processType_ == "network")
        {
            handler->init(std::make_unique<NetworkProcess>());
        }
        else
        {
            throw std::runtime_error("Invalid process type");
        }

        Communicator::getInstance().receiveCreationMessage();
        handlers_.push_back(std::move(handler));
    }

    void Controller::createHandlers(int numHandlers)
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

    void Controller::CreateMonitoringThreads()
    {
        if (concurrency::Synchro::getInstance().pauseMonitoring())
            return;
            
        for (auto &handler: handlers_)
        {
            if (!handler->monitoring())
                handler->createMonitorProcessThread();
        }
    }

    void Controller::run(const std::string &processType, int numProcesses)
    {
        setProcessType(processType);
        setNumProcesses(numProcesses);
        createHandlers(numProcesses_);
        CreateMonitoringThreads();
        std::thread readerThread(cli::driver::consoleLoop);
        processLifecycleLoop();
        readerThread.join(); // Ensure the reader thread is joined before exiting
    }

    void Controller::removeHandler()
    {
        if (!handlers_.empty())
        {
            try
            {
                pid_t pid = concurrency::Synchro::getInstance().removeFrontPidQueue();
                if (pid != -1)
                {
                    // Find and remove the handler with the matching PID
                    auto it = std::remove_if(
                            handlers_.begin(), handlers_.end(),
                            [pid](const std::unique_ptr<ControllerBase> &handler) { return handler->getPid() == pid; });

                    if (it != handlers_.end())
                    {
                        handlers_.erase(it, handlers_.end());
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
    }

    void Controller::processLifecycleLoop()
    {
        while (running())
        {
            if (concurrency::Synchro::getInstance().pauseMonitoring())
            {
                // Sleep for a random short duration to simulate pause
                tools::sleepRandomMs();
                continue; // Check again if monitoring has been resumed
            }

            bool processedEvent = false;

            // Process all events if available
            while (!concurrency::Synchro::getInstance().isPidQueueEmpty())
            {
                removeHandler();
                processedEvent = true;
            }

            if (!processedEvent)
            {
                // Sleep briefly if no events were processed to prevent tight looping
                std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::SMALL));
                continue;
            }

            restoreHandlerCount();

            if (handlers_.empty())
            {
                running(false);
                tools::LoggerManager::getInstance() << "[PARENT PROCESS] | All handlers removed, exiting...";
                tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);
            }
        }
    }

    void Controller::restoreHandlerCount()
    {
        // Check if the number of handlers is less than numProcesses_
        if (running() && respawn() && (handlers_.size() < numProcesses_))
        {
            int numHandlersToCreate = numProcesses_ - handlers_.size();
            createHandlers(numHandlersToCreate);
            CreateMonitoringThreads();
        }
    }

} // namespace process
