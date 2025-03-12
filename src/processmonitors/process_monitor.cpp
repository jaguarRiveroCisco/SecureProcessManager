#include "process_monitor.h"
#include <csignal>
#include <process_controller.h>
#include <thread>
#include <unistd.h> // Include this header for _exit
#include "logger_instance.h"
#include "nap_time.h"
#include "communicator.h"
#include "process_status.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h> // For clone
#include <sys/mman.h> // For mmap

namespace process
{

    pid_t ProcessMonitor::getPid() const
    {
        return pid_;
    }
    
    void  ProcessMonitor::terminateProcess() const { tools::sendSignal(SIGTERM, pid_); }

    void ProcessMonitor::killProcess() const { tools::sendSignal(SIGKILL, pid_); }

    void ProcessMonitor::intProcess() const { tools::sendSignal(SIGINT, pid_); }

    std::atomic<bool>& ProcessMonitor::monitoring() { return monitor_; }

    void ProcessMonitor::createMonitorProcessThread()
    {
        if( monitoring() )
        {
            tools::LoggerManager::getInstance().logWarning("[PARENT PROCESS] | Monitoring thread already running.");
            return;
        }
        // Parent process
        // Create a thread to check the state of the child process
        try
        {
            std::thread checkThread(&ProcessMonitor::monitorProcessThread, this);
            checkThread.detach();
        }
        catch (const std::system_error &e)
        {
            tools::LoggerManager::getInstance() << "[PARENT PROCESS] | Thread creation failed: " << e.what();
            tools::LoggerManager::getInstance().flush(tools::LogLevel::ERROR);
            exit(EXIT_FAILURE); // Ensure the child process exits
        }
    }

    void ProcessMonitor::monitorProcessThread()
    {
        int  status   = -1;
        monitoring() = true;
        bool monPrinted = false;
        while (ProcessController::running() && monitoring())
        {
            if (concurrency::Synchro::getInstance().pauseMonitoring())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::VERYSMALL));
                if (!monPrinted)
                {
                    monPrinted = true;
                    tools::LoggerManager::getInstance().logInfo("[MONITOR THREAD] | Not monitoring " + std::to_string(pid_));
                }
                continue;
            }
            monPrinted = false;

            // Check if the process with PID = pid_ is running
            if (!tools::isProcessRunning(pid_))
                break;

            pid_t result = waitpid(pid_, &status, WNOHANG);
            if (result == 0)
            {
                // Child still running
                std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::VERYSMALL));
            }
            else if (result == pid_)
            {
                if(auto res = tools::displayProcessStatus(status, pid_); res != tools::STATUS_OK)
                {
                    concurrency::Communicator::getInstance().sendTerminationMessage("", pid_, status);
                }
                break;
            }
            else
            {
                // Error occurred
                perror("waitpid");
                tools::LoggerManager::getInstance().logError("[MONITOR THREAD] | Error occurred while waiting for process " + std::to_string(pid_) + ".");
                break;
            }
        }
        monitoring() = false;
    }

    void ProcessMonitor::collectAndLaunch(std::unique_ptr<IProcess> process)
    {
        process_ = std::move(process);
        launchChildProcess();
    }

#ifdef __linux__
    struct ChildArgs
    {
        IProcess* process;
        pid_t real_pid;     // Real PID as seen by parent
        bool pid_set;       // Flag to sync parent/child
    };

    static int child_func(void* arg)
    {
        const auto* args = static_cast<ChildArgs*>(arg);
        IProcess* process = args->process;

        while (!args->pid_set)
        {
            usleep(100); // Wait for real_pid
        }
        pid_t real_pid = args->real_pid;

        // Log entry
        tools::LoggerManager::getInstance() << "[CHILD " << real_pid << "] Starting";
        tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);

        // Check parent before proceeding
        if (getppid() == 1)
        {
            tools::LoggerManager::getInstance() << "[CHILD " << real_pid << "] Parent gone, exiting early";
            tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);
            _exit(EXIT_FAILURE);
        }

        try
        {
            process->setPid(real_pid);
            process->work();
        }
        catch (const std::exception& e)
        {
            tools::LoggerManager::getInstance() << "[CHILD " << real_pid << "] Exception: " << e.what();
            tools::LoggerManager::getInstance().flush(tools::LogLevel::EXCEPTION);
            _exit(EXIT_FAILURE);
        }

        tools::LoggerManager::getInstance() << "[CHILD " << real_pid << "] Work done, exiting";
        tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);
        _exit(EXIT_SUCCESS);
        return 0;
    }

#endif

    constexpr size_t stack_size = 65536;

    void ProcessMonitor::launchChildProcess()
    {
        try
        {
            if (ProcessController::processType() == "clone")
            {
#ifdef __linux__
                stack_ = mmap(nullptr, stack_size, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
                if (stack_ == MAP_FAILED)
                {
                    perror("mmap");
                    throw std::runtime_error("[PARENT PROCESS] Failed to allocate stack");
                }

                // Allocate shared memory for args
                auto* args = static_cast<ChildArgs*>(
                    mmap(nullptr, sizeof(ChildArgs), PROT_READ | PROT_WRITE,
                         MAP_SHARED | MAP_ANONYMOUS, -1, 0)
                );
                if (args == MAP_FAILED)
                {
                    perror("mmap args");
                    munmap(stack_, stack_size);
                    throw std::runtime_error("[PARENT PROCESS] Failed to allocate args");
                }

                // Initialize args
                args->process = process_.get();
                args->real_pid = 0;
                args->pid_set = false;

                pid_ = clone(child_func, (char*)stack_+ stack_size,
                            CLONE_NEWPID | SIGCHLD, args);
                if (pid_ == -1)
                {
                    perror("clone");
                    munmap(stack_, stack_size);
                    munmap(args, sizeof(ChildArgs));
                    throw std::runtime_error("[PARENT PROCESS] Failed to clone process");
                }

                // Parent sets real PID and signals child
                args->real_pid = pid_;
                args->pid_set = true;

                args_ = args;   // Store for cleanup, add void* args_ to ProcessMonitor
#endif
            }
            else
            {
                pid_ = fork();
                if (pid_ == 0)
                {
                    process_->work();
                }
                else if (pid_ < 0)
                {
                    perror("fork");
                    throw std::runtime_error("[PARENT PROCESS] Failed to fork process");
                }
            }
        }
        catch (const std::exception &e)
        {
            // Handle exceptions in child process
            tools::LoggerManager::getInstance() << "[PARENT PROCESS] Exception in child process: " << e.what();
            tools::LoggerManager::getInstance().flush(tools::LogLevel::EXCEPTION);
            exit(EXIT_FAILURE); // Ensure child process exits
        }
    }

       // Cleanup in destructor
    ProcessMonitor::~ProcessMonitor()
    {
        if (stack_)
        {
            munmap(stack_, stack_size);
            stack_ = nullptr;
        }
        if (args_)
        {
            munmap(args_, sizeof(ChildArgs));
            args_ = nullptr;
        }
    }
} // namespace process
