#pragma once

#include "Light.h"

namespace Graphics
{
    namespace Light
    {
        class PointLight : public Light
        {
        public:
            PointLight(const glm::vec4& colour, const glm::vec3& position, float radius);
            virtual ~PointLight();

            const glm::vec3& GetPosition() const;
            void SetPosition(const glm::vec3& pos);

            float GetRadius() const;
            void SetRadius(float radius);

            void RenderToShader(unsigned int ID, const Shaders::Shader& shader, const glm::mat4& view) const override;

        protected:
            glm::vec3 m_Position;
            float m_Radius;
        };
    }
}