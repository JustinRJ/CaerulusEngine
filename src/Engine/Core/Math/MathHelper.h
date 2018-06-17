#pragma once

#define CORE_MATH __declspec(dllexport)

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/type_ptr.hpp>

namespace Core
{
    namespace Math
    {
        using namespace glm;

        enum Index
        {
            X = 0,
            Y = 1,
            Z = 2,
            W = 3
        };

        const vec3 UnitUp      (0.0f, 1.0f, 0.0f);
        const vec3 UnitRight   (1.0f, 0.0f, 0.0f);
        const vec3 UnitForward (0.0f, 0.0f, -1.0f);

        class MathHelper
        {
        public:

            static CORE_MATH vec3 UpVector(const quat& q)
            {
                return normalize(rotate(q, UnitUp));
            }
            static CORE_MATH vec3 RightVector(const quat& q)
            {
                return normalize(rotate(q, UnitRight));
            }
            static CORE_MATH vec3 ForwardVector(const quat& q)
            {
                return normalize(rotate(q, UnitForward));
            }

            static CORE_MATH vec3 GetScale(const mat4& transform);
            static void Scale(mat4& outTransform, const vec3& scale);

            static CORE_MATH quat GetRotation(const mat4& transform);
            static void SetRotation(mat4& outTransform, const quat& rotation);
            static void Rotate(mat4& outTransform, const vec3& rotation);

            static CORE_MATH vec3 GetTranslation(const mat4& transform);
            static void SetTranslation(mat4& outTransform, const vec3& translation);
            static void Translate(mat4& outTransform, const vec3& translation);

            static CORE_MATH vec3 GetAxis(const mat4& transform, Index axisIndex);
            static void SetAxis(mat4& outTransform, const vec3& axis, Index axisIndex);

            static CORE_MATH vec3 GetColumn(const mat4& transform, Index columnIndex);
            static void SetColumn(mat4& outTransform, const vec3& column, Index columnIndex);

            static void Decompose(const mat4& transform, vec3& outTranslation, quat& outRotation, vec3& outScale);
            static void CreateTansform(mat4& outTransform, const vec3& translation, const quat& rotation, const vec3& scale);
            static void TransformVector(vec3& outVector, const mat4& transform);
        };
    }
}
