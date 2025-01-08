#include <csignal>
#include <iostream>
#include <thread>
#include "process_base.h"
#include <chrono>


ProcessBase::ProcessBase() : startTime_(std::chrono::high_resolution_clock::now()) {}

ProcessBase::~ProcessBase()
{
    auto endTime  = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime_).count();
    std::cout << "Child process " << pid_ << " lifetime: " << duration << " milliseconds." << std::endl;
}

void ProcessBase::displayProcessStatus(int &status)
{
    // Child finished
    if (WIFEXITED(status))
    {
        //std::cout << "Child process " << pid_ << " exited normally with status " << WEXITSTATUS(status) << ".\n";
    }
    else if (WIFSIGNALED(status))
    {
        std::cout << "Child process " << pid_ << " was terminated by signal " << WTERMSIG(status) << ".\n";
    }
    else
    {
        std::cout << "Child process " << pid_ << " exited with status " << status << ".\n";
    }
}

bool ProcessBase::isProcessRunning() const
{
    if (kill(pid_, 0) == -1 && errno == ESRCH)
    {
        std::cerr << "Process " << pid_ << " is not running.\n";
        return false;
    }
    return true;
}

void ProcessBase::terminateProcess() { sendSignal(SIGTERM); }

void ProcessBase::killProcess() { sendSignal(SIGKILL); }

void ProcessBase::sendSignal(int signal)
{
    if (kill(pid_, signal) == -1)
    {
        perror("kill");
    }
}

void ProcessBase::createCheckProcessThread()
{
    // Parent process
    // Create a thread to check the state of the child process
    try
    {
        std::thread checkThread(&ProcessBase::checkProcessState, this);
        checkThread.detach();
    }
    catch (const std::system_error &e)
    {
        std::cerr << "Thread creation failed: " << e.what() << std::endl;
    }
}

void ProcessBase::checkProcessState()
{
    int status  = -1;
    while (true)
    {
        // Check if the process with PID = pid_ is running
        if (!isProcessRunning())
            break;

        pid_t result = waitpid(pid_, &status, WNOHANG);
        if (result == 0)
        {
            // Child still running
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        else if (result == pid_)
        {
            displayProcessStatus(status);
            // Child finished
            {
                std::lock_guard<std::mutex> lock(synchro_->mtx);
                synchro_->eventQueue.push(pid_);
            }
            synchro_->cv.notify_one();
            break;
        }
        else
        {
            // Error occurred
            perror("waitpid");
            break;
        }
    }
}
