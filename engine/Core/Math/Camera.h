#pragma once

#include "Frustrum.h"
#include "Transform.h"

namespace Core
{
    namespace Math
    {
        class Camera
        {
        public:
            Camera() = delete;
            ~Camera() = default;

            Camera(const vec3& position, const vec3& forward, const vec3& up = UnitUp) :
                m_proj(54.0f, 1.25f, 1.0f, 1000.0f),
                m_view(lookAt(position, glm::normalize(position + forward), up))
            {}

            Camera(const Camera& camera) :
                m_proj(camera.m_proj),
                m_view(camera.m_view)
            {}

            Camera& operator=(const Camera& camera)
            {
                m_proj = camera.m_proj;
                m_view = camera.m_view;
                return *this;
            }

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

            void Rotate(const vec3& eulerDelta, const vec3& forcedUp = UnitUp)
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

            const Transform& GetTransform() const
            {
                return m_view;
            }

            Transform& GetTransform()
            {
                return m_view;
            }

            const Frustrum& GetFrustrum() const
            {
                return m_proj;
            }

            Frustrum& GetFrustrum()
            {
                return m_proj;
            }

        private:

            Frustrum m_proj;
            Transform m_view;
        };
    }
}

