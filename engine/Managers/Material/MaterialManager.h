#pragma once

#include "../Manager.h"
#include "../Texture/TextureManager.h"
#include "../../Graphics/Resource/Material.h"

namespace Managers
{
    namespace Material
    {
        class CAERULUS_MANAGERS MaterialManager : public Manager<Graphics::Resource::Material>
        {
        public:
            MaterialManager(Texture::TextureManager& textureManager);
            virtual ~MaterialManager() {}

            bool Create(const std::string& name, const std::vector<Graphics::Resource::Texture*>& textures);

            bool Load(const std::string& path);

        private:
            Texture::TextureManager& m_TextureManager;
        };
    }
}