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

            void AddUniformCallback(const Shader& shader, const std::function<void(const ShaderUniformCallback&, const Shader& shader)>& uniformCallback)
            {
                bool shaderFound = false;
                for (const auto& pair : m_shaderCallbacks)
                {
                    if (pair.first == &shader)
                    {
                        shaderFound = true;
                    }
                }

                if (!shaderFound)
                {
                    m_shaderCallbacks.push_back({ &shader, uniformCallback });
                }
            }

            void InvokeUniformCallbacks() const
            {
                for (const auto& pair : m_shaderCallbacks)
                {
                    pair.first->Bind();
                    pair.second(*this, *pair.first);
                }
            }

        private:
            std::vector<std::pair<const Shader*, std::function<void(const ShaderUniformCallback&, const Shader&)>>> m_shaderCallbacks;
        };
    }
}