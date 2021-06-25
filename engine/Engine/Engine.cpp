#include "stdafx.h"

#include "Engine.h"

#include "Core/Node/Node.h"
#include "Core/Math/Math.h"
#include "Core/Math/Camera.h"

#include "Core/Logging/Log.h"
#include "Core/Logging/LogToFile.h"

#include "Core/Time/FPSLimiter.h"
#include "Core/Time/FixedTimer.h"

#include "Core/Interface/ITickable.h"
#include "Core/Interface/NonCopyable.h"

#include "Graphics/GraphicsEngine.h"
#include "Graphics/Window/GLWindow.h"
#include "Graphics/Rendering/GLRenderer.h"

#include "Core/Input/MouseInputManager.h"
#include "Core/Input/KeyboardInputManager.h"

#include "Graphics/Managers/TextureManager.h"
#include "Graphics/Managers/MaterialManager.h"
#include "Graphics/Managers/ModelManager.h"
#include "Graphics/Managers/ShaderSrcManager.h"
#include "Graphics/Managers/ShaderManager.h"

#include "Graphics/Lighting/IBL.h"

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
using namespace Graphics::Lighting;
using namespace Graphics::Rendering;

Engine::Engine(int argc, char** argv) :
    m_argCount(argc),
    m_argValue(argv)
{
    m_fpsLimiter            = std::make_shared<FPSLimiter>();
    m_fixedTimer            = std::make_shared<FixedTimer>();
    m_camera                = std::make_shared<Camera>(vec3(0, 0, 0), UnitForward, UnitUp);
    m_window                = std::make_shared<GLWindow>(m_camera, "Caerulus", 1280, 1024, 32, false);
    m_renderer              = std::make_shared<GLRenderer>();
    m_graphicsEngine        = std::make_shared<GraphicsEngine>(m_window, m_renderer);
    m_keyboardInputManager  = std::make_shared<KeyboardInputManager>(m_window);
    m_mouseInputManager     = std::make_shared<MouseInputManager>(m_window);
    m_textureManager        = std::make_shared<TextureManager>();
    m_materialManager       = std::make_shared<MaterialManager>(*m_textureManager);
    m_modelManager          = std::make_shared<ModelManager>(*m_materialManager);
    m_shaderSrcManager      = std::make_shared<ShaderSrcManager>();
    m_shaderManager         = std::make_shared<ShaderManager>(*m_shaderSrcManager);

    m_tickable.push_back(m_keyboardInputManager);
    m_tickable.push_back(m_mouseInputManager);
    m_tickable.push_back(m_graphicsEngine);

    InitInput();
    InitGLRenderer();
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
        LogException("Error in Caerulus Engine", e.what());
    }
    catch (...)
    {
        m_running = false;
        LogError("Unknown error in Caerulus Engine");
    }
}

