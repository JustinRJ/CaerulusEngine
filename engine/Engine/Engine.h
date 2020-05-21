#pragma once

#include "../Graphics/GraphicsEngine.h"
#include "../Graphics/Window/GLWindow.h"

#include "../Core/Input/MouseInputManager.h"
#include "../Core/Input/KeyboardInputManager.h"
#include "../Core/Time/FPSLimiter.h"
#include "../Core/Time/FixedTimer.h"
#include "../Core/Interface/ITickable.h"
#include "../Core/Interface/NonCopyable.h"

#include "../Managers/Managers/TextureManager.h"
#include "../Managers/Managers/MaterialManager.h"
#include "../Managers/Managers/ModelManager.h"
#include "../Managers/Managers/ShaderManager.h"
#include "../Managers/Managers/ShaderSrcManager.h"

using namespace Core::Time;
using namespace Core::Input;
using namespace Core::Interface;
using namespace Graphics;
using namespace Graphics::Window;
using namespace Graphics::Render;
using namespace Managers;

class Engine : public NonCopyable
{
public:

    Engine(int argc, char** argv);
    virtual ~Engine() = default;

    void Run();

    virtual void InitInput();
    virtual void InitScene();
    virtual void InitRenderer();

private:

    void Tick();

    int m_argCount;
    char** m_argValue;

    bool m_running = false;

    float m_deltaTime = 0.0f;
    float m_fixedTime = 0.0f;
    float m_fpsLimit = 1.0f / 144.0f;
    float m_normalSpeed = 150.0f;
    float m_sprintSpeed = 300.0f;
    double m_mouseSensitivity = 50.0;

    std::unique_ptr<FPSLimiter> m_fpsLimiter;
    std::unique_ptr<FixedTimer> m_fixedTimer;

    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<GLWindow> m_window;

    std::shared_ptr<GraphicsEngine> m_graphicsEngine;
    std::shared_ptr<KeyboardInputManager> m_keyboardInputManager;
    std::shared_ptr<MouseInputManager> m_mouseInputManager;

    std::vector<std::shared_ptr<ITickable>> m_tickable;

    TextureManager m_textureManager;
    MaterialManager m_materialManager;
    ModelManager m_modelManager;
    ShaderSrcManager m_shaderSrcManager;
    ShaderManager m_shaderManager;
};
