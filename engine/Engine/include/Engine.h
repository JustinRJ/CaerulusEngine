#pragma once

#define CAERULUS_ENGINE __declspec(dllexport)

#include <memory>
#include <vector>

#include "Interface/NonCopyable.h"

#include "Time/FPSLimiter.h"
#include "Time/FixedTimer.h"

namespace Core
{
    namespace ECS
    {
        class Entity;
        class ManagerFactory;
    }
    namespace Input
    {
        class MouseInputSystem;
        class KeyboardInputSystem;
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

    namespace Rendering
    {
        class GLRenderer;
    }

    namespace AssetManagers
    {
        class TextureManager;
        class MaterialManager;
        class ShaderSourceManager;
        class ShaderManager;
        class ModelManager;
    }
}

class CAERULUS_ENGINE Engine : Core::Interface::NonCopyable
{
public:
    Engine(int argc, char** argv);

    void Run();

    void InitInput();
    void InitScene();
    void InitLighting();
    void InitGLRenderer();

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

    Core::Time::FPSLimiter m_fpsLimiter;
    Core::Time::FixedTimer m_fixedTimer;

    std::shared_ptr<Core::Math::Camera> m_camera;
    std::shared_ptr<Core::ECS::Entity> m_rootEntity;
    std::shared_ptr<Graphics::Window::GLWindow> m_window;
    std::shared_ptr<Graphics::Rendering::GLRenderer> m_renderer;

    std::shared_ptr<Graphics::GraphicsEngine> m_graphicsEngine;
    std::shared_ptr<Core::Input::MouseInputSystem> m_mouseInputSystem;
    std::shared_ptr<Core::Input::KeyboardInputSystem> m_keyboardInputSystem;

    std::shared_ptr<Graphics::AssetManagers::ShaderSourceManager> m_shaderSrcManager;
    std::shared_ptr<Graphics::AssetManagers::ShaderManager> m_shaderManager;
    std::shared_ptr<Graphics::AssetManagers::TextureManager> m_textureManager;
    std::shared_ptr<Graphics::AssetManagers::MaterialManager> m_materialManager;
    std::shared_ptr<Graphics::AssetManagers::ModelManager> m_modelManager;

    Core::ECS::ManagerFactory* m_managerFactory;

    std::vector<Core::Interface::ITickable*> m_tickable;
};
