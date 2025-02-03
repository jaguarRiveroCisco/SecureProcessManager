#pragma once

#ifndef SYNCHRO_H
#define SYNCHRO_H

#include <condition_variable>
#include <mutex>
#include <queue>

namespace concurrency
{
    class Synchro final {
    public:
        void            pauseMonitoring(bool) noexcept;
        bool            pauseMonitoring() const noexcept;
        static Synchro &getInstance();
        Synchro(const Synchro &)                                   = delete;
        Synchro                        &operator=(const Synchro &) = delete;

    private:
        Synchro()                                                  = default;
        mutable std::mutex              mtx_;
        std::atomic<bool>               pauseMonitoring_{};
    };
}

#endif // SYNCHRO_H
