#pragma once

#include "Math.h"

namespace Core
{
    namespace Math
    {
        class Transform
        {
        public:
            Transform() = default;

            Transform(const vec3& scale, const vec3& rotation, const vec3& translation) :
                m_isDirty(true),
                m_scale(scale),
                m_rotation(rotation),
                m_translation(translation)
            {}

            const mat4& GetMatrix() const
            {
                if (m_isDirty)
                {
                    m_isDirty = false;
                    m_matrix = scale(mat4(1.f), m_scale);
                    m_matrix = m_matrix * toMat4(quat(m_rotation));
                    m_matrix = translate(m_matrix, m_translation);
                }
                return m_matrix;
            }

            const vec3& GetScale() const
            {
                return m_scale;
            }

            void SetScale(const vec3& scale)
            {
                m_isDirty = true;
                m_scale = scale;
            }

            void Scale(const vec3& scale)
            {
                m_isDirty = true;
                m_scale *= scale;
            }

            const vec3& GetRotation() const
            {
                return m_rotation;
            }

            void SetRotation(const vec3& rotation)
            {
                m_isDirty = true;
                m_rotation = rotation;
            }

            void Rotate(const vec3& rotation)
            {
                m_isDirty = true;
                m_rotation += rotation;
            }

            const vec3& GetTranslation() const
            {
                return m_translation;
            }

            void SetTranslation(const vec3& translation)
            {
                m_isDirty = true;
                m_translation = translation;
            }

            void Translate(const vec3& translation)
            {
                m_isDirty = true;
                m_translation += translation;
            }

        private:
            mutable bool m_isDirty = false;
            vec3 m_scale = vec3(1.0);
            // Stored in euler radians
            vec3 m_rotation = vec3(0.0);
            vec3 m_translation = vec3(0.0);
            mutable mat4 m_matrix = mat4(1.f);
        };

        inline Transform operator*(const Transform& lhs, const Transform& rhs)
        {
            return Transform(
                lhs.GetScale() * rhs.GetScale(),
                lhs.GetRotation() + rhs.GetRotation(),
                lhs.GetTranslation() + rhs.GetTranslation());
        }
    }
}
