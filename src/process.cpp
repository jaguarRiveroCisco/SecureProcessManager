#include "process.h"
#include <iostream>
#include "process_helper.h"
namespace process
{
    void Process::work()
    {
        // Real process work implementation
        Communicator::getInstance().sendCreationMessage();
        std::cout << "Real process is working." << std::endl;
        // Add real process work code here
        _exit(0); // Ensure the child process exits immediately
    }
} // namespace process
