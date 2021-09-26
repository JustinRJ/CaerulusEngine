#include "stdafx.h"

#include "Engine.h"

#include "Node/Node.h"
#include "Math/Math.h"
#include "Math/Camera.h"

#include "Logging/Log.h"
#include "Logging/LogToFile.h"

#include "Time/FPSLimiter.h"
#include "Time/FixedTimer.h"

#include "Interface/ITickable.h"
#include "Interface/NonCopyable.h"

#include "GraphicsEngine.h"
#include "Window/GLWindow.h"
#include "Rendering/GLRenderer.h"

#include "Input/MouseInputManager.h"
#include "Input/KeyboardInputManager.h"

#include "Managers/TextureManager.h"
#include "Managers/MaterialManager.h"
#include "Managers/ModelManager.h"
#include "Managers/ShaderSourceManager.h"
#include "Managers/ShaderManager.h"

#include "Lighting/IBL.h"

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
    m_keyboardInputManager  = std::make_shared<KeyboardInputManager>(m_window);
    m_mouseInputManager     = std::make_shared<MouseInputManager>(m_window);
    m_shaderSrcManager      = std::make_shared<ShaderSourceManager>();
    m_shaderManager         = std::make_shared<ShaderManager>(*m_shaderSrcManager);
    m_textureManager        = std::make_shared<TextureManager>();
    m_materialManager       = std::make_shared<MaterialManager>(*m_shaderManager, *m_textureManager);
    m_modelManager          = std::make_shared<ModelManager>(*m_materialManager);
    m_pointLightManager     = std::make_shared<PointLightManager>(*m_shaderManager);

    m_graphicsEngine = std::make_shared<GraphicsEngine>(
        *m_shaderManager, *m_textureManager, *m_materialManager, *m_modelManager, *m_pointLightManager, m_window, m_renderer);

    m_rootNode = std::make_shared<Node>(nullptr);
    //Transform rootTransform;
    //rootTransform.Scale(vec3(2.01f, 2.01f, 2.01f));
    //m_rootNode->SetLocalTransform(rootTransform);

    m_tickable.push_back(m_keyboardInputManager);
    m_tickable.push_back(m_mouseInputManager);
    m_tickable.push_back(m_graphicsEngine);

    InitInput();
    InitGLRenderer();
    InitScene();
    InitLighting();
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
        for (const std::shared_ptr<ITickable>& tickable : m_tickable)
        {
            tickable->Reset();
        }
        m_reset = false;
    }

    for (const std::shared_ptr<ITickable>& tickable : m_tickable)
    {
        tickable->PreUpdate(m_deltaTime);
    }

    for (const std::shared_ptr<ITickable>& tickable : m_tickable)
    {
        tickable->Update(m_deltaTime);
    }

    for (const std::shared_ptr<ITickable>& tickable : m_tickable)
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

    m_mouseInputManager->AddDragMouseCallback(m_window, [&](const DragData& dd)
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

    Lighting::IBLShaders iblShaders = { "pbr", "cubemap", "irradiance", "prefilter", "brdf", "background" };

    m_textureManager->Load("city", "assets/textures/city_ref.hdr");
    std::shared_ptr<Lighting::IBL> ibl = std::make_shared<IBL>(m_rootNode.get(), *m_shaderManager, *m_textureManager,
        iblShaders, "city", m_renderer, m_window, m_camera, m_graphicsEngine->GetFrameBuffer());

    ibl->AddUniformCallback("background", [&](const Shader& shader)
    {
        shader.SetMat4fv("projection", m_camera->GetPerspective());
        shader.SetMat4fv("view", m_camera->GetTransform());

        m_graphicsEngine->GetIBL()->Draw();
    });
    ibl->AddUniformCallback("brdf", [&](const Shader& shader)
    {
        m_window->Update();
    });

    m_graphicsEngine->SetIBL(ibl);
}

