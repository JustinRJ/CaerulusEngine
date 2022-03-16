#include "stdafx.h"

#include "Engine.h"

#include "ECS/Entity.h"
#include "Math/Math.h"
#include "Math/Camera.h"

#include "Logging/Log.h"
#include "Logging/LogToFile.h"

#include "Interface/ITickable.h"
#include "Interface/NonCopyable.h"

#include "GraphicsEngine.h"
#include "Window/GLWindow.h"
#include "Rendering/GLRenderer.h"

#include "Input/MouseInputSystem.h"
#include "Input/KeyboardInputSystem.h"

#include "AssetManagers/TextureManager.h"
#include "AssetManagers/MaterialManager.h"
#include "AssetManagers/ShaderSourceManager.h"
#include "AssetManagers/ShaderManager.h"
#include "ComponentManagers/ModelManager.h"
#include "ComponentManagers/PointLightManager.h"

#include "Lighting/IBL.h"
#include "Geometry/Mesh.h"

using namespace Core::ECS;
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
    m_camera                = std::make_shared<Camera>(vec3(0, 0, 0), UnitForward, UnitUp);
    m_window                = std::make_shared<GLWindow>(m_camera, "Caerulus", 1280, 1024, 32, false);
    m_renderer              = std::make_shared<GLRenderer>();
    m_keyboardInputSystem   = std::make_shared<KeyboardInputSystem>(m_window);
    m_mouseInputSystem      = std::make_shared<MouseInputSystem>(m_window);
    m_shaderSrcManager      = std::make_shared<ShaderSourceManager>();
    m_shaderManager         = std::make_shared<ShaderManager>(*m_shaderSrcManager);
    m_textureManager        = std::make_shared<TextureManager>();
    m_materialManager       = std::make_shared<MaterialManager>(*m_shaderManager, *m_textureManager);
    m_modelManager          = std::make_shared<ModelManager>(*m_materialManager, m_renderer.get());
    m_pointLightManager     = std::make_shared<PointLightManager>();

    m_graphicsEngine = std::make_shared<GraphicsEngine>(*m_modelManager, *m_pointLightManager, m_window, m_renderer);

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
        for (const std::shared_ptr<ITickable>& tickable : m_tickable)
        {
            tickable->Reset();
        }
        m_reset = false;
    }

    for (const std::shared_ptr<ITickable>& tickable : m_tickable)
    {
        tickable->EarlyTick();
    }

    for (const std::shared_ptr<ITickable>& tickable : m_tickable)
    {
        tickable->Tick(m_deltaTime);
    }

    for (const std::shared_ptr<ITickable>& tickable : m_tickable)
    {
        tickable->FixedTick(m_fixedTime);
    }

    for (const std::shared_ptr<ITickable>& tickable : m_tickable)
    {
        tickable->LateTick();
    }

    glfwPollEvents();
}

