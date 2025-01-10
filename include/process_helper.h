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
        void                 sendCreationMessage(int sleepDuration = 0, pid_t pid = getpid());
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
        static Messenger messenger_;
    };
} // namespace process

#endif // PROCESS_HELPER_H
