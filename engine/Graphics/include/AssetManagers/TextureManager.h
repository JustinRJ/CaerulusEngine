#pragma once

#include "Template/AssetManager.h"
#include "Surface/Texture.h"

namespace Graphics
{
    namespace Managers
    {
        class CAERULUS_GRAPHICS TextureManager : public Core::Template::AssetManager<Surface::Texture>
        {
        public:
            TextureManager() = default;

            void Load(const std::string& textureName, const std::string& path);
        };
    }
}