#pragma once

#include <glew.h>
#include "../../Core/Math/MathHelper.h"

namespace Graphics
{
    namespace Geometry
    {
        using namespace Core::Math;

        class Geometry
        {
        public:
            Geometry();
            Geometry(const mat4& transform);
            virtual ~Geometry() = default;

            virtual void Draw() const = 0;

            GLuint GetVAO() const;
            GLuint GetVBO() const;

            void SetTransform(const mat4& transform);
            const mat4& GetTransform() const;

        protected:

            virtual void GenerateBuffer() = 0;

            GLuint m_VAO = 0;
            GLuint m_VBO = 0;
            mat4 m_Transform;
        };
    }
}
