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

void parseArguments(int argc, char *argv[], int &numProcesses, std::string &processType, int &rndUpper,
                    std::atomic<bool> &g_display)
{
    int opt;
    while ((opt = getopt(argc, argv, "n:t:r:d:h")) != -1)
    {
        switch (opt)
        {
            case 'n':
                // Set the number of processes from the argument
                numProcesses = std::atoi(optarg);
                break;
            case 't':
                // Set the process type from the argument
                processType = optarg;
                break;
            case 'r':
                // Set the random upper limit from the argument
                rndUpper = std::atoi(optarg);
                break;
            case 'd':
                // Set the display flag from the argument (0 or 1)
                g_display = std::atoi(optarg) != 0;
                break;
            case 'h':
            default:
                // Display usage information and exit
                std::cerr << "Usage: " << argv[0]
                          << " -n <number of processes> -t <process type 'real' or 'simul' (default)> -r <random upper "
                             "limit> -d <display (0 or 1)> -h -> help\n";
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
}

auto main(int argc, char *argv[]) -> int
{
    int         numProcesses = 4;
    std::string processType  = "simul";
    int         rndUpper     = 10; // Default value for rndUpper



    parseArguments(argc, argv, numProcesses, processType, rndUpper, g_display);

    process::controller::printHelp(); // Call to printHelp

    process::ProcessSimulator::setRndUpper(rndUpper); // Call to setRndUpper with the parsed value

    std::thread readerThread(process::controller::main);

    process::Controller::run(processType, numProcesses);

    readerThread.join(); // Ensure the reader thread is joined before exiting
    return 0;
}