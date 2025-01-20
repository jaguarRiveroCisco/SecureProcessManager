#ifndef FILE_LOGGER_H
#define FILE_LOGGER_H

#include <fstream>
#include <memory>
#include "logger.h"

namespace tools
{
    class FileLogger final : public Logger 
    {
        friend LoggerManager;

    public:
        ~FileLogger() override;
        FileLogger();

    protected:
        void outputLog(const std::string &message) override;

    private:
        std::ofstream outputFile;
        static std::unique_ptr<FileLogger> instance;
        static void ensureLogsDirectoryExists();
        static FileLogger &getInstance();

        // Reset the singleton instance
        static void resetInstance();
    };
}
#endif // FILE_LOGGER_H