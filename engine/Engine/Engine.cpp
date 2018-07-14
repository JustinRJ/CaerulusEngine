#include "stdafx.h"
#include "Engine.h"

#include "../Graphics/Graphics.h"

namespace Engine
{
    Engine::Engine(int argc, char** argv) :
        m_ArgCount(argc),
        m_ArgValue(argv),
        m_Running(false),
        m_DeltaTime(0.0f),
        m_FPSLimit(1.0f / 60.0f)
    {
        m_FPSLimiter.reset(std::make_unique<FPSLimiter>().release());
        m_InputDevice.reset(std::make_unique<OISInputDevice>().release());

        m_InputDevice->Initialize(Hwnd());
        m_InputDevice->Command("Quit").Set([&]() { m_Running = false; }).Bind(Key::KEY_ESCAPE);
        m_InputDevice->Command("Press1").Set([&]() { std::cout << "Press1" << std::endl; }, State::STATE_PRESS).Bind(Key::KEY_P);
        m_InputDevice->Command("Press2").Set([&]() { std::cout << "Press2" << std::endl; }, State::STATE_HOLD).Bind(Key::KEY_P);
        m_InputDevice->Command("Release1").Set([&]() { std::cout << "Release1" << std::endl; }, State::STATE_RELEASE).Bind(Key::KEY_R);
        m_InputDevice->Command("Release2").Set([&]() { std::cout << "Release2" << std::endl; }, State::STATE_HOLD).Bind(Key::KEY_R);
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
                //std::cout << "DeltaTime : " << m_DeltaTime << std::endl;
                m_InputDevice->Update();
            }
        }
        catch (...)
        {
            m_Running = false;
            std::cerr << "Error in Caerulus Engine!"  << std::endl;
        }
    }
}