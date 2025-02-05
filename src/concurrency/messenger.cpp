#include "messenger.h"

#include <nap_time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <thread>
#include "process_controller.h"
#include "message.h"

namespace concurrency
{

    static constexpr size_t TIMEOUT_SECS = 5;
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
        struct msqid_ds buf = {};
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

        auto start = std::chrono::steady_clock::now();
        auto timeout = std::chrono::seconds(TIMEOUT_SECS); // Set a timeout duration

        while (process::ProcessController::running())
        {
            std::lock_guard<std::mutex> lock(mutex); // Lock the mutex to synchronize access
            if (msgsnd(msgid_, &message, sizeof(message.msgText), IPC_NOWAIT) != -1)
            {
                return; // Message sent successfully
            }
            if (errno != EAGAIN) // Check for non-recoverable errors
            {
                perror("msgsnd");
                throw std::runtime_error("Failed to send message");
            }
            // Check for timeout
            auto now = std::chrono::steady_clock::now();
            if (now - start > timeout)
            {
                break;
            }

            // Sleep for a short duration before retrying
            std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::VERYSMALL));
        }

        throw std::runtime_error("Failed to send message within timeout period");
    }

    std::string Messenger::receiveMessage(int msgType)
    {
        Message message;
        message.msgType = msgType;
        auto start = std::chrono::steady_clock::now();
        auto timeout = std::chrono::seconds(TIMEOUT_SECS); // Set a timeout duration

        while (process::ProcessController::running())
        {
            {
                std::lock_guard<std::mutex> lock(mutex); // Lock the mutex to synchronize access within the process
                if (msgrcv(msgid_, &message, sizeof(message.msgText), msgType, IPC_NOWAIT) != -1)
                {
                    return {message.msgText};
                }
            }

            if (errno != ENOMSG)
            {
                perror("msgrcv");
                throw std::runtime_error("Failed to receive message");
            }
        
            // Check for timeout
            auto now = std::chrono::steady_clock::now();
            if (now - start > timeout)
            {
                break;
            }
        
            // Sleep for a short duration before retrying
            std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::VERYSMALL));
        }
        return message.msgText;
    }
} // namespace concurrency
