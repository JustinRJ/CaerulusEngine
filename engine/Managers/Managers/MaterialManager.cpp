#include "stdafx.h"

#include "MaterialManager.h"

namespace Managers
{
    MaterialManager::MaterialManager(TextureManager& textureManager) :
        m_textureManager(textureManager)
    {
    }

    void MaterialManager::Create(const std::string& name, const std::vector<std::shared_ptr<Texture>>& textures)
    {
        using namespace Core::Logging;
        if (IsLoaded(name))
        {
            Log::LogInDebug("Material with name " + name + " already loaded or created");
            return;
        }

        Log::LogMessage("Creating material " + name);
        std::shared_ptr<Material> material = std::make_shared<Material>(name);
        material->SetTextures(textures);
        Insert(name, material);
    }

    void MaterialManager::Load(const std::string& path)
    {
        using namespace Core::Logging;

        std::filebuf fb;
        fb.open(path.c_str(), std::ios::in);
        if (fb.is_open())
        {
            std::istream is(&fb);
            std::vector<std::string> materialNamesInFile = Material::GetFileMaterialNames(is);
            for (unsigned int i = 0; i < materialNamesInFile.size(); ++i)
            {
                if (IsLoaded(materialNamesInFile[i]))
                {
                    Log::LogInDebug("Material with name " + materialNamesInFile[i] + " already loaded or created");
                    break;
                }

                Log::LogMessage("Loading material " + materialNamesInFile[i] + " with path: " + path);
                std::shared_ptr<Material> newMaterial = std::make_shared<Material>("", path);
                newMaterial->LoadMaterialTexturesNames(i, is);

                std::vector<std::shared_ptr<Texture>> textures;
                for (const std::string& materialName : newMaterial->GetTextureNames())
                {
                    if (materialName != "")
                    {
                        // general path before material name
                        std::string generalPath = path;
                        generalPath.erase(generalPath.rfind("/"));
                        std::string textureName(materialName);
                        textureName.erase(textureName.rfind("."));

                        m_textureManager.Load(textureName, std::string(generalPath + "/" + materialName));
                        textures.push_back(m_textureManager.Get(textureName));
                    }
                }
                newMaterial->SetTextures(textures);
                Insert(newMaterial->GetName(), newMaterial);
            }
            fb.close();
        }
    }
}