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
            Camera() :
                m_view(1),
                m_perspective()
            {}

            void Translate(const vec3& translation, bool translateY = true)
            {
                vec3 temp(m_view[0][3], m_view[1][3], m_view[2][3]);
                temp += normalize(vec3(m_view[0][0], translateY ? m_view[0][2] : 0.0f, m_view[2][0])) * translation.x;
                if (translateY)
                {
                    temp += normalize(vec3(m_view[0][1], m_view[1][1], m_view[2][1])) * translation.y;
                }
                temp += normalize(vec3(m_view[0][2], translateY ? m_view[1][2] : 0.0f, m_view[2][2])) * translation.z;
                m_view *= translate(mat4(1.0f), -temp);
            }

            void Rotate(const vec3& eulerDelta, const vec3& forcedUp = UnitUp)
            {
                mat4 transform = inverse(m_view);
                quat orig_rot = normalize(quat_cast(transform));

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

                mat4 temp = mat4_cast(rotation);
                temp[3] = transform[3];
                m_view = inverse(temp);
            }

            vec3 GetTranslation() const
            {
                return m_view[3];
            }

            const mat4& GetView() const
            {
                return m_view;
            }

            void SetView(const vec3& position, const vec3& forward, const vec3& up = UnitUp)
            {
                m_view = lookAt(position, Math::normalize(position + forward), up);
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
            mat4 m_view;
            Perspective m_perspective;
        };
    }
}

