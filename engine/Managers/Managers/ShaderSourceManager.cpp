#include "stdafx.h"
#include "ShaderSourceManager.h"

namespace Managers
{
    ShaderSourceManager::ShaderSourceManager()
    {}

    bool ShaderSourceManager::Load(const std::string& name, const std::string& path, ShaderType type, const std::vector<std::string>& uniformLocations)
    {
        using namespace Core::Logging;
        if (IsLoaded(name))
        {
            Log::LogInDebug("Shader stage with name " + name + " already loaded with path " + path);
            return false;
        }
        Log::LogMessage("Loading shader stage " + name + " with path " + path);
        auto ss = std::make_shared<ShaderSource>(type, path);
        ss->Load();
        Insert(name, ss);
        return true;
    }
}