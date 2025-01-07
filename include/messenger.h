#ifndef MESSENGER_H
#define MESSENGER_H

#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstdio>
#include <stdexcept>
#include <string>
#include "message.h"

class Messenger 
{
public:
    Messenger()
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
        std::cout << "CTOR Message queue created with id " << msgid_ << std::endl;
    }

    ~Messenger()
    {
        // Destroy the message queue
        if (msgctl(msgid_, IPC_RMID, nullptr) == -1)
        {
            perror("msgctl");
        }
        std::cout << "DTOR ~Message queue with id " << msgid_ << " destroyed" << std::endl;
    }

    void sendMessage(const std::string &text, int msgType)
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

    std::string receiveMessage(int msgType)
    {
        Message message;
        if (msgrcv(msgid_, &message, sizeof(message.msgText), msgType, 0) == -1)
        {
            perror("msgrcv");
            throw std::runtime_error("Failed to receive message");
        }
        return std::string(message.msgText);
    }
    // Delete the new and delete operators to prevent dynamic allocation
    void *operator new(size_t)   = delete;
    void operator delete(void *) = delete;

private:
    int msgid_;
};

#endif // MESSENGER_H
