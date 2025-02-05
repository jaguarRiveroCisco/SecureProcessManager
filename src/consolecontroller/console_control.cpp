#include "console_control.h"
#include <unistd.h>
#include "logger_instance.h"
#include "console_loop.h"
#include "api.h"

namespace cli::driver
{

    static constexpr int MAX_PROCESSES = 20;

    void parseArguments(int argc, char *argv[], int &numProcesses, std::string &processType, tools::ConsoleLogger& cl)
    {
        int opt;
        while ((opt = getopt(argc, argv, "n:t:s:l:T:h")) != -1) // Removed colon after 'h'
        {
            switch (opt)
            {
                case 'n':
                    // Set the number of processes from the argument
                    numProcesses = std::strtol(optarg, nullptr, 10);
                    if (numProcesses <= 0)
                    {
                        cl << "Number of processes must be greater than 0. Defaulting to 4.";
                        cl.flush(tools::LogLevel::WARNING);
                        numProcesses = 4;
                    }
                    numProcesses = std::min(numProcesses, MAX_PROCESSES);
                    break;
                case 't':
                    // Set the process type from the argument
                    processType = optarg;
                    if (processType != "real" && processType != "simul" && processType != "network" &&
                        processType != "system")
                    {
                        cl << "Valid process types are: real, network, system, simul. Defaulting to simul.";
                        cl.flush(tools::LogLevel::WARNING);
                        processType = "simul";
                    }
                    break;
                case 's':
                    api::respawn(std::strtol(optarg, nullptr, 10) != 0);
                    break;
                case 'l':
                    // Set the logging type from the argument
                    if (std::string(optarg) == "file")
                    {
                        api::loggerType("file");
                    }
                    else
                    {
                        cl << "Invalid logging type defaulting to " << "console";
                        cl.flush(tools::LogLevel::WARNING);
                        api::loggerType("console");
                    }
                    break;
                case 'T':
                    // Set the nap time type from the argument
                    if (std::string(optarg) == "MS")
                    {
                        api::SleepType(tools::NapType::MS);
                    }
                    else if (std::string(optarg) == "SEC")
                    {
                        api::SleepType(tools::NapType::SEC);
                    }
                    else if (std::string(optarg) == "MIN")
                    {
                        api::SleepType(tools::NapType::MIN);
                    }
                    else
                    {
                        cl << "Invalid nap time type defaulting to " << "MS";
                        cl.flush(tools::LogLevel::WARNING);
                        api::SleepType(tools::NapType::MS);
                    }
                    break;
                case 'h':
                default:
                    // Display usage information and exit
                    cl  << argv[0]
                        << "\n"
                        << "Usage:\n"
                        << "-n <number of processes> (max 20)\n"
                        << "-t <process type 'real', 'network', 'system' or 'simul' (default)>\n"
                        << "-s <respawn (0 or 1)>\n"
                        << "-l <logging type 'console' or 'file'>\n"
                        << "-T <nap time type 'MS', 'SEC', 'MIN'>\n"
                        << "-h -> help";
                    cl.flush(tools::LogLevel::INFO);
                    std::exit(EXIT_SUCCESS);
            }
        }
        printContext(numProcesses, processType);
        printCommands(); // Call to printHelp
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
