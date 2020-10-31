#include "stdafx.h"

#include "Material.h"
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
            m_textures(std::vector<std::shared_ptr<Texture>>())
        {
        }

        void Material::LoadMaterialTexturesNames(unsigned int materialIndex, std::istream& is)
        {
            std::map<std::string, int> materialNames = std::map<std::string, int>();
            std::vector<tinyobj::material_t> materials = std::vector<tinyobj::material_t>();
            tinyobj::LoadMtl(&materialNames, &materials, &is);
            is.seekg(0, is.beg);

            m_name = materials[materialIndex].name;

            // TODO - Fix mapping
            m_textureNames.push_back(materials[materialIndex].diffuse_texname);
            m_textureNames.push_back(materials[materialIndex].bump_texname);
            m_textureNames.push_back(materials[materialIndex].specular_highlight_texname);
            m_textureNames.push_back(materials[materialIndex].ambient_texname);
            m_textureNames.push_back("");
        }

        const std::vector<std::shared_ptr<Texture>>& Material::GetTextures() const
        {
            return m_textures;
        }

        void Material::SetTextures(const std::vector<std::shared_ptr<Texture>>& textures)
        {
            m_textures = textures;
        }

        void Material::Bind() const
        {
            for (unsigned int i = 0; i < m_textures.size(); ++i)
            {
                if (std::shared_ptr<Texture> texture = m_textures[i])
                {
                    texture->Bind(i);
                }
            }
        }

        void Material::Unbind() const
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        const std::vector<std::string>& Material::GetTextureNames() const
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
            for (tinyobj::material_t& mat : materials)
            {
                actualMaterialNames.push_back(mat.name);
            }
            return actualMaterialNames;
        }

        void Material::SetShaders(const std::vector<std::shared_ptr<Pipeline::Shader>>& shaders)
        {
            m_shaders = shaders;
        }

        const std::vector<std::shared_ptr<Pipeline::Shader>>& Material::GetShaders() const
        {
            return m_shaders;
        }
    }
}
