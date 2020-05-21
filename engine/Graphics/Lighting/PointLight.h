#pragma once

#include "Light.h"
#include "Graphics/Pipeline/PipelineUniform.h"

namespace Graphics
{
    namespace Light
    {
        class PointLight : public Light, public PipeLine::PipelineUniform
        {
        public:
            PointLight(std::shared_ptr<PipeLine::Shader> shader, std::shared_ptr<Resource::Camera> camera);
            virtual ~PointLight() = default;

            const vec3& GetPosition() const;
            void SetPosition(const vec3& pos);

            float GetRadius() const;
            void SetRadius(float radius);

        protected:
            vec3 m_position;
            float m_radius;
        };
    }
}