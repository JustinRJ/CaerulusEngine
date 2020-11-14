#include "stdafx.h"

#include "TextureManager.h"

namespace Managers
{
    void TextureManager::Load(const std::string& name, const std::string& path)
    {
        if (IsLoaded(name))
        {
            Log::LogInDebug("Texture " + name + " already loaded with path: " + path);
        }
        else
        {
            Log::LogMessage("Loading texture " + name + " with path: " + path);
            std::shared_ptr<Texture> texture = std::make_shared<Texture>(path);

            if (texture->IsLoaded())
            {
                Insert(name, texture);
            }
            else
            {
                Log::LogInDebug("Failed to load " + name + " texture with path: " + path);
            }
        }
    }
}