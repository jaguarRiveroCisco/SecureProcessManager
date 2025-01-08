#ifndef SIMUL_PROCESS_H
#define SIMUL_PROCESS_H
#include "process_helper.h"
class SimulProcess : public ProcessHelper
{
    public:
        virtual void work();
        virtual ~SimulProcess() = default;

    protected:
        void setSleepDuration();
        int  sleepDuration_ = 0;
};

#endif // SIMUL_PROCESS_H
