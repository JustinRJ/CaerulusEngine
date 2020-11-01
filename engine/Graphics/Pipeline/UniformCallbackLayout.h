#pragma once

#include "Core/Math/Math.h"

namespace Graphics
{
    namespace Pipeline
    {
        template<typename T>
        struct UniformCallbackMap
        {
            std::map<std::string, std::function<T>> Map;
        };

        struct UniformCallbackLayout
        {
            UniformCallbackMap<GLint()> I1Callbacks;
            UniformCallbackMap<GLfloat()> F1Callbacks;
            UniformCallbackMap<Core::Math::fvec2()> F2Callbacks;
            UniformCallbackMap<Core::Math::fvec3()> F3Callbacks;
            UniformCallbackMap<Core::Math::fvec4()> F4Callbacks;
            UniformCallbackMap<Core::Math::mat3()> Mat3Callbacks;
            UniformCallbackMap<Core::Math::mat4()> Mat4Callbacks;
        };
    }
}