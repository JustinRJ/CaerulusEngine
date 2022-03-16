#pragma once

#include "TextureManager.h"
#include "ShaderManager.h"
#include "Surface/Material.h"

namespace Graphics
{
    namespace Managers
    {
        class CAERULUS_GRAPHICS MaterialManager : public Core::Template::AssetManager<Surface::Material>
        {
        public:
            MaterialManager(ShaderManager& shaderManager, TextureManager& textureManager);

            void Create(const std::string& materialName, const std::vector<std::string>& textureNames);

            void Load(const std::string& materialPath);

            void SetMaterialTexture(const std::string& materialName, const std::string& textureName, Surface::TextureType type);
            void AddMaterialUniformCallback(const std::string& materialName, const Pipeline::Shader& shader, std::function<void(const Pipeline::ShaderUniformCallback&, const Pipeline::Shader& shader)> uniformCallback);

            ShaderManager& GetShaderManager();
            TextureManager& GetTextureManager();

        private:
            ShaderManager& m_shaderManager;
            TextureManager& m_textureManager;
        };
    }
}