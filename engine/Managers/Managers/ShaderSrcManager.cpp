#include "stdafx.h"

#include "ShaderSrcManager.h"

namespace Managers
{
    bool ShaderSrcManager::Load(const std::string& path, ShaderType type)
    {
        using namespace Core::Logging;
        if (IsLoaded(path))
        {
            Log::LogInDebug("\tShader stage already loaded with path " + path);
            return false;
        }

        Log::LogMessage("\tLoading shader stage with path " + path);
        auto ss = std::make_shared<ShaderSrc>(type, path);
        ss->Load();
        Insert(path, ss);
        return true;
    }
}