#pragma once

#include "AssetManagers/TextureManager.h"
#include "Pipeline/ShaderUniformCallback.h"

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
            Material(const Managers::TextureManager& textureManager, const std::string& path);

            void Bind() const;
            void Bind(TextureType type) const;
            static void Unbind();

            const std::string& GetPath() const;

            const std::string& GetTexture(TextureType type) const;
            void SetTexture(const std::string& textureName, TextureType type);

            static GLint GetTextureSlotForTextureType(TextureType type);
            static void SetMaterialTextureSlotOffset(unsigned int slotOffset);

            static std::vector<std::string> GetTextureNamesFromFile(std::istream& is, TextureType type);
            static std::vector<std::string> GetMaterialNamesFromFile(std::istream& is);

        private:
            const std::string m_path;
            std::vector<std::string> m_textures;
            const Managers::TextureManager& m_textureManager;

            // Lighting techniques require different texture slots for pre and post processing
            // so all textures that are bound must be offset by the graphics engines requirements
            static unsigned int s_materialTextureSlotOffset;
        };
    }
}
