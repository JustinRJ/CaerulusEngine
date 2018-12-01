#pragma once

#include "Geometry.h"

namespace Caerulus
{
    namespace Render
    {
        namespace Geometry
        {
            class CubeGeometry : public Geometry
            {
            public:
                static const GLfloat sm_CubeVertices[];

                CubeGeometry(const glm::vec3& scale, const glm::vec3& rot, const glm::vec3& translation);
                virtual ~CubeGeometry();

                void Draw() const override;
            };
        }
    }
}
