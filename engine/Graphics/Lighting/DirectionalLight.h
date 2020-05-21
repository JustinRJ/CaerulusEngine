#pragma once

#include "Light.h"

namespace Graphics
{
    namespace Light
    {
        class DirectionalLight : public Light, public PipelineUniform
        {
        public:
            DirectionalLight(std::shared_ptr<Shader> shader, std::shared_ptr<Camera> camera);
            virtual ~DirectionalLight() = default;

            const vec3& GetDirection() const;
            void SetDirection(const vec3& direction);

        protected:
            vec3 m_direction;
        };
    }
}