#pragma once

#include "Geometry.h"

namespace Graphics
{
    namespace Geometry
    {
        class QuadGeometry : public Geometry
        {
        public:
            static const GLfloat s_QuadVertices[];

            QuadGeometry();
            QuadGeometry(const mat4& transform);
            virtual ~QuadGeometry();

            void Draw() const override;

        private:
            virtual void GenerateBuffer() override;
        };
    }
}
