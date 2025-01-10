#include "controller_base.h"

namespace process
{
    int         ControllerBase::numProcesses_ = 4; // Default number of processes
    std::string ControllerBase::processType_  = "simul"; // Default process type
    bool        ControllerBase::running_      = true;

    bool &ControllerBase::running() { return running_; }
}