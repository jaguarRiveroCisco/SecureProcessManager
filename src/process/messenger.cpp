#include "messenger.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include "logger_instance.h"
#include "base_process.h"
namespace process
{
    static int counter = 0; // Initialize the counter

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

        if (buf.msg_qnum == 0)
        {
            counter++;

            if(process::BaseProcess::consoleFlag())
            {
                tools::LoggerManager::consoleLogger() << "[IPC MESSAGE QUEUE CREATED] ID " << msgid_
                << " | Total active queues: " << counter;
                tools::LoggerManager::consoleLogger().flush(tools::LogLevel::INFO);
            }
            else
            {
                tools::LoggerManager::getInstance() << "[IPC MESSAGE QUEUE CREATED] ID " << msgid_
                << " | Total active queues: " << counter;    
                tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);
            }
        }
        else
        {
            if(process::BaseProcess::consoleFlag())
            {
                tools::LoggerManager::consoleLogger() << "[IPC MESSAGE QUEUE OPEN] ID " << msgid_
                << " | Total active queues: " << counter;
                tools::LoggerManager::consoleLogger().flush(tools::LogLevel::INFO);
            }
            else
            {
                tools::LoggerManager::getInstance() << "[IPC MESSAGE QUEUE OPEN] ID " << msgid_
                << " | Total active queues: " << counter;
                tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);
            }
        }
    }

    Messenger::~Messenger()
    {
        // Destroy the message queue
        if (msgctl(msgid_, IPC_RMID, nullptr) == -1)
        {
            perror("msgctl");
        }
        else
        {
            tools::LoggerManager::getInstance() << "[IPC MESSAGE QUEUE DESTROYED] ID " << msgid_;
        }

        counter--;
        tools::LoggerManager::getInstance() << " | Total remaining queues: " << counter;
        tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);
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
