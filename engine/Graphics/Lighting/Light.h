#pragma once

#include "Core/Math/Math.h"
#include "Core/Math/Camera.h"
#include "Core/Interface/NonCopyable.h"
#include "Graphics/PipeLine/Shader.h"

namespace Graphics
{
    namespace Light 
    {
        using namespace Core::Math;
        using namespace Graphics::PipeLine;

        class Light : public Core::Interface::NonCopyable
        {
        public:
            virtual ~Light() = default;

            const vec4& GetColour() const
            {
                return m_colour;
            }

            void SetColour(const vec4& colour)
            {
                m_colour = colour;
            }

            std::shared_ptr<Shader> GetShader() const
            {
                return m_shader;
            }

            std::shared_ptr<Camera> GetCamera() const
            {
                return m_camera;
            }

        protected:
            Light(std::shared_ptr<Shader> shader, std::shared_ptr<Camera> camera) :
                m_shader(shader),
                m_camera(camera)
            {}

            vec4 m_colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
            std::shared_ptr<PipeLine::Shader> m_shader;
            std::shared_ptr<Core::Math::Camera> m_camera;
        };
    }
}
