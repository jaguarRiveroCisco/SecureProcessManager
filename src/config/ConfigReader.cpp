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

    bool isPathInRestrictedDirectory(const std::string& filePath) {
        // List of restricted directories
        static const std::vector<std::string> restrictedDirs = {
            "/usr/lib", "/etc", "/bin", "/sbin", "/lib", "/lib64",
            "/usr/bin", "/usr/sbin", "/var", "/boot", "/proc", "/sys",
            "/dev", "/run", "/tmp", "/mnt", "/media", "/srv", "/opt",
            "/home", "/root", "/lost+found", "/usr/local", "/usr/src",
            "/usr/include", "/usr/share"
        };

        return std::any_of(restrictedDirs.begin(), restrictedDirs.end(), [&](const std::string& dir) {
            return filePath.find(dir) == 0;
        });
    }

    void ConfigReader::parseConfigFile(const std::string& filePath)
    {
        if (isPathInRestrictedDirectory(filePath))
        {
            throw std::runtime_error("Access to system folder is not allowed: " + filePath);
        }

        std::ifstream file(filePath);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open configuration file: " + filePath);
        }

        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '#')
            {
                continue;
            }
            std::istringstream iss(line);
            std::string        key;
            if (std::string value; std::getline(iss, key, '=') && std::getline(iss, value))
            {
                configMap[key] = value;
            }
        }

        file.close();

        consecutiveParameters_.resize(configMap.size() * 2);
        for (const auto& [key, value] : configMap)
        {
            if (key != "process_file")
            {
                consecutiveParameters_.push_back("-" + key);
                consecutiveParameters_.push_back(value);
            }
        }
    }

    void ConfigReader::printMap() const
    {
        for (const auto& [key, value] : configMap)
        {
            std::cout << key << " = " << value << std::endl;
        }
    }
} // config