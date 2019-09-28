#pragma once

#define CAERULUS_MANAGERS __declspec(dllexport)

#include "ShaderSourceManager.h"
#include "../../Graphics/Shader/Shader.h"

namespace Managers
{
    namespace Shader
    {
        class CAERULUS_MANAGERS ShaderManager : Manager<Graphics::Shaders::Shader>
        {
        public:
            ShaderManager(ShaderSourceManager& shaderStageManager);
            virtual ~ShaderManager() {}

            virtual Graphics::Shaders::Shader* Get(const std::string& name) const;

            virtual std::vector<Graphics::Shaders::Shader*> GetAll(const std::vector<std::string>& names) const;

            virtual bool IsLoaded(const std::string& name) const;

            virtual bool Remove(const std::string& name);

            bool Load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath,
                const std::vector<std::string>& vertexUniformLocations, const std::vector<std::string>& fragmentUniformLocations);

        private:
            ShaderSourceManager& m_ShaderStageManager;
        };
    }
}