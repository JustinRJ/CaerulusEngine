#pragma once

#include "../Manager.h"
#include "../../Graphics/Resource/Texture.h"

namespace Managers
{
    namespace Texture
    {
        class CAERULUS_MANAGERS TextureManager : public Manager<Graphics::Resource::Texture>
        {
        public:
            TextureManager();
            virtual ~TextureManager() {}

            bool Load(const std::string& name, const std::string& path);

            bool LoadHDR(const std::string& name, const std::string& path);
        };
    }
}