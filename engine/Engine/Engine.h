#pragma once

#include "../Graphics/RenderSystem.h"
#include "../Graphics/Window/GLWindow.h"

#include "../Core/Input/MouseInputManager.hpp"
#include "../Core/Input/KeyboardInputManager.hpp"
#include "../Core/Time/FPSLimiter.h"
#include "../Core/Time/FixedLimiter.h"
#include "../Core/Interface/ITickable.h"

#include "../Managers/Model/ModelManager.h"
#include "../Managers/Shader/ShaderManager.h"
#include "../Managers/Texture/TextureManager.h"
#include "../Managers/Material/MaterialManager.h"

namespace Engine
{
    using namespace Core::Time;
    using namespace Core::Input;
    using namespace Core::Interface;

    using namespace Graphics::Window;
    using namespace Graphics::Render;

    using namespace Managers::Model;
    using namespace Managers::Texture;
    using namespace Managers::Material;
    using namespace Managers::PipeLine;

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

        bool m_Paused = false;
        bool m_Running = false;

        float m_DeltaTime = 0.0f;
        float m_FixedTime = 0.0f;
        float m_FPSLimit = 1.0f / 60.0f;
        double m_MouseSensitivity = 10.0;

        std::unique_ptr<FPSLimiter> m_FPSLimiter;
        std::unique_ptr<FixedLimiter> m_FixedLimiter;

        std::shared_ptr<GLWindow> m_Window;
        std::shared_ptr<RenderSystem> m_RenderSystem;
        std::shared_ptr<KeyboardInputManager> m_KeyboardInputManager;
        std::shared_ptr<MouseInputManager> m_MouseInputManager;

        std::vector<std::shared_ptr<ITickable>> m_Tickable;

        TextureManager m_TextureManager;
        MaterialManager m_MaterialManager;
        ModelManager m_ModelManager;
    };
}