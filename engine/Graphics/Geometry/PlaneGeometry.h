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

            PlaneGeometry();
            PlaneGeometry(const mat4& transform);
            virtual ~PlaneGeometry() = default;

            void Draw() const override;

        private:
            void GenerateBuffer() override;
        };
    }
}
