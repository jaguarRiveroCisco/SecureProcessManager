#ifndef PROCESS_HANDLER_H
#define PROCESS_HANDLER_H

class ProcessHandler final : protected ProcessBase, protected SimulProcess, public ProcessHelper 
{
public:
    void start(Synchro *synchro)
    {
        synchro_ = synchro;
        createChild();
    }
    static std::vector<std::unique_ptr<ProcessHandler>> handlers_;
    static void                                         numProcesses(int numProcesses) { numProcesses_ = numProcesses; }
    static int                                          numProcesses() { return numProcesses_; }

private:
    static int numProcesses_;

    void createChild()
    {
        pid_ = fork();
        if (pid_ == 0)
        {
            setSleepDuration();
            sendMessage(sleepDuration_);
            work();
        }
        else if (pid_ < 0)
        {
            // Fork failed
            perror("fork");
        }
        else
        {
            createCheckProcessThread();
        }
    }
};

#endif // PROCESS_HANDLER_H
