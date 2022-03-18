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
    }

    namespace ComponentManagers
    {
        class ModelManager;
        class PointLightManager;
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

    // TODO - create a CameraComponentManager
    Core::Math::Camera* m_camera;
    Core::ECS::Entity* m_rootEntity;
    Graphics::Window::GLWindow* m_window;
    Graphics::Rendering::GLRenderer* m_renderer;

    Graphics::GraphicsEngine* m_graphicsEngine;
    Core::Input::MouseInputSystem* m_mouseInputSystem;
    Core::Input::KeyboardInputSystem* m_keyboardInputSystem;

    Graphics::AssetManagers::TextureManager* m_textureManager;
    Graphics::AssetManagers::MaterialManager* m_materialManager;
    Graphics::AssetManagers::ShaderSourceManager* m_shaderSrcManager;
    Graphics::AssetManagers::ShaderManager* m_shaderManager;

    Graphics::ComponentManagers::ModelManager* m_modelManager;
    Graphics::ComponentManagers::PointLightManager* m_pointLightManager;

    std::vector<Core::Interface::ITickable*> m_tickable;
};
