#include "stdafx.h"

#include "Engine.h"

#include "ECS/Scene.h"
#include "ECS/ManagerFactory.h"

#include "GraphicsEngine.h"
#include "Window/GLWindow.h"
#include "Lighting/IBL.h"
#include "Lighting/PointLight.h"
#include "Geometry/Mesh.h"
#include "Geometry/Model.h"
#include "Rendering/RenderInstance.h"

#include "Input/MouseInputSystem.h"
#include "Input/KeyboardInputSystem.h"

#include "AssetManagers/MaterialManager.h"
#include "AssetManagers/ModelManager.h"

#include "Thread/Thread.h"
#include "Thread/LockT.h"

using namespace Core::Log;
using namespace Core::ECS;
using namespace Core::Math;
using namespace Core::Time;
using namespace Core::Input;
using namespace Core::Thread;
using namespace Core::Interface;

using namespace Graphics;
using namespace Graphics::Window;
using namespace Graphics::Surface;
using namespace Graphics::Geometry;
using namespace Graphics::Pipeline;
using namespace Graphics::Lighting;
using namespace Graphics::Rendering;
using namespace Graphics::AssetManagers;

Engine::Engine(int argc, char** argv) :
    m_argCount(argc),
    m_argValue(argv)
{
    //struct TestThread : public Thread
    //{
    //    void Run() override
    //    {
    //        while (IsRunning())
    //        {
    //            std::cout << "TestThread" << std::endl;
    //        }
    //    }
    //};
    //auto t = TestThread();
    //t.Start();

    //LockT<std::vector<int>> lock;
    //auto[vec, l] = lock.GetT();
    //vec.push_back(1);
    //l.~scoped_lock();

    m_scene = std::make_unique<Scene>();
    auto em = m_scene->GetEntityManager();

    m_managerFactory = std::make_unique<ManagerFactory>();
    m_managerFactory->CreateComponentManager<RenderInstance>(*em);
    m_managerFactory->CreateComponentManager<PointLight>(*em);
    m_managerFactory->CreateComponentManager<Camera>(*em);

    Entity& camera = em->CreateEntity(*m_scene->GetRootEntity());
    m_camera = camera.AddComponentOfType<Camera>();
    m_camera->SetView(vec3(0.0), UnitForward, UnitUp);
    m_camera->GetPerspective().SetPerspective(54.0f, 1.25f, 1.0f, 1000.0f);

    m_window = std::make_unique<GLWindow>(m_camera, "Caerulus", 1280, 1024, 32, false);
    m_renderer = std::make_unique<GLRenderer>();

    m_scene->RegisterTickable(std::make_unique<KeyboardInputSystem>(*m_window));
    m_scene->RegisterTickable(std::make_unique<MouseInputSystem>(*m_window));

    auto shaderSrcManager = std::make_unique<ShaderSourceManager>();
    auto shaderManager = std::make_unique<ShaderManager>(*shaderSrcManager);
    auto textureManager = std::make_unique<TextureManager>();
    auto materialManager = std::make_unique<MaterialManager>(*textureManager);
    auto modelManager = std::make_unique<ModelManager>(*materialManager, m_renderer.get());

    m_managerFactory->AddAssetManager(std::move(shaderSrcManager));
    m_managerFactory->AddAssetManager(std::move(shaderManager));
    m_managerFactory->AddAssetManager(std::move(textureManager));
    m_managerFactory->AddAssetManager(std::move(materialManager));
    m_managerFactory->AddAssetManager(std::move(modelManager));

    m_scene->RegisterTickable(std::make_unique<GraphicsEngine>(*m_managerFactory));
    auto graphicsEngine = m_scene->GetTickableOfType<GraphicsEngine>();

    graphicsEngine->SetWindow(m_window.get());
    graphicsEngine->SetRenderer(m_renderer.get());

    InitInput();
    InitGLRenderer();
    InitScene();
    InitLighting();

    m_tickables = m_scene->GetTickables();
}

