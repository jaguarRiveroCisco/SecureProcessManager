#include "process_control.h"
#include <atomic>
#include <unistd.h>
#include <iostream>
#include <string>
#include <thread>
#include "process_handler.h"
#include "logger.h"

extern std::atomic<bool> g_display;

namespace process::controller
{
    void killPid(const std::string &input);
    void terminatePid(const std::string &input);
    void doCommand(const std::string &input);
    void printContext(int numProcesses = -1, const std::string &processType = "", int rndUpper = -1);
    tools::Logger logger;

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
                    logger.logInfo("Number of processes: " + std::to_string(numProcesses));
                    break;
                case 't':
                    // Set the process type from the argument
                    processType = optarg;
                    logger.logInfo("Process type: " + processType);
                    break;
                case 'r':
                    // Set the random upper limit from the argument
                    rndUpper = std::atoi(optarg);
                    logger.logInfo("Random upper limit: " + std::to_string(rndUpper));
                    break;
                case 'd':
                    // Set the display flag from the argument (0 or 1)
                    g_display = std::atoi(optarg) != 0;
                    logger.logInfo("Display flag: " + std::string(g_display ? "Enabled" : "Disabled"));
                    break;
                case 's':
                    // Set the respawn flag from the argument (0 or 1)
                    process::ControllerBase::respawn() = std::atoi(optarg) != 0;
                    logger.logInfo("Respawn flag: " + std::string(process::ControllerBase::respawn() ? "Enabled" : "Disabled"));
                    break;
                case 'h':
                default:
                    // Display usage information and exit
                    logger.logError("Usage: " + std::string(argv[0]) +
                                    " -n <number of processes> -t <process type 'real' or 'simul' (default)> -r <random "
                                    "upper limit> -d <display (0 or 1)> -r <respawn (0 or 1)> -h -> help");
                    std::exit(EXIT_SUCCESS);
            }
        }

        if (numProcesses <= 0)
        {
            logger.logWarning("Number of processes must be greater than 0. Defaulting to 4.");
            numProcesses = 4;
        }

        if (processType != "real" && processType != "simul")
        {
            logger.logWarning("Invalid process type: " + processType + ". Defaulting to 'simul'.");
            processType = "simul";
        }

        if (rndUpper < 10)
        {
            logger.logWarning("Random upper limit must be greater than 10. Defaulting to 10.");
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

    void doCommand(const std::string &input)
    {
        if (input.empty())
        {
            return;
        }

        logger.logInfo(">> Command: " + input); // Highlight the command being processed

        if (input == "print on")
        {
            g_display = true;
            logger.logInfo("Display progress is now ON.");
        }
        else if (input == "print off")
        {
            g_display = false;
            logger.logInfo("Display progress is now OFF.");
        }
        else if (input == "context")
        {
            printContext();
        }
        else if (input == "exit")
        {
            process::ControllerBase::running() = false;
            logger.logInfo("Exiting program after current process completes.");
        }
        else if (input == "terminate all")
        {
            process::ControllerBase::running() = false;
            logger.logInfo("Terminating all processes and exiting.");
            process::Controller::terminateAll();
        }
        else if (input.rfind("terminate ", 0) == 0)
        {
            logger.logInfo("Terminating process with PID: " + input.substr(10));
            terminatePid(input);
        }
        else if (input == "kill all")
        {
            process::ControllerBase::running() = false;
            logger.logInfo("Killing all processes and exiting.");
            process::Controller::killAll();
        }
        else if (input.rfind("kill ", 0) == 0)
        {
            logger.logInfo("Killing process with PID: " + input.substr(5));
            killPid(input);
        }
        else if (input == "display pids")
        {
            logger.logInfo("Current Process IDs:");
            process::Controller::displayAllPids();
        }
        else if (input == "help")
        {
            printHelp();
        }
        else if (input == "respawn on")
        {
            process::ControllerBase::respawn() = true;
            logger.logInfo("Respawn feature is now ON.");
        }
        else if (input == "respawn off")
        {
            process::ControllerBase::respawn() = false;
            logger.logInfo("Respawn feature is now OFF.");
        }
        else
        {
            logger.logError("Unknown command. Type 'help' for a list of available commands.");
        }

        logger.logInfo(std::string(40, '-')); // Separator for readability
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
        try
        {
            pid_t pid = std::stoi(input.substr(5)); // Extract PID from input
            process::Controller::killProcessByPid(pid);
        }
        catch (const std::invalid_argument &)
        {
            logger.logError("Invalid PID format.");
        }
        catch (const std::out_of_range &)
        {
            logger.logError("PID out of range.");
        }
    }

    void terminatePid(const std::string &input)
    {
        try
        {
            pid_t pid = std::stoi(input.substr(10)); // Extract PID from input
            process::Controller::terminateProcessByPid(pid);
        }
        catch (const std::invalid_argument &)
        {
            logger.logError("Invalid PID format.");
        }
        catch (const std::out_of_range &)
        {
            logger.logError("PID out of range.");
        }
    }
} // namespace process