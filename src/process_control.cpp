#include "process_control.h"
#include <atomic>
#include <iostream>
#include <string>
#include <thread>
#include "process_handler.h"

extern std::atomic<bool> g_display;

namespace process::controller
{
    void killPid(const std::string &input);
    void terminatePid(const std::string &input);
    void doCommand(const std::string &input);

    void parseArguments(int argc, char *argv[], int &numProcesses, std::string &processType, int &rndUpper)  
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
                              << " -n <number of processes> -t <process type 'real' or 'simul' (default)> -r <random "
                                 "upper "
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
    void main()
    {
        std::string input;
        while (process::ControllerBase::running())
        {
            std::getline(std::cin, input);
            doCommand(input);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    void doCommand(const std::string& input)
    {
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
                process::ControllerBase::running() = false; // Set running to false to signal the main thread
                std::cout << "Program signalled to exiting once the next process is done" << std::endl;
            }
            else if (input == "terminate all")
            {
                process::ControllerBase::running() = false; // Set running to false to signal the main thread
                std::cout << "Terminating all processes and exiting the program." << std::endl;
                process::Controller::terminateAll();
            }
            else if (input.rfind("terminate ", 0) == 0) // Check if input starts with "terminate "
            {
                std::cout << "Terminating process with PID: " << input.substr(10) << std::endl;
                terminatePid(input);
            }
            else if (input == "kill all")
            {
                process::ControllerBase::running() = false; // Set running to false to signal the main thread
                std::cout << "Killing all processes and exiting the program." << std::endl;
                process::Controller::killAll();
            }
            else if (input.rfind("kill ", 0) == 0) // Check if input starts with "kill "
            {
                std::cout << "Killing process with PID: " << input.substr(5) << std::endl;
                killPid(input);
            }
            else if (input == "display pids")
            {
                std::cout << "Displaying all current PIDs:" << std::endl;
                process::Controller::displayAllPids();
            }
            else if (input == "help")
            {
                printHelp();
            }
            else
            {
                std::cout << "You entered: " << input << std::endl;
            }
        }
    }
    void printHelp()
    {
        std::cout << "Process Control Help Menu\n"
                  << "==========================\n"
                  << "Available commands:\n"
                  << "  print on        - Turn on display progress\n"
                  << "  print off       - Turn off display progress\n"
                  << "  exit            - Sets exist signal to gracefully exits the program once the next process is done\n"
                  << "  terminate all   - Terminate all processes and exit the program\n"
                  << "  terminate <pid> - Terminate a specific process by PID\n"
                  << "  kill all        - Kill all processes and exit the program\n"
                  << "  kill <pid>      - Kill a specific process by PID\n"
                  << "  display pids    - Display all current PIDs\n"
                  << "  help            - Display this help message\n";
    }

    void killPid(const std::string &input)
    {
        {
            try
            {
                pid_t pid = std::stoi(input.substr(5)); // Extract PID from input
                process::Controller::killProcessByPid(pid);
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
    }

    void terminatePid(const std::string &input)
    {
        {
            try
            {
                pid_t pid = std::stoi(input.substr(10)); // Extract PID from input
                process::Controller::terminateProcessByPid(pid);
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
    }
} // namespace process
 