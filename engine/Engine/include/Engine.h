#pragma once

#define CAERULUS_ENGINE __declspec(dllexport)

#include <memory>

#include "Interface/NonCopyable.h"

#include "Time/DeltaTimer.h"
#include "Time/FixedTimer.h"

#include "ECS/Scene.h"
#include "ECS/ManagerFactory.h"
#include "Window/GLWindow.h"
#include "Rendering/GLRenderer.h"

namespace Core
{
    namespace Math
    {
        class Camera;
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
    std::unique_ptr<Core::ECS::Scene> m_scene;
    std::unique_ptr<Core::ECS::ManagerFactory> m_managerFactory;
    std::unique_ptr<Graphics::Window::GLWindow> m_window;
    std::unique_ptr<Graphics::Rendering::GLRenderer> m_renderer;

    std::vector<Core::Interface::ITickable*> m_tickables;
};
