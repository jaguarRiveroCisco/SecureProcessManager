//
// Created by Jaguar Rivero on 2025-02-04.
//

#ifndef PROCESS_STATUS_H
#define PROCESS_STATUS_H
#include <sys/types.h>

namespace tools
{
    enum Status { STATUS_OK, STATUS_SIGNAL, STATUS_UNKNOWN };

    int displayProcessStatus(int, pid_t);
    bool isProcessRunning(pid_t);
    void sendSignal(int, pid_t);
}

#endif //PROCESS_STATUS_H
