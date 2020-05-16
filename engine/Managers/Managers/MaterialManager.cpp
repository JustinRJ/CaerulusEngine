#include "stdafx.h"

#include "MaterialManager.h"
#include <fstream>

namespace Managers
{
    MaterialManager::MaterialManager(TextureManager& textureManager) :
        m_TextureManager(textureManager)
    {
    }

    bool MaterialManager::Create(const std::string& name, const std::vector<std::shared_ptr<Texture>>& material)
    {
        using namespace Core::Logging;
        if (IsLoaded(name))
        {
            Log::LogInDebug("Material with name " + name + " already loaded or created");
            return false;
        }

        Log::LogMessage("Creating material " + name);
        auto m = std::make_shared<Material>(name);
        for (int i = 0; i < MaterialType::Size; ++i)
        {
            m->SetTexture(material.at(i), static_cast<MaterialType>(i));
        }

        Insert(name, m);
        return true;
    }

    bool MaterialManager::Load(const std::string& path)
    {
        using namespace Core::Logging;

        std::filebuf fb;
        fb.open(path.c_str(), std::ios::in);
        if (fb.is_open())
        {
            std::istream is(&fb);
            auto materialNamesInFile = Material::GetFileMaterialNames(is);
            for (unsigned int i = 0; i < materialNamesInFile.size(); ++i)
            {
                if (IsLoaded(materialNamesInFile.at(i)))
                {
                    Log::LogInDebug("Material with name " + materialNamesInFile.at(i) + " already loaded or created");
                    break;
                }

                Log::LogMessage("Loading material " + materialNamesInFile.at(i) + " with path " + path);
                auto newMaterial = std::make_shared<Material>("", path);
                newMaterial->LoadMaterialTexturesNames(i, is);

                for (auto& newTextureIt : newMaterial->GetTextureNames())
                {
                    if (newTextureIt.second != "")
                    {
                        // general path before material name
                        std::string generalPath = path;
                        generalPath.erase(generalPath.rfind("/"));
                        std::string textureName(newTextureIt.second);
                        textureName.erase(textureName.rfind("."));

                        m_TextureManager.Load(textureName, std::string(generalPath + "/" + newTextureIt.second));
                        newMaterial->SetTexture(m_TextureManager.Get(textureName), newTextureIt.first);
                    }
                }
                Insert(newMaterial->GetName(), newMaterial);
            }
            fb.close();
            return true;
        }
        return false;
    }
}