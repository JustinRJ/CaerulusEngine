#include "stdafx.h"

#include "Engine.h"

#include "ECS/AssetManagerFactory.h"
#include "ECS/ComponentManagerFactory.h"

#include "GraphicsEngine.h"
#include "Window/GLWindow.h"
#include "Lighting/IBL.h"
#include "Geometry/Mesh.h"

#include "Input/MouseInputSystem.h"
#include "Input/KeyboardInputSystem.h"

#include "AssetManagers/MaterialManager.h"
#include "ComponentManagers/ModelManager.h"
#include "ComponentManagers/PointLightManager.h"

using namespace Core::ECS;
using namespace Core::Math;
using namespace Core::Time;
using namespace Core::Input;
using namespace Core::Logging;
using namespace Core::Interface;

using namespace Graphics;
using namespace Graphics::Window;
using namespace Graphics::Surface;
using namespace Graphics::Geometry;
using namespace Graphics::Pipeline;
using namespace Graphics::Lighting;
using namespace Graphics::Rendering;
using namespace Graphics::AssetManagers;
using namespace Graphics::ComponentManagers;

Engine::Engine(int argc, char** argv) :
    m_argCount(argc),
    m_argValue(argv)
{
    m_camera                = new Camera(vec3(0, 0, 0), UnitForward, UnitUp);
    m_window                = new GLWindow(m_camera, "Caerulus", 1280, 1024, 32, false);
    m_renderer              = new GLRenderer();
    m_keyboardInputSystem   = new KeyboardInputSystem(*m_window);
    m_mouseInputSystem      = new MouseInputSystem(*m_window);
    m_shaderSrcManager      = new ShaderSourceManager();
    m_shaderManager         = new ShaderManager(*m_shaderSrcManager);
    m_textureManager        = new TextureManager();
    m_materialManager       = new MaterialManager(*m_textureManager);
    m_modelManager          = new ModelManager();
    m_pointLightManager     = new PointLightManager();

    // TODO - populate this with all component managers then pass into individual pointlight / model component managers
    Core::ECS::ComponentManagerFactory factory;
    factory.CreateComponentManagerForType<Graphics::Geometry::Model>();
    factory.CreateComponentManagerForType<Graphics::Lighting::PointLight>();

    m_graphicsEngine = new GraphicsEngine(*m_modelManager, *m_pointLightManager);
    m_graphicsEngine->SetWindow(m_window);
    m_graphicsEngine->SetRenderer(m_renderer);

    m_rootEntity = new Entity(nullptr);

    m_tickable.push_back(m_keyboardInputSystem);
    m_tickable.push_back(m_mouseInputSystem);
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
            m_deltaTime = m_fpsLimiter.Delta(m_fpsLimit);
            m_fixedTime = m_fixedTimer.Fixed(m_fpsLimit);
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
        for (ITickable* tickable : m_tickable)
        {
            tickable->Reset();
        }
        m_reset = false;
    }

    for (ITickable* tickable : m_tickable)
    {
        tickable->EarlyTick();
    }

    for (ITickable* tickable : m_tickable)
    {
        tickable->Tick(m_deltaTime);
    }

    for (ITickable* tickable : m_tickable)
    {
        tickable->FixedTick(m_fixedTime);
    }

    for (ITickable* tickable : m_tickable)
    {
        tickable->LateTick();
    }

    glfwPollEvents();
}

