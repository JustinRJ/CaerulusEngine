#pragma once

#include "Geometry.h"

namespace Graphics
{
    namespace Geometry
    {
        class CAERULUS_GRAPHICS Cube : public Geometry
        {
        public:
            static const GLfloat s_cubeVertices[];

            Cube();
            Cube(const mat4& transform);
            virtual ~Cube() = default;

            void Draw(bool wireframe) const override;

        private:
            void SetVertices() override;
        };
    }
}
