#pragma once

#include "Managers/Manager.h"
#include "TextureManager.h"
#include "Graphics/Resource/Material.h"

namespace Managers
{
    using namespace Graphics::Resource;
    class CAERULUS_MANAGERS MaterialManager : public Manager<Material>
    {
    public:
        MaterialManager(TextureManager& textureManager);
        virtual ~MaterialManager() = default;

        bool Create(const std::string& name, const std::vector<std::shared_ptr<Texture>>& textures);

        bool Load(const std::string& path);

    private:
        TextureManager& m_textureManager;
    };
}