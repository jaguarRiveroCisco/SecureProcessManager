
#include <unistd.h>
#include "process.h"

#include "simul_process.h"
#include "process_handler.h"
#include "process_helper.h"

extern std::atomic<bool> g_display;
namespace process
{
    std::vector<std::unique_ptr<Controller>> Controller::handlers_;
    int                                      Controller::numProcesses_ = 4; // Default number of processes
    std::string                              Controller::processType_  = "simul"; // Default process type
    bool                                     Controller::running_      = true;

    Synchro *Controller::synchro()
    {
        static Synchro instance;
        return &instance;
    }

    void Controller::numProcesses(int numProcesses) { numProcesses_ = numProcesses; }

    int Controller::numProcesses() { return numProcesses_; }

    std::string Controller::receiveCreationMessage() { return Communicator::getInstance().receiveCreationMessage(); }

    void Controller::init(Synchro *synchro, std::unique_ptr<IProcess> process)
    {
        if (synchro == nullptr)
        {
            throw std::runtime_error("Synchro object is null");
        }
        synchro_ = synchro;
        process_ = std::move(process);
        createChild();
    }

    void Controller::start() { createCheckProcessThread(); }

    void Controller::createChild()
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
            // std::cout << "Parent process created child process with PID: " << pid_ << std::endl;
        }
    }

    void Controller::setProcessType(const std::string &processType) { processType_ = processType; }

    void Controller::createHandlers(int numProcesses)
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

    void Controller::run(const std::string &processType, int numProcesses)
    {
        setProcessType(processType);
        createHandlers(numProcesses);
        waitForEvents();
    }

    void Controller::createHandler()
    {
        auto handler = std::make_unique<Controller>();
        if (processType_ == "real")
        {
            handler->init(synchro(), std::make_unique<Process>());
        }
        else if (processType_ == "simul")
        {
            handler->init(synchro(), std::make_unique<ProcessSimulator>());
        }

        std::string messageText =
                handler->receiveCreationMessage() + " Number of processes: " + std::to_string(numProcesses_);
        if (g_display)
            std::cout << messageText << std::endl;
        handler->start();
        handlers_.push_back(std::move(handler));
    }

    void Controller::waitForEvents()
    {
        while (process::Controller::running())
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
                            [pid](const std::unique_ptr<Controller> &handler) { return handler->getPid() == pid; });
                    handlers_.erase(it, handlers_.end());
                    if (process::Controller::running())
                    {
                        if(g_display)
                            std::cout << "\trespawn!  ";
                        createHandler();
                    }
                }
            }
        }
    }

    void Controller::terminateAll()
    {
        for (auto &handler: handlers_)
        {
            handler->terminateProcess();
        }
    }
    
    void Controller::terminateProcessByPid(pid_t pid)
    {
        auto it =
                std::find_if(handlers_.begin(), handlers_.end(), [pid](const std::unique_ptr<Controller> &handler) {
                    return handler->getPid() == pid;
                });
        if (it != handlers_.end())
        {
            (*it)->terminateProcess();
        }
        else
        {
            std::cerr << "Process with PID: " << pid << " not found." << std::endl;
        }
    }
    
    void Controller::displayAllPids()
    {
        std::cout << "Current PIDs:" << std::endl;
        for (const auto &handler: handlers_)
        {
            std::cout << handler->getPid() << std::endl;
        }
        std::cout << "Total number of processes: " << handlers_.size() << std::endl;
    }
    
    void Controller::killAll()
    {
        for (auto &handler: handlers_)
        {
            handler->killProcess();
        }
    }
    
    void Controller::killProcessByPid(pid_t pid)
    {
        auto it =
                std::find_if(handlers_.begin(), handlers_.end(), [pid](const std::unique_ptr<Controller> &handler) {
                    return handler->getPid() == pid;
                });
        if (it != handlers_.end())
        {
            (*it)->killProcess();
        }
        else
        {
            std::cerr << "Process with PID: " << pid << " not found." << std::endl;
        }
    }

    bool &Controller::running() { return running_; }

} // namespace process
