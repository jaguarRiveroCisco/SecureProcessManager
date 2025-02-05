//
// Created by Jaguar Rivero on 2025-02-05.
//

#ifndef CONSOLE_LOOP_H
#define CONSOLE_LOOP_H
#include <string>
namespace cli::driver
{
    void                      consoleLoop(bool run = true);
    void printContext(int numProcesses = -1, const std::string &processType = "");
    void printCommands();
}

#endif //CONSOLE_LOOP_H
