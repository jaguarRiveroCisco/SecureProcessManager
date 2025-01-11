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
    void printContext(int numProcesses = -1, const std::string &processType = "", int rndUpper = -1);

    void parseArguments(int argc, char *argv[], int &numProcesses, std::string &processType, int &rndUpper)  
    {
        int opt;
        while ((opt = getopt(argc, argv, "n:t:r:d:s:h")) != -1)
        {
            switch (opt)
            {
                case 'n':
                    // Set the number of processes from the argument
                    numProcesses = std::atoi(optarg);
                    std::cout << "Number of processes: " << numProcesses << std::endl;
                    break;
                case 't':
                    // Set the process type from the argument
                    processType = optarg;
                    std::cout << "Process type: " << processType << std::endl;
                    break;
                case 'r':
                    // Set the random upper limit from the argument
                    rndUpper = std::atoi(optarg);
                    std::cout << "Random upper limit: " << rndUpper << std::endl;
                    break;
                case 'd':
                    // Set the display flag from the argument (0 or 1)
                    g_display = std::atoi(optarg) != 0;
                    std::cout << "Display flag: " << g_display << std::endl;
                    break;
                case 's':
                    // Set the respawn flag from the argument (0 or 1)
                    process::ControllerBase::respawn() = std::atoi(optarg) != 0;
                    std::cout << "Respawn flag: " << process::ControllerBase::respawn() << std::endl;
                    break;
                case 'h':
                default:
                    // Display usage information and exit
                    std::cerr << "Usage: " << argv[0]
                              << " -n <number of processes> -t <process type 'real' or 'simul' (default)> -r <random "
                                 "upper limit> -d <display (0 or 1)> -r <respawn (0 or 1)> -h -> help\n";
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

        printContext(numProcesses, processType, rndUpper);
    }

    void printContext(int numProcesses, const std::string &processType, int rndUpper)
    {
        // Static variables to store the latest values
        static int         lastNumProcesses = 0;
        static std::string lastProcessType  = "unknown";
        static int         lastRndUpper     = 0;

        // Update static variables only if new values are provided
        if (numProcesses != -1)
            lastNumProcesses = numProcesses;
        if (!processType.empty())
            lastProcessType = processType;
        if (rndUpper != -1)
            lastRndUpper = rndUpper;

        // Print the stored context
        std::cout << "\n========================= Context =========================\n"
                  << " Number of Processes : " << lastNumProcesses << "\n"
                  << " Process Type        : " << lastProcessType << "\n"
                  << " Random Upper Limit  : " << lastRndUpper << "\n"
                  << " Display Flag        : " << (g_display ? "Enabled" : "Disabled") << "\n"
                  << " Respawn             : " << (process::ControllerBase::respawn() ? "Enabled" : "Disabled") << "\n"
                  << "==========================================================\n\n"
                  << std::flush;
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
            else if (input == "context")
            {
                printContext();
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
            else if (input == "respawn on")
            {
                process::ControllerBase::respawn() = true;
                std::cout << "Respawn turned on." << std::endl;
            }
            else if (input == "respawn off")
            {
                process::ControllerBase::respawn() = false;
                std::cout << "Respawn turned off." << std::endl;
            }
            else
            {
                std::cout << "You entered: " << input << std::endl;
            }
        }
    }
    void printHelp()
    {
        std::cout << "\n==========================================================\n"
                  << "Process Control Help Menu\n"
                  << "==========================================================\n"
                  << "Available commands:\n"
                  << "  context         - Display the current context\n"
                  << "  print on        - Turn on display progress\n"
                  << "  print off       - Turn off display progress\n"
                  << "  exit            - Sets exist signal to gracefully exits the program once the next process is "
                     "done\n"
                  << "  terminate all   - Terminate all processes and exit the program\n"
                  << "  terminate <pid> - Terminate a specific process by PID\n"
                  << "  kill all        - Kill all processes and exit the program\n"
                  << "  kill <pid>      - Kill a specific process by PID\n"
                  << "  display pids    - Display all current PIDs\n"
                  << "  respawn on      - Turn on respawn\n"
                  << "  respawn off     - Turn off respawn\n"
                  << "  help            - Display this help message\n\n"
                  << "==========================================================\n\n"
                  << std::flush;
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
 