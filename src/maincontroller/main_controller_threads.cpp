
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
    /// @brief This function is executed in its own thread.
    /// It listens for creation messages from the child processes and creates monitoring threads for them.
    /// The handlers are created in createHandler() function. Once the handler is created, the process
    /// sends a creation message and this function receives it and generates the monitoring thread for
    // the process.
    void MainController::CreateMonitoringThreads()
    {
        static std::mutex CreateMonitoringThreadsMutex;

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

                    tools::LoggerManager::getInstance().logInfo("[PARENT PROCESS] | Creation message: " +
                                                                creationMessage);
                    auto vec = tools::string::splitString(creationMessage);

                    if (vec.size() < 2)
                    {
                        tools::LoggerManager::getInstance().logWarning(
                                "[PARENT PROCESS] | Unexpected format in creation message: " + creationMessage);
                        continue;
                    }

                    pid_t pid;
                    try
                    {
                        pid = std::stoi(vec[1]);
                    }
                    catch (const std::invalid_argument &e)
                    {
                        tools::LoggerManager::getInstance().logError("[PARENT PROCESS] | Invalid PID in message: " +
                                                                     vec[1]);
                        continue;
                    }
                    catch (const std::out_of_range &e)
                    {
                        tools::LoggerManager::getInstance().logError(
                                "[PARENT PROCESS] | PID out of range in message: " + vec[1]);
                        continue;
                    }

                    std::lock_guard<std::mutex> lock(CreateMonitoringThreadsMutex);
                    auto                        it = std::find_if(
                            ProcessController::handlers().begin(), ProcessController::handlers().end(),
                            [pid](const std::unique_ptr<ProcessMonitor> &handler) { return handler->getPid() == pid; });

                    if (it != ProcessController::handlers().end() && !(*it)->monitoring())
                    {
                        (*it)->createMonitorProcessThread();
                    }
                    else
                    {
                        tools::LoggerManager::getInstance().logWarning(
                                "[PARENT PROCESS] | Handler not found for PID: " + std::to_string(pid));
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
