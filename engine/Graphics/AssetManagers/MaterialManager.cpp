#include "stdafx.h"

#include "AssetManagers/MaterialManager.h"

#include <fstream>

using namespace Core::Log;
using namespace Graphics::Surface;

namespace Graphics
{
    namespace AssetManagers
    {
        MaterialManager::MaterialManager(TextureManager& textureManager) :
            m_textureManager(textureManager)
        {}

        void MaterialManager::Create(std::string_view name, const std::vector<std::string>& textureNames)
        {
            if (Get(name.data()))
            {
                LogInDebug("Material with name " + std::string(name) + " already loaded or created");
            }
            else
            {
                LogMessage("Creating material " + std::string(name));
                std::unique_ptr<Material> material = std::make_unique<Material>(m_textureManager, "");

                for (uint32_t i = 0; i < textureNames.size(); ++i)
                {
                    material->SetTexture(textureNames[i], static_cast<TextureType>(i));
                }

                Insert(name.data(), std::move(material));
            }
        }

        void MaterialManager::Load(std::string_view path)
        {
            std::filebuf fb;
            fb.open(path.data(), std::ios::in);
            if (fb.is_open())
            {
                std::istream is(&fb);
                std::vector<std::string> materialNamesInFile = Material::GetMaterialNamesFromFile(is);
                for (uint32_t i = 0; i < materialNamesInFile.size(); ++i)
                {
                    const std::string name(materialNamesInFile[i]);

                    if (Get(name))
                    {
                        LogInDebug("Material with name " + name + " already loaded or created");
                    }
                    else
                    {
                        LogMessage("Loading material " + name + " with path: " + std::string(path));

                        std::unique_ptr<Material> newMaterial = std::make_unique<Material>(m_textureManager, path);

                        std::vector<std::string> textures;
                        for (std::string_view textureName : Material::GetTextureNamesFromFile(is, static_cast<TextureType>(i)))
                        {
                            if (textureName != "")
                            {
                                // general path before material name
                                std::string generalPath = path.data();
                                generalPath.erase(generalPath.rfind("/"));
                                std::string newTextureName(textureName);
                                newTextureName.erase(newTextureName.rfind("."));

                                m_textureManager.Load(newTextureName, std::string(generalPath + "/" + textureName.data()));
                                textures.push_back(newTextureName);
                            }
                        }

                        for (uint32_t j = 0; j < textures.size(); ++j)
                        {
                            newMaterial->SetTexture(textures[j], static_cast<TextureType>(j));
                        }

                        Insert(name, std::move(newMaterial));
                    }
                }
                fb.close();
            }
        }

        TextureManager& MaterialManager::GetTextureManager()
        {
            return m_textureManager;
        }
    }
}