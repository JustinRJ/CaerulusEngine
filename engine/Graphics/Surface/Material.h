#pragma once

#include "Graphics/Pipeline/ShaderUniformFunctor.h"

namespace Graphics
{
    namespace Surface
    {
        class Texture;

        class CAERULUS_GRAPHICS Material : public Pipeline::ShaderUniformFunctor

        {
        public:
            enum class TextureType : unsigned int
            {
                Ambient,
                Diffuse,
                Specular,
                Highlight,
                Bump,
                Displacement,
                Alpha
            };

            Material(const std::string& path);
            ~Material() = default;

            void Bind() const;
            void Bind(TextureType type) const;
            static void Unbind();

            const std::string& GetPath() const;

            std::shared_ptr<Texture> GetTexture(TextureType type) const;
            void SetTexture(std::shared_ptr<Texture> texture, TextureType type);

            static std::vector<std::string> GetTextureNamesFromFile(std::istream& is, TextureType type);
            static std::vector<std::string> GetMaterialNamesFromFile(std::istream& is);

        private:
            const std::string m_path;
            std::vector<std::shared_ptr<Texture>> m_textures;
        };
    }
}
