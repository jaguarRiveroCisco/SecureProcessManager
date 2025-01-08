#include <iostream>
#include <algorithm>
#include "process_handler.h"
#include "process.h"
#include "simul_process.h"

auto main(int argc, char *argv[]) -> int
{
    int         numProcesses = 4;
    std::string processType = "simul";

    auto parseArguments = [&](int argc, char *argv[]) 
    {
        if (argc >= 2)
        {
            numProcesses = ::atoi(argv[1]);
        }
        if (argc >= 3)
        {
            processType  = argv[2];
        }
        if (numProcesses <= 0)
        {
            std::cerr << "Number of processes must be greater than 0. Defaulting to 4.\n";
            numProcesses = 4;
        }

        if (processType != "real" && processType != "simul")
        {
            std::cerr << "Invalid process type: " << processType << ". Defaulting to 'simul'.\n";
            processType = "simul";
        }
    };

    parseArguments(argc, argv);

    ProcessHandler::numProcesses(numProcesses);

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

                // Find and remove the handler with the matching PID
                auto it = std::remove_if(ProcessHandler::handlers_.begin(), ProcessHandler::handlers_.end(),
                                         [pid](const std::unique_ptr<ProcessHandler>& handler) {
                                             return handler->getPid() == pid;
                                         });
                ProcessHandler::handlers_.erase(it, ProcessHandler::handlers_.end());
            }
        }
    }

    return 0;
}