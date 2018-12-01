#include "stdafx.h"

#include "Engine.h"

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
        m_FPSLimiter.reset(std::make_unique<Time::FPSLimiter>().release());
        m_FixedLimiter.reset(std::make_unique<Time::FixedLimiter>().release());

        m_InputDevice.reset(std::make_unique<OISInputDevice>().release());
        m_Updatable.push_back(m_InputDevice);

        m_InputDevice->Initialize(window.GetHandle());
        m_InputDevice->Command("Quit").Set([&]() { m_Running = false; }).Bind(Key::KEY_ESCAPE);
        m_InputDevice->Command("P-Press").Set([&]() { std::cout << "P-Press" << std::endl; }, State::STATE_PRESS).Bind(Key::KEY_P);
        m_InputDevice->Command("P-Hold").Set([&]() { std::cout << "P-Hold" << std::endl; }, State::STATE_HOLD).Bind(Key::KEY_P);
        m_InputDevice->Command("P-Release").Set([&]() { std::cout << "R-Release" << std::endl; }, State::STATE_RELEASE).Bind(Key::KEY_P);
        //m_InputDevice->Command("R-Hold").Set([&]() { std::cout << "R-Hold" << std::endl; }, State::STATE_HOLD).Bind(Key::KEY_R);
        m_InputDevice->Command("Focus").Set([&]() { window.Focus(); }, State::STATE_HOLD).Bind(Key::KEY_TAB);
        m_InputDevice->PrintCommands();
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
        for (auto updatable : m_Updatable)
        {
            updatable->PreUpdate();
        }

        if (m_DeltaTime != 0.0)
        {
            //std::cout << "DeltaTime : " << m_DeltaTime << std::endl;
            for (auto updatable : m_Updatable)
            {
                updatable->Update(m_DeltaTime);
            }
        }

        if (m_FixedTime != 0.0)
        {
            //std::cout << "FixedTime : " << m_FixedTime << std::endl;
            for (auto updatable : m_Updatable)
            {
                updatable->FixedUpdate(m_FixedTime);
            }
        }

        for (auto updatable : m_Updatable)
        {
            updatable->PostUpdate();
        }
    }
}