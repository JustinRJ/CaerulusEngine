#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Graphics/Pipeline/IBindable.h"

namespace Graphics
{
    namespace Pipeline
    {
        class Shader;
    }

    namespace Resource
    {
        class Texture;

        class CAERULUS_GRAPHICS Material : public Pipeline::IBindable
        {
        public:
            Material(const std::string& name, const std::string& path = "");
            virtual ~Material() = default;

            void Bind() const override;
            void Unbind() const override;

            const std::vector<std::shared_ptr<Texture>>& GetTextures() const;
            void SetTextures(const std::vector<std::shared_ptr<Texture>>& textures);

            void SetShaders(const std::vector<std::shared_ptr<Pipeline::Shader>>& shaders);
            const std::vector<std::shared_ptr<Pipeline::Shader>>& GetShaders() const;

            void LoadMaterialTexturesNames(unsigned int materialIndex, std::istream& is);
            const std::vector<std::string>& GetTextureNames() const;

            const std::string& GetName() const;

            static std::vector<std::string> GetFileMaterialNames(std::istream& is);

        private:
            std::string m_name;
            std::string m_path;
            std::vector<std::string> m_textureNames;
            std::vector<std::shared_ptr<Texture>> m_textures;
            std::vector<std::shared_ptr<Pipeline::Shader>> m_shaders;
        };
    }
}
