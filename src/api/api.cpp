//
// Created by Jaguar Rivero on 2025-02-05.
//

#include "api.h"
#include "logger_instance.h"
#include "nap_time.h"
#include "process_controller.h"
#include "main_controller.h"
#include "console_logger.h"
#include "console_loop.h"

namespace api
{
    static tools::ConsoleLogger cl;

    void initialize(const int &numProcesses, const std::string &processType)
    {
        tools::LoggerManager::createLoggerType();
        process::MainController::initializeController(processType, numProcesses);

    }

    void execute()
    {
        process::MainController::processLifecycleLoop();
    }

    void respawn(const bool val)
    {
        process::ProcessController::respawn() = val;;
    }

    void loggerType(const std::string & type)
    {
        tools::LoggerManager::loggerType() = type;
    }

    void SleepType(const tools::NapType type)
    {
        tools::SleepTime::type = type;
    }

    void stop()
    {
        process::MainController::stop();
    }

    void terminateAll()
    {
        process::ProcessController::terminateAll();
    }

    void killAll()
    {
        process::ProcessController::killAll();
    }

    void killPid(const pid_t pid)
    {
        try
        {
            process::ProcessController::killProcessByPid(pid);
        }
        catch (const std::invalid_argument &)
        {
            cl << "Invalid PID format.";
            cl.flush(tools::LogLevel::ERROR);
        }
        catch (const std::out_of_range &)
        {
            cl << "PID out of range.";
            cl.flush(tools::LogLevel::ERROR);
        }
    }

    void terminatePid(const pid_t pid)
    {
        try
        {
            process::ProcessController::terminateProcessByPid(pid);
        }
        catch (const std::invalid_argument &)
        {
            cl << "Invalid PID format.";
            cl.flush(tools::LogLevel::ERROR);
        }
        catch (const std::out_of_range &)
        {
            cl << "PID out of range.";
            cl.flush(tools::LogLevel::ERROR);
        }
    }

    void intPid(const pid_t pid)
    {
        try
        {
            process::ProcessController::intProcessByPid(pid);
        }
        catch (const std::invalid_argument &)
        {
            cl << "Invalid PID format.";
            cl.flush(tools::LogLevel::ERROR);
        }
        catch (const std::out_of_range &)
        {
            cl << "PID out of range.";
            cl.flush(tools::LogLevel::ERROR);
        }
    }

    void displayAllPids()
    {
        process::ProcessController::displayAllPids();
    }

    void readConfigFile(const std::string &configFilePath)
    {
        process::ProcessController::readConfigFile(configFilePath);
    }

}

