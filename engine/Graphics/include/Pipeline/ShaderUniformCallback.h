#pragma once

#include "Interface/NonCopyable.h"
#include "Pipeline/Shader.h"

namespace Graphics
{
    namespace Pipeline
    {
        class CAERULUS_GRAPHICS ShaderUniformCallback : Core::Interface::NonCopyable
        {
        public:
            ShaderUniformCallback() = default;
            virtual ~ShaderUniformCallback() = default;

            void AddUniformCallback(Shader& shader, const std::function<void(ShaderUniformCallback&, Shader& shader)>& uniformCallback)
            {
                bool shaderFound = false;
                for (const auto& [shader, uniformCallback] : m_shaderCallbacks)
                {
                    if (shader == shader)
                    {
                        shaderFound = true;
                    }
                }

                if (!shaderFound)
                {
                    m_shaderCallbacks.push_back({ &shader, uniformCallback });
                }
            }

            void InvokeUniformCallbacks()
            {
                for (const auto& [shader, uniformCallback] : m_shaderCallbacks)
                {
                    shader->Bind();
                    uniformCallback(*this, *shader);
                }
            }

        private:
            std::vector<std::pair<Shader*, std::function<void(ShaderUniformCallback&, Shader&)>>> m_shaderCallbacks;
        };
    }
}