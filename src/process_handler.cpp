
#include <unistd.h>
#include "synchro.h"
#include "process_handler.h"

std::vector<std::unique_ptr<ProcessHandler>> ProcessHandler::handlers_;
int ProcessHandler::numProcesses_ = 4; // Default number of processes
std::string ProcessHandler::processType_ = "simul"; // Default process type

extern std::atomic<bool> g_display;
extern std::atomic<bool> g_running;

Synchro *ProcessHandler::synchro()
{
    static Synchro instance;
    return &instance;
}

void ProcessHandler::init(Synchro *synchro, std::unique_ptr<ProcessInterface> process)
{
    if (synchro == nullptr)
    {
        throw std::runtime_error("Synchro object is null");
    }
    synchro_ = synchro;
    process_ = std::move(process);
    createChild();
}

std::string ProcessHandler::receiveCreationMessage() { return process_->receiveCreationMessage(); }

void ProcessHandler::start() { createCheckProcessThread(); }

void ProcessHandler::numProcesses(int numProcesses) { numProcesses_ = numProcesses; }

int ProcessHandler::numProcesses() { return numProcesses_; }

void ProcessHandler::createChild()
{
    pid_ = fork();
    if (pid_ == 0)
    {
        try
        {
            process_->work();
        }
        catch (const std::exception &e)
        {
            // Handle exceptions in child process
            std::cerr << "Exception in child process: " << e.what() << std::endl;
            _exit(EXIT_FAILURE); // Ensure child process exits
        }
    }
    else if (pid_ < 0)
    {
        // Fork failed
        perror("fork");
        throw std::runtime_error("Failed to fork process");
    }
    else
    {
        // Parent process
        //std::cout << "Parent process created child process with PID: " << pid_ << std::endl;
    }
}

pid_t ProcessHandler::getPid() const { return pid_; }

void ProcessHandler::setProcessType(const std::string &processType) { processType_ = processType; }

void ProcessHandler::createHandlers(int numProcesses)
{
    numProcesses_ = numProcesses;

    std::cout << "Creating " << numProcesses_ << " child processes of type " << processType_ << ".\n";

    for (int i = 0; i < numProcesses_; ++i)
    {
        try
        {
            createHandler();
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error creating process handler: " << e.what() << std::endl;
        }
    }
}

void ProcessHandler::createHandler()
{
    auto handler = std::make_unique<ProcessHandler>();
    if (processType_ == "real")
    {
        handler->init(synchro(), std::make_unique<Process>());
    }
    else if (processType_ == "simul")
    {
        handler->init(synchro(), std::make_unique<SimulProcess>());
    }

    std::string messageText = handler->receiveCreationMessage() + " Number of processes: " + std::to_string(numProcesses_);
    if (g_display)
        std::cout << messageText << std::endl;
    handler->start();
    handlers_.push_back(std::move(handler));
}

void ProcessHandler::waitForEvents()
{
    while (g_running)
    {
        std::unique_lock<std::mutex> lock(synchro()->mtx);
        synchro()->cv.wait(lock, [&] { return !synchro()->eventQueue.empty(); });

        // Process all events
        while (!synchro()->eventQueue.empty())
        {
            pid_t pid = synchro()->getAndPopFront();
            if (pid != -1)
            {
                // Find and remove the handler with the matching PID
                auto it = std::remove_if(
                        handlers_.begin(), handlers_.end(),
                        [pid](const std::unique_ptr<ProcessHandler> &handler) { return handler->getPid() == pid; });
                handlers_.erase(it, handlers_.end());
                if(g_running)   
                    createHandler();
            }
        }
    }
}

void ProcessHandler::terminateAll()
{
    for (auto &handler : handlers_)
    {
        handler->terminateProcess();
    }
}
void ProcessHandler::terminateProcessByPid(pid_t pid)
{
    auto it = std::find_if(handlers_.begin(), handlers_.end(),
                           [pid](const std::unique_ptr<ProcessHandler> &handler) { return handler->getPid() == pid; });
    if (it != handlers_.end())
    {
        (*it)->terminateProcess();
    }
    else
    {
        std::cerr << "Process with PID: " << pid << " not found." << std::endl;
    }
}
void ProcessHandler::displayAllPids()
{
    std::cout << "Current PIDs:" << std::endl;
    for (const auto &handler: handlers_)
    {
        std::cout << handler->getPid() << std::endl;
    }
    std::cout << "Total number of processes: " << handlers_.size() << std::endl;
}
