#pragma once

#include "Interface/NonCopyable.h"
#include "Pipeline/Shader.h"
#include "Managers/ShaderManager.h"

namespace Graphics
{
    namespace Pipeline
    {
        class CAERULUS_GRAPHICS ShaderUniformCallback : Core::Interface::NonCopyable
        {
        public:
            ShaderUniformCallback(const Managers::ShaderManager& shaderManager) :
                m_shaderManager(shaderManager)
            {}

            virtual ~ShaderUniformCallback() = default;

            void AddUniformCallback(const std::string& shaderName, const std::function<void(const Shader& shader)>& uniformCallback)
            {
                bool shaderFound = false;
                for (const auto& pair : m_shaderCallbacks)
                {
                    if (pair.first == shaderName)
                    {
                        shaderFound = true;
                    }
                }

                if (!shaderFound)
                {
                    m_shaderCallbacks.push_back({ shaderName, uniformCallback });
                }
            }

            void InvokeUniformCallbacks() const
            {
                for (const auto& pair : m_shaderCallbacks)
                {
                    if (const Shader* const shader = m_shaderManager.Get(pair.first))
                    {
                        shader->Bind();
                        pair.second(*shader);
                    }
                }
            }

        private:
            std::vector<std::pair<const std::string, std::function<void(const Shader& shader)>>> m_shaderCallbacks;

            const Managers::ShaderManager& m_shaderManager;
        };
    }
}