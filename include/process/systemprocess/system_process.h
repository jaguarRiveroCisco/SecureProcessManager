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
        static std::vector<std::string> args;

        static void populate();
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
            auto executeCommand(const std::vector<char *> &c_args) const -> void;
        };
    };
}

#endif // SYSTEM_PROCESS_H