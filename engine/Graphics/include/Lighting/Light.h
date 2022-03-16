#pragma once

#include "Node/Component.h"
#include "Pipeline/ShaderUniformCallback.h"

namespace Graphics
{
    namespace Lighting
    {
        class Light : public Core::Node::Component, public Pipeline::ShaderUniformCallback
        {
        public:

            Light(Core::Node::Node& node) :
                Core::Node::Component(node)
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
