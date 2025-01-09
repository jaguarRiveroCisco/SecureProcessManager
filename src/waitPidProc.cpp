#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include "process.h"
#include "process_handler.h"
#include "simul_process.h"

std::atomic<bool> g_display = true;
std::atomic<bool> g_running = true;

void consoleReader()
{
    std::string input;
    while (g_running)
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
            else if (input == "exit")
            {
                g_running = false; // Set running to false to signal the main thread
                std::cout << "Exiting program once all processes are done" << std::endl;
            }
            else if (input == "terminate")
            {
                ProcessHandler::terminateAll();
                g_running = false; // Set running to false to signal the main thread
            }
            else if (input.rfind("terminate ", 0) == 0) // Check if input starts with "terminate "
            {
                try
                {
                    pid_t pid = std::stoi(input.substr(10)); // Extract PID from input
                    ProcessHandler::terminateProcessByPid(pid);
                }
                catch (const std::invalid_argument &)
                {
                    std::cerr << "Invalid PID format." << std::endl;
                }
                catch (const std::out_of_range &)
                {
                    std::cerr << "PID out of range." << std::endl;
                }
            }
            else if (input == "display pids")
            {
                ProcessHandler::displayAllPids();
            }
            else if (input == "help")
            {
                std::cout << "Available commands:\n"
                          << "  print on       - Turn on display progress\n"
                          << "  print off      - Turn off display progress\n"
                          << "  exit           - Exit the program once all processes are done\n"
                          << "  terminate      - Terminate all processes and exit the program\n"
                          << "  terminate <pid> - Terminate a specific process by PID\n"
                          << "  display pids   - Display all current PIDs\n"
                          << "  help           - Display this help message\n";
            }
            else
            {
                std::cout << "You entered: " << input << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

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
                    ProcessHandler::setProcessType(processType); // Call to setProcessType with the parsed value
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

        SimulProcess::setRndUpper(rndUpper); // Call to setRndUpper with the parsed value
    };

    std::thread readerThread(consoleReader);

    parseArguments(argc, argv);

    // Create process handlers
    ProcessHandler::createHandlers(numProcesses);
    ProcessHandler::waitForEvents();

    readerThread.join(); // Ensure the reader thread is joined before exiting
    return 0;
}
