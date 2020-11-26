#pragma once

#include "Core/Template/Manager.h"
#include "Graphics/Surface/Texture.h"

namespace
{
    using namespace Graphics::Surface;
}

namespace Graphics
{
    namespace Managers
    {
        class CAERULUS_CORE TextureManager : public Core::Template::Manager<Texture>
        {
        public:
            TextureManager() = default;
            ~TextureManager() = default;

            void Load(const std::string& name, const std::string& path);
        };
    }
}