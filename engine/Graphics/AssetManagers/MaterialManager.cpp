#include "stdafx.h"

#include "AssetManagers/MaterialManager.h"

#include <fstream>

using namespace Core::Logging;
using namespace Graphics::Surface;

namespace Graphics
{
    namespace AssetManagers
    {
        MaterialManager::MaterialManager(TextureManager& textureManager) :
            m_textureManager(textureManager)
        {}

        void MaterialManager::Create(const std::string& name, const std::vector<std::string>& textureNames)
        {
            if (Get(name))
            {
                LogInDebug("Material with name " + name + " already loaded or created");
            }
            else
            {
                LogMessage("Creating material " + name);
                std::shared_ptr<Material> material = std::make_unique<Material>(m_textureManager, "");

                for (unsigned int i = 0; i < textureNames.size(); ++i)
                {
                    material->SetTexture(textureNames[i], static_cast<TextureType>(i));
                }

                Insert(name, material);
            }
        }

        void MaterialManager::Load(const std::string& path)
        {
            std::filebuf fb;
            fb.open(path.c_str(), std::ios::in);
            if (fb.is_open())
            {
                std::istream is(&fb);
                std::vector<std::string> materialNamesInFile = Material::GetMaterialNamesFromFile(is);
                for (unsigned int i = 0; i < materialNamesInFile.size(); ++i)
                {
                    const std::string name(materialNamesInFile[i]);

                    if (Get(name))
                    {
                        LogInDebug("Material with name " + name + " already loaded or created");
                    }
                    else
                    {
                        LogMessage("Loading material " + name + " with path: " + path);

                        std::shared_ptr<Material> newMaterial = std::make_unique<Material>(m_textureManager, path);

                        std::vector<std::string> textures;
                        for (const std::string& textureName : Material::GetTextureNamesFromFile(is, static_cast<TextureType>(i)))
                        {
                            if (textureName != "")
                            {
                                // general path before material name
                                std::string generalPath = path;
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

                        Insert(name, newMaterial);
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