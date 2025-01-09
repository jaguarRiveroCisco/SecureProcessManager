#ifndef PROCESS_HANDLER_H
#define PROCESS_HANDLER_H
#include "process_interface.h"
#include "process_base.h"

namespace process
{
    class Controller final : protected BaseHandler 
    {
    public:
        void        init(Synchro *synchro, std::unique_ptr<IProcess> process);
        void        start();
        static void numProcesses(int numProcesses);
        static int  numProcesses();
        static std::vector<std::unique_ptr<Controller>> handlers_;
        static Synchro                                     *synchro();
        std::string                                         receiveCreationMessage();
        pid_t                                               getPid() const;
        static void                                         terminateAll();
        static void                                         terminateProcessByPid(pid_t pid);
        static void                                         killAll();
        static void                                         killProcessByPid(pid_t pid);
        static void                                         displayAllPids();
        static void                                         run(const std::string &processType, int numProcesses);

    private:
        static void                       waitForEvents();
        static void                       createHandlers(int numProcesses);
        static void                       setProcessType(const std::string &processType);
        static void                       createHandler();
        static int                        numProcesses_;
        static std::string                processType_;
        void                              createChild();
        std::unique_ptr<IProcess> process_;
    };
} // namespace process

#endif // PROCESS_HANDLER_H
