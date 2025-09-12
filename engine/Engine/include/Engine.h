#pragma once

#define CAERULUS_ENGINE __declspec(dllexport)

#include <memory>

#include "Interface/NonCopyable.h"

#include "Time/EngineTimer.h"

#include "ECS/Scene.h"
#include "Window/GLWindow.h"
#include "Rendering/GLRenderer.h"
#include "ECS/AssetManagerFactory.h"

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

    double m_fpsLimit = 144.;
    double m_stepLimit = 72.0;
    double m_fpsLimit1Over = 1.0 / m_fpsLimit;
    double m_stepLimit1Over = 1.0 / m_stepLimit;

    float m_normalSpeed = 50.0f;
    float m_sprintSpeed = 100.0f;
    float m_mouseSensitivity = 50.0f;

    Core::Time::EngineTimer m_timer;

    Core::Math::Camera* m_camera;
    std::unique_ptr<Core::ECS::Scene> m_scene;
    std::unique_ptr<Graphics::Window::GLWindow> m_window;
    std::unique_ptr<Graphics::Rendering::GLRenderer> m_renderer;
    std::unique_ptr<Core::ECS::AssetManagerFactory> m_assetManagerFactory;

    std::vector<Core::Interface::ITickable*> m_tickables;
};
