#include "process_control.h"
#include <atomic>
#include <iostream>
#include <string>
#include <thread>
#include "process_handler.h"

extern std::atomic<bool> g_display;
extern std::atomic<bool> g_running;

namespace process::controller
{
    void killPid(const std::string &input);
    void terminatePid(const std::string &input);

    void main()
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
                    std::cout << "Gracefully exiting program once the first process is done" << std::endl;
                }
                else if (input == "terminate all")
                {
                    g_running = false; // Set running to false to signal the main thread
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
                    g_running = false; // Set running to false to signal the main thread
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
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void printHelp()
    {
        std::cout << "Process Control Help Menu\n"
                  << "==========================\n"
                  << "Available commands:\n"
                  << "  print on        - Turn on display progress\n"
                  << "  print off       - Turn off display progress\n"
                  << "  exit            - Gracefully exits the program once the first process is done\n"
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
 