#include "process_base.h"
#include "process_interface.h"

namespace process
{

    class ControllerBase : public BaseHandler 
    {
        public:
            static bool &running();
            static void  numProcesses(int numProcesses);
            static int   numProcesses();
            static void  terminateAll();
            static void  terminateProcessByPid(pid_t pid);
            static void  killAll();
            static void  killProcessByPid(pid_t pid);
            static void  displayAllPids();
            static Synchro *synchro();
            void            init(Synchro *synchro, std::unique_ptr<IProcess> process);
            std::string     receiveCreationMessage();
            void            start();
            static void     setProcessType(const std::string &processType);

        protected:
            static int         numProcesses_;
            static std::string processType_;
            static bool        running_;
            static std::vector<std::unique_ptr<ControllerBase>> handlers_;
            std::unique_ptr<IProcess>                           process_;
            void                                                createChild();
    };
}