#include "system_monitor.h"
#include <unistd.h>
#include "logger_instance.h"

namespace process
{
    void SystemMonitor::launchChildProcess() 
    { 
        process_->work(); 
    }
}