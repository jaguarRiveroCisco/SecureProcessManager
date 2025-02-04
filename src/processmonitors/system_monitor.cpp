#include "system_monitor.h"
#include "logger_instance.h"

namespace process
{
    void SystemMonitor::launchChildProcess() 
    { 
        process_->work();
        this->pid_ = process_->getPid();
    }
}