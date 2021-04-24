#include "stdafx.h"

#include "TextureManager.h"
#include "Core/File/FileUtility.h"

namespace
{
    using namespace Core::File;
    using namespace Core::Logging;
    using namespace Graphics::Surface;
}

namespace Graphics
{
    namespace Managers
    {
        void TextureManager::Load(const std::string& name, const std::string& path)
        {
            if (IsLoaded(name))
            {
                LogInDebug("Texture " + name + " already loaded with path: " + path);
            }
            else
            {
                LogMessage("Loading texture " + name + " with path: " + path);
                std::shared_ptr<Texture> texture = std::make_shared<Texture>(path, Core::File::GetFileExtension(path) == "hdr");

                if (texture->IsLoaded())
                {
                    Insert(name, texture);
                }
                else
                {
                    LogInDebug("Failed to load " + name + " texture with path: " + path);
                }
            }
        }
    }
}