#include "system_process.h"
#include <cstring>
#include "logger_instance.h"
#include "random_stuff.h"
#include <unistd.h>
namespace process
{
    bool SystemProcess::spawnChildProcess(const std::vector<char *> &args)
    {
        SpawnChild spawnChild(this, args);
        return *pidP_ > 0;
    }

    void SystemProcess::work()
    {
        preWork(getpid());
        Arguments arguments;
        while (proceed())
        {
            if (!spawnChildProcess(arguments.args))
            {
                tools::LoggerManager::getInstance().logError("[SYSTEM PROCESS] Failed to spawn child process");
            }

            tools::sleepRandomMs(true);
            timeManager_.currentTime() = std::chrono::high_resolution_clock::now();
        }
        postWork(getpid());
    }

    SystemProcess::SpawnChild::SpawnChild(SystemProcess *parent, const std::vector<char *> &args) : parent_(parent)
    {
        posix_spawn_file_actions_init(&actions);
        posix_spawnattr_init(&attrs);
        tools::LoggerManager::getInstance().logInfo("[SYSTEM PROCESS] SpawnChild. Parent process PID: " +
                                                    std::to_string(*parent_->pidP_));

        int status = posix_spawn(parent_->pidP_, args[0], &actions, &attrs, const_cast<char *const *>(args.data()),
                                 &environ[0]);
        if (status != 0)
        {
            tools::LoggerManager::getInstance().logError("[SYSTEM PROCESS] Failed to execute command");
        }
    }

    SystemProcess::SpawnChild::~SpawnChild()
    {
        posix_spawn_file_actions_destroy(&actions);
        posix_spawnattr_destroy(&attrs);
    }
} // namespace process
