#pragma once

#include "Core/Math/Math.h"
#include "Core/Interface/NonCopyable.h"

namespace Graphics
{
    namespace Pipeline
    {
        class Shader;
    }

    namespace Lighting
    {
        class Light : public Core::Interface::NonCopyable
        {
        public:

            Light(std::shared_ptr<Pipeline::Shader> shader) :
                m_shader(shader)
            {}

            virtual ~Light() = default;

            const Core::Math::vec4& GetColour() const
            {
                return m_colour;
            }

            void SetColour(const Core::Math::vec4& colour)
            {
                m_colour = colour;
            }

            std::shared_ptr<Pipeline::Shader> GetShader() const
            {
                return m_shader;
            }

        private:
            Core::Math::vec4 m_colour = Core::Math::vec4(1.0f, 1.0f, 1.0f, 1.0f);

            std::shared_ptr<Pipeline::Shader> m_shader;
        };
    }
}
