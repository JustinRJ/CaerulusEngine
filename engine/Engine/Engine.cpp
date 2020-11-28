#include "stdafx.h"

#include "Engine.h"

#include "Core/Math/Math.h"
#include "Core/Node/Node.h"
#include "Core/Math/Camera.h"
#include "Core/Logging/Log.h"
#include "Core/Logging/LogToFile.h"

#include "Core/Time/FPSLimiter.h"
#include "Core/Time/FixedTimer.h"
#include "Core/Interface/ITickable.h"
#include "Core/Interface/NonCopyable.h"

#include "Graphics/GraphicsEngine.h"
#include "Graphics/Window/GLWindow.h"
#include "Graphics/Pipeline/Renderer.h"

#include "Core/Input/MouseInputManager.h"
#include "Core/Input/KeyboardInputManager.h"

#include "Graphics/Managers/TextureManager.h"
#include "Graphics/Managers/MaterialManager.h"
#include "Graphics/Managers/ModelManager.h"
#include "Graphics/Managers/ShaderSrcManager.h"
#include "Graphics/Managers/ShaderManager.h"

namespace
{
    using namespace Core::Node;
    using namespace Core::Math;
    using namespace Core::Time;
    using namespace Core::Input;
    using namespace Core::Logging;
    using namespace Core::Interface;

    using namespace Graphics;
    using namespace Graphics::Window;
    using namespace Graphics::Surface;
    using namespace Graphics::Managers;
    using namespace Graphics::Geometry;
    using namespace Graphics::Pipeline;
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
    using namespace Core::Interface;

    Log::LogInDebug("DeltaTime: " + std::to_string(m_deltaTime));
    Log::LogInDebug("FixedTime: " + std::to_string(m_fixedTime));

    if (m_reset)
    {
        Log::LogMessage("Resetting...");
        for (std::shared_ptr<ITickable> tickable : m_tickable)
        {
            tickable->Reset();
        }
        m_reset = false;
    }

    for (std::shared_ptr<ITickable> tickable : m_tickable)
    {
        tickable->PreUpdate(m_deltaTime);
    }

    for (std::shared_ptr<ITickable> tickable : m_tickable)
    {
        tickable->Update(m_deltaTime);
    }

    for (std::shared_ptr<ITickable> tickable : m_tickable)
    {
        tickable->FixedUpdate(m_fixedTime);
    }

    glfwPollEvents();
}

void Engine::InitInput()
{
    m_keyboardInputManager->AddWindowKeyCallback(m_window, GLFW_KEY_ESCAPE, Action::Release, [&](Modifier) { m_running = false; });
    m_keyboardInputManager->AddWindowKeyCallback(m_window, GLFW_KEY_X, Action::Release, [&](Modifier)   { m_window->ToggleLockedCursor(); });
    m_keyboardInputManager->AddWindowKeyCallback(m_window, GLFW_KEY_R, Action::Release, [&](Modifier)   { if (m_window->IsCursorLocked()) { m_reset = true; }});
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
}

void Engine::InitScene()
{
    static std::shared_ptr<Node> sponzaNode = std::make_shared<Node>();
    Core::Math::mat4& sponzaTransform = sponzaNode->GetTransform().GetMatrix();
    sponzaTransform = translate(mat4(1.0f), vec3(0, 0, -10));
    sponzaTransform = scale(sponzaTransform, vec3(0.25, 0.25, 0.25));

    m_modelManager->Load("sponza", "assets/models/Sponza/sponza.obj");

    std::shared_ptr<Shader> positionShader = m_shaderManager->Get("position");

    for (std::shared_ptr<Model> model : m_modelManager->GetAll())
    {
        if (model)
        {
            // Set uniform functor to update each models MVP before it's rendered
            model->SetUniformFunctor(positionShader, [camera = m_camera, node = sponzaNode](const Shader& shader)
            {
                shader.SetMat4fv("u_MVP", camera->GetFrustrum().GetMatrix() * camera->GetTransform().GetMatrix() * node->GetTransform().GetMatrix());
            });

            for (std::shared_ptr<Material> material : model->GetMaterials())
            {
                if (material)
                {
                    // Set uniform functor to update each materials Texture before it's rendered
                    material->SetUniformFunctor(positionShader, [](const Shader& shader)
                    {
                        shader.Set1i("u_Texture", static_cast<GLint>(Material::TextureType::Diffuse));
                    });
                }
            }

            // Can set uniform functor for meshes if required
            //for (std::shared_ptr<Mesh> mesh : model->GetMeshes())
            //{
            //    if (mesh)
            //    {
            //        mesh->SetUniformFunctor(nullptr, [](std::shared_ptr<Shader> shader) {});
            //    }
            //}
        }
    }

    // Can set uniforms at any stage of the pipeline if required, can be scaled
    //std::shared_ptr<ShaderUniformFunctor> processhaderUniformFunctor = std::make_shared<ShaderUniformFunctor>();
    //processhaderUniformFunctor->SetUniformFunctor(nullptr, [](const Shader& shader) {});
    //m_graphicsEngine->SetProcessUniformFunctor(PipelineProcess::PreProcess, processhaderUniformFunctor);
    
    m_graphicsEngine->SetModels(m_modelManager->GetAll());
}
