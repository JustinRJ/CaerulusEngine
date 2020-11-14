#pragma once

#include "Managers/Manager.h"
#include "Graphics/Surface/Texture.h"

namespace
{
    using namespace Graphics::Surface;
}

namespace Managers
{
    class CAERULUS_MANAGERS TextureManager : public Manager<Texture>
    {
    public:
        TextureManager() = default;
        virtual ~TextureManager() = default;

        void Load(const std::string& name, const std::string& path);
    };
}