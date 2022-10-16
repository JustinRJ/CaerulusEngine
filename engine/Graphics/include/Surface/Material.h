#pragma once

#include "AssetManagers/TextureManager.h"
#include "Pipeline/ShaderUniformCallback.h"

namespace Graphics
{
    namespace Surface
    {
        class Texture;

        enum class TextureType : uint32_t
        {
            Ambient,
            Diffuse,
            Specular,
            Highlight,
            Bump,
            Displacement,
            Alpha,
            Roughness,
            Metallic,
            Sheen,
            Emmisive,
            Normal
        };

        class CAERULUS_GRAPHICS Material : public Pipeline::ShaderUniformCallback
        {
        public:
            Material(const AssetManagers::TextureManager& textureManager, std::string_view path);

            void Bind();
            void Bind(TextureType type);
            static void Unbind();

            std::string_view GetPath() const;

            std::string_view GetTexture(TextureType type) const;
            void SetTexture(std::string_view textureName, TextureType type);

            static GLint GetTextureSlotForTextureType(TextureType type);
            static void SetMaterialTextureSlotOffset(uint32_t slotOffset);

            static std::vector<std::string> GetTextureNamesFromFile(std::istream& is, TextureType type);
            static std::vector<std::string> GetMaterialNamesFromFile(std::istream& is);

        private:
            const std::string m_path;
            std::vector<std::string> m_textures;
            const AssetManagers::TextureManager& m_textureManager;

            // Lighting techniques require different texture slots for pre and post processing
            // so all textures that are bound must be offset by the graphics engines requirements
            inline static uint32_t s_materialTextureSlotOffset = 0;
        };
    }
}
