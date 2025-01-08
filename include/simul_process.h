#ifndef SIMUL_PROCESS_H
#define SIMUL_PROCESS_H

class SimulProcess 
{
    public:
        void         setSleepDuration();
        virtual void work();
        int sleepDuration() const { return sleepDuration_; }
        SimulProcess()          = default;
        virtual ~SimulProcess() = default;

    protected:
        int sleepDuration_      = 0;
};

#endif // SIMUL_PROCESS_H