void Engine::Run()
{
    m_running = true;
    try
    {
        while (m_running)
        {
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
    m_deltaTime = m_deltaTimer.Delta(m_fpsLimit);
    m_fixedTime = m_fixedTimer.Fixed(m_fpsLimit);

    LogMessage("DeltaTime: " + std::to_string(m_deltaTime));
    LogMessage("FixedTime: " + std::to_string(m_fixedTime));
    LogMessage("FPS: " + std::to_string(m_deltaTimer.GetFPS()));

    if (m_reset)
    {
        LogMessage("Resetting...");
        for (ITickable* tickable : m_tickables)
        {
            tickable->Reset();
        }
        m_reset = false;
    }

    for (ITickable* tickable : m_tickables)
    {
        tickable->EarlyTick();
    }

    for (ITickable* tickable : m_tickables)
    {
        tickable->Tick(m_deltaTime);
    }

    for (ITickable* tickable : m_tickables)
    {
        tickable->FixedTick(m_fixedTime);
    }

    for (ITickable* tickable : m_tickables)
    {
        tickable->LateTick();
    }

    glfwPollEvents();
}

void Engine::InitInput()
{
    auto keyboardInputSystem = m_scene->GetTickableOfType<KeyboardInputSystem>();
    auto mouseInputSystem = m_scene->GetTickableOfType<MouseInputSystem>();

    keyboardInputSystem->AddWindowKeyCallback(*m_window, GLFW_KEY_ESCAPE, Action::Release, [&](Modifier) { m_running = false; });
    keyboardInputSystem->AddWindowKeyCallback(*m_window, GLFW_KEY_X, Action::Release, [&](Modifier)   { m_window->ToggleLockedCursor(); });
    keyboardInputSystem->AddWindowKeyCallback(*m_window, GLFW_KEY_R, Action::Release, [&](Modifier)   { if (m_window->IsCursorLocked()) { m_reset = true; }});
    keyboardInputSystem->AddWindowKeyCallback(*m_window, GLFW_KEY_C, Action::Release, [&](Modifier)   { if (m_window->IsCursorLocked()) { m_renderer->SetWireframeActive(!m_renderer->IsWireframeActive()); }});

    keyboardInputSystem->AddWindowKeyCallback(*m_window, GLFW_KEY_A, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitRight   * -m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed), false); }});
    keyboardInputSystem->AddWindowKeyCallback(*m_window, GLFW_KEY_D, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitRight   *  m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed), false); }});
    keyboardInputSystem->AddWindowKeyCallback(*m_window, GLFW_KEY_W, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitForward *  m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed)); }});
    keyboardInputSystem->AddWindowKeyCallback(*m_window, GLFW_KEY_S, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitForward * -m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed)); }});
    keyboardInputSystem->AddWindowKeyCallback(*m_window, GLFW_KEY_Q, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitUp      * -m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed)); }});
    keyboardInputSystem->AddWindowKeyCallback(*m_window, GLFW_KEY_E, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitUp      *  m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed)); }});

    mouseInputSystem->AddDragMouseCallback([&](const DragData& dd)
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
    auto shaderManager = static_cast<ShaderManager*>(m_managerFactory->GetAssetManager<Shader>());
    auto textureManager = static_cast<TextureManager*>(m_managerFactory->GetAssetManager<Texture>());

    shaderManager->Load("pbr",        "assets/shaders/pbr.vert",          "assets/shaders/pbr.frag");
    shaderManager->Load("cubemap",    "assets/shaders/cubemap.vert",      "assets/shaders/equirectangular_to_cubemap.frag");
    shaderManager->Load("irradiance", "assets/shaders/cubemap.vert",      "assets/shaders/irradiance_convolution.frag");
    shaderManager->Load("prefilter",  "assets/shaders/cubemap.vert",      "assets/shaders/prefilter.frag");
    shaderManager->Load("brdf",       "assets/shaders/brdf.vert",         "assets/shaders/brdf.frag");
    shaderManager->Load("background", "assets/shaders/background.vert",   "assets/shaders/background.frag");

    IBLShaders iblShaders = { "pbr", "cubemap", "irradiance", "prefilter", "brdf", "background" };

    auto graphicsEngine = m_scene->GetTickableOfType<GraphicsEngine>();

    textureManager->Load("city", "assets/textures/city_ref.hdr");
    IBL* ibl = new IBL(*shaderManager, *textureManager,
        iblShaders, "city", m_renderer.get(), m_window.get(), m_camera, &graphicsEngine->GetFrameBuffer());

    ibl->AddUniformCallback(*shaderManager->Get("background"), [&](ShaderUniformCallback& shaderUniformCallback, Shader& shader)
    {
        shader.SetMat4fv("projection", m_camera->GetPerspective().GetMatrix());
        shader.SetMat4fv("view",       m_camera->GetView());
        m_scene->GetTickableOfType<GraphicsEngine>()->GetIBL()->Draw();
    });
    ibl->AddUniformCallback(*shaderManager->Get("brdf"), [&](ShaderUniformCallback& shaderUniformCallback, Shader& shader)
    {
        m_window->Update();
    });

    graphicsEngine->SetIBL(ibl);
}

