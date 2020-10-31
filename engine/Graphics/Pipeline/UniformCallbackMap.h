#pragma once

#include "Core/Math/Math.h"

namespace Graphics
{
    namespace Pipeline
    {
        struct UniformCallbackMap
        {
            std::map<std::string, std::function<GLint()>> IntCallbacks;
            std::map<std::string, std::function<GLfloat()>> FloatCallbacks;
            std::map<std::string, std::function<glm::fvec2()>> Vec2Callbacks;
            std::map<std::string, std::function<glm::fvec3()>> Vec3Callbacks;
            std::map<std::string, std::function<glm::fvec4()>> Vec4Callbacks;
            std::map<std::string, std::function<glm::mat3()>> Mat3Callbacks;
            std::map<std::string, std::function<glm::mat4()>> Mat4Callbacks;
        };
    }
}