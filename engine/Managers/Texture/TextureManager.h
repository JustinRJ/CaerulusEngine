#pragma once

#include "../Manager.h"
#include "../../Graphics/Resource/Texture.h"

namespace Managers
{
    namespace Texture
    {
        class TextureManager : Manager<Graphics::Resource::Texture>
        {
        public:
            TextureManager();

            virtual Graphics::Resource::Texture* Get(const std::string& name) const;

            virtual std::vector<Graphics::Resource::Texture*> GetAll(const std::vector<std::string>& names) const;

            virtual bool IsLoaded(const std::string& name) const;

            virtual bool Remove(const std::string& name);

            bool Load(const std::string& name, const std::string& path);

            bool LoadHDR(const std::string& name, const std::string& path);
        };
    }
}