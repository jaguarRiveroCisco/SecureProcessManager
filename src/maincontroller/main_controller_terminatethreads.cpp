
#include <mutex>
#include <thread>
#include "communicator.h"
#include "console_control.h"
#include "logger_instance.h"
#include "main_controller.h"
#include "nap_time.h"
#include "string_tools.h"
#include "system_monitor.h"

namespace process
{
    void MainController::terminateMonitorThread(const std::string& pidStr)
    {
        pid_t pid = tools::string::strToPid(pidStr);
        if (pid == -1)
        {
            return;
        }

        auto it = ProcessRegistry::findMonitor(pid);

        if(it)
        {
            if(it->monitoring())
            {
                it->monitoring() = false;
                auto start       = std::chrono::steady_clock::now();
                while (it->monitoring())
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::VERYSMALL));
                    auto now = std::chrono::steady_clock::now();
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() >
                        tools::NapTimeMs::MEDIUM)
                    {
                        tools::LoggerManager::getInstance().logWarning("[TERMINATE MONITOR THREAD] | Timeout while "
                                                                       "waiting for monitoring to stop for PID: " +
                                                                       pidStr);
                        break;
                    }
                }
            }
            if(it->monitoring())
            {
                tools::LoggerManager::getInstance().logWarning(
                        "[TERMINATE MONITOR THREAD] | Monitoring thread did not stop for PID: " + pidStr);
                std::throw_with_nested(std::runtime_error(
                        "[TERMINATE MONITOR THREAD] | Monitoring thread did not stop for PID: " + pidStr));
            }
            else
            {
                if (!ProcessRegistry::markProcessAsDeceased(pid))
                {
                    tools::LoggerManager::getInstance().logWarning(
                            "[TERMINATE MONITOR THREAD] | Handler not found for PID: " + pidStr);
                }            }
        }
        else
        {
            tools::LoggerManager::getInstance().logWarning("[TERMINATE MONITOR THREAD] | Handler not found for PID: " + pidStr);
        }
    }

    void MainController::MonitorProcessTermination()
    {
        while(ProcessController::running())
        {
            try
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
                terminateMonitorThread(vec[1]);
            }
            catch (const std::exception &e)
            {
                tools::LoggerManager::getInstance().logException("[MONITOR PROCESS TERMINATION] | Exception occurred: " +
                                                                 std::string(e.what()));
            }

            // Optional sleep to reduce CPU usage in case of tight loop
            std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::SMALL));
        }
        tools::LoggerManager::getInstance().logInfo("Ending MonitorProcessTermination thread");

    }
} // namespace process
