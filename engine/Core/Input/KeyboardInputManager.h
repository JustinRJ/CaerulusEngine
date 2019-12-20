#pragma once

#include "InputDefines.h"
#include <functional>
#include <map>
#include <string>
#include "../Interface/ITickable.h"

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
                glfwSetWindowUserPointer(window->GetGLFWWindow().get(), this);
            };

            ~KeyboardInputManager() = default;

            void Reset() override
            {
                m_KeyBindingMap.clear();
            }

            void PreUpdate(float deltaTime) override
            {
                InvokeCallbacks();
            }

            void Update(float deltaTime) override
            {
                UpdateActionState();
            }

            void FixedUpdate(float fixedTime) override {}

            void AddWindowKeyCallback(std::shared_ptr<GLWindow> window, int key, Action action, std::function<void(Modifier)> callback, std::string name = "")
            {
                KeyBinding newBinding;
                newBinding.TargetAction = action;
                newBinding.Callback = callback;
                newBinding.Name = name;

                m_KeyBindingMap.insert(std::make_pair(key, newBinding));

                glfwSetKeyCallback(window->GetGLFWWindow().get(), [](GLFWwindow* windowI, int keyI, int scancodeI, int actionI, int modeI)
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

            bool Invoke(const std::string& name, Modifier mod = None)
            {
                bool invoked = false;
                for (auto& bindingMap : m_KeyBindingMap)
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
                return m_KeyDataMap;
            }

            std::multimap<int, KeyBinding>& GetKeyBindingMap()
            {
                return m_KeyBindingMap;
            }

            void SetModifier(Modifier mod)
            {
                m_currentModifier = mod;
            }

            void UpdateActionState()
            {
                for (auto& keyDataPair : m_KeyDataMap)
                {
                    if (keyDataPair.second == Press)
                    {
                        keyDataPair.second = Hold;
                    }
                    else if (keyDataPair.second == Release)
                    {
                        keyDataPair.second = Unknown;
                    }
                }
            }

            void InvokeCallbacks()
            {
                for (auto& bindingPair : m_KeyBindingMap)
                {
                    int key = bindingPair.first;
                    if (m_KeyDataMap.find(key) != m_KeyDataMap.end())
                    {
                        Action action = m_KeyDataMap.at(key);
                        Action targetAction = bindingPair.second.TargetAction;

                        if (action == targetAction && action != Unknown)
                        {
                            bindingPair.second.Callback(m_currentModifier);
                        }
                    }
                }
            }

            Modifier m_currentModifier;
            std::map<int, Action> m_KeyDataMap;
            std::multimap<int, KeyBinding> m_KeyBindingMap;
        };
    }
}
