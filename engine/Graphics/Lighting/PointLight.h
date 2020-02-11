#pragma once

#include "Light.h"

namespace Graphics
{
    namespace Light
    {
        class PointLight : public Light
        {
        public:
            PointLight(const vec4& colour, const vec3& position, float radius);
            virtual ~PointLight() = default;

            const vec3& GetPosition() const;
            void SetPosition(const vec3& pos);

            float GetRadius() const;
            void SetRadius(float radius);

            void UpdateUniforms(const std::vector<std::string>& uniforms,
                const PipeLine::Shader& shader, const mat4& view) const override;

        protected:
            vec3 m_Position;
            float m_Radius;
        };
    }
}