#ifndef FILE_LOGGER_H
#define FILE_LOGGER_H

#include <fstream>
#include "logger.h"

namespace tools
{
    class FileLogger : protected Logger 
    {
    public:
        explicit FileLogger(const std::string &filename = "");
        ~FileLogger() override;
        static FileLogger &getInstance()
        {
            static FileLogger instance;
            return instance;
        }

    protected:
        void outputLog(const std::string &message) override;

    private:
        std::ofstream outputFile;
        static void ensureLogsDirectoryExists();
    };
}
#endif // FILE_LOGGER_H