void Engine::InitInput()
{
    m_keyboardInputSystem->AddWindowKeyCallback(*m_window, GLFW_KEY_ESCAPE, Action::Release, [&](Modifier) { m_running = false; });
    m_keyboardInputSystem->AddWindowKeyCallback(*m_window, GLFW_KEY_X, Action::Release, [&](Modifier)   { m_window->ToggleLockedCursor(); });
    m_keyboardInputSystem->AddWindowKeyCallback(*m_window, GLFW_KEY_R, Action::Release, [&](Modifier)   { if (m_window->IsCursorLocked()) { m_reset = true; }});
    m_keyboardInputSystem->AddWindowKeyCallback(*m_window, GLFW_KEY_C, Action::Release, [&](Modifier)   { if (m_window->IsCursorLocked()) { m_renderer->SetWireframeActive(!m_renderer->IsWireframeActive()); }});

    m_keyboardInputSystem->AddWindowKeyCallback(*m_window, GLFW_KEY_A, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitRight   * -m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed), false); }});
    m_keyboardInputSystem->AddWindowKeyCallback(*m_window, GLFW_KEY_D, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitRight   *  m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed), false); }});
    m_keyboardInputSystem->AddWindowKeyCallback(*m_window, GLFW_KEY_W, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitForward *  m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed)); }});
    m_keyboardInputSystem->AddWindowKeyCallback(*m_window, GLFW_KEY_S, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitForward * -m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed)); }});
    m_keyboardInputSystem->AddWindowKeyCallback(*m_window, GLFW_KEY_Q, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitUp      * -m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed)); }});
    m_keyboardInputSystem->AddWindowKeyCallback(*m_window, GLFW_KEY_E, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitUp      *  m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed)); }});

    m_mouseInputSystem->AddDragMouseCallback(m_window, [&](const DragData& dd)
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
    Lighting::IBL* ibl = new IBL(*m_shaderManager, *m_textureManager,
        iblShaders, "city", m_renderer, m_window, m_camera, &m_graphicsEngine->GetFrameBuffer());

    ibl->AddUniformCallback(*m_shaderManager->GetMutable("background"), [&](Pipeline::ShaderUniformCallback& shaderUniformCallback, Pipeline::Shader& shader)
    {
        shader.SetMat4fv("projection", m_camera->GetPerspective().GetMatrix());
        shader.SetMat4fv("view",       m_camera->GetView());

        m_graphicsEngine->GetIBL()->Draw();
    });
    ibl->AddUniformCallback(*m_shaderManager->GetMutable("brdf"), [&](Pipeline::ShaderUniformCallback& shaderUniformCallback, Pipeline::Shader& shader)
    {
        m_window->Update();
    });

    m_graphicsEngine->SetIBL(ibl);
}

void Engine::InitLighting()
{
    Entity* redEntity = new Entity(m_rootEntity);
    Entity* blueEntity = new Entity(m_rootEntity);
    Entity* greenEntity = new Entity(m_rootEntity);
    Entity* whiteEntity = new Entity(m_rootEntity);

    Transform redTransform;
    redTransform.Translate(vec3(-150, 15., 0));
    redEntity->SetLocalTransform(redTransform);

    Transform blueTransform;
    blueTransform.Translate(vec3(0, 15., 0));
    blueEntity->SetLocalTransform(blueTransform);

    Transform greenTransform;
    greenTransform.Translate(vec3(150, 15., 0));
    greenEntity->SetLocalTransform(greenTransform);

    Transform whiteTransform;
    whiteTransform.Translate(vec3(0., 30., 0));
    whiteEntity->SetLocalTransform(whiteTransform);

    PointLight* redLight = redEntity->AddComponentOfType<PointLight>();
    redLight->SetColour(vec3(255, 0, 0));
    redLight->AddUniformCallback(*m_shaderManager->GetMutable("pbr"),
    [](Pipeline::ShaderUniformCallback& shaderUniformCallback, Pipeline::Shader& shader)
    {
        auto light = static_cast<const PointLight*>(&shaderUniformCallback);
        shader.Set3f("lightPositions[0]", light->GetEntity().GetLocalTransform().GetTranslation());
        shader.Set3f("lightColours[0]", light->GetColour());
    });

    PointLight* blueLight = blueEntity->AddComponentOfType<PointLight>();
    blueLight->SetColour(vec3(0, 255, 0));
    blueLight->AddUniformCallback(*m_shaderManager->GetMutable("pbr"),
    [](Pipeline::ShaderUniformCallback& shaderUniformCallback, Pipeline::Shader& shader)
    {
        auto light = static_cast<const PointLight*>(&shaderUniformCallback);
        shader.Set3f("lightPositions[1]", light->GetEntity().GetLocalTransform().GetTranslation());
        shader.Set3f("lightColours[1]", light->GetColour());
    });

    PointLight* greenLight = greenEntity->AddComponentOfType<PointLight>();
    greenLight->SetColour(vec3(0, 0, 255));
    greenLight->AddUniformCallback(*m_shaderManager->GetMutable("pbr"),
    [](Pipeline::ShaderUniformCallback& shaderUniformCallback, Pipeline::Shader& shader)
    {
        auto light = static_cast<const PointLight*>(&shaderUniformCallback);
        shader.Set3f("lightPositions[2]", light->GetEntity().GetLocalTransform().GetTranslation());
        shader.Set3f("lightColours[2]", light->GetColour());
    });

    PointLight* whiteLight = whiteEntity->AddComponentOfType<PointLight>();
    whiteLight->SetColour(vec3(255, 255, 255));
    whiteLight->AddUniformCallback(*m_shaderManager->GetMutable("pbr"),
    [](Pipeline::ShaderUniformCallback& shaderUniformCallback, Pipeline::Shader& shader)
    {
        auto light = static_cast<const PointLight*>(&shaderUniformCallback);
        shader.Set3f("lightPositions[3]", light->GetEntity().GetLocalTransform().GetTranslation());
        shader.Set3f("lightColours[3]", light->GetColour());
    });

    // Tests all recursive templated functions
    // m_rootEntity->RemoveComponentsOfType<Lighting::PointLight>();
    // Tests node deletion behaviour
    // delete redEntity;
}

