//
// Created by Jaguar Rivero on 2025-02-05.
//

#include "ConfigReader.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>

namespace config
{
    ConfigReader::ConfigReader(const std::string& filePath)
    {
        parseConfigFile(filePath);
    }

    const std::string& ConfigReader::getValue(const std::string& key) const
    {
        auto it = configMap.find(key);
        if (it != configMap.end())
        {
            return it->second;
        }
        else
        {
            throw std::runtime_error("Key not found: " + key);
        }
    }

    void ConfigReader::parseConfigFile(const std::string& filePath)
    {
        std::ifstream file(filePath);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open configuration file: " + filePath);
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string        value;
            if (std::string key; std::getline(iss, value, '=') && std::getline(iss, key))
            {
                configMap[key] = value;
            }
        }

        file.close();
    }

    void ConfigReader::printMap() const
    {
        for (const auto& [key, value] : configMap)
        {
            std::cout << key << " = " << value << std::endl;
        }
    }
} // config