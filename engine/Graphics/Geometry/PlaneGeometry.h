#pragma once

#include "Geometry.h"

namespace Graphics
{
    namespace Geometry
    {
        class PlaneGeometry : public Geometry
        {
        public:
            static const GLfloat s_PlaneVertices[];

            PlaneGeometry(const glm::mat4& transform);
            virtual ~PlaneGeometry();

            void Draw() const override;
        };
    }
}