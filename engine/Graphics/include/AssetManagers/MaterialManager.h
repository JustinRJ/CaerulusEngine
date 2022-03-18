#pragma once

#include "TextureManager.h"
#include "ShaderManager.h"
#include "Surface/Material.h"

namespace Graphics
{
    namespace AssetManagers
    {
        class CAERULUS_GRAPHICS MaterialManager : public Core::ECS::AssetManager<Surface::Material>
        {
        public:
            MaterialManager(TextureManager& textureManager);

            void Create(const std::string& materialName, const std::vector<std::string>& textureNames);

            void Load(const std::string& materialPath);

            void SetMaterialTexture(const std::string& materialName, const std::string& textureName, Surface::TextureType type);
            void AddMaterialUniformCallback(const std::string& materialName, Pipeline::Shader& shader, std::function<void(Pipeline::ShaderUniformCallback&, Pipeline::Shader& shader)> uniformCallback);

            TextureManager& GetTextureManager();

        private:
            TextureManager& m_textureManager;
        };
    }
}