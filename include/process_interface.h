#ifndef PROCESS_INTERFACE_H
#define PROCESS_INTERFACE_H
#include "process_helper.h"

namespace process
{
    class IProcess
    {
    public:
        virtual ~IProcess() = default;
        virtual void work()         = 0;
    };
} // namespace process

#endif // PROCESS_INTERFACE_H