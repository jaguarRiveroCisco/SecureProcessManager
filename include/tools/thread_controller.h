#pragma once

#ifndef CONTROLLER_COMMAND_CONTROLLER_H
#define CONTROLLER_COMMAND_CONTROLLER_H

#include <atomic>
#include <functional>
#include <thread>
#include <condition_variable>
#include <mutex>
namespace cli::driver
{
    class ThreadController 
    {
    public:
        virtual ~ThreadController() = default;

        void run(std::function<void(const std::string &)> commandFunc);
        void stop();
        virtual bool hasStopped();

    protected:
        virtual void runThread() = 0;

        std::atomic<bool>                        stopFlag{false};
        std::function<void(const std::string &)> commandFunc;

        std::thread readerThread;
        std::mutex mutex;
        std::condition_variable cv;
        bool stopped = false;
    };

} // namespace cli::driver

#endif // CONTROLLER_COMMAND_CONTROLLER_H
