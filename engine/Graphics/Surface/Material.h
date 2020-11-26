#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Core/Interface/NonCopyable.h"

namespace Graphics
{
    namespace Pipeline
    {
        class Shader;
    }

    namespace Surface
    {
        class Texture;

        class CAERULUS_GRAPHICS Material : public Core::Interface::NonCopyable
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

            const std::string& GetName() const;
            const std::string& GetPath() const;

            void SetShader(std::shared_ptr<Pipeline::Shader> shader);
            std::shared_ptr<Pipeline::Shader> GetShader() const;

            void SetUniform(const std::string& uniform, TextureType type);
            void SetTexture(std::shared_ptr<Texture> texture, TextureType type);

            // TODO - move to managers
            static std::vector<std::string> GetTextureNamesFromFile(std::istream& is, TextureType type);
            static std::vector<std::string> GetMaterialNamesFromFile(std::istream& is);

        private:
            const std::string m_path;

            std::shared_ptr<Pipeline::Shader> m_shader;

            struct TextureConfig
            {
                std::string UniformName;
                std::shared_ptr<Texture> Texture;
            };
            std::vector<TextureConfig> m_textureConfigs;
        };
    }
}
