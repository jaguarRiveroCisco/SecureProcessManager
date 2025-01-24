#pragma once

#ifndef CONTROLLER_COMMAND_CONTROLLER_H
#define CONTROLLER_COMMAND_CONTROLLER_H

#include <atomic>
#include <functional>
#include <iostream>
#include <thread>

namespace cli::driver
{
    class CommandController 
    {
    public:
        void run(std::function<void(const std::string &)> commandFunc)
        {
            stopFlag          = false;
            this->commandFunc = commandFunc;
            readerThread      = std::thread(&CommandController::consoleLoop, this);
        }

        void stop()
        {
            stopFlag = true;
            if (readerThread.joinable())
            {
                readerThread.join();
            }
        }

    private:
        std::thread                              readerThread;
        std::atomic<bool>                        stopFlag;
        std::function<void(const std::string &)> commandFunc;

        void consoleLoop()
        {
            std::string input;
            while (!stopFlag)
            {
                try
                {
                    if (std::cin.peek() != EOF)
                    {
                        std::getline(std::cin, input);
                        if (validateInput(input))
                        {
                            commandFunc(input);
                        }
                        else
                        {
                            std::cerr << "Invalid input: " << input << std::endl;
                        }
                    }
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }

        bool validateInput(const std::string &input)
        {
            // Perform input validation
            return !input.empty(); // Example validation
        }
    };
} // namespace cli::driver

#endif // CONTROLLER_COMMAND_CONTROLLER_H
