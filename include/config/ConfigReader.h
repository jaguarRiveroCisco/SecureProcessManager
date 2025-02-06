//
// Created by Jaguar Rivero on 2025-02-05.
//

#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <unordered_map>
#include <string>

namespace config
{
    class ConfigReader
    {
    public:
        explicit ConfigReader(const std::string& filePath);
        const std::string& getValue(const std::string& key) const;
        void printMap() const;

    private:
        void parseConfigFile(const std::string& filePath);

        std::unordered_map<std::string, std::string> configMap;
    };
}

#endif //CONFIGREADER_H
