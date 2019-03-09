#include "stdafx.h"
#include "ShaderSourceManager.h"

namespace Managers
{
    namespace Shader
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
            Manager::Insert(name, ss);
            return true;
        }

        ShaderSource* ShaderSourceManager::Get(const std::string& name) const
        {
            return Manager::Get(name);
        }

        std::vector<ShaderSource*> ShaderSourceManager::GetAll(const std::vector<std::string>& names) const
        {
            return Manager::GetAll(names);
        }

        bool ShaderSourceManager::IsLoaded(const std::string& name) const
        {
            return Manager::IsLoaded(name);
        }

        bool ShaderSourceManager::Remove(const std::string& name)
        {
            return Manager::Remove(name);
        }
    }
}