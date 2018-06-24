#include "stdafx.h"
#include "Engine.h"
#include <iostream>

namespace Engine
{
    Engine::Engine() :
        m_DeltaTime(0.0f),
        m_FPSLimit(1.0f / 60.0f),
        m_FPSLimiter()
    {
    }

    Engine::~Engine()
    {
    }

    void Engine::Start()
    {
        bool run = true;
        try
        {
            while (run)
            {
                m_DeltaTime = m_FPSLimiter.Delta(m_FPSLimit);
                std::cout << "m_delta : " << m_DeltaTime << std::endl;
            }

        }
        catch(...)
        {
            std::cerr << "Error in Engine solution!"  << std::endl;
        }
    }
}