#include "stdafx.h"

#include "MaterialManager.h"
#include <iostream>

// define this in only *one* .cpp
#define TINYOBJLOADER_IMPLEMENTATION 
#include <tiny_obj_loader.h>

namespace Managers
{
    namespace Material
    {
        MaterialManager::MaterialManager(Texture::TextureManager& textureManager) :
            m_TextureManager(textureManager)
        {
        }

        bool MaterialManager::IsLoaded(const std::string& name) const
        {
            return Manager::IsLoaded(name);
        }

        bool MaterialManager::Create(const std::string& name, const std::vector<Graphics::Resource::Texture*>& material)
        {
            if (Manager::IsLoaded(name))
            {
                return false;
            }
            using namespace Graphics::Resource;
            std::cout << "Creating material " + name << std::endl;
            Graphics::Resource::Material *m = new Graphics::Resource::Material("");

            m->SetTexture(material.at(Albedo),       Albedo);
            m->SetTexture(material.at(Normal),       Normal);
            m->SetTexture(material.at(Roughness),    Roughness);
            m->SetTexture(material.at(Metallic),      Metallic);
            m->SetTexture(material.at(AO),           AO);

            Manager::Insert(name, m);
            return true;
        }

        Graphics::Resource::Material* MaterialManager::Get(const std::string& name) const
        {
            return Manager::Get(name);
        }

        std::vector<Graphics::Resource::Material*> MaterialManager::GetAll(const std::vector<std::string>& names) const
        {
            return Manager::GetAll(names);
        }

        bool MaterialManager::Remove(const std::string& name)
        {
            return Manager::Remove(name);
        }

        bool MaterialManager::SetTexture(Graphics::Resource::Texture* texture, Graphics::Resource::MaterialType materialType, const std::string& name)
        {
            if (Graphics::Resource::Material* material = Get(name))
            {
                material->SetTexture(texture, materialType);
                return true;
            }
            return false;
        }

        bool MaterialManager::Load(const std::string& path)
        {
            using namespace Graphics::Resource;

            std::filebuf fb;
            fb.open(path.c_str(), std::ios::in);

            if (!fb.is_open())
            {
                return false;
            }

            std::map<std::string, int> materialNames = std::map<std::string, int>();
            std::vector<tinyobj::material_t> materials = std::vector<tinyobj::material_t>();

            std::istream is(&fb);

            tinyobj::LoadMtl(&materialNames, &materials, &is);
            fb.close();

            //general path before material name
            std::string generalPath = path;
            generalPath.erase(generalPath.rfind("/"));

            for (unsigned int i = 0; i < materials.size(); i++)
            {
                Graphics::Resource::Material* newMaterial = new Graphics::Resource::Material(path);

                if (!IsLoaded(materials.at(i).name))
                {
                    std::cout << "Loading material " + materials.at(i).name + " with path " + path << std::endl;
                }
                else
                {
                    std::cerr << "Material " + materials.at(i).name + " already found!" << std::endl;
                    return false;
                }

                ///TODO change this to the correct mapping
                ///Change current .mtl file to follow the correct standard!
                std::string diffuseTexName = materials.at(i).diffuse_texname;
                diffuseTexName.erase(diffuseTexName.rfind("."));
                m_TextureManager.Load(diffuseTexName, generalPath + "/" + materials.at(i).diffuse_texname);
                newMaterial->SetTexture(m_TextureManager.Get(diffuseTexName), Albedo);

                std::string normalTexName = materials.at(i).bump_texname;
                normalTexName.erase(normalTexName.rfind("."));
                m_TextureManager.Load(normalTexName, generalPath + "/" + materials.at(i).bump_texname);
                newMaterial->SetTexture(m_TextureManager.Get(normalTexName), Normal);

                std::string roughnessTexName = materials.at(i).specular_highlight_texname;
                roughnessTexName.erase(roughnessTexName.rfind("."));
                m_TextureManager.Load(roughnessTexName, generalPath + "/" + materials.at(i).specular_highlight_texname);
                newMaterial->SetTexture(m_TextureManager.Get(roughnessTexName), Roughness);

                std::string metallicTexName = materials.at(i).ambient_texname;
                metallicTexName.erase(metallicTexName.rfind("."));
                m_TextureManager.Load(metallicTexName, generalPath + "/" + materials.at(i).ambient_texname);
                newMaterial->SetTexture(m_TextureManager.Get(metallicTexName), Metallic);

                newMaterial->SetTexture(nullptr, AO);

                Manager::Insert(materials.at(i).name, newMaterial);
            }

            return true;
        }
        
    }
}