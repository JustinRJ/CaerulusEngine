#include "stdafx.h"
#include "ShaderSourceManager.h"

namespace Managers
{
    namespace PipeLine
    {
        ShaderSourceManager::ShaderSourceManager()
        {}

        bool ShaderSourceManager::Load(const std::string& name, const std::string& path, ShaderType type, const std::vector<std::string>& uniformLocations)
        {
            if (IsLoaded(name))
            {
                return false;
            }
            std::cout << "Loading shader stage " + name + " with path " + path << std::endl;
            ShaderSource* ss = new ShaderSource(type, path);
            ss->Load();
            Insert(name, ss);
            return true;
        }
    }
}