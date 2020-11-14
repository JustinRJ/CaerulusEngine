#include "stdafx.h"

#include "Material.h"
#include "Texture.h"
#include "Graphics/Pipeline/Shader.h"
#include "Core/Logging/Log.h"

namespace Graphics
{
    namespace Surface
    {
        Material::Material(const std::string& name, const std::string& path) :
            m_name(name),
            m_path(path),
            m_textureConfigs(7)
        {
        }

        std::vector<std::string> Material::GetMaterialNamesFromFile(std::istream& is)
        {
            std::map<std::string, int> materialMap = std::map<std::string, int>();
            std::vector<tinyobj::material_t> materials = std::vector<tinyobj::material_t>();

            tinyobj::LoadMtl(&materialMap, &materials, &is);
            is.seekg(0, is.beg);

            std::vector<std::string> actualMaterialNames;
            for (tinyobj::material_t& mat : materials)
            {
                actualMaterialNames.push_back(mat.name);
            }
            return actualMaterialNames;
        }

        std::vector<std::string> Material::GetTextureNamesFromFile(std::istream& is, TextureType type)
        {
            std::map<std::string, int> materialMap = std::map<std::string, int>();
            std::vector<tinyobj::material_t> materials = std::vector<tinyobj::material_t>();
            tinyobj::LoadMtl(&materialMap, &materials, &is);
            is.seekg(0, is.beg);

            unsigned int slot = static_cast<unsigned int>(type);

            std::vector<std::string> textureNames(7);
            textureNames[0] = materials[slot].ambient_texname;            // map_Ka
            textureNames[1] = materials[slot].diffuse_texname;            // map_Kd
            textureNames[2] = materials[slot].specular_texname;           // map_Ks
            textureNames[3] = materials[slot].specular_highlight_texname; // map_Ns
            textureNames[4] = materials[slot].bump_texname;               // map_bu
            textureNames[5] = materials[slot].displacement_texname;       // disp
            textureNames[6] = materials[slot].alpha_texname;              // map_d
            return textureNames;
        }

        void Material::SetUniform(const std::string& uniform, TextureType type)
        {
            unsigned int slot = static_cast<unsigned int>(type);
            if (slot < m_textureConfigs.size())
            {
                m_textureConfigs[slot].UniformName = uniform;
            }
        }

        void Material::SetTexture(std::shared_ptr<Texture> texture, TextureType type)
        {
            unsigned int slot = static_cast<unsigned int>(type);
            if (slot < m_textureConfigs.size())
            {
                m_textureConfigs[slot].Texture = texture;
            }
        }

        void Material::Bind(TextureType type) const
        {
            unsigned int slot = static_cast<unsigned int>(type);
            if (m_shader && slot < m_textureConfigs.size())
            {
                const Material::TextureConfig& config = m_textureConfigs[slot];

                if (config.Texture && config.UniformName != "")
                {
                    config.Texture->Bind(slot);
                    m_shader->Set1i(config.UniformName, slot);
                }
            }
        }

        void Material::Bind() const
        {
            for (unsigned int i = 0; i < m_textureConfigs.size(); ++i)
            {
                Bind(static_cast<TextureType>(i));
            }
        }

        void Material::Unbind()
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        const std::string& Material::GetName() const
        {
            return m_name;
        }

        const std::string& Material::GetPath() const
        {
            return m_path;
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
