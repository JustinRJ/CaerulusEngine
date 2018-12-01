#pragma once

#include "Geometry.h"

namespace Caerulus
{
    namespace Render
    {
        namespace Geometry
        {
            class PlaneGeometry : public Geometry
            {
            public:
                static const GLfloat sm_PlaneVertices[];

                PlaneGeometry(const glm::vec3& scale, const glm::vec3& rot, const glm::vec3& translation);
                virtual ~PlaneGeometry();

                void Draw() const override;
            };
        }
    }
}
