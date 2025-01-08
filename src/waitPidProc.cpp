#include <iostream>
#include <algorithm>
#include "process_handler.h"
#include "process.h"
#include "simul_process.h"

auto main(int argc, char *argv[]) -> int
{
    int         numProcesses = 4;
    std::string processType = "simul";

    auto parseArguments = [&](int argc, char *argv[]) 
    {
        if (argc >= 2)
        {
            numProcesses = ::atoi(argv[1]);
        }
        if (argc >= 3)
        {
            processType  = argv[2];
        }
        if (numProcesses <= 0)
        {
            std::cerr << "Number of processes must be greater than 0. Defaulting to 4.\n";
            numProcesses = 4;
        }

        if (processType != "real" && processType != "simul")
        {
            std::cerr << "Invalid process type: " << processType << ". Defaulting to 'simul'.\n";
            processType = "simul";
        }
    };

    parseArguments(argc, argv);

    ProcessHandler::createHandlers(numProcesses, processType);

    // Main thread waits for events
    ProcessHandler::waitForEvents();

    return 0;
}