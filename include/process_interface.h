#ifndef PROCESS_INTERFACE_H
#define PROCESS_INTERFACE_H
#include "process_helper.h"
class ProcessInterface : public ProcessHelper
{
public:
    virtual ~ProcessInterface() = default;
    virtual void work() = 0;
};

#endif // PROCESS_INTERFACE_H