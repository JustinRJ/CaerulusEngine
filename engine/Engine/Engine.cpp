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
        window(Graphics::Window::GLWindow("Caerulus", 800, 600, 32))
    {
        m_FPSLimiter = std::make_unique<FPSLimiter>();
        m_FixedLimiter = std::make_unique<FixedLimiter>();

        m_InputManager = std::make_shared<InputManager>();
        m_Tickable.push_back(m_InputManager);

        m_InputManager->AddWindowKeyCallback(
            window.GetGLFWWindow(), GLFW_KEY_ESCAPE, GLFW_PRESS, NULL,
            [&]() -> void { m_Running = false; });

        m_InputManager->AddWindowKeyCallback(
            window.GetGLFWWindow(), GLFW_KEY_T, GLFW_PRESS, NULL,
            [&]() -> void { std::cout << "T Pressed" << std::endl; });

        m_InputManager->AddWindowKeyCallback(
            window.GetGLFWWindow(), GLFW_KEY_T, GLFW_RELEASE, NULL,
            [&]() -> void { std::cout << "T Released" << std::endl; });

        m_InputManager->AddWindowKeyCallback(
            window.GetGLFWWindow(), GLFW_KEY_TAB, GLFW_PRESS, NULL,
            [&]() -> void {
            if (focused)
            {
                glfwSetInputMode(window.GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            else
            {
                glfwSetInputMode(window.GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            }
            focused = !focused;
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