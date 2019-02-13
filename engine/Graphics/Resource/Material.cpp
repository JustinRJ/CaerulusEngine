#include "stdafx.h"

#include "Material.h"
#include <iostream>

namespace Graphics
{
    namespace Resource
    {
        Material::Material(const std::string& path) :
            m_Path(path),
            m_Textures(*new std::vector<Texture*>(5))
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

        Texture* Material::GetTexture(MaterialType materialType) const
        {
            return m_Textures.at(materialType);
        }

        void Material::SetTexture(Texture* texture, MaterialType materialType)
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

        void Material::RenderToShader(const Texture& defaultAO) const
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

            if (m_Textures.at(AO) != nullptr)
            {
                m_Textures.at(AO)->UseTexture();
            }
            else
            {
                defaultAO.UseTexture();
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}
