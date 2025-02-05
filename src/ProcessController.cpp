
#include "api.h"
#include "console_control.h"
#include "nap_time.h"
#include <thread>
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
    api::execute(numProcesses, processType);
    std::cout << "[PARENT PROCESS] Main process exiting" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::LONG));

    return 0;
}
