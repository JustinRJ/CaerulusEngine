#pragma once

#include "Graphics/Pipeline/ShaderUniformFunctor.h"

namespace Graphics
{
    namespace Surface
    {
        class Texture;

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

        class CAERULUS_GRAPHICS Material : public Pipeline::ShaderUniformFunctor

        {
        public:
            Material(const std::string& path);
            ~Material() = default;

            void Bind() const;
            void Bind(TextureType type) const;
            static void Unbind();

            const std::string& GetPath() const;

            std::shared_ptr<Texture> GetTexture(TextureType type) const;
            void SetTexture(std::shared_ptr<Texture> texture, TextureType type);

            static GLint GetTextureSlotForTextureType(TextureType type);
            static void SetMaterialTextureSlotOffset(unsigned int slotOffset);

            static std::vector<std::string> GetTextureNamesFromFile(std::istream& is, TextureType type);
            static std::vector<std::string> GetMaterialNamesFromFile(std::istream& is);

        private:
            const std::string m_path;
            // Lighting techniques require different texture slots for pre and post processing
            // so all textures that are bound must be offset by the graphics engines requirements
            static unsigned int s_materialTextureSlotOffset;
            std::vector<std::shared_ptr<Texture>> m_textures;
        };
    }
}
