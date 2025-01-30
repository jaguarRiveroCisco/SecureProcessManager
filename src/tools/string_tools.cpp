#include "string_tools.h"
#include <vector>
#include <sstream>

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
}
