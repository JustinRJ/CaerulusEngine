#pragma once

#include "Core/Template/Manager.h"
#include "Graphics/Surface/Texture.h"

namespace Graphics
{
    namespace Managers
    {
        class CAERULUS_GRAPHICS TextureManager : public Core::Template::Manager<Surface::Texture>
        {
        public:
            TextureManager() = default;
            ~TextureManager() = default;

            void Load(const std::string& name, const std::string& path);
        };
    }
}