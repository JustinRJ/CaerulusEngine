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

            void Create(const std::string& name, const std::vector<std::string>& textureNames);

            void Load(const std::string& path);

            TextureManager& GetTextureManager();

        private:
            TextureManager& m_textureManager;
        };
    }
}