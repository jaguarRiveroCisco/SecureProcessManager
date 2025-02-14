#include "console_control.h"
#include <unistd.h>
#include "logger_instance.h"
#include "api.h"
#include "process_controller.h"

namespace cli::driver
{

    static constexpr int MAX_PROCESSES = 20;

    void parseArguments(int argc, char *argv[], int &numProcesses, std::string &processType, tools::ConsoleLogger &cl) {
        int opt;
        bool configFileProvided = false;

        while ((opt = getopt(argc, argv, "n:t:s:l:T:c:h")) != -1)
        { // Added 'c' for config file path
            try
            {
                switch (opt)
                {
                    case 'n':
                        // Set the number of processes from the argument
                        try
                        {
                            numProcesses = std::stoi(optarg);
                            if (numProcesses <= 0 || numProcesses > MAX_PROCESSES)
                            {
                                throw std::out_of_range("Number of processes out of valid range.");
                            }
                        }
                        catch (const std::exception &e)
                        {
                            cl << "Invalid number of processes. Defaulting to 4.";
                            cl.flush(tools::LogLevel::WARNING);
                            numProcesses = 4;
                        }
                        break;
                    case 't':
                        // Set the process type from the argument
                        processType = optarg;
                        if (processType != "real" && processType != "simul" && processType != "network" &&
                            processType != "system" && processType != "custom") {
                            cl << "Valid process types are: real, network, system, simul, custom. Defaulting to simul.";
                            cl.flush(tools::LogLevel::WARNING);
                            processType = "simul";
                        }
                        break;
                    case 's':
                        api::respawn(std::strtol(optarg, nullptr, 10) != 0);
                        break;
                    case 'l':
                        // Set the logging type from the argument
                        if (std::string(optarg) == "file") {
                            api::loggerType("file");
                        } else {
                            cl << "Invalid logging type defaulting to console";
                            cl.flush(tools::LogLevel::WARNING);
                            api::loggerType("console");
                        }
                        break;
                    case 'T':
                        // Set the nap time type from the argument
                        if (std::string(optarg) == "MS") {
                            api::SleepType(tools::NapType::MS);
                        } else if (std::string(optarg) == "SEC") {
                            api::SleepType(tools::NapType::SEC);
                        } else if (std::string(optarg) == "MIN") {
                            api::SleepType(tools::NapType::MIN);
                        } else {
                            cl << "Invalid nap time type defaulting to MS";
                            cl.flush(tools::LogLevel::WARNING);
                            api::SleepType(tools::NapType::MS);
                        }
                        break;
                    case 'c':
                        // Set the configuration file path from the argument
                        api::readConfigFile(optarg);
                        configFileProvided = true;
                        break;
                    case 'h':
                    default:
                        // Display usage information and exit
                        cl << argv[0]
                           << "\n"
                           << "Usage:\n"
                           << "-n <number of processes> (max " << MAX_PROCESSES << ")\n"
                           << "-t <process type 'real', 'network', 'system', 'custom' or 'simul' (default)>\n"
                           << "-s <respawn (0 or 1)>\n"
                           << "-l <logging type 'console' or 'file'>\n"
                           << "-T <nap time type 'MS', 'SEC', 'MIN'>\n"
                           << "-c <configuration file path>\n"
                           << "-h help";
                        cl.flush(tools::LogLevel::INFO);
                        std::exit(EXIT_SUCCESS);
                }
            }
            catch (const std::exception &e)
            {
                cl << "Error: " << e.what();
                cl.flush(tools::LogLevel::EXCEPTION);
                std::exit(EXIT_FAILURE);
            }
        }

        // Check if processType is 'system' and if the configuration file was provided
        if (processType == "system" && !configFileProvided)
        {
            cl << "Configuration file with parameters (option -c file) must be provided when the process type is 'system'.";
            cl.flush(tools::LogLevel::ERROR);
            std::exit(EXIT_FAILURE);
        }
    }

    void printContext(const int numProcesses, const std::string &processType)
    {
        // Static variables to store the latest values
        static int         lastNumProcesses = 0;
        static std::string lastProcessType  = "unknown";

        // Update static variables only if new values are provided
        if (numProcesses != -1)
            lastNumProcesses = numProcesses;
        if (!processType.empty())
            lastProcessType = processType;

        // Print the stored context
        std::cout << "\n========================= Context =========================\n"
                  << " PID                 : " << getpid() << "\n"
                  << " Number of Processes : " << lastNumProcesses << "\n"
                  << " Process Type        : " << lastProcessType << "\n"
                  << " Respawn             : " << (process::ProcessController::respawn() ? "Enabled" : "Disabled") << "\n"
                  << " Logging Type        : " << process::ProcessController::loggingTypeToString() << "\n"
                  << " Nap Time Type       : " << tools::SleepTime::NapTypeToString() << "\n"
                  << "==========================================================\n\n"
                  << std::flush;
    }

    void printCommands()
    {
        std::cout
                << "\n==========================================================\n"
                << "Process Commands Help Menu (" << "PID: " << getpid() << ")\n"
                << "==========================================================\n"
                << "Available commands:\n"
                << "  context         - Display the current context\n"
                << "  quit            - Signals the program to gracefully quit at the next loop\n"
                << "  term all        - Terminate (SIGTERM) all processes and exit the program\n"
                << "  term <pid>      - Terminate (SIGTERM) a specific process by PID\n"
                << "  int all         - Interrupt (SIGINT) all processes and exit the program\n"
                << "  int <pid>       - Interrupt (SIGINT) a specific process by PID\n"
                << "  kill all        - Kill all (SIGKILL) processes and exit the program\n"
                << "  kill <pid>      - Kill a (SIGKILL) specific process by PID\n"
                << "  display pids    - Display all current PIDs\n"
                << "  respawn on      - Turn on respawn\n"
                << "  respawn off     - Turn off respawn\n"
                << "  monitor on      - Turn on monitoring: spawn monitoring threads\n"
                << "  monitor off     - Turn off monitoring: end monitoring threads\n"
                << "  help            - Display this help message\n\n"
                << "==========================================================\n\n"
                << std::flush;
    }

    int LoggerExample()
    {
        //
        tools::ConsoleLogger cl;
        cl.log(tools::LogLevel::INFO, "This is an info message.");
        cl.log(tools::LogLevel::WARNING, "This is a warning message.");
        cl.log(tools::LogLevel::ERROR, "This is an error message.");
        cl.logInfo("This is an info message.");
        cl.logWarning("This is a warning message.");
        cl.logError("This is an error message.");
        cl << "Starting a new log entry with operator<< ";
        cl << "and adding more details.";
        cl.flush(tools::LogLevel::INFO);
        //
        return 0;
    }
} // namespace cli::driver
