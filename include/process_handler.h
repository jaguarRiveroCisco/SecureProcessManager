#ifndef PROCESS_HANDLER_H
#define PROCESS_HANDLER_H
#include "process_interface.h"
#include "controller_base.h"

namespace process
{
    class Controller final : protected ControllerBase 
    {
    public:
        void        init(Synchro *synchro, std::unique_ptr<IProcess> process);
        void        start();
        static void numProcesses(int numProcesses);
        static int  numProcesses();
        static Synchro *synchro();
        std::string     receiveCreationMessage();
        
        static void     terminateAll();
        static void     terminateProcessByPid(pid_t pid);
        static void     killAll();
        static void     killProcessByPid(pid_t pid);
        static void     displayAllPids();
        static void     run(const std::string &processType, int numProcesses);


    private:
        static void                       waitForEvents();
        static void                       createHandlers(int numProcesses);
        static void                       setProcessType(const std::string &processType);
        static void                       createHandler();

        void                              createChild();
        std::unique_ptr<IProcess> process_;
        static std::vector<std::unique_ptr<Controller>> handlers_;
    };
} // namespace process

#endif // PROCESS_HANDLER_H