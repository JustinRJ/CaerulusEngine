#include "stdafx.h"

#include "Engine.h"

#include "Graphics/GraphicsEngine.h"
#include "Graphics/Window/GLWindow.h"
#include "Graphics/PipeLine/Renderer.h"

#include "Core/Logging/Log.h"
#include "Core/Math/Camera.h"
#include "Core/Input/MouseInputManager.h"
#include "Core/Input/KeyboardInputManager.h"
#include "Core/Time/FPSLimiter.h"
#include "Core/Time/FixedTimer.h"
#include "Core/Interface/ITickable.h"
#include "Core/Interface/NonCopyable.h"

#include "Managers/Managers/TextureManager.h"
#include "Managers/Managers/MaterialManager.h"
#include "Managers/Managers/ModelManager.h"
#include "Managers/Managers/ShaderManager.h"
#include "Managers/Managers/ShaderSrcManager.h"

#include "Core/Math/Rect.h"
#include "Core/Math/Cube.h"
#include "Core/Math/Circle.h"
#include "Core/Math/Sphere.h"
#include "Core/Math/Line.h"
#include "Core/Math/Ray.h"


namespace
{
    using namespace Core::Math;
    using namespace Core::Time;
    using namespace Core::Input;
    using namespace Graphics;
    using namespace Graphics::PipeLine;
    using namespace Managers;
}

Engine::Engine(int argc, char** argv) :
    m_argCount(argc),
    m_argValue(argv)
{
    m_fpsLimiter = std::make_unique<FPSLimiter>();
    m_fixedTimer = std::make_unique<FixedTimer>();

    m_window = std::make_shared<GLWindow>("Caerulus", 1280, 1024, 32, false);
    m_camera = std::make_shared<Camera>(vec3(0, 0, 0), Core::Math::UnitForward, Core::Math::UnitUp);
    m_renderer = std::make_shared<Renderer>();
    m_graphicsEngine = std::make_shared<GraphicsEngine>(m_renderer);

    m_keyboardInputManager = std::make_shared<KeyboardInputManager>(m_window);
    m_mouseInputManager = std::make_shared<MouseInputManager>(m_window);

    m_tickable.push_back(m_keyboardInputManager);
    m_tickable.push_back(m_mouseInputManager);
    m_tickable.push_back(m_graphicsEngine);

    m_textureManager = std::make_shared<TextureManager>();
    m_materialManager = std::make_shared<MaterialManager>(*m_textureManager);
    m_modelManager = std::make_shared<ModelManager>(*m_materialManager);
    m_shaderSrcManager = std::make_shared<ShaderSrcManager>();
    m_shaderManager = std::make_shared<ShaderManager>(*m_shaderSrcManager);

    InitInput();
    InitScene();
    InitRenderer();
}

void Engine::Run()
{
    using namespace Core::Logging;
    m_running = true;
    try
    {
        while (m_running)
        {
            m_deltaTime = m_fpsLimiter->Delta(m_fpsLimit);
            m_fixedTime = m_fixedTimer->Fixed(m_fpsLimit);
            Tick();
        }
    }
    catch (const std::exception& e)
    {
        m_running = false;
        Log::LogException("Error in Caerulus Engine", e.what());
    }
    catch (...)
    {
        m_running = false;
        Log::LogError("Unknown error in Caerulus Engine");
    }
}

void Engine::Tick()
{
    using Core::Logging::Log;
    Log::LogInDebug("DeltaTime: " + std::to_string(m_deltaTime));
    Log::LogInDebug("FixedTime: " + std::to_string(m_fixedTime));

    for (auto& updatable : m_tickable)
    {
        updatable->PreUpdate(m_deltaTime);
    }

    for (auto& updatable : m_tickable)
    {
        updatable->Update(m_deltaTime);
    }

    for (auto& updatable : m_tickable)
    {
        updatable->FixedUpdate(m_fixedTime);
    }

    glfwPollEvents();
}

void Engine::InitInput()
{
    using namespace Core::Math;
    using namespace Core::Input;
    m_keyboardInputManager->AddWindowKeyCallback(m_window, GLFW_KEY_ESCAPE, Action::Release, [=](Modifier) { m_running = false; });
    m_keyboardInputManager->AddWindowKeyCallback(m_window, GLFW_KEY_X, Action::Release, [&](Modifier) { m_window->ToggleLockedCursor(); });
    m_keyboardInputManager->AddWindowKeyCallback(m_window, GLFW_KEY_C, Action::Release, [&](Modifier) { m_graphicsEngine->ToggleWireframe(); });

    m_keyboardInputManager->AddWindowKeyCallback(m_window, GLFW_KEY_A, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitRight   * -m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed), false); }});
    m_keyboardInputManager->AddWindowKeyCallback(m_window, GLFW_KEY_D, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitRight   *  m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed), false); }});
    m_keyboardInputManager->AddWindowKeyCallback(m_window, GLFW_KEY_W, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitForward *  m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed)); }});
    m_keyboardInputManager->AddWindowKeyCallback(m_window, GLFW_KEY_S, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitForward * -m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed)); }});
    m_keyboardInputManager->AddWindowKeyCallback(m_window, GLFW_KEY_Q, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitUp      * -m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed)); }});
    m_keyboardInputManager->AddWindowKeyCallback(m_window, GLFW_KEY_E, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitUp      *  m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed)); }});

    m_mouseInputManager->AddDragMouseCallback(m_window, [&](DragData dd)
    {
        if (m_window->IsCursorLocked())
        {
            float rotateMultiplier = m_deltaTime * m_mouseSensitivity;
            m_camera->Rotate(vec3(dd.DeltaX * rotateMultiplier, dd.DeltaY * rotateMultiplier, 0.0f));
        }
    });
}

void Engine::InitScene()
{
    m_modelManager->Load("shaderBall", "assets/models/shaderball.obj");
    m_graphicsEngine->SetModels(m_modelManager->GetAll());
}

void Engine::InitRenderer()
{
    m_shaderManager->Load("position", "assets/shaders/position.vert", "assets/shaders/position.frag");

    m_renderer->SetCamera(m_camera);
    m_renderer->SetWindow(m_window);
    m_renderer->SetShader(m_shaderManager->Get("position"));
}
