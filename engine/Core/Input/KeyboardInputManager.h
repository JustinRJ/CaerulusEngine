#pragma once

#include <map>
#include <string>
#include <functional>
#include "InputDefines.h"
#include "Core/Interface/ITickable.h"

namespace Core
{
    namespace Input
    {
        class Graphics::Window::GLWindow;

        struct KeyBinding
        {
            Action TargetAction = Action::Unknown;
            std::function<void(Modifier)> Callback = {};
            std::string Name = "";
        };

        using namespace Graphics::Window;
        class KeyboardInputManager : public Interface::ITickable
        {
        public:

            KeyboardInputManager(std::shared_ptr<GLWindow> window)
            {
                glfwSetWindowUserPointer(window->GetGLFWWindow(), this);
            };

            virtual ~KeyboardInputManager() = default;

            void Reset() override
            {
                m_keyBindingMap.clear();
            }

            void PreUpdate(float deltaTime) override
            {
                InvokeCallbacks();
                UpdateActionState();
            }

            void Update(float deltaTime) override {}

            void FixedUpdate(float fixedTime) override {}

            void AddWindowKeyCallback(std::shared_ptr<GLWindow> window, int key, Action action, std::function<void(Modifier)> callback, std::string name = "")
            {
                KeyBinding newBinding;
                newBinding.TargetAction = action;
                newBinding.Callback = callback;
                newBinding.Name = name;

                m_keyBindingMap.insert(std::make_pair(key, newBinding));

                glfwSetKeyCallback(window->GetGLFWWindow(), [](GLFWwindow* windowI, int keyI, int scancodeI, int actionI, int modeI)
                {
                    auto self = static_cast<KeyboardInputManager*>(glfwGetWindowUserPointer(windowI));
                    self->SetModifier(static_cast<Modifier>(modeI));

                    if (self->GetKeyBindingMap().find(keyI) != self->GetKeyBindingMap().end())
                    {
                        Action action = static_cast<Action>(actionI);
                        self->GetKeyDataMap()[keyI] = action;
                    }
                });
            }

            bool Invoke(const std::string& name, Modifier mod = Modifier::None)
            {
                bool invoked = false;
                for (auto& bindingMap : m_keyBindingMap)
                {
                    if (name.compare(bindingMap.second.Name) == 0)
                    { 
                        bindingMap.second.Callback(mod);
                        invoked = true;
                    }
                }
                return invoked;
            }

        private:

            std::map<int, Action>& GetKeyDataMap()
            {
                return m_keyDataMap;
            }

            std::multimap<int, KeyBinding>& GetKeyBindingMap()
            {
                return m_keyBindingMap;
            }

            void SetModifier(Modifier mod)
            {
                m_currentModifier = mod;
            }

            void UpdateActionState()
            {
                for (auto& keyDataPair : m_keyDataMap)
                {
                    if (keyDataPair.second == Action::Press)
                    {
                        keyDataPair.second = Action::Hold;
                    }
                    else if (keyDataPair.second == Action::Release)
                    {
                        keyDataPair.second = Action::Unknown;
                    }
                }
            }

            void InvokeCallbacks()
            {
                for (auto& bindingPair : m_keyBindingMap)
                {
                    int key = bindingPair.first;
                    if (m_keyDataMap.find(key) != m_keyDataMap.end())
                    {
                        Action action = m_keyDataMap.at(key);
                        Action targetAction = bindingPair.second.TargetAction;

                        if (action == targetAction && action != Action::Unknown)
                        {
                            bindingPair.second.Callback(m_currentModifier);
                        }
                    }
                }
            }

            Modifier m_currentModifier;
            std::map<int, Action> m_keyDataMap;
            std::multimap<int, KeyBinding> m_keyBindingMap;
        };
    }
}
