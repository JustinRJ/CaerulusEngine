#pragma once

#include "Light.h"

namespace Graphics
{
    namespace Light
    {
        class DirectionalLight : public Light
        {
        public:
            DirectionalLight(const glm::vec4& colour, const glm::vec3& direction);
            virtual ~DirectionalLight();

            const glm::vec3& GetDirection() const;
            void SetDirection(const glm::vec3& direction);

            void RenderToShader(unsigned int ID, const Shaders::Shader& shader, const glm::mat4& view) const override;

        protected:
            glm::vec3 m_Direction;
        };
    }
}