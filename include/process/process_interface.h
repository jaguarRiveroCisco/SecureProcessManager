#ifndef PROCESS_INTERFACE_H
#define PROCESS_INTERFACE_H
#include <iostream>
#include "process_helper.h"
namespace process
{
    class IProcess {
    public:
        virtual ~IProcess() {}
        virtual void work() = 0;

    protected:
        IProcess() = default;
        void setupSignalHandling()
        {
            signal(SIGTERM, IProcess::signalHandler);
            signal(SIGINT, IProcess::signalHandler);
        }

    private:
        static void signalHandler(int signum)
        {
            std::cout << "Process " << getpid() << " received signal " << signum << std::endl;
            exit(signum);
        }
    };

} // namespace process

#endif // PROCESS_INTERFACE_H
