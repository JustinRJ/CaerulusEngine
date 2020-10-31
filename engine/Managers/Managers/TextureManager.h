#pragma once

#include "Managers/Manager.h"
#include "Graphics/Resource/Texture.h"

namespace
{
    using namespace Graphics::Resource;
}

namespace Managers
{
    class CAERULUS_MANAGERS TextureManager : public Manager<Texture>
    {
    public:
        TextureManager() = default;
        virtual ~TextureManager() = default;

        void Load(const std::string& name, const std::string& path, bool HDR = false);
    };
}