#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Texture.h"
#include <vector>
#include <map>

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
            AO = 4,
            Size = 5
        };

        class CAERULUS_GRAPHICS Material
        {

        public:
            Material(const std::string& name, const std::string& path = "");
            ~Material();
            void RenderToShader() const;

            std::shared_ptr<Texture> GetTexture(MaterialType materialType) const;
            void SetTexture(std::shared_ptr<Texture> texture, MaterialType materialType);

            static std::vector<std::string> GetFileMaterialNames(std::istream& is);

            void LoadMaterialTexturesNames(unsigned int materialIndex, std::istream& is);
            const std::map<MaterialType, std::string>& GetTextureNames() const;

            const std::string& GetName() const;

        private:
            std::string m_Name;
            std::string m_Path;
            std::map<MaterialType, std::string> m_TextureNames;
            std::vector<std::shared_ptr<Texture>> m_Textures;
        };
    }
}
