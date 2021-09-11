#pragma once

#include "Interface/NonCopyable.h"
#include "Pipeline/Shader.h"
#include "Managers/ShaderManager.h"

namespace Graphics
{
    namespace Pipeline
    {
        class CAERULUS_GRAPHICS ShaderUniformFunctor : public Core::Interface::NonCopyable
        {
        public:
            ShaderUniformFunctor(const Managers::ShaderManager& shaderManager) :
                m_shaderManager(shaderManager)
            {}

            virtual ~ShaderUniformFunctor() = default;

            void AddUniformFunctor(const std::string& shaderName, const std::function<void(const Shader& shader)>& uniformFunctor)
            {
                bool shaderFound = false;
                for (const auto& pair : m_shaderFunctors)
                {
                    if (pair.first == shaderName)
                    {
                        shaderFound = true;
                    }
                }

                if (!shaderFound)
                {
                    m_shaderFunctors.push_back({ shaderName, uniformFunctor });
                }
            }

            void InvokeUniformFunctors() const
            {
                for (const auto& pair : m_shaderFunctors)
                {
                    if (const Shader* const shader = m_shaderManager.Get(pair.first))
                    {
                        shader->Bind();
                        pair.second(*shader);
                    }
                }
            }

        private:
            std::vector<std::pair<const std::string, std::function<void(const Shader& shader)>>> m_shaderFunctors;

            const Managers::ShaderManager& m_shaderManager;
        };
    }
}