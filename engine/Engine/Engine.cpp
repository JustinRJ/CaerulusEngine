#include "stdafx.h"

#include "Engine.h"
#include <iostream>
#include "../Core/Node.h"

namespace Engine
{
    Engine::Engine(int argc, char** argv) :
        m_ArgCount(argc),
        m_ArgValue(argv),
        m_ModelManager(),
        m_TextureManager(),
        m_MaterialManager(m_TextureManager)
    {
        m_FPSLimiter = std::make_unique<FPSLimiter>();
        m_FixedLimiter = std::make_unique<FixedLimiter>();

        m_Window = std::make_shared<GLWindow>("Caerulus", 800, 600, 32);
        m_RenderSystem = std::make_shared<RenderSystem>(*m_Window);
        m_KeyboardInputManager = std::make_shared<KeyboardInputManager>(&m_RenderSystem->GetGLWindow());
        m_MouseInputManager = std::make_shared<MouseInputManager>(&m_RenderSystem->GetGLWindow());

        m_Tickable.push_back(m_KeyboardInputManager);
        m_Tickable.push_back(m_MouseInputManager);
        m_Tickable.push_back(m_RenderSystem);

        m_TextureManager.LoadHDR("skyBox", "assets/textures/hdr/pisa.hdr");
        m_RenderSystem->SetSkyBox(*m_TextureManager.Get("skyBox"));

        m_ModelManager.Load("shaderBall", "assets/models/shaderBall.obj");

        //gold textures
        m_TextureManager.Load("goldAlbedo",     "assets/textures/pbr/gold/gold_albedo.png");
        m_TextureManager.Load("goldNormal",     "assets/textures/pbr/gold/gold_normal.png");
        m_TextureManager.Load("goldRoughness",  "assets/textures/pbr/gold/gold_roughness.png");
        m_TextureManager.Load("goldMetallic",   "assets/textures/pbr/gold/gold_metallic.png");
        m_TextureManager.Load("goldAO",         "assets/textures/pbr/gold/gold_ao.png");

        //gold Materials
        std::vector<Texture*> gold = std::vector<Texture*>(5);
        gold[Albedo] = m_TextureManager.Get("goldAlbedo");
        gold[Normal] = m_TextureManager.Get("goldNormal");
        gold[Roughness] = m_TextureManager.Get("goldRoughness");
        gold[Metallic] = m_TextureManager.Get("goldMetallic");
        gold[AO] = m_TextureManager.Get("goldAO");
        m_MaterialManager.Create("gold", gold);

        //shaderBall
        Core::Node shaderBall = Core::Node();

        mat4* position = new mat4();
        MathHelper::CreateTansform(*position, glm::vec3(0, 0, -20.0f), glm::quat(), glm::vec3(1.0f));
        auto transformMap = new std::map<unsigned int, mat4*>();
        transformMap->insert(std::make_pair(shaderBall.GetID(), position));
        m_RenderSystem->SetTransformMap(*transformMap);

        Model* model = m_ModelManager.Get("shaderBall");
        model->SetMaterials(std::vector<Material*>({ m_MaterialManager.Get("gold") }));

        auto modelMap = new std::map<unsigned int, Model*>();
        modelMap->insert(std::make_pair(shaderBall.GetID(), model));
        m_RenderSystem->SetModelMap(*modelMap);

        using namespace Core::Input;
        GLWindow* window = &m_RenderSystem->GetGLWindow();
        m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_ESCAPE, Release, [=](Modifier) { m_Running = false; });
        m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_TAB, Release, [&](Modifier) { m_RenderSystem->GetGLWindow().ToggleLockedCursor(); });
        m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_X, Release, [&](Modifier) { m_RenderSystem->ToggleWireframe(); });
        m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_J, Release, [&](Modifier) { m_RenderSystem->ToggleSAO(); });
        m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_K, Release, [&](Modifier) { m_RenderSystem->ToggleFXAA(); });
        m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_L, Release, [&](Modifier) { m_RenderSystem->ToggleMotionBlur(); });
        m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_I, Release, [&](Modifier) { m_RenderSystem->TogglePointLightRender(); });
        m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_O, Release, [&](Modifier) { m_RenderSystem->ToggleDirectionalLightRender(); });
        m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_P, Release, [&](Modifier) { m_RenderSystem->ToggleEnviromentLightRender(); });
        m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_T, Release, [&](Modifier) { m_RenderSystem->ToggleToneMapping(1); });
        m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_Y, Release, [&](Modifier) { m_RenderSystem->ToggleToneMapping(2); });
        m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_U, Release, [&](Modifier) { m_RenderSystem->ToggleToneMapping(3); });

        m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_A, Hold, [&](Modifier) { m_RenderSystem->GetCamera().TranslateXZ(-MathHelper::UnitRight()); });
        m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_D, Hold, [&](Modifier) { m_RenderSystem->GetCamera().TranslateXZ(MathHelper::UnitRight()); });
        m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_W, Hold, [&](Modifier) { m_RenderSystem->GetCamera().Translate(MathHelper::UnitForward()); });
        m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_S, Hold, [&](Modifier) { m_RenderSystem->GetCamera().Translate(-MathHelper::UnitForward()); });
        m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_Q, Hold, [&](Modifier) { m_RenderSystem->GetCamera().Translate(-MathHelper::UnitUp()); });
        m_KeyboardInputManager->AddWindowKeyCallback(window, GLFW_KEY_E, Hold, [&](Modifier) { m_RenderSystem->GetCamera().Translate(MathHelper::UnitUp()); });

        m_MouseInputManager->AddDragMouseCallback(window, [&](DragData dd)
        { 
            if (m_RenderSystem->GetGLWindow().IsCursorLocked())
            {
                m_RenderSystem->GetCamera().Rotate(glm::vec3(dd.DeltaX * m_DeltaTime * m_MouseSensitivity, dd.DeltaY * m_DeltaTime * m_MouseSensitivity, 0.0f));
            }
        });
    }

    Engine::~Engine()
    {
    }

    void Engine::Run()
    {
        m_Running = true;
        try
        {
            while (m_Running)
            {
                m_DeltaTime = m_FPSLimiter->Delta(m_FPSLimit);
                m_FixedTime = m_FixedLimiter->Fixed(m_FPSLimit);
                Tick();
            }
        }
        catch (...)
        {
            m_Running = false;
            std::cerr << "Error in Caerulus Engine!"  << std::endl;
        }
    }

    void Engine::Tick()
    {
        glfwPollEvents();

        for (auto updatable : m_Tickable)
        {
            updatable->Update(m_DeltaTime);
        }

        //std::cout << "DeltaTime : " << m_DeltaTime << std::endl;
        for (auto updatable : m_Tickable)
        {
            updatable->Update(m_DeltaTime);
        }

        //std::cout << "FixedTime : " << m_FixedTime << std::endl;
        for (auto updatable : m_Tickable)
        {
            updatable->FixedUpdate(m_FixedTime);
        }

        for (auto updatable : m_Tickable)
        {
            updatable->LateUpdate(m_DeltaTime);
        }
    }
}