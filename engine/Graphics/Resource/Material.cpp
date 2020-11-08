#include "stdafx.h"

#include "Material.h"
#include "Texture.h"
#include "Graphics/Pipeline/Shader.h"
#include "Core/Logging/Log.h"

namespace Graphics
{
    namespace Resource
    {
        Material::Material(const std::string& name, const std::string& path) :
            m_name(name),
            m_path(path)
        {
        }

        void Material::LoadMaterialTexturesNames(unsigned int materialIndex, std::istream& is)
        {
            std::map<std::string, int> materialNames = std::map<std::string, int>();
            std::vector<tinyobj::material_t> materials = std::vector<tinyobj::material_t>();
            tinyobj::LoadMtl(&materialNames, &materials, &is);
            is.seekg(0, is.beg);

            m_name = materials[materialIndex].name;

            m_materialConfigs.resize(7);
            m_materialConfigs[0].TextureName = materials[materialIndex].ambient_texname;            // map_Ka
            m_materialConfigs[1].TextureName = materials[materialIndex].diffuse_texname;            // map_Kd
            m_materialConfigs[2].TextureName = materials[materialIndex].specular_texname;           // map_Ks
            m_materialConfigs[3].TextureName = materials[materialIndex].specular_highlight_texname; // map_Ns
            m_materialConfigs[4].TextureName = materials[materialIndex].bump_texname;               // map_bump, bump
            m_materialConfigs[5].TextureName = materials[materialIndex].displacement_texname;       // disp
            m_materialConfigs[6].TextureName = materials[materialIndex].alpha_texname;              // map_d
        }

        const std::vector<Material::MaterialConfig>& Material::GetMaterialConfig() const
        {
            return m_materialConfigs;
        }

        std::vector<Material::MaterialConfig>& Material::GetMaterialConfig()
        {
            return m_materialConfigs;
        }

        void Material::Bind(unsigned int slot) const
        {
            if (m_shader && slot < m_materialConfigs.size())
            {
                const Material::MaterialConfig& config = m_materialConfigs[slot];

                if (config.Texture && config.UniformName != "")
                {
                    config.Texture->Bind(slot);
                    m_shader->Set1i(config.UniformName, slot);
                }
            }
        }

        void Material::Bind() const
        {
            for (unsigned int i = 0; i < m_materialConfigs.size(); ++i)
            {
                Bind(i);
            }
        }

        void Material::Unbind() const
        {
            glBindTexture(GL_TEXTURE_2D, 0);
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

        std::vector<std::string> Material::GetTextureNames() const
        {
            std::vector<std::string> textureNames;
            for (const auto &config : m_materialConfigs)
            {
                textureNames.push_back(config.TextureName);
            }
            return textureNames;
        }

        std::shared_ptr<Pipeline::Shader> Material::GetShader() const
        {
            return m_shader;
        }

        void Material::SetShader(std::shared_ptr<Pipeline::Shader> shader)
        {
            m_shader = shader;
        }
    }
}
