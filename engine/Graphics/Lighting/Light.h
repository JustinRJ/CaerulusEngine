#pragma once

#include "../Pipeline/PipelineUniform.h"
#include "../PipeLine/Shader.h"
#include "../Resource/Camera.h"

namespace Graphics
{
    namespace Light
    {
        using namespace PipeLine;
        using namespace Resource;
        using namespace Core::Math;

        class Light
        {
        public:

            Light(std::shared_ptr<Shader> shader, std::shared_ptr<Camera> camera);
            virtual ~Light() = default;

            void SetColour(const vec4& colour);
            const vec4& GetColour() const;

            std::shared_ptr<Shader> GetShader() const;
            std::shared_ptr<Camera> GetCamera() const;

        protected:
            vec4 m_Colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
            std::shared_ptr<Shader> m_Shader;
            std::shared_ptr<Camera> m_Camera;
        };
    }
}
