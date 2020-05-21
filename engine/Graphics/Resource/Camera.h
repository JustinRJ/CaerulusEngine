#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Core/Math/Transform.h"
#include "Core/Interface/NonCopyable.h"

namespace Graphics
{
    namespace Resource
    {
        class CAERULUS_GRAPHICS Camera : public Core::Interface::NonCopyable
        {
        public:

            Camera();
            virtual ~Camera() = default;

            const mat4& GetViewMatrix() const;
            const mat4& GetProjMatrix() const;

            vec3 GetPosition() const;
            vec3 GetForward() const;
            vec3 GetUp() const;
            void SetPosition(const vec3& position);
            void SetForward(const vec3& forward);
            void SetUp(const vec3& up);

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

            void Translate(const vec3& translation, bool translateY = true);
            void Rotate(const vec3& eulerDelta, const vec3& forcedUp = Core::Math::UnitUp());

        private:
            float m_degFOV;
            float m_aspect;
            float m_near;
            float m_far;

            float m_aperture;
            float m_shutterSpeed;
            float m_ISO;
            mat4 m_proj;
            Core::Math::Transform m_view;
        };
    }
}

