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

            CubeGeometry();
            CubeGeometry(const mat4& transform);
            virtual ~CubeGeometry() = default;

            void Draw() const override;

        private:
            void GenerateBuffer() override;
        };
    }
}
