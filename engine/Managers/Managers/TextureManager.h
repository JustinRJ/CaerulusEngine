#pragma once

#include "../Manager.h"
#include "../../Graphics/Resource/Texture.h"

namespace Managers
{
    using namespace Graphics::Resource;
    class CAERULUS_MANAGERS TextureManager : public Manager<Texture>
    {
    public:
        TextureManager();
        virtual ~TextureManager() = default;

        bool Load(const std::string& name, const std::string& path);

        bool LoadHDR(const std::string& name, const std::string& path);
    };
}