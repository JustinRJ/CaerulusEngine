#include "stdafx.h"

#include "TextureManager.h"

namespace Managers
{
    TextureManager::TextureManager()
    {
    }

    bool TextureManager::Load(const std::string& name, const std::string& path, bool HDR)
    {
        using namespace Core::Logging;
        if (IsLoaded(name))
        {
            Log::LogInDebug("Texture " + name + " already loaded with path " + path);
            return false;
        }

        Log::LogMessage("Loading texture " + name + " with path " + path);
        auto t = std::make_shared<Texture>();
        if (HDR)
        {
            t->LoadHDR(path.c_str(), true);
        }
        else
        {
            t->Load(path.c_str(), true);
        }
        Insert(name, t);
        return true;
    }
}