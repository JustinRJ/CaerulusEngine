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

        CAERULUS_CORE vec3 UnitUp();
        CAERULUS_CORE vec3 UnitRight();
        CAERULUS_CORE vec3 UnitForward();

        CAERULUS_CORE vec3 UpVector(const quat& q);
        CAERULUS_CORE vec3 RightVector(const quat& q);
        CAERULUS_CORE vec3 ForwardVector(const quat& q);

        CAERULUS_CORE vec3 GetScale(const mat4& transform);
        CAERULUS_CORE void Scale(mat4& outTransform, const vec3& scale);

        CAERULUS_CORE quat GetRotation(const mat4& transform);
        CAERULUS_CORE void SetRotation(mat4& outTransform, const quat& rotation);
        CAERULUS_CORE void Rotate(mat4& outTransform, const vec3& rotation);

        CAERULUS_CORE vec3 GetTranslation(const mat4& transform);
        CAERULUS_CORE void SetTranslation(mat4& outTransform, const vec3& translation);
        CAERULUS_CORE void Translate(mat4& outTransform, const vec3& translation);

        CAERULUS_CORE vec3 GetAxis(const mat4& transform, Index axisIndex);
        CAERULUS_CORE void SetAxis(mat4& outTransform, const vec3& axis, Index axisIndex);

        CAERULUS_CORE vec3 GetColumn(const mat4& transform, Index columnIndex);
        CAERULUS_CORE void SetColumn(mat4& outTransform, const vec3& column, Index columnIndex);

        CAERULUS_CORE vec3 TransformVector(const vec3& vector, const mat4& transform);
        CAERULUS_CORE void Decompose(const mat4& transform, vec3& outTranslation, quat& outRotation, vec3& outScale);
        CAERULUS_CORE void CreateTansform(mat4& outTransform, const vec3& translation, const quat& rotation, const vec3& scale);

        CAERULUS_CORE float Dot(const vec3& left, const vec3& right);
        CAERULUS_CORE vec3 Cross(const vec3& left, const vec3& right);
    }
}
