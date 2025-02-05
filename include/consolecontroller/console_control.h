#ifndef PROCESS_CONTROL_H
#define PROCESS_CONTROL_H
#include <string>
#include "console_logger.h"
namespace cli::driver
{
    void parseArguments(int argc, char *argv[], int &numProcesses, std::string &processType, tools::ConsoleLogger& cl);

} // namespace cli::driver

#endif // PROCESS_CONTROL_H
