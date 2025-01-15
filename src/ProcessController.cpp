#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include "process.h"
#include "process_handler.h"
#include "simul_process.h"
#include "process_control.h"
#include "logger_instance.h"

std::atomic<bool> g_display = true;


void displayCompilationInfo(const char *appName)
{
    std::cout << "*******************************************" << std::endl;
    std::cout << "*                                         *" << std::endl;
    std::cout << "*  Application Name: " << appName << std::endl;
    std::cout << "*  Compiled on: " << __DATE__ << " at " << __TIME__ << std::endl;
    std::cout << "*                                         *" << std::endl;
    std::cout << "*******************************************" << std::endl;
}


// Declare a static instance to trigger the display


auto main(int argc, char *argv[]) -> int
{
    tools::LogOpt::createLoggerType("console");

    displayCompilationInfo(argv[0]);

    int         numProcesses = 4;
    std::string processType  = "simul";
    int         rndUpper     = 10; // Default value for rndUpper

    cli::driver::parseArguments(argc, argv, numProcesses, processType, rndUpper);

    cli::driver::printHelp(); // Call to printHelp

    process::ProcessSimulator::setRndUpper(rndUpper); // Call to setRndUpper with the parsed value

    std::thread readerThread(cli::driver::main);

    process::Controller::run(processType, numProcesses);

    readerThread.join(); // Ensure the reader thread is joined before exiting

    cli::driver::printpid("[INFO] Main process exiting", "");

    // cli::driver::LoggerExample(); // Call to example

    return 0;
}