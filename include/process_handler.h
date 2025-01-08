#ifndef PROCESS_HANDLER_H
#define PROCESS_HANDLER_H
#include "process_base.h"
#include "process_helper.h"
#include "simul_process.h"
class ProcessHandler final : protected ProcessBase, protected SimulProcess, public ProcessHelper 
{
public:
    void start(Synchro *synchro);
    static void numProcesses(int numProcesses);
    static int numProcesses();
    static std::vector<std::unique_ptr<ProcessHandler>> handlers_;

private:
    static int numProcesses_;
    void       createChild();
};

#endif // PROCESS_HANDLER_H
