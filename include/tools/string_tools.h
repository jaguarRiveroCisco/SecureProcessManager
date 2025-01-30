#pragma once

#ifndef STRING_TOOLS_H
#define STRING_TOOLS_H
#include <string>
namespace tools::string
{
    std::vector<std::string> splitString(const std::string &input, char delimiter = ' ');
}

#endif // STRING_TOOLS_H