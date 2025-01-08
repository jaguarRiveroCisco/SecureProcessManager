#ifndef SIMUL_PROCESS_H
#define SIMUL_PROCESS_H

class SimulProcess 
{
protected:
    SimulProcess()              = default;
    virtual ~SimulProcess()     = default;
    int          sleepDuration_ = 0;
    void         setSleepDuration();
    virtual void work();
};

#endif // SIMUL_PROCESS_H
