#include <iostream>
#include <thread>
#include "console_control.h"
#include "logger_instance.h"
#include "nap_time.h"
#include "main_controller.h"

void displayCompilationInfo(const char *appName)
{
    std::cout << "*******************************************" << std::endl;
    std::cout << "*                                         *" << std::endl;
    std::cout << "*  Application Name: " << appName << std::endl;
    std::cout << "*  Compiled on: " << __DATE__ << " at " << __TIME__ << std::endl;
    std::cout << "*                                         *" << std::endl;
    std::cout << "*******************************************" << std::endl;
}

auto main(int argc, char *argv[]) -> int
{

    displayCompilationInfo(argv[0]);

    int         numProcesses = 4;
    std::string processType  = "simul";

    cli::driver::parseArguments(argc, argv, numProcesses, processType);

    tools::LoggerManager::createLoggerType();

    process::MainController::run(processType, numProcesses);

    cli::driver::printpid("[PARENT PROCESS] Main process exiting", "");

    std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::LONG));

    return 0;
}
