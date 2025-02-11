#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <string>
#include <filesystem>

namespace filesystem
{
    struct fs
    {
        static void ensureDirectoryExists(const std::string &dirName)
        {
            // Check if the directory exists
            if (!std::filesystem::exists(dirName))
            {
                // Create the directory
                if (!std::filesystem::create_directory(dirName))
                {
                    throw std::runtime_error("Could not create directory: " + dirName);
                }
            }
        }
    };
}

#endif // FILE_SYSTEM_H