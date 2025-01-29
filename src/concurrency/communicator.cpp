#include <iostream>
#include "communicator.h"
#include "logger_instance.h"

namespace concurrency
{
    void Communicator::sendCreationMessage(const std::string& sleepDuration, pid_t pid)
    {
        std::string messageText = "Child " + std::to_string(pid) +
                                  " created. Sleep duration: " + sleepDuration;
        std::cout << messageText << std::endl;
        tools::LoggerManager::getInstance().logInfo("Sending message: " + messageText);

        try
        {
            messenger_.sendMessage(messageText, Message::CREATION_MSG);
            tools::LoggerManager::getInstance().logInfo("Message sent successfully.");
        }
        catch (const std::runtime_error &e)
        {
            tools::LoggerManager::getInstance().logError("Failed to send message: " + std::string(e.what()));
        }
    }

    std::string Communicator::receiveCreationMessage() 
    { 
        tools::LoggerManager::getInstance().logInfo("Attempting to receive message.");

        std::string message = messenger_.receiveMessage(Message::CREATION_MSG);

        tools::LoggerManager::getInstance().logInfo("Received message: " + message);

        return message; 
    }
} // namespace concurrency
