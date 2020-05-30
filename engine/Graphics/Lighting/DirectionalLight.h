#pragma once

#include "Light.h"
#include "Graphics/Pipeline/PipelineUniform.h"

namespace Graphics
{
    namespace Light
    {
        class DirectionalLight : public Light, public PipeLine::PipelineUniform
        {
        public:
            DirectionalLight(std::shared_ptr<PipeLine::Shader> shader, std::shared_ptr<Core::Math::Camera> camera);
            virtual ~DirectionalLight() = default;

            const vec3& GetDirection() const;
            void SetDirection(const vec3& direction);

        protected:
            vec3 m_direction;
        };
    }
}