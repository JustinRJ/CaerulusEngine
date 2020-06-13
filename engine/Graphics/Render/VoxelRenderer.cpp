#include "stdafx.h"

#include "VoxelRenderer.h"
#define STBVOX_CONFIG_MODE 0
#define STB_VOXEL_RENDER_IMPLEMENTATION
#include "stb_voxel_render.h"

#include "Core/Math/Camera.h"
#include "Graphics/Window/GLWindow.h"

namespace Graphics
{
    namespace Render
    {
        VoxelRenderer::VoxelRenderer(std::shared_ptr<Window::GLWindow> window, std::shared_ptr<Core::Math::Camera> camera) :
            m_window(window),
            m_camera(camera)
        {
        }

        std::shared_ptr<Window::GLWindow> VoxelRenderer::GetWindow() const
        {
            return m_window;
        }

        std::shared_ptr<Core::Math::Camera> VoxelRenderer::GetCamera() const
        {
            return m_camera;
        }
    }
}
