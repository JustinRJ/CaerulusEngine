#pragma once

#include "Light.h"

namespace Graphics
{
    namespace Light
    {
        class DirectionalLight : public Light
        {
        public:
            DirectionalLight(const vec4& colour, const vec3& direction);
            virtual ~DirectionalLight() = default;

            const vec3& GetDirection() const;
            void SetDirection(const vec3& direction);

            void RenderToShader(unsigned int ID, const PipeLine::Shader& shader, const mat4& view) const override;

        protected:
            vec3 m_Direction;
        };
    }
}