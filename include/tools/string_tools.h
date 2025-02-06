#pragma once

#ifndef STRING_TOOLS_H
#define STRING_TOOLS_H
#include <string>
namespace tools::string
{
    std::vector<std::string> splitString(const std::string &input, char delimiter = ' ');
    pid_t                    strToPid(const std::string &pidStr);
    std::vector<char*> createCStyleArgs(const std::vector<std::string>& args);
}

#endif // STRING_TOOLS_H