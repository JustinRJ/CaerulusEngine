#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#define GLM_ENABLE_EXPERIMENTAL

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/hash.hpp>

namespace Core
{
    namespace Math
    {
        using namespace glm;

        const double PI = std::atan(1.) * 4.;

        enum Index
        {
            X = 0,
            Y = 1,
            Z = 2,
            W = 3
        };

        inline vec3 UnitUp()
        {
            return vec3(0., 1., 0.);
        }

        inline vec3 UnitRight()
        {
            return vec3(1., 0., 0.);
        }

        inline vec3 UnitForward()
        {
            return vec3(0., 0., -1.);
        }

        inline vec3 UpVector(const quat& q)
        {
            return normalize(rotate(q, UnitUp()));
        }

        inline vec3 RightVector(const quat& q)
        {
            return normalize(rotate(q, UnitRight()));
        }

        inline vec3 ForwardVector(const quat& q)
        {
            return normalize(rotate(q, UnitForward()));
        }

        inline double ToDegrees(double radians)
        {
            return (180. / PI) * radians;
        }

        inline double ToRadians(double degrees)
        {
            return (180. / PI) / degrees;
        }

        inline mat4 CreateTansform(const vec3& translation, const quat& rotation, const vec3& scale)
        {
            mat4 translateM = translate(mat4(1.0f), translation);
            mat4 rotateM = mat4_cast(normalize(rotation));
            mat4 scaleM = glm::scale(mat4(1.0f), scale);
            return translateM * rotateM * scaleM;
        }
    }
}
