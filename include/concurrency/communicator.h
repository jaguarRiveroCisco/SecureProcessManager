#pragma once

#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H
#include <unistd.h>
#include "messenger.h"

namespace concurrency
{
    class Communicator 
    {
    public:
        virtual ~Communicator() = default;

        virtual std::string  receiveCreationMessage();
        virtual std::string  receiveTerminationMessage();
        void                 sendCreationMessage(const std::string &, pid_t);
        void                 sendTerminationMessage(const std::string &, pid_t, int);
        static Communicator &getInstance()
        {
            static Communicator instance;
            return instance;
        }
        Communicator(const Communicator &)            = delete;
        Communicator(Communicator &&)                 = delete;
        Communicator &operator=(const Communicator &) = delete;
    protected:
        Communicator()                                = default;
        Messenger messenger_{};
    private:
        void sendMessage(const std::string &text, int msgType);
        std::string receiveMessage(int msgType);
        std::string receiveProcessMessage(int type);
    };
} // namespace concurrency

#endif // COMMUNICATOR_H
