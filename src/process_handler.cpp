
#include <unistd.h>
#include "synchro.h"
#include "process_handler.h"

int ProcessHandler::numProcesses_ = 4; // Default number of processes
std::vector<std::unique_ptr<ProcessHandler>> ProcessHandler::handlers_;

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

void ProcessHandler::createHandlers(int numProcesses, const std::string &processType)
{
    numProcesses_ = numProcesses;

    std::cout << "Creating " << numProcesses_ << " child processes of type " << processType << ".\n";

    for (int i = 0; i < numProcesses_; ++i)
    {
        try
        {
            auto handler = std::make_unique<ProcessHandler>();
            if (processType == "real")
            {
                handler->init(synchro(), std::make_unique<Process>());
            }
            else if (processType == "simul")
            {
                handler->init(synchro(), std::make_unique<SimulProcess>());
            }

            std::string messageText = handler->receiveCreationMessage();
            std::cout << messageText << std::endl;
            handler->start();
            handlers_.push_back(std::move(handler));
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error creating process handler: " << e.what() << std::endl;
        }
    }
}