void Engine::InitLighting()
{
    auto shaderManager = static_cast<ShaderManager*>(m_managerFactory->GetAssetManager<Shader>());
    auto textureManager = static_cast<TextureManager*>(m_managerFactory->GetAssetManager<Texture>());

    auto em = m_scene->GetEntityManager();
    auto rootEntity = m_scene->GetRootEntity();

    Entity& redEntity = em->CreateEntity(*rootEntity);
    Entity& blueEntity = em->CreateEntity(*rootEntity);
    Entity& greenEntity = em->CreateEntity(*rootEntity);
    Entity& whiteEntity = em->CreateEntity(*rootEntity);

    redEntity.GetLocalTransform().Translate(vec3(-150, 15., 0));
    blueEntity.GetLocalTransform().Translate(vec3(0, 15., 0));
    greenEntity.GetLocalTransform().Translate(vec3(150, 15., 0));
    whiteEntity.GetLocalTransform().Translate(vec3(0., 30., 0));

    PointLight* redLight = redEntity.AddComponentOfType<PointLight>();
    redLight->SetColour(vec3(255, 0, 0));
    redLight->AddUniformCallback(*shaderManager->Get("pbr"),
    [](ShaderUniformCallback& shaderUniformCallback, Shader& shader)
    {
        auto light = static_cast<const PointLight*>(&shaderUniformCallback);
        shader.Set3f("lightPositions[0]", light->GetEntity().GetLocalTransform().GetTranslation());
        shader.Set3f("lightColours[0]", light->GetColour());
    });

    PointLight* blueLight = blueEntity.AddComponentOfType<PointLight>();
    blueLight->SetColour(vec3(0, 255, 0));
    blueLight->AddUniformCallback(*shaderManager->Get("pbr"),
    [](ShaderUniformCallback& shaderUniformCallback, Shader& shader)
    {
        auto light = static_cast<const PointLight*>(&shaderUniformCallback);
        shader.Set3f("lightPositions[1]", light->GetEntity().GetLocalTransform().GetTranslation());
        shader.Set3f("lightColours[1]", light->GetColour());
    });

    PointLight* greenLight = greenEntity.AddComponentOfType<PointLight>();
    greenLight->SetColour(vec3(0, 0, 255));
    greenLight->AddUniformCallback(*shaderManager->Get("pbr"),
    [](ShaderUniformCallback& shaderUniformCallback, Shader& shader)
    {
        auto light = static_cast<const PointLight*>(&shaderUniformCallback);
        shader.Set3f("lightPositions[2]", light->GetEntity().GetLocalTransform().GetTranslation());
        shader.Set3f("lightColours[2]", light->GetColour());
    });

    PointLight* whiteLight = whiteEntity.AddComponentOfType<PointLight>();
    whiteLight->SetColour(vec3(255, 255, 255));
    whiteLight->AddUniformCallback(*shaderManager->Get("pbr"),
    [](ShaderUniformCallback& shaderUniformCallback, Shader& shader)
    {
        auto light = static_cast<const PointLight*>(&shaderUniformCallback);
        shader.Set3f("lightPositions[3]", light->GetEntity().GetLocalTransform().GetTranslation());
        shader.Set3f("lightColours[3]", light->GetColour());
    });

    // Tests all recursive templated functions
    // m_scene->GetRootEntity()->RemoveComponentsOfType<Lighting::PointLight>();
    // Tests node deletion behaviour
    // rootEntity->RemoveChild(redEntity);
}

void Engine::InitScene()
{
    auto textureManager = static_cast<TextureManager*>(m_managerFactory->GetAssetManager<Texture>());
    auto modelManager = static_cast<ModelManager*>(m_managerFactory->GetAssetManager<Model>());
    auto shaderManager = static_cast<ShaderManager*>(m_managerFactory->GetAssetManager<Shader>());
    auto materialManager = static_cast<MaterialManager*>(m_managerFactory->GetAssetManager<Material>());

    // Sponza model doesn't have AO textures, add default AO texture
    // Use completely black texture to disable IBL lighting
    textureManager->Load("defaultAO", "assets/textures/defaultAO.png");
    textureManager->Load("white", "assets/textures/white.png");
    textureManager->Load("black", "assets/textures/black.png");

    auto em = m_scene->GetEntityManager();
    auto rootEntity = m_scene->GetRootEntity();
    Entity& sponzaEntity = em->CreateEntity(*rootEntity);
    sponzaEntity.SetName("sponza");
    auto& sponzaTransform = sponzaEntity.GetLocalTransform();
    sponzaTransform.Translate(vec3(0.f, -10.f, 0.f));
    sponzaTransform.Scale(vec3(0.25f, 0.25f, 0.25f));

    RenderInstance* sponza = sponzaEntity.AddComponentOfType<RenderInstance>();
    modelManager->Load("sponza", "assets/models/sponza/sponza.obj");
    sponza->Model = modelManager->Get("sponza");

    sponza->AddUniformCallback(*shaderManager->Get("pbr"), [capturedCamera = m_camera](ShaderUniformCallback& shaderUniformCallback, Shader& shader)
    {
        shader.SetMat4fv("projection",  capturedCamera->GetPerspective().GetMatrix());
        shader.SetMat4fv("view",        capturedCamera->GetView());
        shader.Set3f("viewPos",         capturedCamera->GetTranslation());

        auto instance = static_cast<const RenderInstance*>(&shaderUniformCallback);
        shader.SetMat4fv("model", instance->GetEntity().GetWorldTransform().GetMatrix());
    });

    for (auto& mesh : sponza->Model->GetMeshes())
    {
        // Can set uniform callback for meshes if required
        // mesh->AddUniformCallback(*shaderManager->Get("pbr"), [](const Pipeline::ShaderUniformCallback& shaderUniformCallback, const Pipeline::Shader& shader) {});

        auto materialFileName = mesh->GetFileMaterialName();
        if (materialFileName != "")
        {
            if (Material* material = materialManager->Get(materialFileName.data()))
            {
                material->SetTexture("defaultAO", TextureType::Bump);
            }

            materialManager->Get(materialFileName.data())->AddUniformCallback(*shaderManager->Get("pbr"), [](ShaderUniformCallback& shaderUniformCallback, Shader& shader)
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
}
