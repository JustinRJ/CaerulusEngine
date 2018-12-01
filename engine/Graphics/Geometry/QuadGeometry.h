#pragma once

#include "Geometry.h"

namespace Caerulus
{
    namespace Render
    {
        namespace Geometry
        {
            class QuadGeometry : public Geometry
            {
            public:
                static const GLfloat sm_QuadVertices[];

                QuadGeometry(const glm::vec3& scale, const glm::vec3& rot, const glm::vec3& translation);
                virtual ~QuadGeometry();

                void Draw() const override;
            };
        }
    }
}
