#include "stdafx.h"

#include "Engine.h"

#include "Core/Math/Math.h"
#include "Core/Node/Node.h"
#include "Core/Math/Camera.h"
#include "Core/Logging/Log.h"

#include "Core/Time/FPSLimiter.h"
#include "Core/Time/FixedTimer.h"
#include "Core/Interface/ITickable.h"
#include "Core/Interface/NonCopyable.h"

#include "Graphics/GraphicsEngine.h"
#include "Graphics/Window/GLWindow.h"
#include "Graphics/Pipeline/Renderer.h"

#include "Core/Input/MouseInputManager.h"
#include "Core/Input/KeyboardInputManager.h"

#include "Managers/Managers/TextureManager.h"
#include "Managers/Managers/MaterialManager.h"
#include "Managers/Managers/ModelManager.h"
#include "Managers/Managers/ShaderManager.h"
#include "Managers/Managers/ShaderSrcManager.h"

namespace
{
    using namespace Core::Node;
    using namespace Core::Math;
    using namespace Core::Time;
    using namespace Core::Input;
    using namespace Core::Logging;
    using namespace Graphics;
    using namespace Graphics::Window;
    using namespace Graphics::Pipeline;
    using namespace Graphics::Resource;
    using namespace Managers;
}

Engine::Engine(int argc, char** argv) :
    m_argCount(argc),
    m_argValue(argv)
{
    m_fpsLimiter = std::make_unique<FPSLimiter>();
    m_fixedTimer = std::make_unique<FixedTimer>();

    m_camera = std::make_shared<Camera>(vec3(0, 0, 0), UnitForward, UnitUp);
    m_window = std::make_shared<GLWindow>(m_camera, "Caerulus", 1280, 1024, 32, false);
    m_renderer = std::make_shared<Renderer>();
    m_graphicsEngine = std::make_shared<GraphicsEngine>(m_window, m_renderer);

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
    InitRenderer();
    InitScene();
}

void Engine::Run()
{
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
    Log::LogInDebug("DeltaTime: " + std::to_string(m_deltaTime));
    Log::LogInDebug("FixedTime: " + std::to_string(m_fixedTime));

    if (m_reset)
    {
        Log::LogMessage("Resetting...");
        for (auto& updatable : m_tickable)
        {
            updatable->Reset();
        }
        m_reset = false;
    }

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
    m_keyboardInputManager->AddWindowKeyCallback(m_window, GLFW_KEY_X, Action::Release, [&](Modifier)   { m_window->ToggleLockedCursor(); });
    m_keyboardInputManager->AddWindowKeyCallback(m_window, GLFW_KEY_ESCAPE, Action::Release, [&](Modifier) { if (m_window->IsCursorLocked()) { m_running = false; }});
    m_keyboardInputManager->AddWindowKeyCallback(m_window, GLFW_KEY_R, Action::Release, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_reset = true; }});
    m_keyboardInputManager->AddWindowKeyCallback(m_window, GLFW_KEY_C, Action::Release, [&](Modifier)   { if (m_window->IsCursorLocked()) { m_graphicsEngine->SetWireframe(!m_graphicsEngine->GetWireframe()); }});

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

void Engine::InitRenderer()
{
    m_shaderManager->Load("position", "assets/shaders/position.vert", "assets/shaders/position.frag");

    m_graphicsEngine->SetWindow(m_window);
}

void Engine::InitScene()
{
    m_modelManager->Load("sponza", "assets/models/Sponza/sponza.obj");
    
    std::shared_ptr<Shader> shader = m_shaderManager->Get("position");
    UniformCallbackMap& positionCallbackMap = shader->GetUniformCallbackMap();

    static std::shared_ptr<Node> node = std::make_shared<Node>();
    Core::Math::mat4& sponzaModel = node->GetTransform().GetMatrix();
    sponzaModel = translate(mat4(1.0f), vec3(0, 0, -10));
    sponzaModel = scale(sponzaModel, vec3(0.25, 0.25, 0.25));

    positionCallbackMap.F4Callbacks.insert({ "ourColor", []() { return fvec4(0.f, (sin(glfwGetTime()) / 2.0f) + 0.5f, 0.f, 1.0); } });
    positionCallbackMap.Mat4Callbacks.insert({ "mvp", [&camera = m_camera, &model = sponzaModel]()
    {
        return camera->GetFrustrum().GetMatrix() * camera->GetTransform().GetMatrix() * model;
    }});

    for (std::shared_ptr<Material> material : m_modelManager->Get("sponza")->GetMaterials())
    {
        if (material)
        {
            material->SetShaders({ shader });
        }
    }
    
    m_graphicsEngine->SetModels(m_modelManager->GetAll());
}
