#pragma once

#include "Math.h"

namespace Core
{
    namespace Math
    {
        class Frustrum
        {
        public:
            Frustrum() = delete;
            ~Frustrum() = default;

            Frustrum(float fov, float aspect, float n, float f) :
                m_degFOV(fov),
                m_aspect(aspect),
                m_near(n),
                m_far(f),
                m_P(perspective(radians(m_degFOV), m_aspect, m_near, m_far))
            {}

            Frustrum(const mat4& m) :
                m_P(m)
            {}

            Frustrum(const Frustrum& frustrum) :
                m_P(frustrum.m_P)
            {}

            Frustrum(Frustrum&& frustrum) :
                m_P(std::move(frustrum.m_P))
            {}

            Frustrum& operator=(const Frustrum& frustrum)
            {
                m_P = frustrum.m_P;
                return *this;
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

            mat4& GetMatrix()
            {
                return m_P;
            }

            const mat4& GetMatrix() const
            {
                return m_P;
            }

            void SetPerspective(float fov, float aspect, float n, float f)
            {
                m_degFOV = fov;
                m_aspect = aspect;
                m_near = n;
                m_far = f;
                m_P = perspective(radians(m_degFOV), m_aspect, m_near, m_far);
            }

            void SetFOV(float fov)
            {
                m_degFOV = fov;
                m_P = perspective(radians(m_degFOV), m_aspect, m_near, m_far);
            }

            void SetAspect(float aspect)
            {
                m_aspect = aspect;
                m_P = perspective(radians(m_degFOV), m_aspect, m_near, m_far);
            }

            void SetNear(float nearP)
            {
                m_near = nearP;
                m_P = perspective(radians(m_degFOV), m_aspect, m_near, m_far);
            }

            void SetFar(float farP)
            {
                m_far = farP;
                m_P = perspective(radians(m_degFOV), m_aspect, m_near, m_far);
            }

        private:
            float m_degFOV;
            float m_aspect;
            float m_near;
            float m_far;
            mat4 m_P;
        };
    }
}
