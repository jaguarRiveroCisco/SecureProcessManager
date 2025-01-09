#ifndef PROCESS_HELPER_H
#define PROCESS_HELPER_H
#include <unistd.h>
#include "messenger.h"

namespace process
{
    class Communicator 
    {
    public:
        virtual ~Communicator() = default;
        virtual std::string receiveCreationMessage();

    protected:
        Communicator() = default;
        void             sendCreationMessage(int sleepDuration = 0, pid_t pid = getpid());
        static Messenger messenger_;
    };
} // namespace process

#endif // PROCESS_HELPER_H
