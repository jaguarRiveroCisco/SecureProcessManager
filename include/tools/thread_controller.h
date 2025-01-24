#pragma once

#ifndef CONTROLLER_COMMAND_CONTROLLER_H
#define CONTROLLER_COMMAND_CONTROLLER_H

#include <atomic>
#include <functional>
#include <iostream>
#include <thread>

namespace cli::driver
{
    class ThreadController 
    {
    public:
        virtual ~ThreadController() = default;

        void run(std::function<void(const std::string &)> commandFunc)
        {
            this->commandFunc = std::move(commandFunc);
            stopFlag          = false;
            readerThread      = std::thread(&ThreadController::runThread, this);
        }

        void stop()
        {
            stopFlag = true;
            if (readerThread.joinable())
            {
                readerThread.join();
            }
        }

    protected:
        virtual void runThread() = 0;

        std::atomic<bool>                        stopFlag{false};
        std::function<void(const std::string &)> commandFunc;

    private:
        std::thread readerThread;
    };

    class CLIController : public ThreadController 
    {
    protected:
        void runThread() override
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
