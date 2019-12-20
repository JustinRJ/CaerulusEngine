#include "stdafx.h"

#include "Material.h"
#include <iostream>
#include <tiny_obj_loader.h>

namespace Graphics
{
    namespace Resource
    {
        Material::Material(const std::string& name, const std::string& path) :
            m_Name(name),
            m_Path(path),
            m_TextureNames(),
            m_Textures(std::vector<std::shared_ptr<Texture>>(MaterialType::Size))
        {
            m_Textures[Albedo] = nullptr;
            m_Textures[Normal] = nullptr;
            m_Textures[Roughness] = nullptr;
            m_Textures[Metallic] = nullptr;
            m_Textures[AO] = nullptr;
        }

        Material::~Material()
        {
        }

        void Material::LoadMaterialTexturesNames(unsigned int materialIndex, std::istream& is)
        {
            std::map<std::string, int> materialNames = std::map<std::string, int>();
            std::vector<tinyobj::material_t> materials = std::vector<tinyobj::material_t>();
            tinyobj::LoadMtl(&materialNames, &materials, &is);
            is.seekg(0, is.beg);

            m_Name = materials.at(materialIndex).name;

            // todo - Fix mapping
            m_TextureNames[Albedo] = materials.at(materialIndex).diffuse_texname;
            m_TextureNames[Normal] = materials.at(materialIndex).bump_texname;
            m_TextureNames[Roughness] = materials.at(materialIndex).specular_highlight_texname;
            m_TextureNames[Metallic] = materials.at(materialIndex).ambient_texname;
            m_TextureNames[AO] = "";
        }

        std::shared_ptr<Texture> Material::GetTexture(MaterialType materialType) const
        {
            return m_Textures.at(materialType);
        }

        void Material::SetTexture(std::shared_ptr<Texture> texture, MaterialType materialType)
        {
            if (materialType > AO)
            {
                std::cerr << "Texture outside material bounds!" << std::endl;
            }
            else
            {
                m_Textures[materialType] = texture;
            }
        }

        void Material::RenderToShader() const
        {
            glActiveTexture(GL_TEXTURE0);
            m_Textures.at(Albedo) != nullptr ? m_Textures.at(Albedo)->UseTexture() : glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE1);
            m_Textures.at(Normal) != nullptr ? m_Textures.at(Normal)->UseTexture() : glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE2);
            m_Textures.at(Roughness) != nullptr ? m_Textures.at(Roughness)->UseTexture() : glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE3);
            m_Textures.at(Metallic) != nullptr ? m_Textures.at(Metallic)->UseTexture() : glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE4);
            m_Textures.at(AO) != nullptr ? m_Textures.at(AO)->UseTexture() : glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        const std::map<MaterialType, std::string>& Material::GetTextureNames() const
        {
            return m_TextureNames;
        }

        const std::string& Material::GetName() const
        {
            return m_Name;
        }

        std::vector<std::string> Material::GetFileMaterialNames(std::istream& is)
        {
            std::map<std::string, int> materialNames = std::map<std::string, int>();
            std::vector<tinyobj::material_t> materials = std::vector<tinyobj::material_t>();

            tinyobj::LoadMtl(&materialNames, &materials, &is);
            is.seekg(0, is.beg);
            
            std::vector<std::string> actualMaterialNames;
            for (auto mat : materials)
            {
                actualMaterialNames.push_back(mat.name);
            }
            return actualMaterialNames;
        }
    }
}
