#pragma once

#include "ECS/AssetManager.h"
#include "Surface/Texture.h"

namespace Graphics
{
    namespace AssetManagers
    {
        class CAERULUS_GRAPHICS TextureManager : public Core::ECS::AssetManager<Surface::Texture>
        {
        public:
            TextureManager() = default;

            void Load(std::string_view name, std::string_view path);
        };
    }
}