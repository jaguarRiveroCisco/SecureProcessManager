
#include "process_handler.h"
#include <thread>
#include "communicator.h"
#include "console_control.h"
#include "network_process.h"
#include "process.h"
#include "simul_process.h"

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
        for (auto &handler : handlers_)
        {
            if(!handler->monitoring())
                handler->createMonitorProcessThread();
        }
    }

    void Controller::run(const std::string &processType, int numProcesses)
    {
        setProcessType(processType);
        setNumProcesses(numProcesses);
        createHandlers(numProcesses_);
        CreateMonitoringThreads();
        std::thread readerThread(cli::driver::main);
        waitForEvents();
        readerThread.join(); // Ensure the reader thread is joined before exiting
    }

    void Controller::removeHandler()
    {
        if (!handlers_.empty())
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
            }
        }
    }

    void Controller::waitForEvents()
    {
        while (running())
        {
            concurrency::Synchro::getInstance().blockUntilPidAvailable();

            // Process all events
            while (!concurrency::Synchro::getInstance().isPidQueueEmpty())
            {
                removeHandler();
                restoreHandlerCount();
                if (handlers_.empty())
                {
                    running() = false;
                    tools::LoggerManager::getInstance() << "All handlers removed, exiting...";
                    tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);
                }
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
