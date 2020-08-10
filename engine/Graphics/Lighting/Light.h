#pragma once

#include "Core/Math/Math.h"
#include "Core/Interface/NonCopyable.h"

namespace Graphics
{
    namespace PipeLine
    {
        class Shader;
    }
}

namespace Core
{
    namespace Math
    {
        class Camera;
    }
}

namespace Graphics
{
    namespace Light
    {
        using namespace Core::Math;
        class Light : public Core::Interface::NonCopyable
        {
        public:
            Light(std::shared_ptr<PipeLine::Shader> shader, std::shared_ptr<Core::Math::Camera> camera);
            virtual ~Light() = default;

            void SetColour(const vec4& colour);
            const vec4& GetColour() const;

            std::shared_ptr<PipeLine::Shader> GetShader() const;
            std::shared_ptr<Core::Math::Camera> GetCamera() const;

        protected:
            vec4 m_colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
            std::shared_ptr<PipeLine::Shader> m_shader;
            std::shared_ptr<Core::Math::Camera> m_camera;
        };
    }
}
