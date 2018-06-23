#include "stdafx.h"

#include "Engine.h"

#include "../Core/Timer/Timer.h"
#include "../Core/Timer/FPSLimiter.h"
#include "../Core/Math/MathHelper.h"

namespace Engine
{
    Engine::Engine()
    {
    }

    Engine::~Engine()
    {
    }



    void Engine::Start()
    {
        auto timer = Core::Timer::Timer(false);
        timer.Start();

        try
        {
        }
        catch(...)
        {
        }
    }
}