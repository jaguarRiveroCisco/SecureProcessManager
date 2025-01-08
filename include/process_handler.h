#ifndef PROCESS_HANDLER_H
#define PROCESS_HANDLER_H

class ProcessHandler final : protected ProcessBase, protected SimulProcess, public ProcessHelper {
public:
    void                                                start(Synchro *synchro);
    static std::vector<std::unique_ptr<ProcessHandler>> handlers_;
    static void                                         numProcesses(int numProcesses);
    static int                                          numProcesses();

private:
    static int numProcesses_;
    void       createChild();
};

#endif // PROCESS_HANDLER_H
