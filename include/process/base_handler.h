#ifndef PROCESS_BASE_H
#define PROCESS_BASE_H
#include "synchro.h"
namespace process
{
    class BaseHandler 
    {
    public:
        pid_t getPid() const;
        std::atomic<bool>  monitoring();
        void               monitoring(bool value);
        void               createMonitorProcessThread();

    protected:
        BaseHandler() = default;
        virtual ~BaseHandler() = default;
        void          displayProcessStatus(int &status);
        bool          isProcessRunning();
        void          terminateProcess();
        void          killProcess();
        void          intProcess();
        void          sendSignal(int signal);
        void          monitorProcessThread();
        pid_t         pid_{0};
    private:
        class Monitor 
        {
        public:
            bool get() const
            {
                std::lock_guard<std::mutex> lock(monitoring_mutex_);
                return monitoring_;
            }

            void set(bool value)
            {
                std::lock_guard<std::mutex> lock(monitoring_mutex_);
                monitoring_ = value;
            }

        private:
            std::atomic<bool>  monitoring_{false};
            mutable std::mutex monitoring_mutex_;
        };

        Monitor monitor_;
    };

} // namespace process

#endif // PROCESS_BASE_H
