//
// Created by Jaguar Rivero on 2025-02-05.
//

#include "api.h"
#include "logger_instance.h"
#include "nap_time.h"
#include "process_controller.h"
#include "main_controller.h"
namespace api
{
    void execute(const int &numProcesses, const std::string &processType)
    {
        tools::LoggerManager::createLoggerType();
        process::MainController::run(processType, numProcesses);
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

} // namespace api