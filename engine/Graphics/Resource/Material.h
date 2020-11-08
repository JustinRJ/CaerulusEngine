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
            struct MaterialConfig
            {
                std::string UniformName;
                std::string TextureName;
                std::shared_ptr<Texture> Texture;
            };

            Material(const std::string& name, const std::string& path = "");
            virtual ~Material() = default;

            void Bind() const override;
            void Bind(unsigned int slot) const override;
            void Unbind() const override;

            const std::vector<MaterialConfig>& GetMaterialConfig() const;
            std::vector<MaterialConfig>& GetMaterialConfig();

            std::shared_ptr<Pipeline::Shader> GetShader() const;
            void SetShader(std::shared_ptr<Pipeline::Shader> shader);

            const std::string& GetName() const;
            std::vector<std::string> GetTextureNames() const;
            void LoadMaterialTexturesNames(unsigned int materialIndex, std::istream& is);
            static std::vector<std::string> GetFileMaterialNames(std::istream& is);

        private:
            std::string m_name;
            std::string m_path;

            std::shared_ptr<Pipeline::Shader> m_shader;
            std::vector<MaterialConfig> m_materialConfigs;
        };
    }
}
