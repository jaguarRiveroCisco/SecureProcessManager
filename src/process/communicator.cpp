#include <iostream>
#include "communicator.h"
#include "logger_instance.h"
namespace process
{
    Messenger Communicator::messenger_;

    void Communicator::sendCreationMessage(int sleepDuration, pid_t pid)
    {
        std::string messageText = ":) Child " + std::to_string(pid) +
                                  " created. Sleep duration: " + std::to_string(sleepDuration) + " seconds";
        try
        {
            messenger_.sendMessage(messageText, Message::CREATION_MSG);
        }
        catch (const std::runtime_error &e)
        {
            tools::LogOpt::getInstance() << "Failed to send message: " << e.what();
            tools::LogOpt::getInstance().flush(tools::LogLevel::ERROR);
        }
    }

    std::string Communicator::receiveCreationMessage() { return messenger_.receiveMessage(Message::CREATION_MSG); }
} // namespace process
