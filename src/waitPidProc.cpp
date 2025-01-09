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
std::atomic<bool> g_running = true;

auto main(int argc, char *argv[]) -> int
{
    int         numProcesses = 4;
    std::string processType  = "simul";
    int         rndUpper     = 10; // Default value for rndUpper

    auto parseArguments = [&](int argc, char *argv[]) {
        int opt;
        while ((opt = getopt(argc, argv, "n:t:r:d:h")) != -1)
        {
            switch (opt)
            {
                case 'n':
                    numProcesses = std::atoi(optarg);
                    break;
                case 't':
                    processType = optarg;
                    break;
                case 'r':
                    rndUpper = std::atoi(optarg);
                    break;
                case 'd':
                    g_display = std::atoi(optarg) != 0;
                    break;
                case 'h':
                default:
                    std::cerr << "Usage: " << argv[0]
                              << " -n <number of processes> -t <process type 'real' or 'simul' (default)> -r <random upper limit> -d <display (0 "
                                 "or 1)> -h -> help\n";
                    std::exit(EXIT_SUCCESS);
            }
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

        if (rndUpper < 10)
        {
            std::cerr << "Random upper limit must be greater than 10. Defaulting to 10.\n";
            rndUpper = 10;
        }

    };
    parseArguments(argc, argv);

    process::ProcessSimulator::setRndUpper(rndUpper); // Call to setRndUpper with the parsed value
    process::Controller::setProcessType(processType); // Call to setProcessType with the parsed value

    std::thread readerThread(process::processControl);

    // Create process handlers
    process::Controller::createHandlers(numProcesses);
    process::Controller::waitForEvents();

    readerThread.join(); // Ensure the reader thread is joined before exiting
    return 0;
}