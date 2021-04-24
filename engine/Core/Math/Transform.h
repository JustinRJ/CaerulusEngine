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
            ~Transform() = default;

            Transform(const mat4& m) :
                m_T(m)
            {}

            vec3 GetTranslation() const
            {
                return vec3(
                    m_T[W][X],
                    m_T[W][Y],
                    m_T[W][Z]);
            }

            void SetTranslation(const vec3& translation)
            {
                m_T[W][X] = translation.x;
                m_T[W][Y] = translation.y;
                m_T[W][Z] = translation.z;
            }

            void Translate(const vec3& translation)
            {
                m_T[W][X] += translation.x;
                m_T[W][Y] += translation.y;
                m_T[W][Z] += translation.z;
            }

            vec3 GetScale() const
            {
                return vec3(
                    length(GetAxis(X)),
                    length(GetAxis(Y)),
                    length(GetAxis(Z)));
            }

            void Scale(const vec3& scale)
            {
                m_T[X][X] *= scale.x;
                m_T[Y][Y] *= scale.y;
                m_T[Z][Z] *= scale.z;
            }

            quat GetRotation() const
            {
                vec3 scale = GetScale();
                vec3 column[] =
                {
                    GetAxis(X),
                    GetAxis(Y),
                    GetAxis(Z)
                };

                if (scale.x > 0.0f)
                {
                    column[X] /= scale.x;
                }
                if (scale.y > 0.0f)
                {
                    column[Y] /= scale.y;
                }
                if (scale.z > 0.0f)
                {
                    column[Z] /= scale.z;
                }

                return quat(mat3(
                    column[X][X], column[Y][X], column[Z][X],
                    column[X][Y], column[Y][Y], column[Z][Y],
                    column[X][Z], column[Y][Z], column[Z][Z]));
            }

            void SetRotation(const quat& rotation)
            {
                mat4 scaleM = scale(mat4(1.0f), GetScale());
                mat4 rotateM = mat4_cast(normalize(rotation));

                for (unsigned int i = X; i < W; ++i)
                {
                    for (unsigned int j = X; j < W; ++j)
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
                    m_T[axisIndex][X],
                    m_T[axisIndex][Y],
                    m_T[axisIndex][Z]);
            }

            vec3 GetColumn(Index columnIndex) const
            {
                return vec3(
                    m_T[X][columnIndex],
                    m_T[Y][columnIndex],
                    m_T[Z][columnIndex]);
            }

            void Decompose(vec3& outTranslation, quat& outRotation, vec3& outScale) const
            {
                outScale = GetScale();
                outRotation = GetRotation();
                outTranslation = GetTranslation();
            }

            void SetAxis(const vec3& axis, Index axisIndex)
            {
                m_T[axisIndex][X] = axis.x;
                m_T[axisIndex][Y] = axis.y;
                m_T[axisIndex][Z] = axis.z;
            }

            void SetColumn(const vec3& column, Index columnIndex)
            {
                m_T[X][columnIndex] = column.x;
                m_T[Y][columnIndex] = column.y;
                m_T[Z][columnIndex] = column.z;
            }

            mat4& GetMatrix()
            {
                return m_T;
            }

            const mat4& GetMatrix() const
            {
                return m_T;
            }

        private:
            mat4 m_T;
        };
    }
}
