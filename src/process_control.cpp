#include "process_control.h"
#include <atomic>
#include <iostream>
#include <string>
#include <thread>
#include "process_handler.h"

extern std::atomic<bool> g_display;
extern std::atomic<bool> g_running;

namespace process
{
    void killPid(std::__1::string &input);
    void terminatePid(std::__1::string &input);
    void printHelp();

    void controller()
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
                else if (input == "terminate all")
                {
                    g_running = false; // Set running to false to signal the main thread
                    process::Controller::terminateAll();
                }
                else if (input.rfind("terminate ", 0) == 0) // Check if input starts with "terminate "
                    terminatePid(input);
                else if (input == "kill all")
                {
                    g_running = false; // Set running to false to signal the main thread
                    process::Controller::killAll();
                }
                else if (input.rfind("kill ", 0) == 0) // Check if input starts with "kill "
                    killPid(input);
                else if (input == "display pids")
                {
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
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void printHelp()
    {
        std::cout << "Available commands:\n"
                  << "  print on       - Turn on display progress\n"
                  << "  print off      - Turn off display progress\n"
                  << "  exit           - Exit the program once all processes are done\n"
                  << "  terminate all  - Terminate all processes and exit the program\n"
                  << "  terminate <pid> - Terminate a specific process by PID\n"
                  << "  kill all       - Kill all processes and exit the program\n"
                  << "  kill <pid>     - Kill a specific process by PID\n"
                  << "  display pids   - Display all current PIDs\n"
                  << "  help           - Display this help message\n";
    }

    void killPid(std::__1::string &input)
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

    void terminatePid(std::__1::string &input)
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
 