#include "stdafx.h"

#include "Engine.h"
#include <iostream>

namespace Engine
{
    Engine::Engine(int argc, char** argv) :
        m_ArgCount(argc),
        m_ArgValue(argv),
        m_Running(false),
        m_DeltaTime(0.0f),
        m_FixedTime(0.0f),
        m_FPSLimit(1.0f / 60.0f),
        m_ModelManager(),
        m_TextureManager(),
        m_MaterialManager(m_TextureManager)
    {
        m_FPSLimiter = std::make_unique<FPSLimiter>();
        m_FixedLimiter = std::make_unique<FixedLimiter>();

        m_RenderSystem = std::make_shared<RenderSystem>();

        m_InputDevice = std::make_shared<OISInputDevice>();
        m_InputDevice->Initialize(m_RenderSystem->GetGLWindow().GetHandle());
        m_Tickable.push_back(m_InputDevice);

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
        unsigned int shaderBall = 0;
        mat4* position = new mat4();
        Core::Math::CreateTansform(*position, glm::vec3(0, 0, -20.0f), glm::quat(), glm::vec3(1.0f));
        auto transformMap = new std::map<unsigned int, mat4*>();
        transformMap->insert(std::make_pair(shaderBall, position));
        m_RenderSystem->SetTransformMap(*transformMap);

        Model* model = m_ModelManager.Get("shaderBall");
        model->SetMaterials(std::vector<Material*>({ m_MaterialManager.Get("gold") }));

        auto modelMap = new std::map<unsigned int, Model*>();
        modelMap->insert(std::make_pair(shaderBall, model));
        m_RenderSystem->SetModelMap(*modelMap);

        using namespace Input;
        m_InputDevice->Command("quit").Set([&]() { m_Running = false; }).Bind(Key::KEY_ESCAPE);
        m_InputDevice->Command("wireframe").Set([&]() { m_RenderSystem->ToggleWireframe(); }).Bind(Key::KEY_X);
        m_InputDevice->Command("sao").Set([&]() { m_RenderSystem->ToggleSAO(); }).Bind(Key::KEY_J);
        m_InputDevice->Command("fxaa").Set([&]() { m_RenderSystem->ToggleFXAA(); }).Bind(Key::KEY_K);
        m_InputDevice->Command("motion_blur").Set([&]() { m_RenderSystem->ToggleMotionBlur(); }).Bind(Key::KEY_L);
        m_InputDevice->Command("point_render").Set([&]() { m_RenderSystem->TogglePointLightRender(); }).Bind(Key::KEY_I);
        m_InputDevice->Command("directional_render").Set([&]() { m_RenderSystem->ToggleDirectionalLightRender(); }).Bind(Key::KEY_O);
        m_InputDevice->Command("enviroment_render").Set([&]() { m_RenderSystem->ToggleEnviromentLightRender(); }).Bind(Key::KEY_P);
        m_InputDevice->Command("tone1_render").Set([&]() { m_RenderSystem->ToggleToneMapping(1); }).Bind(Key::KEY_T);
        m_InputDevice->Command("tone2_render").Set([&]() { m_RenderSystem->ToggleToneMapping(2); }).Bind(Key::KEY_Y);
        m_InputDevice->Command("tone3_render").Set([&]() { m_RenderSystem->ToggleToneMapping(3); }).Bind(Key::KEY_U);

        m_InputDevice->Command("move_forward").Set([&]() { m_RenderSystem->GetCamera().Translate(UnitForward()); }, Input::State::STATE_HOLD).Bind(Key::KEY_W).Bind(Key::KEY_UP);
        m_InputDevice->Command("move_backward").Set([&]() { m_RenderSystem->GetCamera().Translate(-UnitForward()); }, Input::State::STATE_HOLD).Bind(Key::KEY_S).Bind(Key::KEY_DOWN);
        m_InputDevice->Command("move_right").Set([&]() { m_RenderSystem->GetCamera().Translate(UnitRight()); }, Input::State::STATE_HOLD).Bind(Key::KEY_D).Bind(Key::KEY_RIGHT);
        m_InputDevice->Command("move_left").Set([&]() { m_RenderSystem->GetCamera().Translate(-UnitRight()); }, Input::State::STATE_HOLD).Bind(Key::KEY_A).Bind(Key::KEY_LEFT);
        m_InputDevice->Command("move_up").Set([&]() { m_RenderSystem->GetCamera().Translate(UnitUp()); }, Input::State::STATE_HOLD).Bind(Key::KEY_E);
        m_InputDevice->Command("move_down").Set([&]() { m_RenderSystem->GetCamera().Translate(-UnitUp()); }, Input::State::STATE_HOLD).Bind(Key::KEY_Q);
        m_InputDevice->Command("look").Set<glm::vec2>([&](glm::vec2 xy) { m_RenderSystem->GetCamera().Rotate(glm::vec3(xy.x, xy.y, 0.0f)); }).Bind(MouseButton::MOUSE_XY_DELTA);
    }

    Engine::~Engine()
    {
    }

    void Engine::Run()
    {
        m_Running = true;
        //try
        //{
            while (m_Running)
            {
                m_DeltaTime = m_FPSLimiter->Delta(m_FPSLimit);
                m_FixedTime = m_FixedLimiter->Fixed(m_FPSLimit);
                Tick();
            }
        //}
        //catch (...)
        //{
        //    m_Running = false;
        //    std::cerr << "Error in Caerulus Engine!"  << std::endl;
        //}
    }

    void Engine::Tick()
    {
        for (auto updatable : m_Tickable)
        {
            updatable->PreUpdate();
        }

        if (m_FixedTime != 0.0)
        {
            //std::cout << "FixedTime : " << m_FixedTime << std::endl;
            for (auto updatable : m_Tickable)
            {
                updatable->FixedUpdate(m_FixedTime);
            }
        }

        // Todo
        m_RenderSystem->Update(m_DeltaTime);

        if (m_DeltaTime != 0.0)
        {
            //std::cout << "DeltaTime : " << m_DeltaTime << std::endl;
            for (auto updatable : m_Tickable)
            {
                updatable->Update(m_DeltaTime);
            }
        }

        for (auto updatable : m_Tickable)
        {
            updatable->PostUpdate();
        }
    }
}