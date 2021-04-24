#pragma once

#include "Core/Interface/NonCopyable.h"
#include "Graphics/Pipeline/Shader.h"

namespace Graphics
{
    namespace Pipeline
    {
        class CAERULUS_GRAPHICS ShaderUniformFunctor : public Core::Interface::NonCopyable
        {
        public:
            ShaderUniformFunctor() = default;
            virtual ~ShaderUniformFunctor() = default;

            void AddUniformFunctor(std::shared_ptr<Pipeline::Shader> shader, const std::function<void(const Shader& shader)>& uniformFunctor)
            {
                bool shaderFound = false;
                for (const auto& pair : m_shaderFunctors)
                {
                    if (pair.first == shader)
                    {
                        shaderFound = true;
                    }
                }

                if (!shaderFound)
                {
                    m_shaderFunctors.push_back({ shader, uniformFunctor });
                }
            }

            void InvokeUniformFunctors() const
            {
                for (const auto& pair : m_shaderFunctors)
                {
                    std::shared_ptr<Shader> shader = pair.first;

                    if (shader)
                    {
                        shader->Bind();
                        pair.second(*shader);
                    }
                }
            }

        private:
            std::vector<std::pair<std::shared_ptr<Pipeline::Shader>, std::function<void(const Shader& shader)>>> m_shaderFunctors;
        };
    }
}