#include "system_process.h"
#include "logger_instance.h"
#include "communicator.h"
#include "process_status.h"
#include <thread>
#include "nap_time.h"
#include "process_controller.h"
namespace process
{
    void SystemProcess::work()
    {
        std::thread workerThread([&]() {
        Arguments arguments;
            SpawnChild spawnChild(this, arguments.args);
        });
        workerThread.detach();
    }

    void SystemProcess::postWork()
    {
        logLifetime();
        if (getppid() != 1)
            concurrency::Communicator::getInstance().sendTerminationMessage(timeManager_.getFormattedElapsedTimeStr(),
                                                                            pid_, exitCode_);
    }

    pid_t SystemProcess::getPid() const
    {
        std::unique_lock<std::mutex> lock(pidMutex_);
        pidCondition_.wait(lock, [this]() { return pid_ != 0; });
        return pid_;
    }


SystemProcess::SpawnChild::SpawnChild(SystemProcess *parent, const std::vector<std::string> &args) : parent_(parent)
    {
        posix_spawn_file_actions_init(&actions);
        posix_spawnattr_init(&attrs);

        // Create a vector of C-style strings (char*)
        std::vector<char*> c_args;
        for (const auto& arg : args)
        {
            if (!arg.empty())
            {
                c_args.push_back(const_cast<char*>(arg.c_str()));
            }
        }
        c_args.push_back(nullptr); // Null-terminate the array


        // Pass the C-style array to posix_spawn
        int status = posix_spawn(&parent_->pid_, c_args[0], &actions, &attrs, c_args.data(), &environ[0]);

        if (status != 0)
        {
            tools::LoggerManager::getInstance().logError("[SYSTEM PROCESS] Failed to execute command");
        }
        else
        {
            {
                std::lock_guard<std::mutex> lock(parent_->pidMutex_);
                parent_->pid_ = parent_->pid_; // Notify that pid_ is set
            }
            parent_->pidCondition_.notify_one();
            if (const pid_t result = waitpid(parent_->pid_, &status, 0); result != parent_->pid_)
            {
                tools::displayProcessStatus(status, parent_->pid_);
            }
            else
            {
                if (WIFEXITED(status))
                {
                    exitCode_ = WEXITSTATUS(status);
                }
                else if (WIFSIGNALED(status))
                {
                    exitCode_ = WTERMSIG(status);
                }
            }
            parent_->postWork();
        }
    }

    SystemProcess::SpawnChild::~SpawnChild()
    {
        posix_spawn_file_actions_destroy(&actions);
        posix_spawnattr_destroy(&attrs);
    }

    Arguments::Arguments()
    {
        const auto file = ProcessController::configReader().getValue("process_file");
        args.push_back(file);
        auto params = ProcessController::configReader().getConsecutiveParameters();
        for (const auto& param : params)
        {
            args.push_back(param);
        }
    }

} // namespace process
