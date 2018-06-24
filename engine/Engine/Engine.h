#pragma once

#include "../Core/Time/FPSLimiter.h"
#include "../Core/Input/OISInputDevice.hpp"

#include <memory>

namespace Engine
{
    using namespace Core;

    class Engine
    {
    public:

        Engine(int argc, char** argv);
        ~Engine();

        void Run();

    private:
        int m_ArgCount;
        char** m_ArgValue;
        bool m_Running;
        float m_DeltaTime;
        float m_FPSLimit;

        std::unique_ptr<Time::FPSLimiter> m_FPSLimiter;
        std::unique_ptr<Input::InputDevice> m_InputDevice;
    };
}