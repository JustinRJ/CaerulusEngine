#pragma once

#include "Template/Manager.h"
#include "Surface/Texture.h"

namespace Graphics
{
    namespace Managers
    {
        class CAERULUS_GRAPHICS TextureManager : public Core::Template::Manager<Surface::Texture>
        {
        public:
            TextureManager() = default;

            void Load(const std::string& textureName, const std::string& path);
        };
    }
}