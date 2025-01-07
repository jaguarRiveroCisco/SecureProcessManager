#include <condition_variable>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <signal.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <utility>
#include "messenger.h"
#include "synchro.h"
#include "process_base.h"
#include "simul_process.h"
#include "process_helper.h"
#include "process_handler.h"


Messenger ProcessHelper::messenger_;
int ProcessHandler::numProcesses_ = 4; // Default number of processes
std::vector<std::unique_ptr<ProcessHandler>> ProcessHandler::handlers_;

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        ProcessHandler::numProcesses(::atoi(argv[1]));
    }

    if (ProcessHandler::numProcesses() <= 0)
    {
        std::cerr << "Number of processes must be greater than 0.\n";
        return 1;
    }

    std::cout << "Creating " << ProcessHandler::numProcesses() << " child processes.\n";
    Synchro synchro;

    for (int i = 0; i < ProcessHandler::numProcesses(); ++i)
    {
        try 
        {
            auto handler = std::make_unique<ProcessHandler>();
            handler->start(&synchro);
            std::string messageText = handler->receiveMessage();
            std::cout << messageText << std::endl;
            ProcessHandler::handlers_.push_back(std::move(handler));
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error creating process handler: " << e.what() << std::endl;
        }
    }

    // Main thread waits for events
    std::atomic<int> processedEvents = 0;
    while (processedEvents < ProcessHandler::numProcesses())
    {
        std::unique_lock<std::mutex> lock(synchro.mtx);
        synchro.cv.wait(lock, [&] { return !synchro.eventQueue.empty(); });

        // Process all events
        while (!synchro.eventQueue.empty())
        {
            pid_t pid = synchro.eventQueue.front();
            std::cout << "Event processed for PID: " << pid << std::endl;
            synchro.eventQueue.pop();
            ++processedEvents;
        }
    }

    return 0;
}
