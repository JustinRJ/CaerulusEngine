#pragma once

#include "TextureManager.h"
#include "ShaderManager.h"
#include "Surface/Material.h"

namespace Graphics
{
    namespace AssetManagers
    {
        class CAERULUS_GRAPHICS MaterialManager : public Core::ECS::AssetManager<Surface::Material>
        {
        public:
            MaterialManager(TextureManager& textureManager);

            void Create(std::string_view name, const std::vector<std::string>& textureNames);

            void Load(std::string_view path);

            TextureManager& GetTextureManager();

        private:
            TextureManager& m_textureManager;
        };
    }
}