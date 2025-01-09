#include "process.h"
#include <iostream>

void Process::work()
{
    // Real process work implementation
    sendCreationMessage();
    std::cout << "Real process is working." << std::endl;
    // Add real process work code here
    _exit(0); // Ensure the child process exits immediately
}