#ifndef PROCESS_BASE_H
#define PROCESS_BASE_H
#include "synchro.h"

namespace process
{
    class ProcessBase 
    {
    protected:
        ProcessBase();
        virtual ~ProcessBase();
        void          displayProcessStatus(int &status);
        bool          isProcessRunning() const;
        void          terminateProcess();
        void          killProcess();
        void          sendSignal(int signal);
        void          createCheckProcessThread();
        void          checkProcessState();
        pid_t         pid_{0};
        Synchro      *synchro_{nullptr};
        static size_t processCounter_;

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> startTime_;
    };
} // namespace process

#endif // PROCESS_BASE_H
