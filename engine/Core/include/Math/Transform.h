#pragma once

#include "Math.h"

namespace Core
{
    namespace Math
    {
        class Transform
        {
        public:
            Transform() :
                m_scale(1.f, 1.f, 1.f),
                m_rotation(0.f, 0.f, 0.f),
                m_translation(0.f, 0.f, 0.f)
            {}

            Transform(const mat4& matrix)
            {
                auto d3 = vec3();
                auto d4 = vec4();
                auto q = quat();
                Math::decompose(matrix, m_scale, q, m_translation, d3, d4);
                // TODO - fix this
                m_rotation = vec3(q.x, q.y, q.z);
            }

            Transform(const vec3& scale, const vec3& rotation, const vec3& translation)
            {
                m_scale = scale;
                m_rotation = rotation;
                m_translation = translation;
            }

            mat4 GetMatrix() const
            {
                mat4 matrix(1.f);
                matrix = scale(matrix, m_scale);
                matrix = matrix * toMat4(quat(m_rotation));
                matrix = translate(matrix, m_translation);
                return matrix;
            }

            const vec3& GetScale() const
            {
                return m_scale;
            }

            void SetScale(const vec3& scale)
            {
                m_scale = scale;
            }

            void Scale(const vec3& scale)
            {
                m_scale *= scale;
            }

            const vec3& GetRotation() const
            {
                return m_rotation;
            }

            void SetRotation(const vec3& rotation)
            {
                m_rotation = rotation;
            }

            void Rotate(const vec3& rotation)
            {
                m_rotation += m_rotation;
            }

            const vec3& GetTranslation() const
            {
                return m_translation;
            }

            void SetTranslation(const vec3& translation)
            {
                m_translation = translation;
            }

            void Translate(const vec3& translation)
            {
                m_translation += translation;
            }

        private:
            vec3 m_scale;
            vec3 m_rotation;
            vec3 m_translation;
        };

        inline Transform operator*(const Transform& lhs, const Transform& rhs)
        {
            return Transform(
                lhs.GetScale() * rhs.GetScale(),
                lhs.GetRotation() * rhs.GetRotation(),
                lhs.GetTranslation() * rhs.GetTranslation());
        }
    }
}