void Engine::InitLighting()
{
    m_pointLightManager->Create(m_rootNode.get(), "red",    vec3(-150, 15., 0), vec3(255, 0, 0));
    m_pointLightManager->Create(m_rootNode.get(), "blue",   vec3(0, 15., 0),    vec3(0, 255, 0));
    m_pointLightManager->Create(m_rootNode.get(), "green",  vec3(150, 15., 0),  vec3(0, 0, 255));
    m_pointLightManager->Create(m_rootNode.get(), "white",  vec3(0., 30., 0),   vec3(255, 255, 255));

    const PointLight* red = m_pointLightManager->Get("red");
    m_pointLightManager->AddPointLightUniformCallback("red", "pbr", [capturedLight = red](const Shader& shader)
    {
        shader.Set3f("lightPositions[0]", capturedLight->GetPosition());
        shader.Set3f("lightColours[0]", capturedLight->GetColour());
    });

    const PointLight* blue = m_pointLightManager->Get("blue");
    m_pointLightManager->AddPointLightUniformCallback("blue", "pbr", [capturedLight = blue](const Shader& shader)
    {
        shader.Set3f("lightPositions[1]", capturedLight->GetPosition());
        shader.Set3f("lightColours[1]", capturedLight->GetColour());
    });

    const PointLight* green = m_pointLightManager->Get("green");
    m_pointLightManager->AddPointLightUniformCallback("green", "pbr", [capturedLight = green](const Shader& shader)
    {
        shader.Set3f("lightPositions[2]", capturedLight->GetPosition());
        shader.Set3f("lightColours[2]", capturedLight->GetColour());
    });

    const PointLight* white = m_pointLightManager->Get("white");
    m_pointLightManager->AddPointLightUniformCallback("white", "pbr", [capturedLight = white](const Shader& shader)
    {
        shader.Set3f("lightPositions[3]", capturedLight->GetPosition());
        shader.Set3f("lightColours[3]", capturedLight->GetColour());
    });
}

void Engine::InitScene()
{
    m_modelManager->Load(m_rootNode.get(), "sponza", "assets/models/Sponza/sponza.obj");

    // Only set shaders and unform callback for sponza model 
    if (const Model* model = m_modelManager->Get("sponza"))
    {
        // Sponza model doesn't have AO textures, add default AO texture
        // Use completely black texture to disable IBL lighting
        m_textureManager->Load("defaultAO", "assets/textures/defaultAO.png");
        m_textureManager->Load("white", "assets/textures/white.png");
        m_textureManager->Load("black", "assets/textures/black.png");

        Transform sponzaTransform;
        sponzaTransform.Translate(vec3(0.f, -10.f, 0.f));
        sponzaTransform.Scale(vec3(0.25f, 0.25f, 0.25f));
        m_modelManager->SetModelLocalTransform("sponza", sponzaTransform);

        // Set uniform callback to update each models MVP before it's rendered
        m_modelManager->AddModelUniformCallback("sponza", "pbr", [capturedCamera = m_camera, capturedModel = model](const Shader& shader)
        {
            shader.SetMat4fv("projection",  capturedCamera->GetPerspective());
            shader.SetMat4fv("view",        capturedCamera->GetTransform());
            shader.Set3f("viewPos",         capturedCamera->GetTransform().GetTranslation());

            shader.SetMat4fv("model",       capturedModel->GetWorldTransform());
        });

        for (const std::string& materialName : model->GetMaterialNames())
        {
            if (materialName != "")
            {
                m_materialManager->SetMaterialTexture(materialName, "defaultAO", TextureType::Bump);
                m_materialManager->AddMaterialUniformCallback(materialName, "pbr", [](const Shader& shader)
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

        // Can set uniform callback for meshes if required
        //for (const std::shared_ptr<Mesh>& mesh : model->GetMeshes())
        //{
        //    if (mesh)
        //    {
        //        mesh->AddUniformCallback("", [](std::shared_ptr<Shader> shader) {});
        //    }
        //}
    }
}
