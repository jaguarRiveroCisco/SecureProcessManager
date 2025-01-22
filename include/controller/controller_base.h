#include "base_handler.h"
#include "locked_boolean.h"
#include "process_interface.h"

namespace process
{
    enum class LoggingType { Console, File };

    class ControllerBase : public BaseHandler 
    {
    public:
        static bool running();
        static bool respawn();
        static void running(bool);
        static void respawn(bool);
        static void terminateAll();
        static void terminateProcessByPid(pid_t pid);
        static void intProcessByPid(pid_t pid);
        static void intAll();
        static void killAll();
        static void killProcessByPid(pid_t pid);
        static void displayAllPids();
        static void pauseMonitoring();
        static void continueMonitoring();
        void        init(std::unique_ptr<IProcess> process);
        static void setProcessType(const std::string &processType);

        static std::string  loggingTypeToString(); // New method

    protected:
        static std::vector<std::unique_ptr<ControllerBase>> handlers_;
        static void        setNumProcesses(int numProcesses) { numProcesses_ = numProcesses; }
        static int         numProcesses_;
        static std::string processType_;
        static LoggingType loggingType_;

    private:
        void                              forkAndExecuteChildProcess();
        std::unique_ptr<IProcess>         process_;
        static concurrency::LockedBoolean running_;
        static concurrency::LockedBoolean respawn_;
    };
} // namespace process
