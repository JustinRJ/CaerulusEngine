#include "stdafx.h"
#include "TextureManager.h"

namespace Managers
{
    TextureManager::TextureManager()
    {
    }

    bool TextureManager::Load(const std::string& name, const std::string& path)
    {
        using namespace Core::Logging;
        if (IsLoaded(name))
        {
            Log::LogInDebug("Texture " + name + " already loaded with path " + path);
            return false;
        }
        Log::LogMessage("Loading texture " + name + " with path " + path);
        auto t = std::make_shared<Texture>();
        t->Load(path.c_str(), true);
        Insert(name, t);
        return true;
    }

    bool TextureManager::LoadHDR(const std::string& name, const std::string& path)
    {
        using namespace Core::Logging;
        if (IsLoaded(name))
        {
            Log::LogInDebug("Texture " + name + " already loaded with path " + path);
            return false;
        }
        Log::LogMessage("Loading texture " + name + " with path " + path);
        auto t = std::make_shared<Texture>();
        t->LoadHDR(path.c_str(), true);
        Insert(name, t);
        return true;
    }
}