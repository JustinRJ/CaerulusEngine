#pragma once

#include "../Core/Time/FPSLimiter.h"
#include "../Input/Input/OISInputDevice.h"

namespace Engine
{
    using namespace Core::Time;
    using namespace Core::Input;

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

        std::unique_ptr<FPSLimiter> m_FPSLimiter;
        std::unique_ptr<InputDevice> m_InputDevice;
    };
}