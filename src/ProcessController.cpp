#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include "process.h"
#include "process_handler.h"
#include "simul_process.h"
#include "process_control.h"

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
    displayCompilationInfo(argv[0]);

    int         numProcesses = 4;
    std::string processType  = "simul";
    int         rndUpper     = 10; // Default value for rndUpper

    process::controller::parseArguments(argc, argv, numProcesses, processType, rndUpper);

    process::controller::printHelp(); // Call to printHelp

    process::ProcessSimulator::setRndUpper(rndUpper); // Call to setRndUpper with the parsed value

    std::thread readerThread(process::controller::main);

    process::Controller::run(processType, numProcesses);

    readerThread.join(); // Ensure the reader thread is joined before exiting

    process::controller::printpid("[INFO] Main process exiting", "");

    process::controller::LoggerExample(); // Call to example

    return 0;
}