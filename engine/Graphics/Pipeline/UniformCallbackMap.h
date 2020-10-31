#pragma once

#include "Core/Math/Math.h"

namespace Graphics
{
    namespace Pipeline
    {
        struct UniformCallbackMap
        {
            std::map<std::string, std::function<GLint()>> I1Callbacks;
            std::map<std::string, std::function<GLfloat()>> F1Callbacks;
            std::map<std::string, std::function<Core::Math::fvec2()>> F2Callbacks;
            std::map<std::string, std::function<Core::Math::fvec3()>> F3Callbacks;
            std::map<std::string, std::function<Core::Math::fvec4()>> F4Callbacks;
            std::map<std::string, std::function<Core::Math::mat3()>> Mat3Callbacks;
            std::map<std::string, std::function<Core::Math::mat4()>> Mat4Callbacks;
        };
    }
}