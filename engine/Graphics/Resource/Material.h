#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

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
            CAERULUS_GRAPHICS Material(const std::string& name);
            CAERULUS_GRAPHICS ~Material();
            CAERULUS_GRAPHICS void RenderToShader() const;

            CAERULUS_GRAPHICS Texture* GetTexture(MaterialType materialType) const;
            CAERULUS_GRAPHICS void SetTexture(Texture* texture, MaterialType materialType);

        private:
            const std::string& m_Name;

            std::vector<Texture*>& m_Textures;
        };
    }
}
