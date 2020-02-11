#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "../../Core/Math/MathHelper.h"

namespace Graphics
{
    namespace Resource
    {
        using namespace Core::Math;

        class CAERULUS_GRAPHICS Camera
        {
        public:

            Camera();
            ~Camera();

            const mat4& GetViewMatrix() const;
            const mat4& GetProjMatrix() const;
            void SetViewMatrix(const mat4& view);
            void SetProjMatrix(const mat4& proj);

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

            void TranslateXZ(const vec3& translation);
            void Translate(const vec3& translation);
            void Rotate(const vec3& eulerDelta, const vec3& forcedUp = MathHelper::UnitUp());

            void GetProjViewModel(mat4& out) const;

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

