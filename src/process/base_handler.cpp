#include "base_handler.h"
#include <chrono>
#include <csignal>
#include <thread>
#include <unistd.h> // Include this header for _exit
#include "console_logger.h"
extern std::atomic<bool> g_display;

namespace process
{

    void BaseHandler::displayProcessStatus(int &status)
    {
        // Child finished
        if (!WIFEXITED(status))
        {
            if (WIFSIGNALED(status))
            {
                tools::ConsoleLogger::getInstance() << "Child process " << pid_ << " was terminated by signal " << WTERMSIG(status) << ".";
                tools::ConsoleLogger::getInstance().flush(tools::LogLevel::INFO);
            }
            else
            {
                tools::ConsoleLogger::getInstance() << "Child process " << pid_ << " exited with status " << status << ".";
                tools::ConsoleLogger::getInstance().flush(tools::LogLevel::WARNING);
            }
        }
    }

    pid_t BaseHandler::getPid() const { return pid_; }

    bool BaseHandler::isProcessRunning()
    {
        if (kill(pid_, 0) == -1 && errno == ESRCH)
        {
            tools::ConsoleLogger::getInstance() << "Process " << pid_ << " is not running.";
            tools::ConsoleLogger::getInstance().flush(tools::LogLevel::ERROR);
            return false;
        }
        return true;
    }

    void BaseHandler::terminateProcess() { sendSignal(SIGTERM); }

    void BaseHandler::killProcess() { sendSignal(SIGKILL); }

    void BaseHandler::sendSignal(int signal)
    {
        if (kill(pid_, signal) == -1)
        {
            perror("kill");
        }
    }

    void BaseHandler::createCheckProcessThread()
    {
        // Parent process
        // Create a thread to check the state of the child process
        try
        {
            std::thread checkThread(&BaseHandler::checkProcessState, this);
            checkThread.detach();
        }
        catch (const std::system_error &e)
        {
            tools::ConsoleLogger::getInstance() << "Thread creation failed: " << e.what();
            tools::ConsoleLogger::getInstance().flush(tools::LogLevel::ERROR);
            _exit(EXIT_FAILURE); // Ensure the child process exits
        }
    }

    void BaseHandler::checkProcessState()
    {
        int status = -1;
        while (true)
        {
            // Check if the process with PID = pid_ is running
            if (!isProcessRunning())
                break;

            pid_t result = waitpid(pid_, &status, WNOHANG);
            if (result == 0)
            {
                // Child still running
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
} // namespace process
