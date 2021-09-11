#pragma once

#include "Node/Node.h"
#include "Pipeline/ShaderUniformFunctor.h"

namespace Graphics
{
    namespace Lighting
    {
        class Light : public Core::Node::Node, public Pipeline::ShaderUniformFunctor
        {
        public:

            Light(const Managers::ShaderManager& shaderManager) :
                Pipeline::ShaderUniformFunctor(shaderManager)
            {}

            virtual ~Light() = default;

            const Core::Math::vec3& GetColour() const
            {
                return m_colour;
            }

            void SetColour(const Core::Math::vec3& colour)
            {
                m_colour = colour;
            }

        private:
            Core::Math::vec3 m_colour = Core::Math::vec3(1.0f, 1.0f, 1.0f);
        };
    }
}
