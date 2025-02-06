#pragma once

#ifndef SYSTEM_PROCESS_H
#define SYSTEM_PROCESS_H
#include "process_simulator.h"
#include <spawn.h>
#include <condition_variable>

namespace process
{

    struct Arguments
    {
        std::vector<std::string> args;
        Arguments();
        ~Arguments() = default;
    };
    class SystemProcess final : public ProcessSimulator
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


        struct SpawnChild 
        {
            std::vector<char *>        environ = {nullptr};
            posix_spawn_file_actions_t actions{};
            posix_spawnattr_t          attrs{};
            SystemProcess             *parent_ {nullptr};
            SpawnChild(SystemProcess *parent, const std::vector<std::string> &args);
            ~SpawnChild();
        };
    };
}

#endif // SYSTEM_PROCESS_H