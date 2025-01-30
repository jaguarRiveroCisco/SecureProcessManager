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
        void                 sendTerminationMessage(const std::string &, pid_t);
        static Communicator &getInstance()
        {
            static Communicator instance;
            return instance;
        }

    protected:
        Communicator()                                = default;
        Communicator(const Communicator &)            = delete;
        Communicator &operator=(const Communicator &) = delete;
        Communicator(Communicator &&)                 = delete;
        Messenger messenger_;
    private:
        void sendMessage(const std::string &text, int msgType);
        std::string receiveMessage(int msgType);
        std::string receiveProcessMessage(int type);
    };
} // namespace concurrency

#endif // COMMUNICATOR_H
