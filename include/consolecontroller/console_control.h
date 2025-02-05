#ifndef PROCESS_CONTROL_H
#define PROCESS_CONTROL_H
#include <string>
namespace cli::driver
{
    void                      parseArguments(int argc, char *argv[], int &numProcesses, std::string &processType);

} // namespace cli::driver

#endif // PROCESS_CONTROL_H
