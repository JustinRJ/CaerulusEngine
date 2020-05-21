#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <map>
#include <vector>
#include <memory>
#include "Core/Interface/NonCopyable.h"

namespace Graphics
{
    namespace Resource
    {
        class Texture;
    }
}

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

        class CAERULUS_GRAPHICS Material : public Core::Interface::NonCopyable
        {

        public:
            Material(const std::string& name, const std::string& path = "");
            virtual ~Material() = default;
            void Bind() const;

            std::shared_ptr<Texture> GetTexture(MaterialType materialType) const;
            void SetTexture(std::shared_ptr<Texture> texture, MaterialType materialType);

            static std::vector<std::string> GetFileMaterialNames(std::istream& is);

            void LoadMaterialTexturesNames(unsigned int materialIndex, std::istream& is);
            const std::map<MaterialType, std::string>& GetTextureNames() const;

            const std::string& GetName() const;

        private:
            std::string m_name;
            std::string m_path;
            std::map<MaterialType, std::string> m_textureNames;
            std::vector<std::shared_ptr<Texture>> m_textures;
        };
    }
}
