#pragma once

#include "../Core/Timer/FPSLimiter.h"

namespace Engine
{
    class Engine
    {
    public:

        Engine();
        ~Engine();

        void Start();

    private:
        float m_DeltaTime;
        float m_FPSLimit;

        Core::Timer::FPSLimiter m_FPSLimiter;
    };
}