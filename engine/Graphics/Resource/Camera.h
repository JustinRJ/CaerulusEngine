#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "../../Core/Math/MathFacade.h"

namespace Graphics
{
    namespace Resource
    {
        using namespace Core::Math;

        class Camera
        {
        public:

            CAERULUS_GRAPHICS Camera();
            CAERULUS_GRAPHICS ~Camera();

            CAERULUS_GRAPHICS const mat4& GetViewMatrix() const;
            CAERULUS_GRAPHICS const mat4& GetProjMatrix() const;
            CAERULUS_GRAPHICS void SetViewMatrix(const mat4& view);
            CAERULUS_GRAPHICS void SetProjMatrix(const mat4& proj);

            CAERULUS_GRAPHICS vec3 GetPosition() const;
            CAERULUS_GRAPHICS vec3 GetForward() const;
            CAERULUS_GRAPHICS vec3 GetUp() const;
            CAERULUS_GRAPHICS void SetPosition(const vec3& position);
            CAERULUS_GRAPHICS void SetForward(const vec3& forward);
            CAERULUS_GRAPHICS void SetUp(const vec3& up);

            CAERULUS_GRAPHICS float GetFOV() const;
            CAERULUS_GRAPHICS float GetAspect() const;
            CAERULUS_GRAPHICS float GetNear() const;
            CAERULUS_GRAPHICS float GetFar() const;

            CAERULUS_GRAPHICS float GetAperture() const;
            CAERULUS_GRAPHICS float GetShutterSpeed() const;
            CAERULUS_GRAPHICS float GetISO() const;

            CAERULUS_GRAPHICS void SetFOV(float fov);
            CAERULUS_GRAPHICS void SetAspect(float aspect);
            CAERULUS_GRAPHICS void SetNear(float near);
            CAERULUS_GRAPHICS void SetFar(float far);

            CAERULUS_GRAPHICS void SetAperture(float aperture);
            CAERULUS_GRAPHICS void SetShutterSpeed(float shutterSpeed);
            CAERULUS_GRAPHICS void SetISO(float iso);

            CAERULUS_GRAPHICS void TranslateXZ(const vec3& translation);
            CAERULUS_GRAPHICS void Translate(const vec3& translation);
            CAERULUS_GRAPHICS void Rotate(const vec3& eulerDelta, const vec3& forcedUp = Core::Math::UnitUp());

        private:
            float m_DegFOV;
            float m_Aspect;
            float m_Near;
            float m_Far;

            float m_Aperture;
            float m_ShutterSpeed;
            float m_ISO;

            mat4 m_View;
            mat4 m_Proj;
        };
    }
}
