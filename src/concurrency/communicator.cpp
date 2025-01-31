#include "communicator.h"
#include <thread>
#include "logger_instance.h"

namespace concurrency
{
    std::string createMessage(const std::string &sleepDuration, pid_t pid) 
    {
        if(sleepDuration.empty())
        {
            return "PID: " + std::to_string(pid);
        }
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

    void Communicator::sendTerminationMessage(const std::string &sleepDuration, pid_t pid)
    {
        sendMessage(createMessage(sleepDuration, pid), Message::TERMINATION_MSG);
    }

    std::string Communicator::receiveProcessMessage(int type)
    {
        try
        {
            return messenger_.receiveMessage(type);
        }
        catch (const std::runtime_error &e)
        {
            tools::LoggerManager::getInstance().logException("Failed to receive process message: " + std::string(e.what()));
            return ""; // Return an empty string or handle as needed
        }
    }

    std::string Communicator::receiveCreationMessage()
    {
        return receiveProcessMessage(Message::CREATION_MSG);
    }

    std::string Communicator::receiveTerminationMessage()
    {
        return receiveProcessMessage(Message::TERMINATION_MSG);
    }
} // namespace concurrency