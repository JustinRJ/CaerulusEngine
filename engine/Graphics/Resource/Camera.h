#pragma once

#include "../../Core/Math/MathHelper.h"
#include <vector>

namespace Graphics
{
    namespace Resource
    {
        class Camera
        {
        public:

            Camera();
            ~Camera();

            const glm::mat4& GetViewMatrix() const;
            const glm::mat4& GetProjMatrix() const;
            void SetViewMatrix(const glm::mat4& view);
            void SetProjMatrix(const glm::mat4& proj);

            glm::vec3 GetPosition() const;
            glm::vec3 GetForward() const;
            glm::vec3 GetUp() const;
            void SetPosition(const glm::vec3& position);
            void SetForward(const glm::vec3& forward);
            void SetUp(const glm::vec3& up);

            float GetFOV() const;
            float GetAspect() const;
            float GetNear() const;
            float GetFar() const;

            float GetAperture() const;
            float GetShutterSpeed() const;
            float GetISO() const;

            void SetFOV(float fov);
            void SetAspect(float aspect);
            void SetNear(float near);
            void SetFar(float far);

            void SetAperture(float aperture);
            void SetShutterSpeed(float shutterSpeed);
            void SetISO(float iso);

            void TranslateXZ(const glm::vec3& translation);
            void Translate(const glm::vec3& translation);
            void Rotate(const glm::vec3& eulerDelta, const glm::vec3& forcedUp = Core::Math::UnitUp);

        private:
            float m_DegFOV;
            float m_Aspect;
            float m_Near;
            float m_Far;

            float m_Aperture;
            float m_ShutterSpeed;
            float m_ISO;

            glm::mat4& m_View;
            glm::mat4& m_Proj;
        };
    }
}
