#include "communicator.h"
#include <thread>
#include "logger_instance.h"
#include "message.h"

namespace concurrency
{
    std::string createMessage(pid_t pid, const std::string &sleepDuration)
    {
        std::string message = "PID: " + std::to_string(pid);
    
        if (!sleepDuration.empty())
        {
            message += " | Sleep duration: " + sleepDuration;           
        }
    
        return message;
    }

    std::string createMessage(pid_t pid, const std::string &sleepDuration, int exitCode)
    {
        std::string message = "PID: " + std::to_string(pid);

        if (sleepDuration.empty())
        {
            message += " | Exit code: " + std::to_string(exitCode);
        }
        else
        {
            message += " | Sleep duration: " + sleepDuration;
            message += " | Exit code: " + std::to_string(exitCode);
        }

        return message;
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
        sendMessage(createMessage(pid, sleepDuration), Message::CREATION_MSG);
    }

    void Communicator::sendTerminationMessage(const std::string &sleepDuration, pid_t pid, int exitCode)
    {
        sendMessage(createMessage(pid, sleepDuration, exitCode), Message::TERMINATION_MSG);
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