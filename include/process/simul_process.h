#ifndef SIMUL_PROCESS_H
#define SIMUL_PROCESS_H
#include "base_process.h"

namespace process
{
    class ProcessSimulator : public BaseProcess 
    {
    public:
        void work() override;
        virtual ~ProcessSimulator() = default;
        static void setRndUpper(int rndUpper);

    private:
        void       setSleepDuration();
        static int rndUpper_;
        int        sleepDuration_ = 0;
        int        msSleepDuration = 0;
    };
} // namespace process

#endif // SIMUL_PROCESS_H
