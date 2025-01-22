#pragma once

#ifndef SYNCHRO_LOCKED_MEMBER_H
#define SYNCHRO_LOCKED_MEMBER_H

#include <mutex>
#include <atomic>

namespace concurrency
{
    template<typename T> 
    class LockedVariable 
    {
    public:
        LockedVariable() = default;
        explicit LockedVariable(const T &value) : value_(value) {}

        // Safely set the value
        void set(const T &value)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            value_ = value;
        }

        // Safely get the value
        T get() const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return value_;
        }

    private:
        mutable std::mutex mutex_;
        std::atomic<T>     value_;
    };
}

#endif // SYNCHRO_LOCKED_MEMBER_H