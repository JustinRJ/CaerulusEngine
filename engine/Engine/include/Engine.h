#pragma once

#define CAERULUS_ENGINE __declspec(dllexport)

#include <memory>

#include "Interface/NonCopyable.h"

#include "Time/DeltaTimer.h"
#include "Time/FixedTimer.h"

namespace Core
{
    namespace ECS
    {
        class Scene;
        class Entity;
        class ManagerFactory;
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

    Core::Time::DeltaTimer m_deltaTimer;
    Core::Time::FixedTimer m_fixedTimer;

    Core::Math::Camera* m_camera;
    std::shared_ptr<Graphics::Window::GLWindow> m_window;
    std::shared_ptr<Graphics::Rendering::GLRenderer> m_renderer;
    std::shared_ptr<Core::ECS::Scene> m_scene;
    std::shared_ptr<Core::ECS::ManagerFactory> m_managerFactory;
};
