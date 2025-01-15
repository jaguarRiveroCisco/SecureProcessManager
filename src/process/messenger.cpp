#include "messenger.h"
#include <sys/ipc.h>
#include <sys/msg.h>
namespace process
{
    Messenger::Messenger()
    {
        // Generate a unique key for the message queue
        auto key = ftok("progfile", 65);
        // Create or open a message queue
        msgid_ = msgget(key, 0666 | IPC_CREAT);
        if (msgid_ == -1)
        {
            perror("msgget");
            throw std::runtime_error("Failed to create or open message queue");
        }

        // Determine whether a new queue was created or an existing one was opened
        struct msqid_ds buf;
        if (msgctl(msgid_, IPC_STAT, &buf) == -1)
        {
            perror("msgctl");
            throw std::runtime_error("Failed to retrieve message queue status");
        }
        /*
              // Log some attributes of the message queue
        tools::LogOpt::getInstance().logInfo("Message queue created with ID " + std::to_string(msgid_));
        tools::LogOpt::getInstance().logInfo("Number of messages in queue: " + std::to_string(buf.msg_qnum));
        tools::LogOpt::getInstance().logInfo("Last message sent time: " + std::to_string(buf.msg_stime));
        tools::LogOpt::getInstance().logInfo("Last message received time: " + std::to_string(buf.msg_rtime));
        tools::LogOpt::getInstance().logInfo("Last change time: " + std::to_string(buf.msg_ctime));
        */
    }

    Messenger::~Messenger()
    {
        // Destroy the message queue
        if (msgctl(msgid_, IPC_RMID, nullptr) == -1)
        {
            perror("msgctl");
        }
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

        // tools::LoggerManager::getInstance() << "[MESSAGE] Sent message of type " << msgType << " to queue with ID " << msgid_;
        // tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);
    }

    std::string Messenger::receiveMessage(int msgType)
    {
        Message message;
        if (msgrcv(msgid_, &message, sizeof(message.msgText), msgType, 0) == -1)
        {
            perror("msgrcv");
            throw std::runtime_error("Failed to receive message");
        }

        // tools::LoggerManager::getInstance() << "[MESSAGE] Received message of type " << msgType << " from queue with ID " << msgid_;
        // tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);

        return std::string(message.msgText);
    }
} // namespace process
