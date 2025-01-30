#include "communicator.h"
#include <thread>
#include "logger_instance.h"
#include "process_controller.h"

namespace concurrency
{
    std::string createMessage(const std::string &sleepDuration, pid_t pid) 
    {
        return "PID: " + std::to_string(pid) + " | Sleep duration: " + sleepDuration;
    }

    void Communicator::sendMessage(const std::string &text, int msgType)
    {
        try
        {
            messenger_.sendMessage(text, msgType);
        }
        catch (const std::runtime_error &e)
        {
            tools::LoggerManager::getInstance().logException("Failed to send message: " + std::string(e.what()));
        }
    }

    std::string Communicator::receiveMessage(int msgType)
    {
        std::string message;
        auto        start   = std::chrono::steady_clock::now();
        auto        timeout = std::chrono::seconds(5); // Set a timeout duration

        while (process::ProcessController::running())
        {
            try
            {
                message = messenger_.receiveMessage(msgType);
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


    void Communicator::sendCreationMessage(const std::string &sleepDuration, pid_t pid)
    {
        sendMessage(createMessage(sleepDuration, pid), Message::CREATION_MSG);
    }

    std::string Communicator::receiveCreationMessage() 
    { 
        return receiveMessage(Message::CREATION_MSG); 
    }
} // namespace concurrency