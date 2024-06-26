#pragma once

#include "InputDefines.h"

#include <map>
#include <memory>
#include <string>
#include <functional>

#include "Interface/ITickable.h"

namespace Graphics
{
    namespace Window
    {
        class GLWindow;
    }
}

namespace Core
{
    namespace Input
    {
        struct KeyBinding
        {
            Action TargetAction = Action::Unknown;
            std::function<void(Modifier)> Callback = {};
            std::string Name = "";
        };

        class KeyboardInputSystem : public Interface::ITickable
        {
        public:
            KeyboardInputSystem(Graphics::Window::GLWindow& window)
            {
                glfwSetWindowUserPointer(window.GetGLFWWindow(), this);
            };

            void EarlyTick() override
            {
                InvokeCallbacks();
                UpdateActionState();
            }

            void AddWindowKeyCallback(Graphics::Window::GLWindow& window, int key, Action action, std::function<void(Modifier)> callback, std::string_view name = "")
            {
                KeyBinding newBinding;
                newBinding.TargetAction = action;
                newBinding.Callback = callback;
                newBinding.Name = name;

                m_keyBindingMap.emplace(key, newBinding);

                glfwSetKeyCallback(window.GetGLFWWindow(), [](GLFWwindow* windowI, int keyI, int scancodeI, int actionI, int modeI)
                {
                    KeyboardInputSystem* self = static_cast<KeyboardInputSystem*>(glfwGetWindowUserPointer(windowI));
                    self->m_currentModifier = static_cast<Modifier>(modeI);

                    if (self->m_keyBindingMap.find(keyI) != self->m_keyBindingMap.end())
                    {
                        Action action = static_cast<Action>(actionI);
                        self->m_keyDataMap[keyI] = action;
                    }
                });
            }

            bool Invoke(std::string_view name, Modifier mod = Modifier::None)
            {
                bool invoked = false;
                for (const auto& kv : m_keyBindingMap)
                {
                    if (name.compare(kv.second.Name) == 0)
                    { 
                        kv.second.Callback(mod);
                        invoked = true;
                    }
                }
                return invoked;
            }

        private:
            void UpdateActionState()
            {
                for (auto& kv : m_keyDataMap)
                {
                    if (kv.second == Action::Press)
                    {
                        kv.second = Action::Hold;
                    }
                    else if (kv.second == Action::Release)
                    {
                        kv.second = Action::Unknown;
                    }
                }
            }

            void InvokeCallbacks()
            {
                for (const auto& kv : m_keyBindingMap)
                {
                    int key = kv.first;
                    if (m_keyDataMap.find(key) != m_keyDataMap.end())
                    {
                        Action action = m_keyDataMap[key];
                        Action targetAction = kv.second.TargetAction;

                        if (action == targetAction && action != Action::Unknown)
                        {
                            kv.second.Callback(m_currentModifier);
                        }
                    }
                }
            }

            Modifier m_currentModifier = Modifier::None;
            std::map<int, Action> m_keyDataMap;
            std::multimap<int, KeyBinding> m_keyBindingMap;
        };
    }
}
