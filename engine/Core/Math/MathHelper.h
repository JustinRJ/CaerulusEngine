#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#define GLM_ENABLE_EXPERIMENTAL

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

        class CAERULUS_CORE MathHelper
        {
        public:

            static vec3 UnitUp();
            static vec3 UnitRight();
            static vec3 UnitForward();

            static vec3 UpVector(const quat& q);
            static vec3 RightVector(const quat& q);
            static vec3 ForwardVector(const quat& q);

            static vec3 GetScale(const mat4& transform);
            static void Scale(mat4& outTransform, const vec3& scale);

            static quat GetRotation(const mat4& transform);
            static void SetRotation(mat4& outTransform, const quat& rotation);
            static void Rotate(mat4& outTransform, const vec3& rotation);

            static vec3 GetTranslation(const mat4& transform);
            static void SetTranslation(mat4& outTransform, const vec3& translation);
            static void Translate(mat4& outTransform, const vec3& translation);

            static vec3 GetAxis(const mat4& transform, Index axisIndex);
            static void SetAxis(mat4& outTransform, const vec3& axis, Index axisIndex);

            static vec3 GetColumn(const mat4& transform, Index columnIndex);
            static void SetColumn(mat4& outTransform, const vec3& column, Index columnIndex);

            static vec3 TransformVector(const vec3& vector, const mat4& transform);
            static void Decompose(const mat4& transform, vec3& outTranslation, quat& outRotation, vec3& outScale);
            static void CreateTansform(mat4& outTransform, const vec3& translation, const quat& rotation, const vec3& scale);
        };
    }
}
