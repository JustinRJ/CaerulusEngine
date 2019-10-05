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

        struct KeyData
        {
            Action Action = Action::Unknown;
            Modifier Mode = Modifier::None;
        };

        struct KeyBinding
        {
            Action TargetAction = Action::Unknown;
            std::function<void(Modifier)> Callback = {};
            std::string Name = "";
        };

        class KeyboardInputManager : public Interface::ITickable
        {
        public:

            KeyboardInputManager(Graphics::Window::GLWindow* window) :
                m_KeyDataMap(*new std::map<int, KeyData*>()),
                m_KeyBindingMap(*new std::map<int, KeyBinding*>())
            {
                glfwSetWindowUserPointer(window->GetGLFWWindow(), this);
            };

            virtual ~KeyboardInputManager() {};

            virtual void Reset() override
            {
                m_KeyBindingMap.clear();
            }

            virtual void Update(float deltaTime) override
            {
                InvokeCallbacks();
                UpdateActionState();
            }

            virtual void FixedUpdate(float fixedTime) override {};

            virtual void LateUpdate(float deltaTime) override {};

            void AddWindowKeyCallback(Graphics::Window::GLWindow* window, int key, Action action, std::function<void(Modifier)> callback, Modifier modifier = None, std::string name = "")
            {
                KeyBinding newBinding;
                newBinding.TargetAction = action;
                newBinding.Callback = callback;
                newBinding.Name = name;

                m_KeyBindingMap[key] = std::make_unique<KeyBinding>(newBinding).release();

                glfwSetKeyCallback(window->GetGLFWWindow(), [](GLFWwindow* windowI, int keyI, int scancodeI, int actionI, int modeI)
                {
                    auto self = static_cast<KeyboardInputManager*>(glfwGetWindowUserPointer(windowI));
                    if (self->GetKeyBindingMap().find(keyI) != self->GetKeyBindingMap().end())
                    {
                        KeyData keyData({ static_cast<Action>(actionI) , static_cast<Modifier>(modeI) });
                        self->GetKeyDataMap()[keyI] = std::make_unique<KeyData>(keyData).release();
                    }
                });
            }

            std::map<int, KeyData*>& GetKeyDataMap()
            {
                return m_KeyDataMap;
            }

            std::map<int, KeyBinding*>& GetKeyBindingMap()
            {
                return m_KeyBindingMap;
            }

            bool Invoke(const std::string& name, Modifier mod = None)
            {
                bool invoked = false;
                for (auto& bindingMap : m_KeyBindingMap)
                {
                    if (bindingMap.second && name.compare(bindingMap.second->Name) == 0)
                    { 
                        bindingMap.second->Callback(mod);
                        invoked = true;
                    }
                }
                return invoked;
            }

        private:

            void UpdateActionState()
            {
                for (auto& keyDataPair : m_KeyDataMap)
                {
                    auto& action = keyDataPair.second->Action;
                    if (action == Press)
                    {
                        action = Hold;
                    }
                    else if (action == Release)
                    {
                        action = Unknown;
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
                        Action action = m_KeyDataMap.at(key)->Action;
                        Action targetAction = m_KeyBindingMap.at(key)->TargetAction;

                        if (action == targetAction && action != Unknown)
                        {
                            bindingPair.second->Callback(m_KeyDataMap.at(key)->Mode);
                        }
                    }
                }
            }

            std::map<int, KeyData*>& m_KeyDataMap;
            std::map<int, KeyBinding*>& m_KeyBindingMap;
        };
    }
}
