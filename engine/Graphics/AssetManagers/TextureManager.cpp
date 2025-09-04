#include "stdafx.h"

#include "AssetManagers/TextureManager.h"

#include "File/FileUtility.h"

using namespace Core::File;
using namespace Core::Log;
using namespace Graphics::Surface;

namespace Graphics
{
    namespace AssetManagers
    {
        void TextureManager::Load(std::string_view name, std::string_view path)
        {
            if (Get(name.data()))
            {
                LogInDebug("Texture " + std::string(name) + " already loaded with path: " + std::string(path));
            }
            else
            {
                LogMessage("Loading texture " + std::string(name) + " with path: " + std::string(path));
                std::unique_ptr<Texture> texture = std::make_unique<Texture>(path, Core::File::GetFileExtension(path) == "hdr");

                if (texture->IsLoaded())
                {
                    Insert(name.data(), std::move(texture));
                }
                else
                {
                    LogInDebug("Failed to load " + std::string(name) + " texture with path: " + std::string(path));
                }
            }
        }
    }
}