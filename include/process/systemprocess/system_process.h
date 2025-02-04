#pragma once

#ifndef SYSTEM_PROCESS_H
#define SYSTEM_PROCESS_H
#include "process_simulator.h"
#include <spawn.h>
#include <condition_variable>

namespace process
{

    class SystemProcess : public ProcessSimulator 
    {
    public:
        ~SystemProcess() override = default;
        void work() override;
    protected:
        void postWork() override;
        pid_t getPid() const override;

    private:
        mutable std::mutex pidMutex_;
        mutable std::condition_variable pidCondition_;

        struct Arguments
        {
            std::vector<char *> args;
            Arguments() :
                args{strdup("/Users/jrivero/dev/programs/testprogs/lenghty/lengthy_process"), strdup("5"), nullptr}
            {
            }
            ~Arguments()
            {
                for (char *arg: args)
                {
                    free(arg);
                }
            }
        };
        struct SpawnChild 
        {
            std::vector<char *>        environ = {nullptr};
            posix_spawn_file_actions_t actions{};
            posix_spawnattr_t          attrs{};
            SystemProcess             *parent_ {nullptr};
            SpawnChild(SystemProcess *parent, const std::vector<char *> &args);
            ~SpawnChild();
        };
    };
}

#endif // SYSTEM_PROCESS_H