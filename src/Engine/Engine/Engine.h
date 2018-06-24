#pragma once

#include "../Core/Time/FPSLimiter.h"

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

        Core::Time::FPSLimiter m_FPSLimiter;
    };
}