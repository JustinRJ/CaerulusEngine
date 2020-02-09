#include "stdafx.h"

#include "Engine.h"
#include <iostream>
#include "../Core/Node.h"
#include "../Core/Logging/Log.h"

namespace
{
    class Graphics::Window::GLWindow;
}

Engine::Engine(int argc, char** argv) :
    m_ArgCount(argc),
    m_ArgValue(argv),
    m_TextureManager(),
    m_MaterialManager(m_TextureManager),
    m_ModelManager(m_MaterialManager),
    m_ShaderSourceManager(),
    m_ShaderManager(m_ShaderSourceManager)
{
    m_FPSLimiter = std::make_unique<FPSLimiter>();
    m_FixedTimer = std::make_unique<FixedTimer>();

    m_Camera = std::make_shared<Camera>();
    m_Window = std::make_shared<GLWindow>("Caerulus", 1280, 1024, 32, false);
    m_RenderSystem = std::make_shared<RenderSystem>(m_Window, m_Camera);
    m_KeyboardInputManager = std::make_shared<KeyboardInputManager>(m_RenderSystem->GetGLWindow());
    m_MouseInputManager = std::make_shared<MouseInputManager>(m_RenderSystem->GetGLWindow());

    m_Tickable.push_back(m_KeyboardInputManager);
    m_Tickable.push_back(m_MouseInputManager);
    m_Tickable.push_back(m_RenderSystem);

    /// ------------------------------------------------------------------------------------------------------------------------------
    /// TODO - Create component managers which map a 'resource' to a Node ID.
    auto transformMap = std::make_shared<std::map<unsigned int, std::shared_ptr<mat4>>>();
    auto modelMap = std::make_shared<std::map<unsigned int, std::shared_ptr<Model>>>();
    /// ------------------------------------------------------------------------------------------------------------------------------

    // Skybox
    m_TextureManager.LoadHDR("skyBox", "assets/textures/hdr/pisa.hdr");
    m_RenderSystem->SetSkyBox(m_TextureManager.Get("skyBox"));

    // Default material
    //auto defaultMaterial = std::vector<std::shared_ptr<Texture>>(5);
    //m_MaterialManager.Create("defaultMaterial", defaultMaterial);
    //m_RenderSystem->SetDefaultMaterial(m_MaterialManager.Get("defaultMaterial"));

    // Shaderball model
    m_ModelManager.Load("shaderBall", "assets/models/shaderBall.obj");

    // Gold textures
    m_TextureManager.Load("goldAlbedo",     "assets/textures/pbr/gold/gold_albedo.png");
    m_TextureManager.Load("goldNormal",     "assets/textures/pbr/gold/gold_normal.png");
    m_TextureManager.Load("goldRoughness",  "assets/textures/pbr/gold/gold_roughness.png");
    m_TextureManager.Load("goldMetallic",   "assets/textures/pbr/gold/gold_metallic.png");
    m_TextureManager.Load("goldAO",         "assets/textures/pbr/gold/gold_ao.png");

    // Gold material
    auto gold = std::vector<std::shared_ptr<Texture>>(MaterialType::Size);
    gold[Albedo] = m_TextureManager.Get("goldAlbedo");
    gold[Normal] = m_TextureManager.Get("goldNormal");
    gold[Roughness] = m_TextureManager.Get("goldRoughness");
    gold[Metallic] = m_TextureManager.Get("goldMetallic");
    gold[AO] = m_TextureManager.Get("goldAO");
    m_MaterialManager.Create("gold", gold);

    // ShaderBall entity setup
    Core::Node shaderBall = Core::Node();
    auto position1 = std::make_shared<mat4>();
    auto model1 = m_ModelManager.Get("shaderBall");
    MathHelper::CreateTansform(*position1, vec3(0.0f, 0.0f, -20.0f), quat(), vec3(1.0f));
    transformMap->insert(std::make_pair(shaderBall.GetID(), position1));
    model1->SetMaterials(std::vector<std::shared_ptr<Material>>({ m_MaterialManager.Get("gold") }));
    modelMap->insert(std::make_pair(shaderBall.GetID(), model1));

    // Sponza model and material
    m_ModelManager.Load("sponza", "assets/models/sponza/sponza.obj" /*no material path - in same dir as obj*/);

    // Sponza entity setup
    Core::Node sponza = Core::Node();
    auto position2 = std::make_shared<mat4>();
    auto model2 = m_ModelManager.Get("sponza");
    MathHelper::CreateTansform(*position2, vec3(0.0f, -7.0f, 0.0f), quat(), vec3(0.33f));
    transformMap->insert(std::make_pair(sponza.GetID(), position2));
    modelMap->insert(std::make_pair(sponza.GetID(), model2));

    // Fix sponza model no AOs
    m_TextureManager.Load("default", "assets/textures/default.png");
    for (auto material : model2->GetMaterials())
    {
        if (material && !material->GetTexture(MaterialType::AO))
        {
            material->SetTexture(m_TextureManager.Get("default"), MaterialType::AO);
        }
    }

    /// ------------------------------------------------------------------------------------------------------------------------------
    /// Set shaders for render system   - needs to be switched to a vector or map of shaders that execute in a specified order
    ///                                 - need to pull all log for setting up shader out of render system into the shader class and make
    ///                                 - it dynamically set uniform locations

    m_ShaderManager.Load("gBuffer",         "assets/shaders/gBuffer.vert",                  "assets/shaders/gBuffer.frag");
    m_ShaderManager.Load("latlongToCube",   "assets/shaders/latlongToCube.vert",            "assets/shaders/latlongToCube.frag");
    m_ShaderManager.Load("simple",          "assets/shaders/lighting/simple.vert",          "assets/shaders/lighting/simple.frag");
    m_ShaderManager.Load("lightingBRDF",    "assets/shaders/lighting/lightingBRDF.vert",    "assets/shaders/lighting/lightingBRDF.frag");
    m_ShaderManager.Load("irradianceIBL",   "assets/shaders/lighting/irradianceIBL.vert",   "assets/shaders/lighting/irradianceIBL.frag");
    m_ShaderManager.Load("prefilterIBL",    "assets/shaders/lighting/prefilterIBL.vert",    "assets/shaders/lighting/prefilterIBL.frag");
    m_ShaderManager.Load("integrateIBL",    "assets/shaders/lighting/integrateIBL.vert",    "assets/shaders/lighting/integrateIBL.frag");
    m_ShaderManager.Load("firstpass",       "assets/shaders/postprocess/postprocess.vert",  "assets/shaders/postprocess/firstpass.frag");
    m_ShaderManager.Load("sao",             "assets/shaders/postprocess/sao.vert",          "assets/shaders/postprocess/sao.frag");
    m_ShaderManager.Load("saoBlur",         "assets/shaders/postprocess/sao.vert",          "assets/shaders/postprocess/saoBlur.frag");

    auto shaders = std::make_shared<StandardShaders>();
    shaders->GBuffer = m_ShaderManager.Get("gBuffer");
    shaders->LatlongToCube = m_ShaderManager.Get("latlongToCube");
    shaders->Simple = m_ShaderManager.Get("simple");
    shaders->LightingBRDF = m_ShaderManager.Get("lightingBRDF");
    shaders->IrradianceIBL = m_ShaderManager.Get("irradianceIBL");
    shaders->PrefilterIBL = m_ShaderManager.Get("prefilterIBL");
    shaders->IntegrateIBL = m_ShaderManager.Get("integrateIBL");
    shaders->FirstPassPostProcess = m_ShaderManager.Get("firstpass");
    shaders->SAO = m_ShaderManager.Get("sao");
    shaders->SAOBlur = m_ShaderManager.Get("saoBlur");

    m_RenderSystem->SetShaders(shaders);
    m_RenderSystem->LoadShaders();

    m_RenderSystem->SetTransformMap(*transformMap);
    m_RenderSystem->SetModelMap(*modelMap);

    /// ------------------------------------------------------------------------------------------------------------------------------

    using namespace Core::Input;
    auto window = m_RenderSystem->GetGLWindow();
    m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_ESCAPE,   Action::Release, [=](Modifier) { m_Running = false; });
    m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_TAB,      Action::Release, [&](Modifier) { m_RenderSystem->GetGLWindow()->ToggleLockedCursor(); });
    m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_X,        Action::Release, [&](Modifier) { m_RenderSystem->ToggleWireframe(); });
    m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_J,        Action::Release, [&](Modifier) { m_RenderSystem->ToggleSAO(); });
    m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_K,        Action::Release, [&](Modifier) { m_RenderSystem->ToggleFXAA(); });
    m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_L,        Action::Release, [&](Modifier) { m_RenderSystem->ToggleMotionBlur(); });
    m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_P,        Action::Release, [&](Modifier) { m_RenderSystem->TogglePointLightRender(); });
    m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_O,        Action::Release, [&](Modifier) { m_RenderSystem->ToggleDirectionalLightRender(); });
    m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_I,        Action::Release, [&](Modifier) { m_RenderSystem->ToggleEnviromentLightRender(); });
    //m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_T, Release, [&](Modifier) { m_RenderSystem->ToggleToneMapping(1); });
    //m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_Y, Release, [&](Modifier) { m_RenderSystem->ToggleToneMapping(2); });
    //m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_U, Release, [&](Modifier) { m_RenderSystem->ToggleToneMapping(3); });

    m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_A, Action::Hold, [&](Modifier m) { m_RenderSystem->GetCamera()->TranslateXZ(-MathHelper::UnitRight() * m_DeltaTime * (m == Modifier::Shift ? m_SprintSpeed : m_NormalSpeed)); });
    m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_D, Action::Hold, [&](Modifier m) { m_RenderSystem->GetCamera()->TranslateXZ(MathHelper::UnitRight()  * m_DeltaTime * (m == Modifier::Shift ? m_SprintSpeed : m_NormalSpeed)); });
    m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_W, Action::Hold, [&](Modifier m) { m_RenderSystem->GetCamera()->Translate(MathHelper::UnitForward()  * m_DeltaTime * (m == Modifier::Shift ? m_SprintSpeed : m_NormalSpeed)); });
    m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_S, Action::Hold, [&](Modifier m) { m_RenderSystem->GetCamera()->Translate(-MathHelper::UnitForward() * m_DeltaTime * (m == Modifier::Shift ? m_SprintSpeed : m_NormalSpeed)); });
    m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_Q, Action::Hold, [&](Modifier m) { m_RenderSystem->GetCamera()->Translate(-MathHelper::UnitUp()      * m_DeltaTime * (m == Modifier::Shift ? m_SprintSpeed : m_NormalSpeed)); });
    m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_E, Action::Hold, [&](Modifier m) { m_RenderSystem->GetCamera()->Translate(MathHelper::UnitUp()       * m_DeltaTime * (m == Modifier::Shift ? m_SprintSpeed : m_NormalSpeed )); });

    m_MouseInputManager->AddDragMouseCallback(window, [&](DragData dd)
    { 
        if (m_RenderSystem->GetGLWindow()->IsCursorLocked())
        {
            m_RenderSystem->GetCamera()->Rotate(vec3(dd.DeltaX * m_DeltaTime * m_MouseSensitivity, dd.DeltaY * m_DeltaTime * m_MouseSensitivity, 0.0f));
        }
    });
}

Engine::~Engine()
{
}

void Engine::Run()
{
    using namespace Core::Logging;
    m_Running = true;
    try
    {
        while (m_Running)
        {
            m_DeltaTime = m_FPSLimiter->Delta(m_FPSLimit);
            m_FixedTime = m_FixedTimer->Fixed(m_FPSLimit);
            Tick();
        }
    }
    catch (const std::exception& e)
    {
        m_Running = false;
        Log::LogException("Error in Caerulus Engine", e.what());
    }
    catch (...)
    {
        m_Running = false;
        Log::LogError("Unknown error in Caerulus Engine");
    }
}

void Engine::Tick()
{
    using Core::Logging::Log;
    Log::LogInDebug("DeltaTime: " + std::to_string(m_DeltaTime));
    Log::LogInDebug("FixedTime: " + std::to_string(m_FixedTime));

    glfwPollEvents();

    for (auto updatable : m_Tickable)
    {
        updatable->PreUpdate(m_DeltaTime);
    }

    for (auto updatable : m_Tickable)
    {
        updatable->Update(m_DeltaTime);
    }

    for (auto updatable : m_Tickable)
    {
        updatable->FixedUpdate(m_FixedTime);
    }
}
