#include "stdafx.h"

#include "MaterialManager.h"
#include <iostream>
#include <fstream>

namespace Managers
{
    namespace Material
    {
        MaterialManager::MaterialManager(Texture::TextureManager& textureManager) :
            m_TextureManager(textureManager)
        {
        }

        bool MaterialManager::Create(const std::string& name, const std::vector<Graphics::Resource::Texture*>& material)
        {
            if (IsLoaded(name))
            {
                return false;
            }
            using namespace Graphics::Resource;
            std::cout << "Creating material " + name << std::endl;
            Graphics::Resource::Material *m = new Graphics::Resource::Material(name);

            for (int i = 0; i <= AO; ++i)
            {
                m->SetTexture(material.at(i), static_cast<MaterialType>(i));
            }

            Insert(name, m);
            return true;
        }

        bool MaterialManager::Load(const std::string& path)
        {
            using namespace Graphics::Resource;

            std::filebuf fb;
            fb.open(path.c_str(), std::ios::in);
            if (fb.is_open())
            {
                std::istream is(&fb);
                auto materialNamesInFile = Graphics::Resource::Material::GetFileMaterialNames(is);
                for (unsigned int i = 0; i < materialNamesInFile.size(); ++i)
                {
                    if (IsLoaded(materialNamesInFile.at(i)))
                    {
                        break;
                    }
                    std::cout << "Creating material " + materialNamesInFile.at(i) << std::endl;
                    Graphics::Resource::Material* newMaterial = new Graphics::Resource::Material("", path);
                    newMaterial->LoadMaterialTexturesNames(i, is);

                    for (auto newTextureIt : newMaterial->GetTextureNames())
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
}