#include "system_process.h"
#include "logger_instance.h"
#include "communicator.h"
#include "process_status.h"
#include <thread>
#include "nap_time.h"
namespace process
{
    void SystemProcess::work()
    {
        std::thread workerThread([this]() {
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


    SystemProcess::SpawnChild::SpawnChild(SystemProcess *parent, const std::vector<char *> &args) : parent_(parent)
    {
        posix_spawn_file_actions_init(&actions);
        posix_spawnattr_init(&attrs);
        tools::LoggerManager::getInstance().logInfo("[SYSTEM PROCESS] SpawnChild. Parent process PID: " +
                                                    std::to_string(parent_->pid_));

        int status = posix_spawn(&parent_->pid_, args[0], &actions, &attrs, const_cast<char *const *>(args.data()),
                                 &environ[0]);
        if (status != 0)
        {
            tools::LoggerManager::getInstance().logError("[SYSTEM PROCESS] Failed to execute command");
        }
        else
        {
            parent_->preWork(parent_->pid_);
            if (const pid_t result = waitpid(parent_->pid_, &status, 0); result != parent_->pid_)
            {
                tools::displayProcessStatus(status, parent_->pid_);
            }
            parent_->postWork();
            std::this_thread::sleep_for(std::chrono::seconds(tools::NapTimeSec::MEDIUMS));

        }

    }

    SystemProcess::SpawnChild::~SpawnChild()
    {
        posix_spawn_file_actions_destroy(&actions);
        posix_spawnattr_destroy(&attrs);
    }
} // namespace process
