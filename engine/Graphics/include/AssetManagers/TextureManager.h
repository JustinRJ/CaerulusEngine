#pragma once

#include "ECS/AssetManager.h"
#include "Surface/Texture.h"

namespace Graphics
{
    namespace Managers
    {
        class CAERULUS_GRAPHICS TextureManager : public Core::ECS::AssetManager<Surface::Texture>
        {
        public:
            TextureManager() = default;

            void Load(const std::string& textureName, const std::string& path);
        };
    }
}