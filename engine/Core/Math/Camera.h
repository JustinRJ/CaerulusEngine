#pragma once

#include "Transform.h"
#include "Perspective.h"

namespace Core
{
    namespace Math
    {
        class Camera
        {
        public:
            Camera() = default;
            ~Camera() = default;

            Camera(const vec3& position, const vec3& forward, const vec3& up = UnitUp) :
                m_transform(lookAt(position, Math::normalize(position + forward), up)),
                m_perspective(54.0f, 1.25f, 1.0f, 1000.0f)
            {}

            void Translate(const vec3& translation, bool translateY = true)
            {
                mat4 view = m_transform.GetMatrix();
                vec3 temp(view[X][W], view[Y][W], view[Z][W]);
                temp += normalize(vec3(view[X][X], translateY ? view[X][Z] : 0.0f, view[Z][X])) * translation.x;
                if (translateY)
                {
                    temp += normalize(vec3(view[X][Y], view[Y][Y], view[Z][Y])) * translation.y;
                }
                temp += normalize(vec3(view[X][Z], translateY ? view[Y][Z] : 0.0f, view[Z][Z])) * translation.z;
                view *= translate(mat4(1.0f), -temp);
                m_transform.SetMatrix(view);
            }

            void Rotate(const vec3& eulerDelta, const vec3& forcedUp = UnitUp)
            {
                Transform transform = inverse(m_transform.GetMatrix());
                quat orig_rot = normalize(quat_cast(transform.GetMatrix()));

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

                temp.SetAxis(transform.GetAxis(Index::W), Index::W);

                m_transform = inverse(temp.GetMatrix());
            }

            const Transform& GetTransform() const
            {
                return m_transform;
            }

            Transform& GetTransform()
            {
                return m_transform;
            }

            const Perspective& GetPerspective() const
            {
                return m_perspective;
            }

            Perspective& GetPerspective()
            {
                return m_perspective;
            }

        private:
            Transform m_transform;
            Perspective m_perspective;
        };
    }
}

