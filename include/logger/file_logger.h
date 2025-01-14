#ifndef FILE_LOGGER_H
#define FILE_LOGGER_H

#include <fstream>
#include "logger.h"

namespace tools
{
    class FileLogger final : public Logger 
    {
        friend struct LogOpt;

    public:
        ~FileLogger() override;
   

    protected:
        void outputLog(const std::string &message) override;

    private:
        std::ofstream outputFile;
        static void ensureLogsDirectoryExists();
        FileLogger();
        static FileLogger &getInstance()
        {
            static FileLogger instance;
            return instance;
        }
    };
}
#endif // FILE_LOGGER_H