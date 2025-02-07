#include "system_process.h"
#include "logger_instance.h"
#include "communicator.h"
#include "process_status.h"
#include "process_controller.h"
#include "string_tools.h"
#include "nap_time.h"
#include <filesystem>
#include <sys/fcntl.h>

namespace process
{
    std::vector<std::string> Arguments::args;
    std::string Arguments::fileNameWithoutExt_;

    void SystemProcess::work()
    {
        Arguments::populate();
        std::thread workerThread([&]() {
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

    auto timeout = std::chrono::milliseconds(tools::NapTimeMs::MEDIUM); // Adjust the duration as needed
    pid_t SystemProcess::getPid() const
    {
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
            //parent_->preWork(parent_->pid_);

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

        // Open the file for writing
        int fd;
        if ((fd = open("output.log", O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
            throw std::runtime_error("Failed to open output file");
        }

        // Redirect stdout and stderr to the file
        posix_spawn_file_actions_adddup2(&actions, fd, STDOUT_FILENO);
        posix_spawn_file_actions_adddup2(&actions, fd, STDERR_FILENO);

        std::vector<char*> c_args = tools::string::createCStyleArgs(args);
        executeCommand(c_args);

        // Close the file descriptor
        close(fd);
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
