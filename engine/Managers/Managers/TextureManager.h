#pragma once

#include "Managers/Manager.h"
#include "Graphics/Resource/Texture.h"

namespace Managers
{
    using namespace Graphics::Resource;
    class CAERULUS_MANAGERS TextureManager : public Manager<Texture>
    {
    public:
        TextureManager() = default;
        virtual ~TextureManager() = default;

        bool Load(const std::string& name, const std::string& path, bool HDR = false);
    };
}