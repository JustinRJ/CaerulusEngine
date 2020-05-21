#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <map>

#include "Render/IRenderer.h"

#include "Window/GLWindow.h"

#include "PipeLine/Shader.h"

#include "Lighting/PointLight.h"
#include "Lighting/DirectionalLight.h"

#include "Geometry/Quad.h"
#include "Geometry/Cube.h"

#include "Resource/Model.h"
#include "Resource/Camera.h"
#include "Resource/Texture.h"
#include "Resource/Material.h"

#include "../Core/Interface/ITickable.h"

namespace Graphics
{
    using namespace Render;
    using namespace Light;
    using namespace Window;
    using namespace PipeLine;
    using namespace Geometry;
    using namespace Resource;

    class CAERULUS_GRAPHICS GraphicsEngine : public Core::Interface::ITickable
    {
    public:

        GraphicsEngine(std::shared_ptr<GLWindow> window, std::shared_ptr<Camera> camera);

        void PreUpdate(float deltaTime) override;
        void Update(float deltaTime) override;
        void FixedUpdate(float fixedTime) override {}
        void Reset() override {}

    };
}
