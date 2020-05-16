#pragma once

#include "Light.h"

namespace Graphics
{
    namespace Light
    {
        class PointLight : public Light, public PipelineUniform<PointLight>
        {
        public:
            PointLight(std::shared_ptr<Shader> shader, std::shared_ptr<Camera> camera);
            virtual ~PointLight() = default;

            const vec3& GetPosition() const;
            void SetPosition(const vec3& pos);

            float GetRadius() const;
            void SetRadius(float radius);

        protected:
            vec3 m_Position;
            float m_Radius;
        };
    }
}