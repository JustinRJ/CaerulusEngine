#pragma once

#include "Core/Math/Math.h"
#include "Graphics/Pipeline/ShaderUniformFunctor.h"

namespace Graphics
{
    namespace Lighting
    {
        class Light : public Pipeline::ShaderUniformFunctor
        {
        public:

            Light() = default;
            virtual ~Light() = default;

            const Core::Math::vec4& GetColour() const
            {
                return m_colour;
            }

            void SetColour(const Core::Math::vec4& colour)
            {
                m_colour = colour;
            }

        private:
            Core::Math::vec4 m_colour = Core::Math::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        };
    }
}
