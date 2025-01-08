#ifndef PROCESS_HANDLER_H
#define PROCESS_HANDLER_H
#include "process_base.h"
#include "process_helper.h"
#include "simul_process.h"
class ProcessHandler final : protected ProcessBase, public ProcessHelper 
{
public:
    void init(Synchro *synchro, std::unique_ptr<SimulProcess> process);
    void start();
    static void numProcesses(int numProcesses);
    static int numProcesses();
    static std::vector<std::unique_ptr<ProcessHandler>> handlers_;
    static Synchro *synchro();

private:
    static int numProcesses_;
    void       createChild();
    std::unique_ptr<SimulProcess> process_;
};

#endif // PROCESS_HANDLER_H
