#include "string_tools.h"
#include <sstream>
#include <vector>
#include "logger_instance.h"

namespace tools::string
{
    std::vector<std::string> splitString(const std::string &input, char delimiter)
    {
        std::vector<std::string> tokens;
        std::istringstream       stream(input);
        std::string              token;

        while (std::getline(stream, token, delimiter))
        {
            tokens.push_back(token);
        }

        return tokens;
    }

    pid_t strToPid(const std::string &pidStr)
    {
        pid_t pid;
        try
        {
            pid = std::stoi(pidStr);
        }
        catch (const std::invalid_argument &e)
        {
            tools::LoggerManager::getInstance().logError("[PARENT PROCESS] | Invalid PID in message: " + pidStr);
            return -1;
        }
        catch (const std::out_of_range &e)
        {
            tools::LoggerManager::getInstance().logError("[PARENT PROCESS] | PID out of range in message: " + pidStr);
            return -1;
        }
        return pid;
    }
}
