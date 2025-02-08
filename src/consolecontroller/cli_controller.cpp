#include "cli_controller.h"
#include <iostream>

namespace cli::driver
{

    bool validateInput(const std::string &input)
    {
        // Perform input validation
        return !input.empty(); // Example validation
    }

    void CLIController::runThread()
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
                        if (commandFunc)
                        {
                            commandFunc(input);
                        }
                        else
                        {
                            stopFlag = true;
                        }
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
        stopped = true;
        cv.notify_one();
    }


} // namespace cli::driver
