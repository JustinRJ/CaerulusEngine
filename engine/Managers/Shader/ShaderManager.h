#pragma once

#define CAERULUS_MANAGERS __declspec(dllexport)

#include "ShaderSourceManager.h"
#include "../../Graphics/Shader/Shader.h"

namespace Managers
{
    namespace Shader
    {
        class ShaderManager : Manager<Graphics::Shaders::Shader>
        {
        public:
            CAERULUS_MANAGERS ShaderManager(ShaderSourceManager& shaderStageManager);
            CAERULUS_MANAGERS virtual ~ShaderManager() {}

            CAERULUS_MANAGERS virtual Graphics::Shaders::Shader* Get(const std::string& name) const;

            CAERULUS_MANAGERS virtual std::vector<Graphics::Shaders::Shader*> GetAll(const std::vector<std::string>& names) const;

            CAERULUS_MANAGERS virtual bool IsLoaded(const std::string& name) const;

            CAERULUS_MANAGERS virtual bool Remove(const std::string& name);

            CAERULUS_MANAGERS bool Load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath,
                const std::vector<std::string>& vertexUniformLocations, const std::vector<std::string>& fragmentUniformLocations);

        private:
            ShaderSourceManager& m_ShaderStageManager;
        };
    }
}