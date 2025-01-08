#ifndef PROCESS_H
#define PROCESS_H

#include "process_interface.h"

class Process : public ProcessInterface
{
public:
    Process() = default;
    virtual ~Process() = default;
    void work() override;
};

#endif // PROCESS_H