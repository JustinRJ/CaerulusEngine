#pragma once

#include "TextureManager.h"
#include "Graphics/Surface/Material.h"

namespace
{
    using namespace Graphics::Surface;
}

namespace Graphics
{
    namespace Managers
    {
        class CAERULUS_CORE MaterialManager : public Core::Template::Manager<Material>
        {
        public:
            MaterialManager(TextureManager& textureManager);
            ~MaterialManager() = default;

            void Create(const std::string& name, const std::vector<std::shared_ptr<Texture>>& textures);

            void Load(const std::string& path);

        private:
            TextureManager& m_textureManager;
        };
    }
}