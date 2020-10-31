#pragma once

#include "Managers/Manager.h"
#include "TextureManager.h"
#include "Graphics/Resource/Material.h"

namespace
{
    using namespace Graphics::Resource;
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