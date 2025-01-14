#ifndef PROCESS_CONTROL_H
#define PROCESS_CONTROL_H
#include <string>
namespace process::controller
{
    void main();
    void printHelp();
    void parseArguments(int argc, char *argv[], int &numProcesses, std::string &processType, int &rndUpper);
    template<typename T> void printpid(const std::string &str, const T &x = T());

    template<typename T> void printpidE(const std::string &str, const T &x = T());
    int LoggerExample();

} // namespace process::controller

#endif // PROCESS_CONTROL_H