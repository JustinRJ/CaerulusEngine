#include "stdafx.h"
#include "ShaderSourceManager.h"

namespace Managers
{
    ShaderSourceManager::ShaderSourceManager()
    {}

    bool ShaderSourceManager::Load(const std::string& path, ShaderType type)
    {
        using namespace Core::Logging;
        if (IsLoaded(path))
        {
            Log::LogInDebug("\tShader stage already loaded with path " + path);
            return false;
        }
        Log::LogMessage("\tLoading shader stage with path " + path);
        auto ss = std::make_shared<ShaderSource>(type, path);
        ss->Load();
        Insert(path, ss);
        return true;
    }
}