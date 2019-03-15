#pragma once

#include "../Graphics/RenderSystem.h"
#include "../Graphics/Window/GLWindow.h"

#include "../Core/Input/InputManager.hpp"
#include "../Core/Interface/ITickable.h"
#include "../Core/Time/FPSLimiter.h"
#include "../Core/Time/FixedLimiter.h"
#include "../Core/Parser/Tokenizer.h"
#include "../Core/Parser/StringHelper.h"

#include "../Managers/Model/ModelManager.h"
#include "../Managers/Shader/ShaderManager.h"
#include "../Managers/Texture/TextureManager.h"
#include "../Managers/Material/MaterialManager.h"

namespace Engine
{
    using namespace Core::Time;
    using namespace Core::Input;
    using namespace Core::Interface;
    using namespace Core::Parser;

    using namespace Graphics::Render;
    using namespace Graphics::Window;

    using namespace Managers::Model;
    using namespace Managers::Shader;
    using namespace Managers::Texture;
    using namespace Managers::Material;

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

        ModelManager m_ModelManager;
        TextureManager m_TextureManager;
        MaterialManager m_MaterialManager;
    };
}