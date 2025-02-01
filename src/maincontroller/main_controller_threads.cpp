
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
        pid_t pid = tools::string::strToPid(pidStr);
        if (pid == -1)
        {
            return;
        }

        auto it = ProcessController::findMonitor(pid);

        if(it &&  !it->monitoring())
        {
            it->createMonitorProcessThread();
        }
        else
        {
            tools::LoggerManager::getInstance().logWarning("[MONITOR THREAD] | Handler not found for PID: " + pidStr);
        }
    }

    void MainController::terminateMonitorThread(const std::string& pidStr)
    {
        pid_t pid = tools::string::strToPid(pidStr);
        if (pid == -1)
        {
            return;
        }

        auto it = ProcessController::findMonitor(pid);

        if(it)
        {
            if(it->monitoring())
            {
                it->monitoring() = false;
                auto start       = std::chrono::steady_clock::now();
                while (it->monitoring())
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::SMALL));
                    auto now = std::chrono::steady_clock::now();
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() >
                        tools::NapTimeMs::MEDIUM)
                    {
                        tools::LoggerManager::getInstance().logWarning("[MONITOR PROCESS TERMINATION] | Timeout while "
                                                                       "waiting for monitoring to stop for PID: " +
                                                                       pidStr);
                        break;
                    }
                }
            }
            if(it->monitoring())
            {
                tools::LoggerManager::getInstance().logWarning(
                        "[MONITOR PROCESS TERMINATION] | Monitoring thread did not stop for PID: " + pidStr);
                std::throw_with_nested(std::runtime_error(
                        "[MONITOR PROCESS TERMINATION] | Monitoring thread did not stop for PID: " + pidStr));
            }
            else
            {
                tools::LoggerManager::getInstance().logInfo(
                        "[MONITOR PROCESS TERMINATION] | Monitoring thread stopped for PID: " + pidStr);
                //if (!ProcessController::removeMonitorProcess(pid))
                //{
                //    tools::LoggerManager::getInstance().logWarning(
                //            "[MONITOR PROCESS TERMINATION] | Handler not found for PID: " + pidStr);
                //}
            }
        }
        else
        {
            tools::LoggerManager::getInstance().logWarning("[MONITOR THREAD] | Handler not found for PID: " + pidStr);
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

                    tools::LoggerManager::getInstance().logInfo("[MONITOR PROCESS TERMINATION] | " + terminationMessage);
                    auto vec = tools::string::splitString(terminationMessage);

                    if (vec.size() < 2)
                    {
                        tools::LoggerManager::getInstance().logWarning(
                                "[MONITOR PROCESS TERMINATION] | Unexpected format in termination message: " +
                                terminationMessage);
                        continue;
                    }
                    --counter_;
                    tools::LoggerManager::getInstance().logInfo("[MONITOR PROCESS TERMINATION] | Counter value: " + std::to_string(counter_));
                    terminateMonitorThread(vec[1]);

                }
            }
            catch (const std::exception &e)
            {
                tools::LoggerManager::getInstance().logException("[MONITOR PROCESS TERMINATION] | Exception occurred: " +
                                                                 std::string(e.what()));
            }

            // Optional sleep to reduce CPU usage in case of tight loop
            std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::SMALL));
        }
    }
} // namespace process
