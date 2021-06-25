#include "stdafx.h"

#include "MaterialManager.h"

#include <fstream>

using namespace Core::Logging;
using namespace Graphics::Surface;

namespace Graphics
{
    namespace Managers
    {
        MaterialManager::MaterialManager(TextureManager& textureManager) :
            m_textureManager(textureManager)
        {}

        void MaterialManager::Create(const std::string& name, const std::vector<std::shared_ptr<Texture>>& textures)
        {
            if (IsLoaded(name))
            {
                LogInDebug("Material with name " + name + " already loaded or created");
            }
            else
            {
                LogMessage("Creating material " + name);
                std::shared_ptr<Material> material = std::make_shared<Material>("");

                for (unsigned int i = 0; i < textures.size(); ++i)
                {
                    material->SetTexture(textures[i], static_cast<TextureType>(i));
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

                    if (IsLoaded(name))
                    {
                        LogInDebug("Material with name " + name + " already loaded or created");
                    }
                    else
                    {
                        LogMessage("Loading material " + name + " with path: " + path);

                        std::shared_ptr<Material> newMaterial = std::make_shared<Material>(path);

                        std::vector<std::shared_ptr<Texture>> textures;
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
                                textures.push_back(m_textureManager.Get(newTextureName));
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
    }
}