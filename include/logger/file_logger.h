#ifndef FILE_LOGGER_H
#define FILE_LOGGER_H

#include <fstream>
#include <memory>
#include "logger.h"

namespace tools
{
    class FileLogger final : public Logger 
    {
        friend LogOpt;

    public:
        ~FileLogger() override;
        FileLogger();

    protected:
        void outputLog(const std::string &message) override;

    private:
        std::ofstream outputFile;
        static std::unique_ptr<FileLogger> instance;
        static void ensureLogsDirectoryExists();
        static FileLogger &getInstance()
        {
            if (!instance)
            {
                instance = std::make_unique<FileLogger>();
            }
            return *instance;
        }

        // Reset the singleton instance
        static void resetInstance()
        {
            instance.reset(); // Automatically deletes the instance and sets to nullptr
        }
    };
}
#endif // FILE_LOGGER_H