#include "console_control.h"
#include <atomic>
#include <unistd.h>
#include <iostream>
#include <string>
#include <thread>
#include "process_handler.h"
#include "logger_instance.h"

namespace cli::driver
{
    void killPid(const std::string &input);
    void terminatePid(const std::string &input);
    void doCommand(const std::string &input);
    void printContext(int numProcesses = -1, const std::string &processType = "", int rndUpper = -1);

    template <typename T>
    void printpid(const std::string& str, const T& x)
    {
        tools::LoggerManager::consoleLogger() << str << " " << x;
        tools::LoggerManager::consoleLogger().flush(tools::LogLevel::INFO);
    }

    template<typename T> 
    void printpidE(const std::string &str, const T &x)
    {

        tools::LoggerManager::consoleLogger() << str << " " << x;
        tools::LoggerManager::consoleLogger().flush(tools::LogLevel::ERROR);
    }

   template<typename T> 
    void printpidW(const std::string &str, const T &x)
    {
        tools::LoggerManager::consoleLogger() << str << " " << x;
        tools::LoggerManager::consoleLogger().flush(tools::LogLevel::WARNING);
    }

    void parseArguments(int argc, char *argv[], int &numProcesses, std::string &processType, int &rndUpper)  
    {
        int opt;
        while ((opt = getopt(argc, argv, "n:t:r:s:l:h")) != -1)
        {
            switch (opt)
            {
            case 'n':
                // Set the number of processes from the argument
                numProcesses = std::atoi(optarg);
                if (numProcesses <= 0)
                {
                    printpidW("Number of processes must be greater than 0. Defaulting to  ", 4);
                    numProcesses = 4;
                }
 
                break;
            case 't':
                // Set the process type from the argument
                processType = optarg;
                if (processType != "real" && processType != "simul")
                {
                    printpidW("Invalid process type defaulting to ", "simul");
                    processType = "simul";
                }
                else
                    printpid("Process type: ", processType);
                break;
            case 'r':
                // Set the random upper limit from the argument
                rndUpper = std::atoi(optarg);
                if (rndUpper < 10)
                {
                    printpidW("Random upper limit must be >= than 10. Defaulting to ", 10);
                    rndUpper = 10;
                }

                break;
    
            case 's':
                // Set the respawn flag from the argument (0 or 1)
                process::ControllerBase::respawn() = std::atoi(optarg) != 0;
                printpid("Respawn flag: ", (process::ControllerBase::respawn() ? "Enabled" : "Disabled"));
                break;
            case 'l':
                // Set the logging type from the argument
                if (std::string(optarg) == "file")
                {
                    tools::LoggerManager::loggerType() = "file";
                }
                else
                {
                    printpidW("Invalid logging type defaulting to ", "console");
                    tools::LoggerManager::loggerType() = "console";
                }
                break;
            case 'h':
            default:
                // Display usage information and exit
                printpid(argv[0], "Usage: -n <number of processes> -t <process type 'real' or 'simul' (default)> -r <random upper limit> -d <display (0 or 1)> -s <respawn (0 or 1)> -l <logging type 'console' or 'file'> -h -> help");
                std::exit(EXIT_SUCCESS);
            }
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
                  << " PID                 : " << getpid() << "\n"
                  << " Number of Processes : " << lastNumProcesses << "\n"
                  << " Process Type        : " << lastProcessType << "\n"
                  << " Random Upper Limit  : " << lastRndUpper << "\n"
                  << " Respawn             : " << (process::ControllerBase::respawn() ? "Enabled" : "Disabled") << "\n"
                  << " Logging Type        : " << process::ControllerBase::loggingTypeToString() << "\n"
                  << "==========================================================\n\n"
                  << std::flush;
    }

    void printHelp()
    {
        std::cout << "\n==========================================================\n"
                  << "Process Control Help Menu (" << "PID: " << getpid() << ")\n"
                  << "==========================================================\n"
                  << "Available commands:\n"
                  << "  context         - Display the current context\n"
                  << "  exit            - Sets exist signal to gracefully exits the program once the next child process is "
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

        if (input == "context")
        {
            printContext();
        }
        else if (input == "exit")
        {
            process::ControllerBase::running() = false;
            printpid("[EXIT] Exiting program after next child process completes.", "");
        }
        else if (input == "terminate all")
        {
            process::ControllerBase::running() = false;
            printpid("[TERMINATE] Terminating all processes and exiting.", "");
            process::Controller::terminateAll();
        }
        else if (input.rfind("terminate ", 0) == 0)
        {
            printpid("[TERMINATE] Terminating process with PID:", input.substr(10));
            terminatePid(input);
        }
        else if (input == "kill all")
        {
            process::ControllerBase::running() = false;
            printpid("[KILL] Killing all processes and exiting.", "");
            process::Controller::killAll();
        }
        else if (input.rfind("kill ", 0) == 0)
        {
            printpid("[KILL] Killing process with PID:", input.substr(5));
            killPid(input);
        }
        else if (input == "display pids")
        {
            process::Controller::displayAllPids();
        }
        else if (input == "help")
        {
            printHelp();
        }
        else if (input == "respawn on")
        {
            process::ControllerBase::respawn() = true;
            printpid("[RESPAWN] Respawn feature is now", "ON");
        }
        else if (input == "respawn off")
        {
            process::ControllerBase::respawn() = false;
            printpid("[RESPAWN] Respawn feature is now", "OFF");
        }
        else
        {
            printpidE("[UNK] Unknown command []" + input + " ] Type 'help' for a list of available commands.","");
        }
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
            printpidE("Invalid PID format.","");
        }
        catch (const std::out_of_range &)
        {
            printpidE("PID out of range.","");
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
            printpidE("Invalid PID format.","");
        }
        catch (const std::out_of_range &)
        {
            printpidE("PID out of range.","");
        }
    }
    int LoggerExample()
    {
        //
        tools::LoggerManager::consoleLogger().log(tools::LogLevel::INFO, "This is an info message.");
        tools::LoggerManager::consoleLogger().log(tools::LogLevel::WARNING, "This is a warning message.");
        tools::LoggerManager::consoleLogger().log(tools::LogLevel::ERROR, "This is an error message.");
        tools::LoggerManager::consoleLogger().logInfo("This is an info message.");
        tools::LoggerManager::consoleLogger().logWarning("This is a warning message.");
        tools::LoggerManager::consoleLogger().logError("This is an error message.");
        tools::LoggerManager::consoleLogger() << "Starting a new log entry with operator<< ";
        tools::LoggerManager::consoleLogger() << "and adding more details.";
        tools::LoggerManager::consoleLogger().flush(tools::LogLevel::INFO);
        //
        return 0;
    }
} // namespace process