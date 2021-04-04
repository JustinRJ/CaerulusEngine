#include "stdafx.h"

#include "Material.h"
#include "Texture.h"
#include "Core/Logging/Log.h"

namespace Graphics
{
    namespace Surface
    {
        Material::Material(const std::string& path) :
            m_path(path),
            m_textures(7)
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

        void Material::SetTexture(std::shared_ptr<Texture> texture, TextureType type)
        {
            unsigned int slot = static_cast<unsigned int>(type);
            if (slot < m_textures.size())
            {
                m_textures[slot] = texture;
            }
        }

        void Material::Bind(TextureType type) const
        {
            unsigned int slot = static_cast<unsigned int>(type);
            if (slot < m_textures.size())
            {
                if (std::shared_ptr<Texture> texture = m_textures[slot])
                {
                    texture->Bind(slot);
                }
            }
        }

        void Material::Bind() const
        {
            for (unsigned int i = 0; i < m_textures.size(); ++i)
            {
                Bind(static_cast<TextureType>(i));
            }
        }

        void Material::Unbind()
        {
            Texture::Unbind();
        }

        const std::string& Material::GetPath() const
        {
            return m_path;
        }

        std::shared_ptr<Texture> Material::GetTexture(TextureType type) const
        {
            std::shared_ptr<Texture> texture = nullptr;
            unsigned int slot = static_cast<unsigned int>(type);
            if (slot < m_textures.size())
            {
                texture = m_textures[slot];
            }
            return texture;
        }
    }
}
