//
// Created by Jaguar Rivero on 2025-02-05.
//
#include "console_loop.h"
#include "console_control.h"
#include <iostream>
#include <thread>
#include <unistd.h>
#include "cli_controller.h"
#include "console_logger.h"
#include "logger_instance.h"
#include "main_controller.h"
#include "string_tools.h"
#include "nap_time.h"
#include "api.h"
namespace cli::driver
{
    static tools::ConsoleLogger cl;





    void printContext(int numProcesses, const std::string &processType)
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
        else if (input == "quit")
        {
            api::stop();
            cl << "[QUIT] Signalling the program to gracefully quit at the next loop.";
            cl.flush(tools::LogLevel::INFO);
        }
        else if (input == "term all")
        {
            cl << "[TERMINATE ALL] Terminating all processes and exiting.";
            cl.flush(tools::LogLevel::INFO);
            api::terminateAll();
        }
        else if (input.rfind("term ", 0) == 0)
        {
            auto vals = tools::string::splitString(input);
            cl << "[TERMINATE ONE] Terminating process with PID:" << vals[1];
            cl.flush(tools::LogLevel::INFO);
            api::terminatePid(std::stoi(vals[1]));
        }
        else if (input == "int all")
        {
            cl << "[INTERRUPT ALL] Interrupting all processes and exiting.";
            cl.flush(tools::LogLevel::INFO);
            process::ProcessController::intAll();
        }
        else if (input.rfind("int ", 0) == 0)
        {
            auto vals = tools::string::splitString(input);

            cl << "[INTERRUPT ONE] Interrupting process with PID:" << vals[1];
            cl.flush(tools::LogLevel::INFO);
            api::intPid( std::stoi(vals[1]) );
        }
        else if (input == "kill all")
        {
            cl << "[KILL ALL] Killing all processes and exiting.";
            cl.flush(tools::LogLevel::INFO);
            api::killAll();
        }
        else if (input == "monitor on")
        {
            cl.logInfo("[MONITOR ON] Turning on monitoring: spawning monitoring threads");
            process::ProcessController::continueMonitoring();
        }
        else if (input == "monitor off")
        {
            cl.logInfo("[MONITOR OFF] Turning off monitoring: ending monitoring threads");
            process::ProcessController::pauseMonitoring();
        }
        else if (input.rfind("kill ", 0) == 0)
        {
            auto vals = tools::string::splitString(input);
            cl << "[KILL ONE] Killing process with PID:" << vals[1];
            cl.flush(tools::LogLevel::INFO);
            api::killPid(std::stoi(vals[1]));
        }
        else if (input == "display pids")
        {
            api::displayAllPids();
        }
        else if (input == "help")
        {
            printCommands();
        }
        else if (input == "respawn on")
        {
            api::respawn(true);
            cl << "[RESPAWN] Respawn feature is now" << "ON";
            cl.flush(tools::LogLevel::INFO);
        }
        else if (input == "respawn off")
        {
            api::respawn(false);
            cl << "[RESPAWN] Respawn feature is now" << "OFF";
            cl.flush(tools::LogLevel::INFO);
        }
        else
        {
            cl << "[UNKNOWN COMMAND] [" + input + " ] Type 'help' for a list of available commands.";
            cl.flush(tools::LogLevel::ERROR);
        }
    }

    void consoleLoop(const bool run)
    {
        static std::unique_ptr<cli::driver::CLIController> cc{nullptr};
        if (run)
        {
            if(!cc)
            {
                cc = std::make_unique<cli::driver::CLIController>();
                cc->run(doCommand);
            }
        }
        else
        {
            if(cc)
            {
                cc->stop();
                cc.reset();
            }
        }
    }


}