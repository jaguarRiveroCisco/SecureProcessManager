#include "system_process.h"
#include <cstring>
#include <spawn.h>
#include "logger_instance.h"
#include "random_stuff.h"
namespace process
{
    struct SpawnChild
    {
        std::vector<char *>        environ = {nullptr};
        posix_spawn_file_actions_t actions;
        posix_spawnattr_t          attrs;

        SpawnChild(pid_t *pidP, const std::vector<char *> &args)
        {
            posix_spawn_file_actions_init(&actions);
            posix_spawnattr_init(&attrs);
            tools::LoggerManager::getInstance().logInfo("[SYSTEM PROCESS] SpawnChild");

            int status =
                    posix_spawn(pidP, args[0], &actions, &attrs, const_cast<char *const *>(args.data()), &environ[0]);
            if (status != 0)
            {
                tools::LoggerManager::getInstance().logError("[SYSTEM PROCESS] Failed to execute command");
            }
        }

        ~SpawnChild() 
        {
            posix_spawn_file_actions_destroy(&actions);
            posix_spawnattr_destroy(&attrs);
        }
    };

    bool SystemProcess::spawnChildProcess(const std::vector<char *> &args)
    {
        SpawnChild spawnChild(pidP_, args);
        return *pidP_ > 0;
    }

    struct Arguments
    {
        std::vector<char *> args;

        Arguments() : args{strdup("/bin/ls"), strdup("-l"), nullptr} {}

        ~Arguments()
        {
            for (char *arg : args)
            {
                free(arg);
            }
        }
    };

    void SystemProcess::work()
    {
        preWork();
        timeManager_.setSleepDuration();
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

        postWork();
    }
} // namespace process
