#pragma once

#include <string>

namespace Core
{
    namespace File
    {
        inline std::string GetFileExtension(const std::string& path)
        {
            std::string extension;
            if (path.find_last_of(".") != std::string::npos)
            {
                extension = path.substr(path.find_last_of(".") + 1);
            }
            return extension;
        }
    }
}
