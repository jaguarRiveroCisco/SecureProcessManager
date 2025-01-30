#pragma once

#ifndef PROCESS_INTERFACE_H
#define PROCESS_INTERFACE_H

#include <sys/types.h>
namespace process
{
    class IProcess 
    {
    public:
        virtual ~IProcess() = default;
        virtual void work() = 0;
        // Add a virtual setter for pid
        virtual void setPid(pid_t* pid) = 0;

    protected:
        virtual void preWork(pid_t pid) = 0;
        virtual void postWork(pid_t pid) = 0;
    };

} // namespace process

#endif // PROCESS_INTERFACE_H
