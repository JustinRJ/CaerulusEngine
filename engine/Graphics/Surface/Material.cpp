#include "stdafx.h"

#include "Surface/Material.h"

#include <tiny_obj_loader.h>

#include "Logging/Log.h"
#include "Surface/Texture.h"

namespace Graphics
{
    namespace Surface
    {
        Material::Material(const AssetManagers::TextureManager& textureManager, const std::string& path) :
            m_path(path),
            m_textures(7),
            m_textureManager(textureManager)
        {}

        GLint Material::GetTextureSlotForTextureType(TextureType type)
        {
            return static_cast<GLint>(type) + s_materialTextureSlotOffset;
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

            std::vector<std::string> textureNames(12);
            textureNames[0] = materials[slot].ambient_texname;            // map_Ka
            textureNames[1] = materials[slot].diffuse_texname;            // map_Kd
            textureNames[2] = materials[slot].specular_texname;           // map_Ks
            textureNames[3] = materials[slot].specular_highlight_texname; // map_Ns
            textureNames[4] = materials[slot].bump_texname;               // map_bu
            textureNames[5] = materials[slot].displacement_texname;       // disp
            textureNames[6] = materials[slot].alpha_texname;              // map_d
            textureNames[7] = materials[slot].roughness_texname;          // map_Pr
            textureNames[8] = materials[slot].metallic_texname;           // map_Pm
            textureNames[9] = materials[slot].sheen_texname;              // map_Ps
            textureNames[10] = materials[slot].emissive_texname;          // map_Ke
            textureNames[11] = materials[slot].normal_texname;            // norm. For normal mapping.

            return textureNames;
        }

        void Material::SetTexture(const std::string& textureName, TextureType type)
        {
            unsigned int slot = static_cast<unsigned int>(type);
            if (slot < m_textures.size())
            {
                m_textures[slot] = textureName;
            }
        }

        void Material::Bind(TextureType type)
        {
            unsigned int slot = static_cast<unsigned int>(type);
            if (slot < m_textures.size())
            {
                const std::string& textureName = m_textures.at(slot);
                if (textureName != "")
                {
                    if (const Texture* const texture = m_textureManager.Get(textureName))
                    {
                        texture->Bind(slot + s_materialTextureSlotOffset);
                    }
                }
            }
        }

        void Material::Bind()
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

        const std::string& Material::GetTexture(TextureType type) const
        {
            unsigned int slot = static_cast<unsigned int>(type);
            if (slot < m_textures.size())
            {
                return m_textures[slot];
            }
            throw std::runtime_error("Material.cpp - TextureType not found on material: " + m_path);
        }

        void Material::SetMaterialTextureSlotOffset(unsigned int slotOffset)
        {
            s_materialTextureSlotOffset = slotOffset;
        }

    }
}
