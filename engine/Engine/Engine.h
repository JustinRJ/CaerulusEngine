#pragma once

#include "../Core/Interface/ITickable.h"
#include "../Core/Time/FPSLimiter.h"
#include "../Core/Time/FixedLimiter.h"
#include "../Core/Input/InputManager.hpp"
#include "../Graphics/Window/GLWindow.h"


#include <vector>

namespace Engine
{
    using namespace Core;
    using namespace Time;
    using namespace Input;
    using namespace Graphics;
    using namespace Interface;

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

        std::unique_ptr<FPSLimiter> m_FPSLimiter;
        std::unique_ptr<FixedLimiter> m_FixedLimiter;

        std::shared_ptr<InputManager> m_InputManager;

        std::vector<std::shared_ptr<ITickable>> m_Tickable;

        //Remove
        Graphics::Window::GLWindow window;
        bool focused;
    };
}