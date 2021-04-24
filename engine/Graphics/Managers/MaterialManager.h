#pragma once

#include "TextureManager.h"
#include "Graphics/Surface/Material.h"

namespace Graphics
{
    namespace Managers
    {
        class CAERULUS_GRAPHICS MaterialManager : public Core::Template::Manager<Surface::Material>
        {
        public:
            MaterialManager(TextureManager& textureManager);
            ~MaterialManager() = default;

            void Create(const std::string& name, const std::vector<std::shared_ptr<Surface::Texture>>& textures);

            void Load(const std::string& path);

        private:
            TextureManager& m_textureManager;
        };
    }
}