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

        class Material
        {

        public:
            Material(const std::string& name);
            ~Material();
            void RenderToShader() const;

            Texture* GetTexture(MaterialType materialType) const;
            void SetTexture(Texture* texture, MaterialType materialType);

        private:
            const std::string& m_Name;

            std::vector<Texture*>& m_Textures;
        };
    }
}
