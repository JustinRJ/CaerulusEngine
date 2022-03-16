#include "stdafx.h"

#include "AssetManagers/MaterialManager.h"

#include <fstream>

using namespace Core::Logging;
using namespace Graphics::Surface;

namespace Graphics
{
    namespace Managers
    {
        MaterialManager::MaterialManager(ShaderManager& shaderManager, TextureManager& textureManager) :
            m_shaderManager(shaderManager),
            m_textureManager(textureManager)
        {}

        void MaterialManager::Create(const std::string& materialName, const std::vector<std::string>& textureNames)
        {
            if (IsLoaded(materialName))
            {
                LogInDebug("Material with name " + materialName + " already loaded or created");
            }
            else
            {
                LogMessage("Creating material " + materialName);
                std::unique_ptr<Material> material = std::make_unique<Material>(m_textureManager, "");

                for (unsigned int i = 0; i < textureNames.size(); ++i)
                {
                    material->SetTexture(textureNames[i], static_cast<TextureType>(i));
                }

                Insert(materialName, std::move(material));
            }
        }

        void MaterialManager::Load(const std::string& materialPath)
        {
            std::filebuf fb;
            fb.open(materialPath.c_str(), std::ios::in);
            if (fb.is_open())
            {
                std::istream is(&fb);
                std::vector<std::string> materialNamesInFile = Material::GetMaterialNamesFromFile(is);
                for (unsigned int i = 0; i < materialNamesInFile.size(); ++i)
                {
                    const std::string name(materialNamesInFile[i]);

                    if (IsLoaded(name))
                    {
                        LogInDebug("Material with name " + name + " already loaded or created");
                    }
                    else
                    {
                        LogMessage("Loading material " + name + " with path: " + materialPath);

                        std::unique_ptr<Material> newMaterial = std::make_unique<Material>(m_textureManager, materialPath);

                        std::vector<std::string> textures;
                        for (const std::string& textureName : Material::GetTextureNamesFromFile(is, static_cast<TextureType>(i)))
                        {
                            if (textureName != "")
                            {
                                // general path before material name
                                std::string generalPath = materialPath;
                                generalPath.erase(generalPath.rfind("/"));
                                std::string newTextureName(textureName);
                                newTextureName.erase(newTextureName.rfind("."));

                                m_textureManager.Load(newTextureName, std::string(generalPath + "/" + textureName));
                                textures.push_back(newTextureName);
                            }
                        }

                        for (unsigned int j = 0; j < textures.size(); ++j)
                        {
                            newMaterial->SetTexture(textures[j], static_cast<TextureType>(j));
                        }

                        Insert(name, std::move(newMaterial));
                    }
                }
                fb.close();
            }
        }

        ShaderManager& MaterialManager::GetShaderManager()
        {
            return m_shaderManager;
        }

        TextureManager& MaterialManager::GetTextureManager()
        {
            return m_textureManager;
        }

        void MaterialManager::SetMaterialTexture(const std::string& materialName, const std::string& textureName, Surface::TextureType type)
        {
            if (Material* material = GetMutable(materialName))
            {
                material->SetTexture(textureName, type);
            }
        }

        void MaterialManager::AddMaterialUniformCallback(const std::string& materialName, const Pipeline::Shader& shader, std::function<void(const Pipeline::ShaderUniformCallback&, const Pipeline::Shader& shader)> uniformCallback)
        {
            if (Material* material = GetMutable(materialName))
            {
                material->AddUniformCallback(shader, uniformCallback);
            }
        }
    }
}