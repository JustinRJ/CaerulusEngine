#pragma once

namespace Core
{
    namespace Math
    {
        using namespace glm;

        const double PI = std::atan(1.f) * 4.f;

        const vec3 UnitUp = vec3(0.f, 1.f, 0.f);
        const vec3 UnitRight = vec3(0.f, 1.f, 0.f);
        const vec3 UnitForward = vec3(0.f, 1.f, 0.f);

        enum Index
        {
            X = 0,
            Y = 1,
            Z = 2,
            W = 3
        };

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
    }
}
