#include "stdafx.h"

#include "AssetManagers/TextureManager.h"

#include "File/FileUtility.h"

using namespace Core::File;
using namespace Core::Logging;
using namespace Graphics::Surface;

namespace Graphics
{
    namespace Managers
    {
        void TextureManager::Load(const std::string& textureName, const std::string& path)
        {
            if (Get(textureName))
            {
                LogInDebug("Texture " + textureName + " already loaded with path: " + path);
            }
            else
            {
                LogMessage("Loading texture " + textureName + " with path: " + path);
                std::unique_ptr<Texture> texture = std::make_unique<Texture>(path, Core::File::GetFileExtension(path) == "hdr");

                if (texture->IsLoaded())
                {
                    Insert(textureName, std::move(texture));
                }
                else
                {
                    LogInDebug("Failed to load " + textureName + " texture with path: " + path);
                }
            }
        }
    }
}