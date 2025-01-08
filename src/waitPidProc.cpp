#include <iostream>
#include <algorithm>
#include "process_handler.h"
#include "process.h"
#include "simul_process.h"
#include <thread>

std::atomic<bool> g_display = true;

void consoleReader()
{
    std::string input;
    while (g_display)
    {
        std::getline(std::cin, input);
        if (!input.empty())
        {
            if (input == "print on")
            {
                g_display = true;
                std::cout << "Display progress turned on." << std::endl;
            }
            else if (input == "print off")
            {
                g_display = false;
                std::cout << "Display progress turned off." << std::endl;
            }
            else
            {
                std::cout << "You entered: " << input << std::endl;
            }
        }
    }
}

auto main(int argc, char *argv[]) -> int
{
    int         numProcesses = 4;
    std::string processType = "simul";
    int         rndUpper = 10; // Default value for rndUpper

    auto parseArguments = [&](int argc, char *argv[]) {
        int opt;
        while ((opt = getopt(argc, argv, "n:t:r:h")) != -1)
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
                case 'h':
                default:
                    std::cerr << "Usage: " << argv[0]
                              << " -n <number of processes> -t <process type> -r <random upper limit> -h -> help\n";
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

        std::cout << "Random upper limit: " << rndUpper << std::endl;
        SimulProcess::setRndUpper(rndUpper); // Call to setRndUpper with the parsed value
    };

    std::thread readerThread(consoleReader);
    readerThread.detach();

    parseArguments(argc, argv);

    ProcessHandler::createHandlers(numProcesses, processType);

    // Main thread waits for events
    ProcessHandler::waitForEvents();

    return 0;
}
