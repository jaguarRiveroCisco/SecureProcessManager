#include <iostream>
#include "process_handler.h"

auto main(int argc, char *argv[]) -> int
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
            handler->init(&synchro);
            std::string messageText = handler->creationMessage();
            std::cout << messageText << std::endl;
            handler->start();
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
