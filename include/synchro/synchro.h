#ifndef SYNCHRO_H
#define SYNCHRO_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include "base_process.h"

namespace concurrency
{
    class Synchro final {
    public:
        void            pushPid(pid_t) noexcept;
        pid_t           removeFrontPidQueue() noexcept;
        bool            isPidQueueEmpty() const noexcept;
        void            pauseMonitoring(bool) noexcept;
        bool            pauseMonitoring() const noexcept;
        static Synchro &getInstance();

    private:
        Synchro()                                                  = default;
        Synchro(const Synchro &)                                   = delete;
        Synchro                        &operator=(const Synchro &) = delete;
        mutable std::mutex              mtx_;
        std::queue<pid_t>               pidQueue_;
        std::atomic<bool>               pauseMonitoring_;
    };
}

#endif // SYNCHRO_H
