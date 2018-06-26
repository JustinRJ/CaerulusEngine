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
        m_FPSLimiter.reset(std::make_unique<Time::FPSLimiter>().release());
        m_InputDevice.reset(std::make_unique<Input::OISInputDevice>().release());
        m_InputDevice->Initialize(Hwnd());
        m_InputDevice->Command("quit").Set([&]() { m_Running = false; }).Bind(Input::KeyCode::KEY_ESCAPE);
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
        catch(...)
        {
            std::cerr << "Error in Engine solution!"  << std::endl;
        }
    }
}