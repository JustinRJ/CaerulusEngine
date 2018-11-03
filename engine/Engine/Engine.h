#pragma once

#include "../Core/Interface/ITickable.h"
#include "../Core/Time/FPSLimiter.h"
#include "../Core/Time/FixedLimiter.h"
#include "../Input/Input/OISInputDevice.h"

namespace Engine
{
    using namespace Core;
    using namespace Input;

    class Engine
    {
    public:

        Engine(int argc, char** argv);
        ~Engine();

        void Run();

    private:

        void Tick();

        int m_ArgCount;
        char** m_ArgValue;

        bool m_Running;
        float m_DeltaTime;
        float m_FixedTime;
        float m_FPSLimit;

        std::unique_ptr<Time::FPSLimiter> m_FPSLimiter;
        std::unique_ptr<Time::FixedLimiter> m_FixedLimiter;

        std::shared_ptr<InputDevice> m_InputDevice;

        std::vector<std::shared_ptr<Interface::ITickable>> m_Updatable;
    };
}