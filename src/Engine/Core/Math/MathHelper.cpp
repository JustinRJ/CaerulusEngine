#include "stdafx.h"

#include "MathHelper.h"

namespace Core
{
    namespace Math
    {
        vec3 MathHelper::GetTranslation(const mat4& transform)
        {
            return vec3(
                transform[W][X],
                transform[W][Y],
                transform[W][Z]);
        }

        void MathHelper::SetTranslation(mat4& outTransform, const vec3& translation)
        {
            outTransform[W][X] = translation.x;
            outTransform[W][Y] = translation.y;
            outTransform[W][Z] = translation.z;
        }

        void MathHelper::Translate(mat4& outTransform, const vec3& translation)
        {
            outTransform[W][X] += translation.x;
            outTransform[W][Y] += translation.y;
            outTransform[W][Z] += translation.z;
        }

        vec3 MathHelper::GetScale(const mat4& transform)
        {
            return vec3(
                length(GetAxis(transform, X)),
                length(GetAxis(transform, Y)),
                length(GetAxis(transform, Z)));
        }

        void MathHelper::Scale(mat4& outTransform, const vec3& scale)
        {
            outTransform[X][X] *= scale.x;
            outTransform[Y][Y] *= scale.y;
            outTransform[Z][Z] *= scale.z;
        }

        quat MathHelper::GetRotation(const mat4& transform)
        {
            vec3 scale = GetScale(transform);
            vec3 column[] =
            {
                GetAxis(transform, X),
                GetAxis(transform, Y),
                GetAxis(transform, Z)
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

        void MathHelper::SetRotation(mat4& outTransform, const quat& rotation)
        {
            mat4 scaleM = scale(mat4(1.0f), GetScale(outTransform));
            mat4 rotateM = mat4_cast(normalize(rotation));

            for (unsigned int i = X; i < W; i++)
            {
                for (unsigned int j = X; j < W; j++)
                {
                    outTransform[i][j] = (rotateM * scaleM)[i][j];
                }
            }
        }

        void MathHelper::Rotate(mat4& outTransform, const vec3& rotation)
        {
            if (rotation.y != 0.0f)
            {
                outTransform = rotate(outTransform, radians(rotation.y), UnitRight);
            }
            if (rotation.x != 0.0f)
            {
                outTransform = rotate(outTransform, radians(rotation.x), UnitUp);
            }
            if (rotation.z != 0.0f)
            {
                outTransform = rotate(outTransform, radians(rotation.z), UnitForward);
            }
        }

        vec3 MathHelper::GetAxis(const mat4& transform, Index axisIndex)
        {
            return vec3(
                transform[axisIndex][X],
                transform[axisIndex][Y],
                transform[axisIndex][Z]);
        }

        vec3 MathHelper::GetColumn(const mat4& transform, Index columnIndex)
        {
            return vec3(
                transform[X][columnIndex],
                transform[Y][columnIndex],
                transform[Z][columnIndex]);
        }

        void MathHelper::Decompose(const mat4& transform, vec3& outTranlastion, quat& outRotation, vec3& outScale)
        {
            outScale = GetScale(transform);
            outRotation = GetRotation(transform);
            outTranlastion = GetTranslation(transform);
        }

        void MathHelper::CreateTansform(mat4& outTransform, const vec3& translation, const quat& rotation, const vec3& scale)
        {
            mat4 translateM = translate(mat4(1.0f), translation);
            mat4 rotateM = mat4_cast(normalize(rotation));
            mat4 scaleM = glm::scale(mat4(1.0f), scale);
            outTransform = translateM * rotateM * scaleM;
        }

        vec3 MathHelper::TransformVector(const vec3& vector, const mat4& transform)
        {
            return vec4(vector.x, vector.y, vector.z, 1.0f) * transpose(transform);
        }

        void MathHelper::SetAxis(mat4& outTransform, const vec3& axis, Index axisIndex)
        {
            outTransform[axisIndex][X] = axis.x;
            outTransform[axisIndex][Y] = axis.y;
            outTransform[axisIndex][Z] = axis.z;
        }

        void MathHelper::SetColumn(mat4& outTransform, const vec3& column, Index columnIndex)
        {
            outTransform[X][columnIndex] = column.x;
            outTransform[Y][columnIndex] = column.y;
            outTransform[Z][columnIndex] = column.z;
        }
    }
}