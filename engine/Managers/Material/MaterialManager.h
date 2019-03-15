#pragma once

#define CAERULUS_MANAGERS __declspec(dllexport)

#include "../Manager.h"
#include "../Texture/TextureManager.h"
#include "../../Graphics/Resource/Material.h"

namespace Managers
{
    namespace Material
    {
        class MaterialManager : Manager<Graphics::Resource::Material>
        {
        public:
            CAERULUS_MANAGERS MaterialManager(Texture::TextureManager& textureManager);
            CAERULUS_MANAGERS virtual ~MaterialManager() {}

            CAERULUS_MANAGERS virtual Graphics::Resource::Material* Get(const std::string& name) const;

            CAERULUS_MANAGERS virtual std::vector<Graphics::Resource::Material*> GetAll(const std::vector<std::string>& names) const;

            CAERULUS_MANAGERS virtual bool IsLoaded(const std::string& name) const;

            CAERULUS_MANAGERS virtual bool Remove(const std::string& name);

            CAERULUS_MANAGERS bool Load(const std::string& path);

            CAERULUS_MANAGERS bool Create(const std::string& name, const std::vector<Graphics::Resource::Texture*>& textures);

            CAERULUS_MANAGERS bool SetTexture(Graphics::Resource::Texture* texture, Graphics::Resource::MaterialType materialType, const std::string& name);

        private:
            Texture::TextureManager& m_TextureManager;
        };
    }
}