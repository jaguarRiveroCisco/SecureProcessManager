
#include <mutex>
#include <thread>
#include "communicator.h"
#include "console_control.h"
#include "logger_instance.h"
#include "main_controller.h"
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
    void MainController::createMonitorThread(const std::string& pidStr)
    {
        static std::mutex           CreateMonitoringThreadsMutex;
        std::lock_guard<std::mutex> lock(CreateMonitoringThreadsMutex);

        pid_t                       pid = tools::string::strToPid(pidStr);
        if (pid == -1)
        {
            return;
        }

        auto it = std::find_if(
                ProcessController::handlers().begin(), ProcessController::handlers().end(),
                [pid](const std::unique_ptr<ProcessMonitor> &handler) { return handler->getPid() == pid; });

        if (it != ProcessController::handlers().end() && !(*it)->monitoring())
        {
            (*it)->createMonitorProcessThread();
        }
        else
        {
            throw std::runtime_error("Handler not found for PID: " + std::to_string(pid));
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
                if (!concurrency::Synchro::getInstance().pauseMonitoring())
                {
                    auto creationMessage = concurrency::Communicator::getInstance().receiveCreationMessage();

                    if (creationMessage.empty())
                    {
                        continue;
                    }

                    tools::LoggerManager::getInstance().logInfo("[PARENT PROCESS] | [CREATION]: " +
                                                                creationMessage);
                    auto vec = tools::string::splitString(creationMessage);

                    if (vec.size() < 2)
                    {
                        tools::LoggerManager::getInstance().logWarning(
                                "[PARENT PROCESS] | Unexpected format in creation message: " + creationMessage);
                        continue;
                    }
                    createMonitorThread(vec[1]);
                 }
            }
            catch (const std::exception &e)
            {
                tools::LoggerManager::getInstance().logException("[PARENT PROCESS] | Exception occurred: " +
                                                                 std::string(e.what()));
            }

            // Optional sleep to reduce CPU usage in case of tight loop
            std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::VERYSMALL));
        }
    }

    void MainController::MonitorProcessTermination()
    {
        while(ProcessController::running())
        {
            try
            {
                if (!concurrency::Synchro::getInstance().pauseMonitoring())
                {
                    auto terminationMessage = concurrency::Communicator::getInstance().receiveTerminationMessage();

                    if (terminationMessage.empty())
                    {
                        continue;
                    }

                    tools::LoggerManager::getInstance().logInfo("[PARENT PROCESS] | [TERMINATION]: " +
                                                                terminationMessage);
                    auto vec = tools::string::splitString(terminationMessage);

                    if (vec.size() < 2)
                    {
                        tools::LoggerManager::getInstance().logWarning(
                                "[PARENT PROCESS] | Unexpected format in termination message: " + terminationMessage);
                        continue;
                    }

                    pid_t pid = tools::string::strToPid(vec[1]);
                    if (pid == -1)
                    {
                        continue;
                    }

                    {
                        std::lock_guard<std::mutex> lock(handlersMutex_);
                        // Find and remove the handler with the matching PID
                        auto it = std::remove_if(ProcessController::handlers().begin(),
                                                 ProcessController::handlers().end(),
                                                 [pid](const std::unique_ptr<ProcessMonitor> &handler) 
                                                 {
                                                     return handler->getPid() == pid;
                                                 });

                        if (it != ProcessController::handlers().end())
                        {
                            ProcessController::handlers().erase(it, ProcessController::handlers().end());
                            tools::LoggerManager::getInstance().logInfo(
                                    "[PARENT PROCESS] | Handler removed for PID: " + std::to_string(pid) +
                                    " | Number of handlers remaining: " +
                                    std::to_string(ProcessController::handlers().size()));
                        }
                        else
                        {
                            // Log a warning if no handler was found for the given PID
                            tools::LoggerManager::getInstance().logWarning(
                                    "[PARENT PROCESS] Warning: No handler found for PID: " + std::to_string(pid));
                        }
                    }
                }
            }
            catch (const std::exception &e)
            {
                tools::LoggerManager::getInstance().logException("[PARENT PROCESS] | Exception occurred: " +
                                                                 std::string(e.what()));
            }

            // Optional sleep to reduce CPU usage in case of tight loop
            std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::SMALL));
        }
    }
} // namespace process
