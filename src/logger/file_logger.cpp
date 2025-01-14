#include "file_logger.h"
#include <filesystem> // C++17 filesystem
#include <unistd.h>

namespace tools
{
    FileLogger::FileLogger() : Logger()
    {
        // Ensure the 'logs' directory exists
        ensureLogsDirectoryExists();

        // Create the log file path
        auto name = "logs/" + std::to_string(getpid()) + "_.log";

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
    void FileLogger::ensureLogsDirectoryExists()
    {
        std::filesystem::path logDir("logs");

        // Check if the directory exists
        if (!std::filesystem::exists(logDir))
        {
            // Create the directory
            if (!std::filesystem::create_directory(logDir))
            {
                throw std::runtime_error("Could not create logs directory.");
            }
        }
    }
}