#ifndef PROCESS_INTERFACE_H
#define PROCESS_INTERFACE_H
#include <string>
class ProcessInterface
{
public:
    virtual ~ProcessInterface() = default;
    virtual void work() = 0;
    virtual std::string receiveCreationMessage() = 0;
};

#endif // PROCESS_INTERFACE_H