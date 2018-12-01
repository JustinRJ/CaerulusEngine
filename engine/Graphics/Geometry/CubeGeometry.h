#pragma once

#include "Geometry.h"

namespace Graphics
{
    namespace Geometry
    {
        class CubeGeometry : public Geometry
        {
        public:
            static const GLfloat s_CubeVertices[];

            CubeGeometry(const glm::mat4& transform);
            virtual ~CubeGeometry();

            void Draw() const override;
        };
    }
}