void Engine::Tick()
{
    using namespace Core::Interface;

    LogInDebug("DeltaTime: " + std::to_string(m_deltaTime));
    LogInDebug("FixedTime: " + std::to_string(m_fixedTime));

    if (m_reset)
    {
        LogMessage("Resetting...");
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

void Engine::InitGLRenderer()
{
    m_shaderManager->Load("pbr",        "assets/shaders/pbr.vert",          "assets/shaders/pbr.frag");
    m_shaderManager->Load("cubemap",    "assets/shaders/cubemap.vert",      "assets/shaders/equirectangular_to_cubemap.frag");
    m_shaderManager->Load("irradiance", "assets/shaders/cubemap.vert",      "assets/shaders/irradiance_convolution.frag");
    m_shaderManager->Load("prefilter",  "assets/shaders/cubemap.vert",      "assets/shaders/prefilter.frag");
    m_shaderManager->Load("brdf",       "assets/shaders/brdf.vert",         "assets/shaders/brdf.frag");
    m_shaderManager->Load("background", "assets/shaders/background.vert",   "assets/shaders/background.frag");

    Lighting::IBLShaders iblShaders = {
        m_shaderManager->Get("pbr"),
        m_shaderManager->Get("cubemap"),
        m_shaderManager->Get("irradiance"),
        m_shaderManager->Get("prefilter"),
        m_shaderManager->Get("brdf"),
        m_shaderManager->Get("background")
    };

    m_textureManager->Load("city", "assets/textures/city_ref.hdr");
    std::shared_ptr<Lighting::IBL> ibl = std::make_shared<IBL>(iblShaders, m_textureManager->Get("city"), m_renderer, m_window, m_camera, m_graphicsEngine->GetFrameBuffer());

    ibl->AddUniformFunctor(m_shaderManager->Get("background"), [&](const Shader& shader)
    {
        shader.SetMat4fv("projection", m_camera->GetPerspective().GetMatrix());
        shader.SetMat4fv("view", m_camera->GetTransform().GetMatrix());

        m_graphicsEngine->GetIBL()->Draw();
    });
    ibl->AddUniformFunctor(m_shaderManager->Get("brdf"), [&](const Shader& shader)
    {
        m_window->Update();
    });

    m_graphicsEngine->SetIBL(ibl);
    Material::SetMaterialTextureSlotOffset(ibl->GetRequiredTextureSlots());
}

void Engine::InitScene()
{
    m_modelManager->Load("sponza", "assets/models/Sponza/sponza.obj");

    // Only set shaders and functors for sponza model 
    if (std::shared_ptr<Model> model = m_modelManager->Get("sponza"))
    {
        // Sponza model doesn't have AO textures, add default AO texture
        // Use completely black texture to disable IBL lighting
        m_textureManager->Load("defaultAO", "assets/textures/defaultAO.png");
        m_textureManager->Load("white", "assets/textures/white.png");
        m_textureManager->Load("black", "assets/textures/black.png");

        Core::Math::mat4 sponzaTransform = model->GetTransform().GetMatrix();
        sponzaTransform = translate(mat4(1.0f), vec3(0, -10, 0));
        sponzaTransform = scale(sponzaTransform, vec3(0.25, 0.25, 0.25));
        model->GetTransform().SetMatrix(sponzaTransform);

        // Set uniform functor to update each models MVP before it's rendered
        model->AddUniformFunctor(m_shaderManager->Get("pbr"), [capturedCamera = m_camera, capturedModel = model](const Shader& shader)
        {
            shader.SetMat4fv("projection",  capturedCamera->GetPerspective().GetMatrix());
            shader.SetMat4fv("view",        capturedCamera->GetTransform().GetMatrix());
            shader.SetMat4fv("model",       capturedModel->GetTransform().GetMatrix());
            shader.Set3f("viewPos",         capturedCamera->GetTransform().GetTranslation());

            shader.Set3f("lightPositions[0]", vec3(-150, 15., 0));
            shader.Set3f("lightPositions[1]", vec3(0, 15., 0));
            shader.Set3f("lightPositions[2]", vec3(150, 15., 0));
            shader.Set3f("lightPositions[3]", vec3(0., 30., 0));
            shader.Set3f("lightColours[0]", vec3(255, 0, 0));
            shader.Set3f("lightColours[1]", vec3(0, 255, 0));
            shader.Set3f("lightColours[2]", vec3(0, 0, 255));
            shader.Set3f("lightColours[3]", vec3(255, 255, 255));
        });

        for (std::shared_ptr<Material> material : model->GetMaterials())
        {
            if (material)
            {
                // Add to bump map as sponza mapping is wrong
                material->SetTexture(m_textureManager->Get("white"), TextureType::Bump);
                // Set uniform functor to update each materials Texture before it's rendered
                material->AddUniformFunctor(m_shaderManager->Get("pbr"), [](const Shader& shader)
                {
                    // Sponza asset has material properties set in the wrong channels
                    shader.Set1i("albedoMap",       Material::GetTextureSlotForTextureType(TextureType::Diffuse));
                    shader.Set1i("normalMap",       Material::GetTextureSlotForTextureType(TextureType::Highlight));
                    shader.Set1i("metallicMap",     Material::GetTextureSlotForTextureType(TextureType::Ambient));
                    shader.Set1i("roughnessMap",    Material::GetTextureSlotForTextureType(TextureType::Specular));
                    shader.Set1i("aoMap",           Material::GetTextureSlotForTextureType(TextureType::Bump));
                });
            }
        }

        // Can set uniform functor for meshes if required
        //for (std::shared_ptr<Mesh> mesh : model->GetMeshes())
        //{
        //    if (mesh)
        //    {
        //        mesh->AddUniformFunctor(nullptr, [](std::shared_ptr<Shader> shader) {});
        //    }
        //}
    }
    
    m_graphicsEngine->SetModels(m_modelManager->GetAll());
}
