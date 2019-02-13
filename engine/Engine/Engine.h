#pragma once

#include "../Graphics/RenderSystem.h"
#include "../Graphics/Window/GLWindow.h"

#include "../Core/Input/InputManager.hpp"
#include "../Core/Interface/ITickable.h"
#include "../Core/Time/FPSLimiter.h"
#include "../Core/Time/FixedLimiter.h"
#include "../Core/Parser/Tokenizer.h"
#include "../Core/Parser/StringHelper.h"

namespace Engine
{
    using namespace Core::Time;
    using namespace Core::Input;
    using namespace Core::Interface;
    using namespace Core::Parser;

    using namespace Graphics::Render;
    using namespace Graphics::Window;


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
        std::shared_ptr<RenderSystem> m_RenderSystem;

        std::vector<std::shared_ptr<ITickable>> m_Tickable;
    };
}