#pragma once

#include "Geometry.h"

namespace Graphics
{
    namespace Geometry
    {
        class CAERULUS_GRAPHICS Plane : public Geometry
        {
        public:
            static const GLfloat s_PlaneVertices[];

            Plane();
            Plane(const mat4& transform);
            virtual ~Plane() = default;

            void Draw(bool wireframe) const override;

        private:
            void SetVertices() override;
        };
    }
}
