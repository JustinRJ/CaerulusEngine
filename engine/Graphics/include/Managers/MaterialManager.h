#pragma once

#include "TextureManager.h"
#include "Surface/Material.h"

namespace Graphics
{
    namespace Managers
    {
        class CAERULUS_GRAPHICS MaterialManager : public Core::Template::Manager<Surface::Material>
        {
        public:
            MaterialManager(ShaderManager& shaderManager, TextureManager& textureManager);

            void Create(const std::string& materialName, const std::vector<std::string>& textureNames);

            void Load(const std::string& materialPath);

            void SetMaterialTexture(const std::string& materialName, const std::string& textureName, Surface::TextureType type);
            void AddMaterialUniformFunctor(const std::string& materialName, const std::string& shaderName, std::function<void(const Pipeline::Shader& shader)> uniformFunctor);

            ShaderManager& GetShaderManager();
            TextureManager& GetTextureManager();

        private:
            ShaderManager& m_shaderManager;
            TextureManager& m_textureManager;
        };
    }
}