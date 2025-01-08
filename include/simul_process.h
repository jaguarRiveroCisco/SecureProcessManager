#ifndef SIMUL_PROCESS_H
#define SIMUL_PROCESS_H
#include "process_helper.h"
#include "process_interface.h"
class SimulProcess : public ProcessInterface
{
    public:
        void work() override;
        virtual ~SimulProcess() = default;

    protected:
        void setSleepDuration();
        int  sleepDuration_ = 0;
};

#endif // SIMUL_PROCESS_H
