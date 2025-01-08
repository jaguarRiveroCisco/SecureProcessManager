#include <iostream>
#include "process_handler.h"
#include "process.h"
#include "simul_process.h"

auto main(int argc, char *argv[]) -> int
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <num_processes> <process_type>\n";
        std::cerr << "process_type: 'real' for Process, 'simul' for SimulProcess\n";
        return 1;
    }

    ProcessHandler::numProcesses(::atoi(argv[1]));

    if (ProcessHandler::numProcesses() <= 0)
    {
        std::cerr << "Number of processes must be greater than 0.\n";
        return 1;
    }

    std::string processType = argv[2];

    std::cout << "Creating " << ProcessHandler::numProcesses() << " child processes of type " << processType << ".\n";

    for (int i = 0; i < ProcessHandler::numProcesses(); ++i)
    {
        try 
        {
            auto handler = std::make_unique<ProcessHandler>();
            if (processType == "real")
            {
                handler->init(ProcessHandler::synchro(), std::make_unique<Process>());
            }
            else if (processType == "simul")
            {
                handler->init(ProcessHandler::synchro(), std::make_unique<SimulProcess>());
            }
            else
            {
                std::cerr << "Invalid process type: " << processType << ". Use 'real' or 'simul'.\n";
                return 1;
            }

            std::string messageText = handler->receiveCreationMessage();
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
        std::unique_lock<std::mutex> lock(ProcessHandler::synchro()->mtx);
        ProcessHandler::synchro()->cv.wait(lock, [&] { return !ProcessHandler::synchro()->eventQueue.empty(); });

        // Process all events
        while (!ProcessHandler::synchro()->eventQueue.empty())
        {
            pid_t pid = ProcessHandler::synchro()->getAndPopFront();
            if (pid != -1)
            {
                std::cout << "Event processed for PID: " << pid << std::endl;
                ++processedEvents;
            }
        }
    }

    return 0;
}