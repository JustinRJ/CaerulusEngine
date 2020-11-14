#pragma once

#include "Managers/Manager.h"
#include "TextureManager.h"
#include "Graphics/Surface/Material.h"

namespace
{
    using namespace Graphics::Surface;
}

namespace Managers
{
    class CAERULUS_MANAGERS MaterialManager : public Manager<Material>
    {
    public:
        MaterialManager(TextureManager& textureManager);
        virtual ~MaterialManager() = default;

        void Create(const std::string& name, const std::vector<std::shared_ptr<Texture>>& textures);

        void Load(const std::string& path);

    private:
        TextureManager& m_textureManager;
    };
}