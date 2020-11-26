#pragma once

#include "Core/Interface/NonCopyable.h"

namespace Core
{
    namespace Time
    {
        class FPSLimiter;
        class FixedTimer;
    }
    namespace Input
    {
        class MouseInputManager;
        class KeyboardInputManager;
    }
    namespace Interface
    {
        class ITickable;
    }
    namespace Math
    {
        class Camera;
    }
}

namespace Graphics
{
    class GraphicsEngine;

    namespace Window
    {
        class GLWindow;
    }

    namespace Pipeline
    {
        class Renderer;
    }

    namespace Managers
    {
        class TextureManager;
        class MaterialManager;
        class ModelManager;
        class ShaderSrcManager;
        class ShaderManager;
    }
}

class Engine : public Core::Interface::NonCopyable
{
public:
    Engine(int argc, char** argv);
    ~Engine() = default;

    void Run();

    void InitInput();
    void InitScene();
    void InitRenderer();

private:
    void Tick();

    int m_argCount;
    char** m_argValue;

    bool m_running = false;
    bool m_reset = false;

    float m_deltaTime = 0.0f;
    float m_fixedTime = 0.0f;
    float m_fpsLimit = 1.0f / 144.0f;
    float m_normalSpeed = 50.0f;
    float m_sprintSpeed = 100.0f;
    float m_mouseSensitivity = 50.0f;

    std::shared_ptr<Core::Time::FPSLimiter> m_fpsLimiter;
    std::shared_ptr<Core::Time::FixedTimer> m_fixedTimer;
    std::shared_ptr<Core::Input::MouseInputManager> m_mouseInputManager;
    std::shared_ptr<Core::Input::KeyboardInputManager> m_keyboardInputManager;

    std::shared_ptr<Core::Math::Camera> m_camera;
    std::shared_ptr<Graphics::Window::GLWindow> m_window;
    std::shared_ptr<Graphics::Pipeline::Renderer> m_renderer;
    std::shared_ptr<Graphics::GraphicsEngine> m_graphicsEngine;

    std::vector<std::shared_ptr<Core::Interface::ITickable>> m_tickable;

    std::shared_ptr<Graphics::Managers::TextureManager> m_textureManager;
    std::shared_ptr<Graphics::Managers::MaterialManager> m_materialManager;
    std::shared_ptr<Graphics::Managers::ModelManager> m_modelManager;
    std::shared_ptr<Graphics::Managers::ShaderSrcManager> m_shaderSrcManager;
    std::shared_ptr<Graphics::Managers::ShaderManager> m_shaderManager;
};
