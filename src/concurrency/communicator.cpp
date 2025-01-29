#include "communicator.h"
#include <thread>
#include "logger_instance.h"

namespace concurrency
{
    void Communicator::sendCreationMessage(const std::string &sleepDuration, pid_t pid)
    {
        std::string messageText = "Child " + std::to_string(pid) + " created. Sleep duration: " + sleepDuration;
        try
        {
            messenger_.sendMessage(messageText, Message::CREATION_MSG);
        }
        catch (const std::runtime_error &e)
        {
            tools::LoggerManager::getInstance().logException("Failed to send message: " + std::string(e.what()));
        }
    }

    std::string Communicator::receiveCreationMessage()
    {
        std::string message;
        auto        start   = std::chrono::steady_clock::now();
        auto        timeout = std::chrono::seconds(5); // Set a timeout duration

        while (true)
        {
            try
            {
                message = messenger_.receiveMessage(Message::CREATION_MSG);
                if (!message.empty())
                {
                    break;
                }
            }
            catch (const std::runtime_error &e)
            {
                tools::LoggerManager::getInstance().logException("Failed to receive message: " + std::string(e.what()));
            }

            // Check for timeout
            auto now = std::chrono::steady_clock::now();
            if (now - start > timeout)
            {
                tools::LoggerManager::getInstance().logError("Timeout while waiting for creation message");
                throw std::runtime_error("Timeout while waiting for creation message");
            }

            // Sleep for a short duration before retrying
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        return message;
    }
} // namespace concurrency