#include "stdafx.h"

#include "Material.h"
#include <tiny_obj_loader.h>
#include "Texture.h"
#include "Core/Logging/Log.h"

namespace Graphics
{
    namespace Resource
    {
        Material::Material(const std::string& name, const std::string& path) :
            m_name(name),
            m_path(path),
            m_textureNames(),
            m_textures(std::vector<std::shared_ptr<Texture>>(MaterialType::Size))
        {
            m_textures[Albedo] = nullptr;
            m_textures[Normal] = nullptr;
            m_textures[Roughness] = nullptr;
            m_textures[Metallic] = nullptr;
            m_textures[AO] = nullptr;
        }

        void Material::LoadMaterialTexturesNames(unsigned int materialIndex, std::istream& is)
        {
            std::map<std::string, int> materialNames = std::map<std::string, int>();
            std::vector<tinyobj::material_t> materials = std::vector<tinyobj::material_t>();
            tinyobj::LoadMtl(&materialNames, &materials, &is);
            is.seekg(0, is.beg);

            m_name = materials[materialIndex].name;

            // TODO - Fix mapping
            m_textureNames[Albedo] = materials[materialIndex].diffuse_texname;
            m_textureNames[Normal] = materials[materialIndex].bump_texname;
            m_textureNames[Roughness] = materials[materialIndex].specular_highlight_texname;
            m_textureNames[Metallic] = materials[materialIndex].ambient_texname;
            m_textureNames[AO] = "";
        }

        std::shared_ptr<Texture> Material::GetTexture(MaterialType materialType) const
        {
            return m_textures[materialType];
        }

        void Material::SetTexture(std::shared_ptr<Texture> texture, MaterialType materialType)
        {
            if (materialType > AO)
            {
                using Core::Logging::Log;
                Log::LogError("Texture outside material bounds!");
            }
            else
            {
                m_textures[materialType] = texture;
            }
        }

        void Material::Bind() const
        {
            glActiveTexture(GL_TEXTURE0);
            m_textures[Albedo] ? m_textures[Albedo]->Bind() : glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE1);
            m_textures[Normal] ? m_textures[Normal]->Bind() : glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE2);
            m_textures[Roughness] ? m_textures[Roughness]->Bind() : glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE3);
            m_textures[Metallic] ? m_textures[Metallic]->Bind() : glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE4);
            m_textures[AO] ? m_textures[AO]->Bind() : glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        const std::map<MaterialType, std::string>& Material::GetTextureNames() const
        {
            return m_textureNames;
        }

        const std::string& Material::GetName() const
        {
            return m_name;
        }

        std::vector<std::string> Material::GetFileMaterialNames(std::istream& is)
        {
            std::map<std::string, int> materialNames = std::map<std::string, int>();
            std::vector<tinyobj::material_t> materials = std::vector<tinyobj::material_t>();

            tinyobj::LoadMtl(&materialNames, &materials, &is);
            is.seekg(0, is.beg);
            
            std::vector<std::string> actualMaterialNames;
            for (auto& mat : materials)
            {
                actualMaterialNames.push_back(mat.name);
            }
            return actualMaterialNames;
        }
    }
}
