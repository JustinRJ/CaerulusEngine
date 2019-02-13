#pragma once

#include <glew.h>
#include "../../Core/Math/MathFacade.h"

namespace Graphics
{
    namespace Geometry
    {
        using namespace Core::Math;

        class Geometry
        {
        public:
            virtual void Draw() const = 0;

            GLuint GetVAO() const;
            GLuint GetVBO() const;

            void SetTransform(const mat4& transform);
            const mat4& GetTransform() const;

        protected:
            Geometry();
            Geometry(const mat4& transform);
            virtual ~Geometry();

            virtual void GenerateBuffer() = 0;

            GLuint m_VAO = NULL;
            GLuint m_VBO = NULL;
            mat4 m_Transform;
        };
    }
}
