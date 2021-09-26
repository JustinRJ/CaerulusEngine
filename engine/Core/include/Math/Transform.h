#pragma once

#include "Math.h"

namespace Core
{
    namespace Math
    {
        enum class Index
        {
            X = 0,
            Y = 1,
            Z = 2,
            W = 3
        };

        class Transform
        {
        public:
            Transform() : m_T(1.0f)
            {}

            Transform(const mat4& m) :
                m_T(m)
            {}

            Transform& operator=(mat4&& mat)
            {
                m_T = std::move(mat);
                return *this;
            }

            operator const mat4&() const
            {
                return m_T;
            }

            vec3 GetTranslation() const
            {
                return vec3(
                    m_T[3][0],
                    m_T[3][1],
                    m_T[3][2]);
            }

            void SetTranslation(const vec3& translation)
            {
                m_T[3][0] = translation.x;
                m_T[3][1] = translation.y;
                m_T[3][2] = translation.z;
            }

            void Translate(const vec3& translation)
            {
                m_T[3][0] += translation.x;
                m_T[3][1] += translation.y;
                m_T[3][2] += translation.z;
            }

            vec3 GetScale() const
            {
                return vec3(
                    length(GetAxis(Index::X)),
                    length(GetAxis(Index::Y)),
                    length(GetAxis(Index::Z)));
            }

            void Scale(const vec3& scale)
            {
                m_T[0][0] *= scale.x;
                m_T[1][1] *= scale.y;
                m_T[2][2] *= scale.z;
            }

            quat GetRotation() const
            {
                vec3 scale = GetScale();
                vec3 column[] =
                {
                    GetAxis(Index::X),
                    GetAxis(Index::Y),
                    GetAxis(Index::Z)
                };

                if (scale.x > 0.0f)
                {
                    column[0] /= scale.x;
                }
                if (scale.y > 0.0f)
                {
                    column[1] /= scale.y;
                }
                if (scale.z > 0.0f)
                {
                    column[2] /= scale.z;
                }

                return quat(mat3(
                    column[0][0], column[1][0], column[2][0],
                    column[0][1], column[1][1], column[2][1],
                    column[0][2], column[1][2], column[2][2]));
            }

            void SetRotation(const quat& rotation)
            {
                mat4 scaleM = scale(mat4(1.0f), GetScale());
                mat4 rotateM = mat4_cast(normalize(rotation));

                for (unsigned int i = 0; i < 3; ++i)
                {
                    for (unsigned int j = 0; j < 3; ++j)
                    {
                        m_T[i][j] = (rotateM * scaleM)[i][j];
                    }
                }
            }

            void Rotate(const vec3& rotation)
            {
                if (rotation.y != 0.0f)
                {
                    m_T = rotate(m_T, radians(rotation.y), UnitRight);
                }
                if (rotation.x != 0.0f)
                {
                    m_T = rotate(m_T, radians(rotation.x), UnitUp);
                }
                if (rotation.z != 0.0f)
                {
                    m_T = rotate(m_T, radians(rotation.z), UnitForward);
                }
            }

            vec3 GetAxis(Index axisIndex) const
            {
                return vec3(
                    m_T[static_cast<unsigned int>(axisIndex)][0],
                    m_T[static_cast<unsigned int>(axisIndex)][1],
                    m_T[static_cast<unsigned int>(axisIndex)][2]);
            }

            vec3 GetColumn(Index columnIndex) const
            {
                return vec3(
                    m_T[0][static_cast<unsigned int>(columnIndex)],
                    m_T[1][static_cast<unsigned int>(columnIndex)],
                    m_T[2][static_cast<unsigned int>(columnIndex)]);
            }

            void Decompose(vec3& outTranslation, quat& outRotation, vec3& outScale) const
            {
                outScale = GetScale();
                outRotation = GetRotation();
                outTranslation = GetTranslation();
            }

            void SetAxis(const vec3& axis, Index axisIndex)
            {
                m_T[static_cast<unsigned int>(axisIndex)][0] = axis.x;
                m_T[static_cast<unsigned int>(axisIndex)][1] = axis.y;
                m_T[static_cast<unsigned int>(axisIndex)][2] = axis.z;
            }

            void SetColumn(const vec3& column, Index columnIndex)
            {
                m_T[0][static_cast<unsigned int>(columnIndex)] = column.x;
                m_T[1][static_cast<unsigned int>(columnIndex)] = column.y;
                m_T[2][static_cast<unsigned int>(columnIndex)] = column.z;
            }

        private:
            mat4 m_T;
        };

        inline Transform operator*(const Transform& lhs, const Transform& rhs)
        {
            return static_cast<mat4>(lhs) * static_cast<mat4>(rhs);
        }

        inline Transform operator/(const Transform& lhs, const Transform& rhs)
        {
            return static_cast<mat4>(lhs) / static_cast<mat4>(rhs);
        }

        inline Transform operator+(const Transform& lhs, const Transform& rhs)
        {
            return static_cast<mat4>(lhs) + static_cast<mat4>(rhs);
        }

        inline Transform operator-(const Transform& lhs, const Transform& rhs)
        {
            return static_cast<mat4>(lhs) - static_cast<mat4>(rhs);
        }
    }
}
