#include "messenger.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include "console_logger.h"

namespace process
{
    static int counter;

    Messenger::Messenger()
    {
        // Generate a unique key for the message queue
        auto key = ftok("progfile", 65);
        // Create a message queue
        msgid_ = msgget(key, 0666 | IPC_CREAT);
        if (msgid_ == -1)
        {
            perror("msgget");
            throw std::runtime_error("Failed to create message queue");
        }
        tools::ConsoleLogger::getInstance() << "[START] Message queue created with id " << msgid_
                  << " | Number of instances " << ++counter;
        tools::ConsoleLogger::getInstance().flush(tools::LogLevel::INFO);
    }

    Messenger::~Messenger()
    {
        // Destroy the message queue
        if (msgctl(msgid_, IPC_RMID, nullptr) == -1)
        {
            perror("msgctl");
        }
        tools::ConsoleLogger::getInstance() << "[END] Message queue with id " << msgid_
                  << " | Number of instances " << --counter;
        tools::ConsoleLogger::getInstance().flush(tools::LogLevel::INFO);
    }

    void Messenger::sendMessage(const std::string &text, int msgType)
    {
        Message message;
        message.msgType = msgType;
        std::snprintf(message.msgText, sizeof(message.msgText), "%s", text.c_str());

        if (msgsnd(msgid_, &message, sizeof(message.msgText), 0) == -1)
        {
            perror("msgsnd");
            throw std::runtime_error("Failed to send message");
        }
    }

    std::string Messenger::receiveMessage(int msgType)
    {
        Message message;
        if (msgrcv(msgid_, &message, sizeof(message.msgText), msgType, 0) == -1)
        {
            perror("msgrcv");
            throw std::runtime_error("Failed to receive message");
        }
        return std::string(message.msgText);
    }
} // namespace process
