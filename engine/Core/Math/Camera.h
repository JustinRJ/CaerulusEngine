#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Transform.h"

namespace Core
{
    namespace Math
    {
        class CAERULUS_GRAPHICS Camera
        {
        public:

            Camera(const vec3& position, const vec3& forward, const vec3& up = Math::UnitUp())
            {
                Camera();
                m_view = lookAt(position, forward + position, up);
            }

            Camera()
            {
                m_degFOV = 54.0f;
                m_aspect = (16.0f / 9.0f);
                m_near = 1.0f;
                m_far = 1000.0f;
                m_aperture = 16.0f;
                m_shutterSpeed = 0.5f;
                m_ISO = 1000.0f;
                m_view = lookAt(vec3(0, 0, 0), UnitForward(), UnitUp());
                m_proj = perspective(radians(m_degFOV), m_aspect, m_near, m_far);
            }

            virtual ~Camera() = default;

            void Translate(const vec3& translation, bool translateY = true)
            {
                mat4& view = m_view.GetMatrix();
                vec3 tempPos(view[X][W], view[Y][W], view[Z][W]);
                tempPos += normalize(vec3(view[X][X], translateY ? view[X][Z] : 0.0f, view[Z][X])) * translation.x;
                if (translateY)
                {
                    tempPos += normalize(vec3(view[X][Y], view[Y][Y], view[Z][Y])) * translation.y;
                }
                tempPos += normalize(vec3(view[X][Z], translateY ? view[Y][Z] : 0.0f, view[Z][Z])) * translation.z;
                view *= translate(mat4(1.0f), -tempPos);
            }

            void Rotate(const vec3& eulerDelta, const vec3& forcedUp = Core::Math::UnitUp())
            {
                Transform model = inverse(m_view.GetMatrix());
                quat orig_rot = normalize(quat_cast(model.GetMatrix()));

                bool force = l1Norm(forcedUp) > 0.0f;

                quat yaw = angleAxis(radians(-eulerDelta.x), force ? forcedUp : UpVector(orig_rot));
                quat pitch = angleAxis(radians(-eulerDelta.y), RightVector(orig_rot));
                quat roll = angleAxis(radians(eulerDelta.z), ForwardVector(orig_rot));
                quat rotation = yaw * pitch * roll * orig_rot;

                if (force)
                {
                    const float Correction = 0.1f;
                    float dotVal = dot(UpVector(normalize(rotation)), forcedUp) - Correction;
                    if (dotVal < 0.0f)
                    {
                        float adjustment = (acosf(dotVal) - half_pi<float>())
                            * (dot(forcedUp, ForwardVector(rotation)) < 0.0f ? 1.0f : -1.0f);

                        rotation = angleAxis(adjustment, RightVector(rotation)) * rotation;
                    }
                }

                Transform temp = mat4_cast(rotation);

                temp.SetAxis(model.GetAxis(Index::W), Index::W);

                m_view = inverse(temp.GetMatrix());
            }

            const mat4& GetViewMatrix() const
            {
                return m_view.GetMatrix();
            }

            const mat4& GetProjMatrix() const
            {
                return m_proj;
            }

            vec3 GetPosition() const
            {
                return m_view.GetTranslation();
            }

            vec3 GetForward() const
            {
                return m_view.GetColumn(Index::Z);
            }

            vec3 GetUp() const
            {
                return m_view.GetColumn(Index::Y);
            }

            float GetFOV() const
            {
                return m_degFOV;
            }

            float GetAspect() const
            {
                return m_aspect;
            }

            float GetNear() const
            {
                return m_near;
            }

            float GetFar() const
            {
                return m_far;
            }

            float GetAperture() const
            {
                return m_aperture;
            }

            float GetShutterSpeed() const
            {
                return m_shutterSpeed;
            }

            float GetISO() const
            {
                return m_ISO;
            }

            void SetPosition(const vec3& position)
            {
                m_view.SetTranslation(position);
            }

            void SetForward(const vec3& forward)
            {
                m_view.SetColumn(forward, Index::Z);
            }

            void SetUp(const vec3& up)
            {
                m_view.SetColumn(up, Index::Y);
            }

            void SetPerspective(float fov, float aspect, float n, float f)
            {
                m_degFOV = fov;
                m_aspect = aspect;
                m_near = n;
                m_far = f;
                m_proj = perspective(radians(m_degFOV), m_aspect, m_near, m_far);
            }

            void SetFOV(float fov)
            {
                m_degFOV = fov;
                m_proj = perspective(radians(m_degFOV), m_aspect, m_near, m_far);
            }

            void SetAspect(float aspect)
            {
                m_aspect = aspect;
                m_proj = perspective(radians(m_degFOV), m_aspect, m_near, m_far);
            }

            void SetNear(float nearP)
            {
                m_near = nearP;
                m_proj = perspective(radians(m_degFOV), m_aspect, m_near, m_far);
            }

            void SetFar(float farP)
            {
                m_far = farP;
                m_proj = perspective(radians(m_degFOV), m_aspect, m_near, m_far);
            }

            void SetAperture(float aperture)
            {
                m_aperture = aperture;
            }

            void SetShutterSpeed(float shutterSpeed)
            {
                m_shutterSpeed = shutterSpeed;
            }

            void SetISO(float iso)
            {
                m_ISO = iso;
            }

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

