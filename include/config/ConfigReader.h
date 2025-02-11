//
// Created by Jaguar Rivero on 2025-02-05.
//

#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <unordered_map>
#include <string>
#include <vector>

namespace config
{
    class ConfigReader
    {
    public:
        explicit ConfigReader(const std::string& filePath);
        [[nodiscard]] const std::string& getValue(const std::string& key) const;
        void printMap() const;
        [[nodiscard]] std::vector<std::string> getConsecutiveParameters() const { return consecutiveParameters_; }

    private:
        void parseConfigFile(const std::string& filePath);

        std::unordered_map<std::string, std::string> configMap;
        std::vector<std::string> consecutiveParameters_;
    };
}

#endif //CONFIGREADER_H
