#include <iostream>
#include "communicator.h"
#include "logger_instance.h"
namespace concurrency
{
    void Communicator::sendCreationMessage(const std::string& sleepDuration, pid_t pid)
    {
        std::string messageText = "Child " + std::to_string(pid) +
                                  " created. Sleep duration: " + sleepDuration;
        try
        {
            messenger_.sendMessage(messageText, Message::CREATION_MSG);
        }
        catch (const std::runtime_error &e)
        {
            tools::LoggerManager::getInstance() << "Failed to send message: " << e.what();
            tools::LoggerManager::getInstance().flush(tools::LogLevel::ERROR);
        }
    }

    std::string Communicator::receiveCreationMessage() { return messenger_.receiveMessage(Message::CREATION_MSG); }
} // namespace concurrency
