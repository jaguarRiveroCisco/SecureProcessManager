//
// Created by Jaguar Rivero on 2025-02-05.
//

#include "api.h"
#include "logger_instance.h"
#include "main_controller.h"
namespace api
{
    void execute(int &numProcesses, std::string &processType)
    {
        tools::LoggerManager::createLoggerType();
        process::MainController::run(processType, numProcesses);
    }

} // namespace api