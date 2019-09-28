#pragma once

#include "../Manager.h"
#include "../Texture/TextureManager.h"
#include "../../Graphics/Resource/Material.h"

namespace Managers
{
    namespace Material
    {
        class CAERULUS_MANAGERS MaterialManager : Manager<Graphics::Resource::Material>
        {
        public:
            MaterialManager(Texture::TextureManager& textureManager);
            virtual ~MaterialManager() {}

            virtual Graphics::Resource::Material* Get(const std::string& name) const;

            virtual std::vector<Graphics::Resource::Material*> GetAll(const std::vector<std::string>& names) const;

            virtual bool IsLoaded(const std::string& name) const;

            virtual bool Remove(const std::string& name);

            bool Load(const std::string& path);

            bool Create(const std::string& name, const std::vector<Graphics::Resource::Texture*>& textures);

            bool SetTexture(Graphics::Resource::Texture* texture, Graphics::Resource::MaterialType materialType, const std::string& name);

        private:
            Texture::TextureManager& m_TextureManager;
        };
    }
}