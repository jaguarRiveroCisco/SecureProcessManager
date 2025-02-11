#include "file_logger.h"
#include <unistd.h>
#include "file_system.h"

namespace tools
{
    // Initialize the static member
    std::unique_ptr<FileLogger> FileLogger::instance = nullptr;

    FileLogger::FileLogger() : Logger()
    {
        // Ensure the 'logs' directory exists
        filesystem::fs::ensureDirectoryExists("logs");

        // Create the log file path
        auto name = "logs/" + std::to_string(getpid()) + ".log";

        // Open the log file
        outputFile.open(name, std::ios::app);
        if (!outputFile.is_open())
        {
            throw std::runtime_error("Could not open log file: " + name);
        }
    }

    FileLogger::~FileLogger()
    {
        if (outputFile.is_open())
        {
            outputFile.close();
        }
    }

    void FileLogger::outputLog(const std::string &message)
    {
        if (outputFile.is_open())
        {
            outputFile << message << std::endl;
        }
    }

    FileLogger &FileLogger::getInstance()
    {
        if (!instance)
        {
            instance = std::make_unique<FileLogger>();
        }
        return *instance;
    }

    // Reset the singleton instance
    void FileLogger::resetInstance()
    {
        instance.reset(); // Automatically deletes the instance and sets to nullptr
    }
}