#include "communicator.h"
#include <thread>
#include "logger_instance.h"

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
        return messenger_.receiveMessage(msgType);
    }


    void Communicator::sendCreationMessage(const std::string &sleepDuration, pid_t pid)
    {
        sendMessage(createMessage(sleepDuration, pid), Message::CREATION_MSG);
    }

    std::string Communicator::receiveCreationMessage()
    {
        try
        {
            return messenger_.receiveMessage(Message::CREATION_MSG);
        }
        catch (const std::runtime_error &e)
        {
            tools::LoggerManager::getInstance().logException("Failed to receive creation message: " +
                                                             std::string(e.what()));
            return ""; // Return an empty string or handle as needed
        }
    }
} // namespace concurrency