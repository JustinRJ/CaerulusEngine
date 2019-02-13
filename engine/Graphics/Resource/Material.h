#pragma once

#include "Texture.h"
#include <vector>

namespace Graphics
{
    namespace Resource
    {
        enum MaterialType
        {
            Albedo = 0,
            Normal = 1,
            Roughness = 2,
            Metallic = 3,
            AO = 4
        };

        //TODO expand this class, look at what tinyobj has to offer
        class Material
        {

        public:
            Material(const std::string& path);
            ~Material();
            void RenderToShader(const Texture& defaultAO) const;

            Texture* GetTexture(MaterialType materialType) const;
            void SetTexture(Texture* texture, MaterialType materialType);

        private:
            const std::string& m_Path;
            std::vector<Texture*>& m_Textures;
        };
    }
}
