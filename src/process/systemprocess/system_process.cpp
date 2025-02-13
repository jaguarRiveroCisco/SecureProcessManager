#include "system_process.h"
#include "logger_instance.h"
#include "communicator.h"
#include "process_status.h"
#include "process_controller.h"
#include "string_tools.h"
#include "nap_time.h"
#include <filesystem>
#include <sys/fcntl.h>
#include "file_descriptor.h"
#include <thread>
#include <sstream>
#include "file_system.h"

namespace process
{
    std::vector<std::string> Arguments::args;
    std::string Arguments::fileNameWithoutExt_;

    void SystemProcess::work()
    {
        Arguments::populate();
        std::thread workerThread([&]()
        {
            SpawnChild spawnChild(this, Arguments::args);
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

    pid_t SystemProcess::getPid() const
    {
        constexpr auto timeout = std::chrono::milliseconds(tools::NapTimeMs::MEDIUM); // Adjust the duration as needed
        std::unique_lock<std::mutex> lock(pidMutex_);
        if (!pidCondition_.wait_for(lock, timeout, [this]() { return pid_ != 0; }))
        {
            tools::LoggerManager::getInstance().logError("[SYSTEM PROCESS] Timeout waiting for pid to be set");
            throw std::runtime_error("[SYSTEM PROCESS] Timeout waiting for pid to be set");
        }
        return pid_;
    }

    auto SystemProcess::SpawnChild::executeCommand(const std::vector<char*>& c_args) const -> void
    {
        int status = posix_spawn(&parent_->pid_, c_args[0], &actions, &attrs, c_args.data(), &environ[0]);

        if (status != 0)
        {
            if (status == ENOMEM) {
                tools::LoggerManager::getInstance().logError("[SYSTEM PROCESS] Insufficient memory to execute command");
            } else if (status == EINVAL) {
                tools::LoggerManager::getInstance().logError("[SYSTEM PROCESS] Invalid arguments provided to posix_spawn");
            } else {
                tools::LoggerManager::getInstance().logError("[SYSTEM PROCESS] Failed to execute command");
            }
        }
        else
        {
            {
                std::unique_lock<std::mutex> lock(parent_->pidMutex_);
                parent_->pidCondition_.notify_one();
            }
            if (const pid_t pid = waitpid(parent_->pid_, &status, 0); pid != parent_->pid_)
            {
                tools::displayProcessStatus(status, parent_->pid_); // process ended
            }
            else
            {
                if (WIFEXITED(status))
                {
                    exitCode_ = WEXITSTATUS(status);
                }
                else if (WIFSIGNALED(status))
                {
                    exitCode_ = WTERMSIG(status);
                }
            }
            parent_->postWork();
        }
    }

    SystemProcess::SpawnChild::SpawnChild(SystemProcess *parent, const std::vector<std::string> &args) : parent_(parent)
    {
        posix_spawn_file_actions_init(&actions);
        posix_spawnattr_init(&attrs);

        try
        {
            filesystem::fs::ensureDirectoryExists("logs");
            // Create a unique filename by appending the thread ID
            std::ostringstream uniqueFileName;
            uniqueFileName << "logs/" << Arguments::fileNameWithoutExt_ << "_" << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) << "_" << std::this_thread::get_id() << ".log";
            const filesystem::FileDescriptor fd(uniqueFileName.str());

            // Redirect stdout and stderr to the file
            posix_spawn_file_actions_adddup2(&actions, fd.get(), STDOUT_FILENO);
            posix_spawn_file_actions_adddup2(&actions, fd.get(), STDERR_FILENO);

            // Convert arguments to C-style and execute the command
            const std::vector<char*> c_args = tools::string::createCStyleArgs(args);
            executeCommand(c_args);
        }
        catch (const std::exception &e)
        {
            tools::LoggerManager::getInstance().logException("[SYSTEM PROCESS] Exception: " + std::string(e.what()));
            throw;
        }
    }


    SystemProcess::SpawnChild::~SpawnChild()
    {
        posix_spawn_file_actions_destroy(&actions);
        posix_spawnattr_destroy(&attrs);
    }

    void Arguments::populate()
    {
        if (args.empty())
        {
            const auto file = ProcessController::configReader().getValue("process_file");

            // Ensure the path is valid and points to a regular file
            std::filesystem::path path(file);
            if (!std::filesystem::exists(path)) {
                throw std::runtime_error("File does not exist: " + file);
            }

            if (!std::filesystem::is_regular_file(path)) {
                throw std::runtime_error("Path is not a regular file: " + file);
            }

            // Extract the file name without the extension
            fileNameWithoutExt_ = path.stem().string();

            args.push_back(file);
            auto params = ProcessController::configReader().getConsecutiveParameters();
            for (const auto& param : params)
            {
                args.push_back(param);
            }
        }
    }

} // namespace process
