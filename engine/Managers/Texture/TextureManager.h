#pragma once

#define CAERULUS_MANAGERS __declspec(dllexport)

#include "../Manager.h"
#include "../../Graphics/Resource/Texture.h"

namespace Managers
{
    namespace Texture
    {
        class TextureManager : Manager<Graphics::Resource::Texture>
        {
        public:
            CAERULUS_MANAGERS TextureManager();
            CAERULUS_MANAGERS virtual ~TextureManager() {}

            CAERULUS_MANAGERS virtual Graphics::Resource::Texture* Get(const std::string& name) const;

            CAERULUS_MANAGERS virtual std::vector<Graphics::Resource::Texture*> GetAll(const std::vector<std::string>& names) const;

            CAERULUS_MANAGERS virtual bool IsLoaded(const std::string& name) const;

            CAERULUS_MANAGERS virtual bool Remove(const std::string& name);

            CAERULUS_MANAGERS bool Load(const std::string& name, const std::string& path);

            CAERULUS_MANAGERS bool LoadHDR(const std::string& name, const std::string& path);
        };
    }
}