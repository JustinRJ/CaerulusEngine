#pragma once

#include "Math.h"

namespace Core
{
    namespace Math
    {
        class Perspective
        {
        public:
            Perspective() = delete;
            ~Perspective() = default;

            Perspective(float degFOV, float aspect, float inear, float ifar)
            {
                SetPerspective(degFOV, aspect, inear, ifar);
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

            const mat4& GetMatrix() const
            {
                return m_perspective;
            }

            void SetPerspective(float degFOV, float aspect, float inear, float ifar)
            {
                m_degFOV = degFOV;
                m_aspect = aspect;
                m_near = inear;
                m_far = ifar;
                m_perspective = perspective(radians(m_degFOV), m_aspect, m_near, m_far);
            }

            void SetDegFOV(float degFOV)
            {
                m_degFOV = degFOV;
                m_perspective = perspective(radians(m_degFOV), m_aspect, m_near, m_far);
            }

            void SetAspect(float aspect)
            {
                m_aspect = aspect;
                m_perspective = perspective(radians(m_degFOV), m_aspect, m_near, m_far);
            }

            void SetNear(float nearP)
            {
                m_near = nearP;
                m_perspective = perspective(radians(m_degFOV), m_aspect, m_near, m_far);
            }

            void SetFar(float farP)
            {
                m_far = farP;
                m_perspective = perspective(radians(m_degFOV), m_aspect, m_near, m_far);
            }

        private:
            float m_degFOV;
            float m_aspect;
            float m_near;
            float m_far;
            mat4 m_perspective;
        };
    }
}
