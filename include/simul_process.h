#ifndef SIMUL_PROCESS_H
#define SIMUL_PROCESS_H
#include "process_helper.h"
#include "process_interface.h"
class SimulProcess : public ProcessInterface
{
    public:
        void work() override;
        virtual ~SimulProcess() = default;
        static void setRndUpper(int rndUpper) { rndUpper_ = rndUpper; }
    protected:
    private:
        void setSleepDuration();
        static int  rndUpper_;
        int  sleepDuration_ = 0;
};

#endif // SIMUL_PROCESS_H