void Engine::InitInput()
{
    m_keyboardInputSystem->AddWindowKeyCallback(m_window, GLFW_KEY_ESCAPE, Action::Release, [&](Modifier) { m_running = false; });
    m_keyboardInputSystem->AddWindowKeyCallback(m_window, GLFW_KEY_X, Action::Release, [&](Modifier)   { m_window->ToggleLockedCursor(); });
    m_keyboardInputSystem->AddWindowKeyCallback(m_window, GLFW_KEY_R, Action::Release, [&](Modifier)   { if (m_window->IsCursorLocked()) { m_reset = true; }});
    m_keyboardInputSystem->AddWindowKeyCallback(m_window, GLFW_KEY_C, Action::Release, [&](Modifier)   { if (m_window->IsCursorLocked()) { m_renderer->SetWireframe(!m_renderer->GetWireframe()); }});

    m_keyboardInputSystem->AddWindowKeyCallback(m_window, GLFW_KEY_A, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitRight   * -m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed), false); }});
    m_keyboardInputSystem->AddWindowKeyCallback(m_window, GLFW_KEY_D, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitRight   *  m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed), false); }});
    m_keyboardInputSystem->AddWindowKeyCallback(m_window, GLFW_KEY_W, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitForward *  m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed)); }});
    m_keyboardInputSystem->AddWindowKeyCallback(m_window, GLFW_KEY_S, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitForward * -m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed)); }});
    m_keyboardInputSystem->AddWindowKeyCallback(m_window, GLFW_KEY_Q, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitUp      * -m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed)); }});
    m_keyboardInputSystem->AddWindowKeyCallback(m_window, GLFW_KEY_E, Action::Hold, [&](Modifier m) { if (m_window->IsCursorLocked()) { m_camera->Translate(UnitUp      *  m_deltaTime * (m == Modifier::Shift ? m_sprintSpeed : m_normalSpeed)); }});

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
    std::shared_ptr<Lighting::IBL> ibl = std::make_shared<IBL>(*m_shaderManager, *m_textureManager,
        iblShaders, "city", m_renderer, m_window, m_camera, m_graphicsEngine->GetFrameBuffer());

    ibl->AddUniformCallback(*m_shaderManager->Get("background"), [&](const Pipeline::ShaderUniformCallback& shaderUniformCallback, const Pipeline::Shader& shader)
    {
        shader.SetMat4fv("projection", m_camera->GetPerspective().GetMatrix());
        shader.SetMat4fv("view",       m_camera->GetView());

        m_graphicsEngine->GetIBL()->Draw();
    });
    ibl->AddUniformCallback(*m_shaderManager->Get("brdf"), [&](const Pipeline::ShaderUniformCallback& shaderUniformCallback, const Pipeline::Shader& shader)
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

    // TODO - transform bug causing all lights to overlap
    Transform redTransform;
    redTransform.Translate(vec3(-150, 15., 0));
    redEntity->SetWorldTransform(redTransform);

    Transform blueTransform;
    blueTransform.Translate(vec3(0, 15., 0));
    blueEntity->SetWorldTransform(blueTransform);

    Transform greenTransform;
    greenTransform.Translate(vec3(150, 15., 0));
    greenEntity->SetWorldTransform(greenTransform);

    Transform whiteTransform;
    whiteTransform.Translate(vec3(0., 30., 0));
    whiteEntity->SetWorldTransform(whiteTransform);

    m_pointLightManager->Create(*redEntity, vec3(255, 0, 0));
    m_pointLightManager->Create(*blueEntity, vec3(0, 255, 0));
    m_pointLightManager->Create(*greenEntity, vec3(0, 0, 255));
    m_pointLightManager->Create(*whiteEntity, vec3(255, 255, 255));

    m_pointLightManager->AddPointLightUniformCallback(*redEntity, *m_shaderManager->Get("pbr"),
        [](const Pipeline::ShaderUniformCallback& shaderUniformCallback, const Pipeline::Shader& shader)
    {
        auto light = dynamic_cast<const PointLight*>(&shaderUniformCallback);
        shader.Set3f("lightPositions[0]", light->GetEntity().GetWorldTransform().GetTranslation());
        shader.Set3f("lightColours[0]", light->GetColour());
    });

    m_pointLightManager->AddPointLightUniformCallback(*blueEntity, *m_shaderManager->Get("pbr"),
        [](const Pipeline::ShaderUniformCallback& shaderUniformCallback, const Pipeline::Shader& shader)
    {
        auto light = dynamic_cast<const PointLight*>(&shaderUniformCallback);
        shader.Set3f("lightPositions[1]", light->GetEntity().GetWorldTransform().GetTranslation());
        shader.Set3f("lightColours[1]", light->GetColour());
    });

    m_pointLightManager->AddPointLightUniformCallback(*greenEntity, *m_shaderManager->Get("pbr"),
        [](const Pipeline::ShaderUniformCallback& shaderUniformCallback, const Pipeline::Shader& shader)
    {
        auto light = dynamic_cast<const PointLight*>(&shaderUniformCallback);
        shader.Set3f("lightPositions[2]", light->GetEntity().GetWorldTransform().GetTranslation());
        shader.Set3f("lightColours[2]", light->GetColour());
    });

    m_pointLightManager->AddPointLightUniformCallback(*whiteEntity, *m_shaderManager->Get("pbr"),
        [](const Pipeline::ShaderUniformCallback& shaderUniformCallback, const Pipeline::Shader& shader)
    {
        auto light = dynamic_cast<const PointLight*>(&shaderUniformCallback);
        shader.Set3f("lightPositions[3]", light->GetEntity().GetWorldTransform().GetTranslation());
        shader.Set3f("lightColours[3]", light->GetColour());
    });
}

void Engine::InitScene()
{
    Entity* sponzaEntity = new Entity(m_rootEntity);
    sponzaEntity->SetName("sponza");
    Transform sponzaTransform;
    sponzaTransform.Translate(vec3(0.f, -10.f, 0.f));
    sponzaTransform.Scale(vec3(0.25f, 0.25f, 0.25f));
    sponzaEntity->SetLocalTransform(sponzaTransform);

    // Only set shaders and unform callback for sponza model 
    if (const Model* model = m_modelManager->Load(*sponzaEntity, "assets/models/Sponza/sponza.obj"))
    {
        // Sponza model doesn't have AO textures, add default AO texture
        // Use completely black texture to disable IBL lighting
        m_textureManager->Load("defaultAO", "assets/textures/defaultAO.png");
        m_textureManager->Load("white", "assets/textures/white.png");
        m_textureManager->Load("black", "assets/textures/black.png");

        // Set uniform callback to update each models MVP before it's rendered
        m_modelManager->AddModelUniformCallback(*sponzaEntity, *m_shaderManager->Get("pbr"), [capturedCamera = m_camera](const Pipeline::ShaderUniformCallback& shaderUniformCallback, const Pipeline::Shader& shader)
        {
            shader.SetMat4fv("projection",  capturedCamera->GetPerspective().GetMatrix());
            shader.SetMat4fv("view",        capturedCamera->GetView());
            shader.Set3f("viewPos",         capturedCamera->GetTranslation());

            auto model = dynamic_cast<const Model*>(&shaderUniformCallback);
            shader.SetMat4fv("model",       model->GetEntity().GetWorldTransform().GetMatrix());
        });

        for (int i = 0; i < model->GetMeshes().size(); ++i)
        {
            auto mesh = model->GetMeshes()[i];

            // Can set uniform callback for meshes if required
            //mesh->AddUniformCallback(*m_shaderManager->Get("example"), [](const Pipeline::ShaderUniformCallback& shaderUniformCallback, const Pipeline::Shader& shader) {});

            auto materialFileName = mesh->GetFileMaterialName();
            mesh->SetMaterial(*m_materialManager->Get(materialFileName));

            if (materialFileName != "")
            {
                m_materialManager->SetMaterialTexture(materialFileName, "defaultAO", TextureType::Bump);
                m_materialManager->AddMaterialUniformCallback(materialFileName, *m_shaderManager->Get("pbr"), [](const Pipeline::ShaderUniformCallback& shaderUniformCallback, const Pipeline::Shader& shader)
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
}
