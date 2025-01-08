#ifndef PROCESS_HELPER_H
#define PROCESS_HELPER_H
#include <unistd.h>
#include "messenger.h"

class ProcessHelper 
{
public:
     virtual ~ProcessHelper() = default;
    virtual std::string receiveCreationMessage() = 0;

protected:
    ProcessHelper() = default;
    void sendCreationMessage(int sleepDuration, pid_t pid = getpid());
    static Messenger messenger_;

};

#endif // PROCESS_HELPER_H
