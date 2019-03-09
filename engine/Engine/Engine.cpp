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
        m_FPSLimit(1.0f / 60.0f)
    {
        m_FPSLimiter = std::make_unique<FPSLimiter>();
        m_FixedLimiter = std::make_unique<FixedLimiter>();

        m_InputManager = std::make_shared<InputManager>();
        m_Tickable.push_back(m_InputManager);

        m_RenderSystem = std::make_shared<RenderSystem>();

        m_InputManager->AddWindowKeyCallback(
            m_RenderSystem->GetGLWindow().GetGLFWWindow(), GLFW_KEY_ESCAPE, GLFW_PRESS, NULL,
            [&]() -> void { m_Running = false; });

        m_InputManager->AddWindowKeyCallback(
            m_RenderSystem->GetGLWindow().GetGLFWWindow(), GLFW_KEY_T, GLFW_PRESS, NULL,
            [&]() -> void { std::cout << "T Pressed" << std::endl; });

        m_InputManager->AddWindowKeyCallback(
            m_RenderSystem->GetGLWindow().GetGLFWWindow(), GLFW_KEY_T, GLFW_RELEASE, NULL,
            [&]() -> void { std::cout << "T Released" << std::endl; });

        m_InputManager->AddWindowKeyCallback(
            m_RenderSystem->GetGLWindow().GetGLFWWindow(), GLFW_KEY_TAB, GLFW_PRESS, NULL,
            [&]() -> void { m_RenderSystem->ToggleWireframe(); });

        m_InputManager->AddWindowKeyCallback(
            m_RenderSystem->GetGLWindow().GetGLFWWindow(), GLFW_KEY_W, GLFW_REPEAT, NULL,
            [&]() -> void { m_RenderSystem->GetCamera().Translate(vec3(0, 0, -10 * m_DeltaTime)); });

        m_InputManager->AddWindowKeyCallback(
            m_RenderSystem->GetGLWindow().GetGLFWWindow(), GLFW_KEY_S, GLFW_REPEAT, NULL,
            [&]() -> void { m_RenderSystem->GetCamera().Translate(vec3(0, 0, 10 * m_DeltaTime)); });

        m_InputManager->AddWindowKeyCallback(
            m_RenderSystem->GetGLWindow().GetGLFWWindow(), GLFW_KEY_A, GLFW_REPEAT, NULL,
            [&]() -> void { m_RenderSystem->GetCamera().Translate(vec3(-10 * m_DeltaTime, 0, 0)); });

        m_InputManager->AddWindowKeyCallback(
            m_RenderSystem->GetGLWindow().GetGLFWWindow(), GLFW_KEY_D, GLFW_REPEAT, NULL,
            [&]() -> void { m_RenderSystem->GetCamera().Translate(vec3(10 * m_DeltaTime, 0, 0)); });

        unsigned int shaderball = 0;

        mat4* position = new mat4();
        Core::Math::CreateTansform(*position, glm::vec3(0, 0, -20.0f), glm::quat(), glm::vec3(1.0f));
        auto transformMap = new std::map<unsigned int, mat4*>();
        transformMap->insert(std::make_pair(shaderball, position));
        m_RenderSystem->SetTransformMap(*transformMap);

        Model* model = new Model("assets/models/shaderball.obj");
        auto modelMap = new std::map<unsigned int, Model*>();
        modelMap->insert(std::make_pair(shaderball, model));
        m_RenderSystem->SetModelMap(*modelMap);
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
        m_InputManager->PreUpdate();
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