void Engine::InitScene()
{
    // Sponza model doesn't have AO textures, add default AO texture
    // Use completely black texture to disable IBL lighting
    m_textureManager->Load("defaultAO", "assets/textures/defaultAO.png");
    m_textureManager->Load("white", "assets/textures/white.png");
    m_textureManager->Load("black", "assets/textures/black.png");

    Entity* sponzaEntity = new Entity(m_rootEntity);
    sponzaEntity->SetName("sponza");
    Transform sponzaTransform;
    sponzaTransform.Translate(vec3(0.f, -10.f, 0.f));
    sponzaTransform.Scale(vec3(0.25f, 0.25f, 0.25f));
    sponzaEntity->SetLocalTransform(sponzaTransform);

    Model* model = sponzaEntity->AddComponentOfType<Model>();
    model->Load("assets/models/Sponza/sponza.obj", m_renderer, m_materialManager);
    model->AddUniformCallback(*m_shaderManager->GetMutable("pbr"), [capturedCamera = m_camera](Pipeline::ShaderUniformCallback& shaderUniformCallback, Pipeline::Shader& shader)
    {
        shader.SetMat4fv("projection",  capturedCamera->GetPerspective().GetMatrix());
        shader.SetMat4fv("view",        capturedCamera->GetView());
        shader.Set3f("viewPos",         capturedCamera->GetTranslation());

        auto model = static_cast<const Model*>(&shaderUniformCallback);
        shader.SetMat4fv("model",       model->GetEntity().GetWorldTransform().GetMatrix());
    });

    for (auto& mesh : model->GetMeshes())
    {
        // Can set uniform callback for meshes if required
        // mesh->AddUniformCallback(*m_shaderManager->GetMutable("example"), [](const Pipeline::ShaderUniformCallback& shaderUniformCallback, const Pipeline::Shader& shader) {});

        auto materialFileName = mesh->GetFileMaterialName();
        if (materialFileName != "")
        {
            m_materialManager->SetMaterialTexture(materialFileName, "defaultAO", TextureType::Bump);
            m_materialManager->AddMaterialUniformCallback(materialFileName, *m_shaderManager->GetMutable("pbr"), [](Pipeline::ShaderUniformCallback& shaderUniformCallback, Pipeline::Shader& shader)
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
