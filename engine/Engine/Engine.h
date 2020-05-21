#pragma once

#include <memory>
#include <vector>
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
}

namespace Graphics
{
    class GraphicsEngine;

    namespace Window
    {
        class GLWindow;
    }

    namespace Resource
    {
        class Camera;
    }
}

namespace Managers
{
    class TextureManager;
    class MaterialManager;
    class ModelManager;
    class ShaderSrcManager;
    class ShaderManager;
}

class Engine : public Core::Interface::NonCopyable
{
public:

    Engine(int argc, char** argv);
    virtual ~Engine() = default;

    void Run();

    virtual void InitInput();
    virtual void InitScene();
    virtual void InitRenderer();

private:

    void Tick();

    int m_argCount;
    char** m_argValue;

    bool m_running = false;

    float m_deltaTime = 0.0f;
    float m_fixedTime = 0.0f;
    float m_fpsLimit = 1.0f / 144.0f;
    float m_normalSpeed = 150.0f;
    float m_sprintSpeed = 300.0f;
    double m_mouseSensitivity = 50.0;

    std::shared_ptr<Core::Time::FPSLimiter> m_fpsLimiter;
    std::shared_ptr<Core::Time::FixedTimer> m_fixedTimer;
    std::shared_ptr<Core::Input::MouseInputManager> m_mouseInputManager;
    std::shared_ptr<Core::Input::KeyboardInputManager> m_keyboardInputManager;

    std::shared_ptr<Graphics::Resource::Camera> m_camera;
    std::shared_ptr<Graphics::Window::GLWindow> m_window;
    std::shared_ptr<Graphics::GraphicsEngine> m_graphicsEngine;

    std::vector<std::shared_ptr<Core::Interface::ITickable>> m_tickable;

    std::shared_ptr<Managers::TextureManager> m_textureManager;
    std::shared_ptr<Managers::MaterialManager> m_materialManager;
    std::shared_ptr<Managers::ModelManager> m_modelManager;
    std::shared_ptr<Managers::ShaderSrcManager> m_shaderSrcManager;
    std::shared_ptr<Managers::ShaderManager> m_shaderManager;
};
