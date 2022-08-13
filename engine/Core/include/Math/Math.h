#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm.hpp>
#include <gtx/hash.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/matrix_decompose.hpp>

#include <vector>
#include <stdint.h>

namespace Core
{
    namespace Math
    {
        using namespace glm;

        const float PIf = std::atan(1.f) * 4.f;
        const double PI = std::atan(1.) * 4.;

        constexpr vec3 UnitUp = vec3(0.f, 1.f, 0.f);
        constexpr vec3 UnitRight = vec3(1.f, 0.f, 0.f);
        constexpr vec3 UnitForward = vec3(0.f, 0.0, -1.f);

        inline vec3 UpVector(const quat& q)
        {
            return normalize(rotate(q, UnitUp));
        }

        inline vec3 RightVector(const quat& q)
        {
            return normalize(rotate(q, UnitRight));
        }

        inline vec3 ForwardVector(const quat& q)
        {
            return normalize(rotate(q, UnitForward));
        }

        inline double ToDegrees(double radians)
        {
            return (180.f / PI) * radians;
        }

        inline double ToRadians(double degrees)
        {
            return (180.f / PI) / degrees;
        }

        inline vec3 TransformVector(const mat4& mat, const vec3& vector)
        {
            return vec4(vector.x, vector.y, vector.z, 1.0f) * transpose(mat);
        }

        inline vec2 TransformVector(const mat3& mat, const vec2& vector)
        {
            return vec3(vector.x, vector.y, 1.0f) * transpose(mat);
        }

        inline mat4 CreateTansform(const vec3& t, const quat& r, const vec3& s)
        {
            mat4 translateM = translate(mat4(1.0f), t);
            mat4 rotateM = mat4_cast(normalize(r));
            mat4 scaleM = scale(mat4(1.0f), s);
            return translateM * rotateM * scaleM;
        }

        // https://en.wikipedia.org/wiki/Fast_inverse_square_root
        inline float FastInvSqrt(float number)
        {
            const float x2 = number * 0.5F;
            const float threehalfs = 1.5F;

            union
            {
                float f;
                uint32_t i;
            } conv = { number };
            conv.i = 0x5f3759df - (conv.i >> 1);
            conv.f *= threehalfs - (x2 * conv.f * conv.f);
            return conv.f;
        }

        inline bool IsIntersecting2D(const std::vector<vec2>& corners1, const std::vector<vec2>& corners2)
        {
            for (unsigned int i = 0u; i < corners1.size(); ++i)
            {
                vec2 current = corners1[i];
                vec2 next = corners1[i == corners1.size() - 1 ? 0 : i + 1];
                vec2 edge = next - current;
                vec2 axis = { -edge[1], edge[0] };

                float aMinProj = std::numeric_limits<float>::max();
                float bMinProj = std::numeric_limits<float>::max();
                float aMaxProj = std::numeric_limits<float>::min();
                float bMaxProj = std::numeric_limits<float>::min();

                for (const vec2& v : corners1)
                {
                    float proj = dot(axis, v);
                    aMinProj = std::min(aMinProj, proj);
                    aMaxProj = std::max(aMaxProj, proj);
                }

                for (const vec2& v : corners2)
                {
                    float proj = dot(axis, v);
                    bMinProj = std::min(bMinProj, proj);
                    bMaxProj = std::max(bMaxProj, proj);
                }

                if (aMaxProj < bMinProj || aMinProj > bMaxProj)
                {
                    return true;
                }
            }
            return false;
        }

        inline bool IsIntersecting3D(const std::vector<vec3> corners1, const std::vector<vec3> corners2)
        {
            for (unsigned int i = 0u; i < corners1.size(); ++i)
            {
                vec3 current = corners1[i];
                vec3 next = corners1[i == corners1.size() - 1 ? 0 : i + 1];
                vec3 last = corners1[i == 0 ? corners1.size() - 1 : i - 1];
                vec3 edge1 = next - current;
                vec3 edge2 = last - current;
                vec3 axis = normalize(cross(edge1, edge2));

                float aMinProj = std::numeric_limits<float>::max();
                float bMinProj = std::numeric_limits<float>::max();
                float aMaxProj = std::numeric_limits<float>::min();
                float bMaxProj = std::numeric_limits<float>::min();

                for (const vec3& v : corners1)
                {
                    float proj = dot(axis, v);
                    aMinProj = std::min(aMinProj, proj);
                    aMaxProj = std::max(aMaxProj, proj);
                }

                for (const vec3& v : corners2)
                {
                    float proj = dot(axis, v);
                    bMinProj = std::min(bMinProj, proj);
                    bMaxProj = std::max(bMaxProj, proj);
                }

                if (aMaxProj < bMinProj || aMinProj > bMaxProj)
                {
                    return true;
                }
            }
            return false;
        }
    }
}
